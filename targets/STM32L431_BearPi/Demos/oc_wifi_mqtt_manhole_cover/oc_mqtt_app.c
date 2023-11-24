/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2020-02-07 09:06  zhangqianfu  The first version
 *
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <iot_link_config.h>
#include <queue.h>
#include <oc_mqtt_al.h>
#include <oc_mqtt_profile.h>
#include <E53_SC2.h>
#include "lcd.h"

#ifndef CONFIG_APP_SERVERIP
#define CONFIG_APP_SERVERIP       "117.78.5.125"
#endif
#ifndef CONFIG_APP_SERVERPORT
#define CONFIG_APP_SERVERPORT     "1883"
#endif

#ifndef CONFIG_APP_DEVICEID
#define CONFIG_APP_DEVICEID       "5f58768785edc002bc69cbf2_hcdemo123456"
#endif

#ifndef CONFIG_APP_DEVICEPWD
#define CONFIG_APP_DEVICEPWD      "123456789"
#endif

#ifndef CONFIG_APP_LIFETIME
#define CONFIG_APP_LIFETIME       60     ///< seconds
#endif

#ifndef CONFIG_QUEUE_TIMEOUT
#define CONFIG_QUEUE_TIMEOUT      (5*1000)
#endif

typedef enum
{
    en_msg_report = 0,
    en_msg_conn,
    en_msg_disconn,
}en_msg_type_t;

typedef struct
{
    int temp;
    int acce_x;
    int acce_y;
    int acce_z;
    char cover_status;
}report_t;

typedef struct
{
    en_msg_type_t msg_type;
    union{
        report_t report;
    }msg;
}app_msg_t;

typedef struct
{
    queue_t                     *app_msg;
    int                          connected;
}app_cb_t;
static app_cb_t  g_app_cb;

static void deal_report_msg(report_t *report)
{
    oc_mqtt_profile_service_t    service;
    oc_mqtt_profile_kv_t temperature;
    oc_mqtt_profile_kv_t Accel_x;
    oc_mqtt_profile_kv_t Accel_y;
    oc_mqtt_profile_kv_t Accel_z;
    oc_mqtt_profile_kv_t status;

    if(g_app_cb.connected != 1){
        return;
    }

    service.event_time = NULL;
    service.service_id = "Manhole_Cover";
    service.service_property = &temperature;
    service.nxt = NULL;

    temperature.key = "Temperature";
    temperature.value = &report->temp;
    temperature.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    temperature.nxt = &Accel_x;

    Accel_x.key = "Accel_x";
    Accel_x.value = &report->acce_x;
    Accel_x.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    Accel_x.nxt = &Accel_y;

    Accel_y.key = "Accel_y";
    Accel_y.value = &report->acce_y;
    Accel_y.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    Accel_y.nxt = &Accel_z;

    Accel_z.key = "Accel_z";
    Accel_z.value = &report->acce_z;
    Accel_z.type = EN_OC_MQTT_PROFILE_VALUE_INT;
    Accel_z.nxt = &status;

    status.key = "Cover_Status";
    status.value = report->cover_status ? "Tilt" : "Level";
    status.type = EN_OC_MQTT_PROFILE_VALUE_STRING;
    status.nxt = NULL;

    oc_mqtt_profile_propertyreport(NULL,&service);
    return;
}

static void deal_conn_msg(void)
{
    int ret;
    oc_mqtt_profile_connect_t  connect_para;
    (void) memset( &connect_para, 0, sizeof(connect_para));

    connect_para.boostrap =      0;
    connect_para.device_id =     CONFIG_APP_DEVICEID;
    connect_para.device_passwd = CONFIG_APP_DEVICEPWD;
    connect_para.server_addr =   CONFIG_APP_SERVERIP;
    connect_para.server_port =   CONFIG_APP_SERVERPORT;
    connect_para.life_time =     CONFIG_APP_LIFETIME;
    connect_para.rcvfunc =       NULL;
    connect_para.security.type = EN_DTLS_AL_SECURITY_TYPE_NONE;
    ret = oc_mqtt_profile_connect(&connect_para);
    if((ret == (int)en_oc_mqtt_err_ok)){
        g_app_cb.connected = 1;
    }
    return;
}

static void deal_disconn_msg(void)
{
    int ret;
    ret = oc_mqtt_profile_disconnect();
    if((ret == (int)en_oc_mqtt_err_ok)){
        g_app_cb.connected = 0;
    }
    return;
}

static int task_main_entry( void *args)
{
    app_msg_t *app_msg;
    while(1){
        app_msg = NULL;
        (void)queue_pop(g_app_cb.app_msg,(void **)&app_msg,(int)cn_osal_timeout_forever);
        if(NULL != app_msg){
            switch(app_msg->msg_type){
                case en_msg_conn:
                    deal_conn_msg();
                    break;
                case en_msg_disconn:
                    deal_disconn_msg();
                    break;
                case en_msg_report:
                    deal_report_msg(&app_msg->msg.report);
                    break;
                default:
                    break;
            }
            osal_free(app_msg);
        }
    }
    return 0;
}

static int task_sensor_entry( void *args)
{
    app_msg_t *app_msg;
    E53_SC2_Data data;
    int X = 0, Y = 0, Z = 0;
    char coverStatus = 0;
    while(1){
        E53_SC2_Read_Data(&data);
        if (X == 0 && Y == 0 && Z == 0) {
            X = (int)data.Accel[ACCEL_X_AXIS];
            Y = (int)data.Accel[ACCEL_Y_AXIS];
            Z = (int)data.Accel[ACCEL_Z_AXIS];
        } else {
            if (X + 100 < data.Accel[ACCEL_X_AXIS] || X - 100 > data.Accel[ACCEL_X_AXIS] ||
                Y + 100 < data.Accel[ACCEL_Y_AXIS] || Y - 100 > data.Accel[ACCEL_Y_AXIS] ||
                Z + 100 < data.Accel[ACCEL_Z_AXIS] || Z - 100 > data.Accel[ACCEL_Z_AXIS]) {
                E53_SC2_LedD1_Set(OFF);
                E53_SC2_LedD2_Set(ON);
                coverStatus = 1;
            } else {
                E53_SC2_LedD1_Set(ON);
                E53_SC2_LedD2_Set(OFF);
                coverStatus = 0;
            }
        }
        LCD_ShowString(10, 135, 200, 16, 16, "Temperature:");
        LCD_ShowNum(140, 135, (int)data.Temperature, 5, 16);
        LCD_ShowString(10, 160, 200, 16, 16, "Acce_X:");
        LCD_ShowNum(140, 160, (int)data.Accel[0], 5, 16);
        LCD_ShowString(10, 185, 200, 16, 16, "Acce_Y:");
        LCD_ShowNum(140, 185, (int)data.Accel[1], 5, 16);
        LCD_ShowString(10, 210, 200, 16, 16, "Acce_Z:");
        LCD_ShowNum(140, 210, (int)data.Accel[2], 5, 16);
        
        app_msg = osal_malloc(sizeof(app_msg_t));
        LINK_LOG_DEBUG("\r\n******************************Temperature      is  %d\r\n", (int)data.Temperature);
        LINK_LOG_DEBUG("\r\n******************************Accel[ACCEL_X_AXIS] is  %d\r\n", (int)data.Accel[ACCEL_X_AXIS]);
        LINK_LOG_DEBUG("\r\n******************************Accel[ACCEL_Y_AXIS] is  %d\r\n", (int)data.Accel[ACCEL_Y_AXIS]);
        LINK_LOG_DEBUG("\r\n******************************Accel[ACCEL_Z_AXIS] is  %d\r\n", (int)data.Accel[ACCEL_Z_AXIS]);
        if (NULL != app_msg) {
            app_msg->msg_type = en_msg_report;
            app_msg->msg.report.temp = (int)data.Temperature;
            app_msg->msg.report.acce_x = (int)data.Accel[ACCEL_X_AXIS];
            app_msg->msg.report.acce_y = (int)data.Accel[ACCEL_Y_AXIS];
            app_msg->msg.report.acce_z = (int)data.Accel[ACCEL_Z_AXIS];
            app_msg->msg.report.cover_status = coverStatus;
            
            if (0 != queue_push(g_app_cb.app_msg, app_msg, CONFIG_QUEUE_TIMEOUT)) {
                osal_free(app_msg);
            }
        }
        osal_task_sleep(2000);
    }
    return 0;
}

#include <shell.h>
static int shell_conn(int argc, const char *argv[])
{
    app_msg_t *app_msg;
    app_msg = osal_malloc(sizeof(app_msg_t));
    if(NULL != app_msg){
        app_msg->msg_type = en_msg_conn;
        if(0 != queue_push(g_app_cb.app_msg,app_msg,CONFIG_QUEUE_TIMEOUT)){
            osal_free(app_msg);
        }
    }
    return 0;
}
OSSHELL_EXPORT_CMD(shell_conn,"con","con atcmd");

static int shell_disconn(int argc, const char *argv[])
{
    app_msg_t *app_msg;
    app_msg = osal_malloc(sizeof(app_msg_t));
    if(NULL != app_msg){
        app_msg->msg_type = en_msg_disconn;
        if(0 != queue_push(g_app_cb.app_msg,app_msg,CONFIG_QUEUE_TIMEOUT)){
            osal_free(app_msg);
        }
    }
    return 0;
}
OSSHELL_EXPORT_CMD(shell_disconn,"discon","discon atcmd");



int standard_app_demo_main()
{
    int ret = -1;
    LINK_LOG_DEBUG("This Is MQTT V5 DEMOS:HC Display");
    g_app_cb.app_msg = queue_create("queue_rcvmsg",10,1);
    if(NULL ==  g_app_cb.app_msg){
        LINK_LOG_ERROR("Create receive msg queue failed");
        return ret;
    }
    E53_SC2_Init();

    LCD_Clear(BLACK);
    POINT_COLOR = GREEN;
    LCD_ShowString(10, 10, 200, 16, 24, "Welcome to BearPi");
    LCD_ShowString(30, 40, 200, 16, 24, "Manhole Demo");
    LCD_ShowString(10, 80, 200, 16, 16, "NCDP_IP:");
    LCD_ShowString(80, 80, 200, 16, 16, CONFIG_APP_SERVERIP);
    LCD_ShowString(10, 110, 200, 16, 16, "NCDP_PORT:");
    LCD_ShowString(100, 110, 200, 16, 16, CONFIG_APP_SERVERPORT);

    (void)osal_task_create("iot_main",task_main_entry,NULL,0x800,NULL,8);
    (void)osal_task_create("sensor",task_sensor_entry,NULL,0x800,NULL,9);
    shell_conn(0,NULL);
    ret = 0;
    return ret;
}
