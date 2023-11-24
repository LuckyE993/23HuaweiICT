#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __KEIL__
#include <unistd.h>
#endif

#include <sal.h>
#include <sal_imp.h>
#include <sal_define.h>
#include <sal_types.h>
#include <at.h>
#include <link_misc.h>
#include <link_endian.h>
#include <osal.h>
#include <iot_link_config.h>
// #include <timeval.h>

#define CMDTIMEOUT      (6*1000)
#define RCVINDEX        "\r\n+RECEIVE:"
#define CACHELEN        (1024 * 5)
#define MAX_NR_FDS      (7)

typedef struct {
    int               sockfd;
    int               domain;
    int               type;
    int               protocol;

    bool_t            connected;
    unsigned int      timeout;
    tag_ring_buffer_t m26_rcvring;

    char              oob_resp[CACHELEN];
    unsigned char     rcvbuf[CACHELEN];
} m26_sock_cb_t;

static m26_sock_cb_t * s_m26_cbs [MAX_NR_FDS] = { NULL, };

static osal_mutex_t         g_atcombo_lock;
static osal_mutex_t         g_iobufer_lock;

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
void hex_dump(const uint8_t *ptr, size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    // RT_ASSERT(ptr != RT_NULL);

    for (i = 0; i < buflen; i += 16)
    {
        printf("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
            {
                printf("%02X ", buf[i + j]);
            }
            else
            {
                printf("   ");
            }
                
        printf(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
            {
                printf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
            }
        printf("\n");
    }
}
static bool_t __m26_atcmd(const char *cmd,const char *index,char *buf, int len)
{
    return at_command((const void *) cmd, strlen(cmd), index, buf, len, CMDTIMEOUT) >= 0;
}

static bool_t __m26_atcmd_simple(const char *cmd,const char *index)
{
    return __m26_atcmd(cmd, index, NULL, 0);
}

static void __m26_atcmd_noindex(const char *cmd)
{
    at_command((const void *) cmd, strlen(cmd), NULL, NULL, 0, CMDTIMEOUT);
}

static void __m26_atcmd_int_noindex(int i)
{
    char buff[12];

    snprintf(buff, 12, "%d", i);

    __m26_atcmd_noindex(buff);
}

static int __m26_poll_cmd(const char *cmd, const char *index, uint32_t timeout, int retry)
{
    do {
        if (at_command((const void *) cmd, strlen(cmd), index, NULL, 0, timeout) >= 0) {
            return 0;
        }
    } while (--retry != 0);

    return -1;
}

static unsigned int __atoi(unsigned char ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }

    ch |= 0x20;

    if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    }

    return (unsigned int) -1;
}

static int __m26_rcvdeal_single(void *msg,size_t len)
{
    char           *data = (char *)msg;
    int             fd;
    unsigned short  data_len = 0;
    int             ret;
    int             got = 0;
    data = data + 12;

    fd = *data - '0';

    /* skip fd and ',' */

    data += 3;

    /* now we are at data */
    for (; *data <= '9' && *data >= '0' ;data++)
    {
        data_len = (data_len * 10 + (*data - '0'));
    }

    if (data_len > (CACHELEN - ring_buffer_datalen(&s_m26_cbs[fd]->m26_rcvring))) {
        return 0;
    }
    data += 2;
    ring_buffer_write(&s_m26_cbs[fd]->m26_rcvring, (unsigned char *)data, data_len);
    ret = data + data_len;

    return ret;
}

static int __m26_rcvdeal(void *args,void *msg,size_t len)
{
    int ret = 0;
    if(osal_mutex_lock(g_iobufer_lock))
    {
        while (msg != NULL)
        {
            int r = __m26_rcvdeal_single(msg, len);

            if (r == -1)
            {
                break;
            }

            ret += r;

            msg = strstr(r * 2 + (char *)msg, RCVINDEX);
        }

        osal_mutex_unlock(g_iobufer_lock);
    }
    return ret == 0 ? -1 : ret;
}

static int __m26_socket(int domain, int type, int protocol)
{
    int i = 0;

    for (i = 1; i < MAX_NR_FDS; i++)
    {
        if (s_m26_cbs[i] == NULL)
        {
            break;
        }
    }

    if (i == MAX_NR_FDS)
        {
        return -1;
        }

    s_m26_cbs[i] = (m26_sock_cb_t *)malloc(sizeof(m26_sock_cb_t));

    if (s_m26_cbs[i] == NULL)
    {
        return -1;
    }

    memset(s_m26_cbs[i], 0, sizeof(m26_sock_cb_t));

	s_m26_cbs[i]->domain   = domain;
	s_m26_cbs[i]->type     = type;
	s_m26_cbs[i]->protocol = protocol;
	s_m26_cbs[i]->sockfd   = i;

    ring_buffer_init(&s_m26_cbs[i]->m26_rcvring, s_m26_cbs[i]->rcvbuf, CACHELEN, 0, 0);

	return i;
}

static int __m26_bind(int fd, const struct sockaddr *addr, int addrlen)
{
	// not implemented for client
    return 0;
}

static int __m26_listen(int fd, int backlog)
{
	// not implemented for client
    return 0;
}

static int __m26_accept(int fd, struct sockaddr *addr, int addrlen)
{
	// not implemented for client
    return 0;
}

static int __m26_connect(int fd, const struct sockaddr *addr, int addrlen)
{
    int ret = -1;
    const struct sockaddr_in *serv_addr;
    uint16_t                  remote_port;
    struct in_addr            remote_ip_int;
    char                     *remote_ip;
    char                      *proto;

    if (addr == NULL)
    {
        return ret;
    }

    if (fd <= 0 || fd >= MAX_NR_FDS || s_m26_cbs[fd] == NULL)
    {
        return ret;
    }

    serv_addr = (const struct sockaddr_in *) addr;
    remote_port = ntohs(serv_addr->sin_port);
    remote_ip_int = serv_addr->sin_addr;
    remote_ip = inet_ntoa(remote_ip_int);

    proto = s_m26_cbs[fd]->type == SOCK_DGRAM ? "UDP" : "TCP";

    //
    // format:
    // AT+QIOPEN=0,"TCP","117.50.111.72",8902
    //
    // proto: 0 - TCP, 1 - UDP
    //

    if(osal_mutex_lock(g_atcombo_lock)){
        __m26_atcmd_noindex("AT+QIOPEN=");
        __m26_atcmd_int_noindex(fd);
        __m26_atcmd_noindex(",\"");
        __m26_atcmd_noindex(proto);
        __m26_atcmd_noindex("\",\"");
        __m26_atcmd_noindex(remote_ip);
        __m26_atcmd_noindex("\",");
        __m26_atcmd_int_noindex(remote_port);


        if (!__m26_atcmd_simple("\r\n", "CONNECT OK")) {
            ret = -1;
        }
        else{
            s_m26_cbs[fd]->connected = true;
            ret = 0;
        }
        osal_mutex_unlock(g_atcombo_lock);
    }
    return ret;
}


static int __m26_send(int fd, const void *buf, int len, int flags)
{
    int ret = -1;
    if (fd <= 0 || fd > MAX_NR_FDS) {
        return ret;
    }

    if(osal_mutex_lock(g_atcombo_lock)){

        __m26_atcmd_noindex("AT+QISEND=");
        __m26_atcmd_int_noindex (fd);
        __m26_atcmd_noindex(",");
        __m26_atcmd_int_noindex(len);
        __m26_atcmd_simple("\r\n", ">");

        ret = at_command((const void *)buf, len, "SEND OK", NULL, 0, CMDTIMEOUT) >= 0 ? len : -1;

        osal_mutex_unlock(g_atcombo_lock);
    }

    return ret;
}

static int __m26_sendto(int fd, const void *msg, int len, int flags,
                         struct sockaddr *addr, int addrlen)
{
	if (!s_m26_cbs[fd]->connected) {
		__m26_connect(fd, addr, addrlen);
	}

    return __m26_send(fd, msg, len, flags);
}

static int __m26_recv(int fd, void *buf, size_t len, int flags)
{
	int          ret     = -1;
	unsigned int timeout = 0;
	timeout = s_m26_cbs[fd]->timeout;

	do {

	    if(osal_mutex_lock(g_iobufer_lock))
	    {
	        if (s_m26_cbs[fd]->type == SOCK_DGRAM) {
	            unsigned short data_len = 0;
	            ret = ring_buffer_read(&s_m26_cbs[fd]->m26_rcvring,
	                                   (unsigned char *)&data_len, sizeof(data_len));
	            ret = ring_buffer_read(&s_m26_cbs[fd]->m26_rcvring,
	                                   (unsigned char *)buf, data_len);

	        } else {
	            ret = ring_buffer_read(&s_m26_cbs[fd]->m26_rcvring, (unsigned char *)buf, len);
	        }

	        osal_mutex_unlock(g_iobufer_lock);
	    }

        if (ret > 0) {
            return ret;
        }

        if (timeout > 0) {
            osal_task_sleep(1);
        }
	} while (timeout-- > 0);

	return -1;
}

static int __m26_recvfrom(int fd, void *msg, int len, int flags, struct sockaddr *addr, socklen_t *addrlen)
{
    return __m26_recv(fd, msg, len, flags);
}

static int __m26_setsockopt(int fd, int level, int option, const void *option_value, int option_len)
{
	struct timeval *timedelay = (struct timeval *)option_value;
	s_m26_cbs[fd]->timeout = (timedelay->tv_sec * 1000) + (timedelay->tv_usec / 1000);
    return 0;
}

static int __m26_getsockopt(int fd, int level, int optname, const void *optval, int optlen)
{
	//not implemented
    return 0;
}

static int __m26_close(int fd)
{
    int ret = -1;

    if(osal_mutex_lock(g_atcombo_lock)){

        __m26_atcmd_noindex("AT+QICLOSE=");
        __m26_atcmd_int_noindex(fd);

        if (__m26_atcmd_simple("\r\n", "CLOSE OK")) {

            osal_free(s_m26_cbs[fd]);
            s_m26_cbs[fd] = NULL;
            ret = 0;
        }

        osal_mutex_unlock(g_atcombo_lock);
    }

    return ret;
}

static int __m26_shutdown(int fd, int how)
{
	return __m26_close(fd);
}

static int __m26_getsockname(int fd, struct sockaddr *addr,socklen_t *addrlen)
{
    //not implemented
    return 0;
}

static int __m26_getpeername(int fd, struct sockaddr *addr,socklen_t *addrlen)
{
    //not implemented
    return 0;
}

static struct hostent *__m26_gethostbyname(const char *name)
{
    char  resp[96];
	char *str;
    int   ipv4[4];

    static struct hostent hostent;
    static char           ipv4_ary[4];
    static char          *ipv4_lst[2] = { NULL, NULL };

    if (sscanf(name, "%d.%d.%d.%d", &ipv4[0], &ipv4[1], &ipv4[2], &ipv4[3]) != 4) {

        if(osal_mutex_lock(g_atcombo_lock)){

            __m26_atcmd_noindex("AT+QIDNSGIP=\"");
            __m26_atcmd_noindex(name);

            if (!__m26_atcmd("\"\r\n", "OK", resp, 96)) {
                osal_mutex_unlock(g_atcombo_lock);
                return NULL;
            }
            else{
                osal_mutex_unlock(g_atcombo_lock);
            }
        }

        memset(&hostent, 0, sizeof(struct hostent));

        if (sscanf(resp, "%d.%d.%d.%d", &ipv4[0], &ipv4[1], &ipv4[2], &ipv4[3]) <= 0) {
            return NULL;
        }
    }

    ipv4_ary[0] = ipv4[0];
    ipv4_ary[1] = ipv4[1];
    ipv4_ary[2] = ipv4[2];
    ipv4_ary[3] = ipv4[3];

    ipv4_lst[0] = ipv4_ary;

    hostent.h_addr_list = ipv4_lst;
    hostent.h_addrtype = AF_INET;
    hostent.h_length = 4;

    return &hostent;
}


static const tag_tcpip_ops s_tcpip_socket_ops =
{
    .socket        = (fn_sal_socket)        __m26_socket,
    .bind          = (fn_sal_bind)          __m26_bind,
    .listen        = (fn_sal_listen)        __m26_listen,
    .connect       = (fn_sal_connect)       __m26_connect,
    .accept        = (fn_sal_accept)        __m26_accept,
    .send          = (fn_sal_send)          __m26_send,
    .sendto        = (fn_sal_sendto)        __m26_sendto,
    .recv          = (fn_sal_recv)          __m26_recv,
    .recvfrom      = (fn_sal_recvfrom)      __m26_recvfrom,
    .setsockopt    = (fn_sal_setsockopt)    __m26_setsockopt,
    .getsockopt    = (fn_sal_getsockopt)    __m26_getsockopt,
    .shutdown      = (fn_sal_shutdown)      __m26_shutdown,
    .closesocket   = (fn_sal_closesocket)   __m26_close,
    .getpeername   = (fn_sal_getpeername)   __m26_getpeername,
    .getsockname   = (fn_sal_getsockname)   __m26_getsockname,
    .gethostbyname = (fn_sal_gethostbyname) __m26_gethostbyname,
};

static const tag_tcpip_domain s_tcpip_socket =
{
    .name = "m26 socket",
    .domain = AF_INET,
    .ops = &s_tcpip_socket_ops,
};

static bool_t _m26_joinnetwork(void)
{
    // extern void atdevice_enable(void);

    (void)__m26_atcmd_simple("ATE0\r\n", "OK");
    
    if (__m26_poll_cmd("AT\r\n", "OK", 1000, 10) != 0) {
        return false;
    }

    if (__m26_poll_cmd("AT+CPIN?\r\n", "READY", 1000, 3) != 0) {
        return false;
    }

    if (__m26_poll_cmd("AT+CSQ\r\n", "+CSQ:", 1000, 3) != 0) {
        return false;
    }

    if (__m26_poll_cmd("AT+CGREG?\r\n", "+CGREG: 0,1", 1000, 3) != 0) {
        return false;
    }

    if (__m26_poll_cmd("AT+QIDEACT\r\n", "DEACT OK", 1000, 8) != 0) {
        return false;
    }
    (void)__m26_atcmd_simple("AT+QIMODE=0\r\n", "OK");

    (void)__m26_atcmd_simple("AT+QINDI=0\r\n", "OK");

    (void)__m26_atcmd_simple("AT+QICSGP=1,\"CMNET\"\r\n", "OK");
    (void)__m26_atcmd_simple("AT+QIMUX=1\r\n", "OK");
    (void)__m26_atcmd_simple("AT+QIFGCNT=0\r\n", "OK");
    (void)__m26_atcmd_simple("AT+QIREGAPP\r\n", "OK");
    (void)__m26_atcmd_simple("AT+QIACT\r\n", "OK");
    // (void)__m26_atcmd_simple("AT+QILOCIP\r\n", "OK");


    at_debugclose();

    return true;
}

int link_tcpip_imp_init(void)
{
    int ret = -1;

    s_m26_cbs[0] = (m26_sock_cb_t *) -1;

    if(false == osal_mutex_create(&g_atcombo_lock))
    {
        printf(" m26 AT MUTEX_ERR\r\n");
        return ret;
    }

    if(false == osal_mutex_create(&g_iobufer_lock))
    {
        osal_mutex_del(g_atcombo_lock);
        printf(" m26 IO MUTEX_ERR\r\n");
        return ret;
    }

    at_oobregister("m26", RCVINDEX, strlen(RCVINDEX), __m26_rcvdeal, NULL);

    while (!_m26_joinnetwork()) {
        printf("Fail to enable network and try another time\r\n");
        osal_task_sleep(1000);
    }

   //reach here means everything is ok, we can go now
    ret = link_sal_install(&s_tcpip_socket);

    printf("sal:install socket %s\r\n", ret == 0 ? "success" : "failed");

    return ret;
}

