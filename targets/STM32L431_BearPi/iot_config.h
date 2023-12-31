#define CONFIG_ARCH_CPU_TYPE "armv7-m"
#define CONFIG_UARTAT_RCVMAX 2048
#define CONFIG_UARTAT_BAUDRATE 9600
#define CONFIG_UARTAT_DEVNAME "atdev"
#define CONFIG_LITEOS_ENABLE 1
#define CONFIG_AT_ENABLE 1
#define CONFIG_AT_DEVNAME "atdev"
#define CONFIG_AT_OOBTABLEN 6
#define CONFIG_AT_RECVMAXLEN 1024
#define CONFIG_AT_TASKPRIOR 10
#define CONFIG_CJSON_ENABLE 1
#define CONFIG_DRIVER_ENABLE 1
#define CONFIG_LINKLOG_ENABLE 1
#define CONFIG_LINKQUEUE_ENABLE 1
#define CONFIG_LINKDEMO_ENABLE 1
#define CONFIG_STIMER_ENABLE 1
#define CONFIG_STIMER_STACKSIZE 2048
#define CONFIG_STIMER_TASKPRIOR 10
#define CONFIG_OCSERVICES_ENABLE 1
#define CONFIG_OCLWM2M_ENABLE 1
#define CONFIG_BOUDICA150_ENABLE 1
#define CONFIG_SHELL_ENABLE 1
#define CONFIG_SHELL_TASK_STACKSIZE 2048
#define CONFIG_SHELL_TASK_PRIOR 10
#define CONFIG_IOT_LINK_CONFIGFILE "iot_config.h"
#define CONFIG_Demo_OC_NB_LWM2M_Smoke 1
#define CONFIG_USER_DEMO "oc_nb_lwm2m_smoke"
