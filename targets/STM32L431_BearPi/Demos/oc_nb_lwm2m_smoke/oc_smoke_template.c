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
 *  2019-05-14 17:21  zhangqianfu  The first version
 *
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <osal.h>
#include <oc_lwm2m_al.h>
#include <link_endian.h>
#include "hal_qspi_flash.h"
#include "E53_SF1.h"
#include "lcd.h"
#include "main.h"
#include <gpio.h>
#include <stm32l4xx_it.h>

#ifndef CONFIG_APP_SERVERIP
#define CONFIG_APP_SERVERIP       "986b5e9238.st1.iotda-coaps.cn-north-4.myhuaweicloud.com"
#endif
#ifndef CONFIG_APP_SERVERPORT
#define CONFIG_APP_SERVERPORT     "5683"
#endif

#ifndef CONFIG_APP_ENDPOIINT_ID
#define CONFIG_APP_ENDPOIINT_ID      "BearPi_0001"
#endif

typedef unsigned char int8u;
typedef char int8s;
typedef unsigned short int16u;
typedef short int16s;
typedef unsigned char int24u;
typedef char int24s;
typedef int int32s;
typedef char string;
typedef char array;
typedef char varstring;
typedef char variant;

#define cn_app_Smoke 0x2
#define cn_app_Smoke_Control_Beep 0x5
#define cn_app_response_Smoke_Control_Beep 0x6
#define cn_app_Threshold 0x9
#pragma pack(1)
typedef struct
{
    int8u messageId;
    int16u Smoke_Value;
} tag_app_Smoke;

typedef struct
{
    int8u messageId;
    int16u mid;
    int8u errcode;
    int8u Beep_State;
} tag_app_Response_Smoke_Control_Beep;

typedef struct
{
    int8u messageId;
    int16u mid;
    string Beep[3];
} tag_app_Smoke_Control_Beep;
typedef struct
{
    int8u messageId;
    int16u Threshold;
} tag_app_Threshold;
#pragma pack()


float ppm;


//if your command is very fast,please use a queue here--TODO
#define cn_app_rcv_buf_len 128
static int             s_rcv_buffer[cn_app_rcv_buf_len];
static int             s_rcv_datalen;
static osal_semp_t     s_rcv_sync;

uint16_t BeepCount = 0;
bool_t BeepStatus = OFF;
bool_t SmokeOverThreshold = false; 
bool_t BeepEnable = ON;
bool_t BeepCloudControl = true;
uint16_t SmokeThreshold = 20;
bool_t LogoFlag = true;
bool_t StatusFlag = true;


//use this function to push all the message to the buffer
static int app_msg_deal(void *usr_data, en_oc_lwm2m_msg_t type, void *data, int len)
{
    unsigned char *msg;
    msg = data;
    int ret = -1;

    if(len <= cn_app_rcv_buf_len)
    {
        if (msg[0] == 0xaa && msg[1] == 0xaa)
        {
            printf("OC respond message received! \n\r");
            return ret;
        }
        memcpy(s_rcv_buffer,msg,len);
        s_rcv_datalen = len;

        (void) osal_semp_post(s_rcv_sync);

        ret = 0;

    }
    return ret;
}


static int app_cmd_task_entry()
{
    int ret = -1;
    tag_app_Response_Smoke_Control_Beep Response_Smoke_Control_Beep;
    tag_app_Smoke_Control_Beep *Smoke_Control_Beep;
    int8_t msgid;
    
    while(1)
    {
        if(osal_semp_pend(s_rcv_sync,cn_osal_timeout_forever))
        {
            msgid = s_rcv_buffer[0] & 0x000000FF;
            switch (msgid)
            {
                 case cn_app_Smoke_Control_Beep:
                    Smoke_Control_Beep = (tag_app_Smoke_Control_Beep *)s_rcv_buffer;
                    printf("Smoke_Control_Beep:msgid:%d mid:%d", Smoke_Control_Beep->messageId, ntohs(Smoke_Control_Beep->mid));
                    /********** code area for cmd from IoT cloud  **********/
                    if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'N')
                    {
                        E53_SF1_Beep_Set(ON);
                        E53_SF1_LED_Set(ON);
                        BeepCloudControl = true;
                        BeepStatus = ON;
                        Response_Smoke_Control_Beep.messageId = cn_app_response_Smoke_Control_Beep;
                        Response_Smoke_Control_Beep.mid = Smoke_Control_Beep->mid;
                        Response_Smoke_Control_Beep.errcode = 0;
                        Response_Smoke_Control_Beep.Beep_State = 1;
                        oc_lwm2m_report((char *)&Response_Smoke_Control_Beep,sizeof(Response_Smoke_Control_Beep),1000);    ///< report cmd reply message
                    }
                    if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'F' && Smoke_Control_Beep->Beep[2] == 'F')
                    {
                        E53_SF1_Beep_Set(OFF);
                        E53_SF1_LED_Set(OFF);
                        BeepStatus = OFF;
                        BeepCloudControl = false;
                        Response_Smoke_Control_Beep.messageId = cn_app_response_Smoke_Control_Beep;
                        Response_Smoke_Control_Beep.mid = Smoke_Control_Beep->mid;
                        Response_Smoke_Control_Beep.errcode = 0;
                        Response_Smoke_Control_Beep.Beep_State = 0;
                        oc_lwm2m_report((char *)&Response_Smoke_Control_Beep,sizeof(Response_Smoke_Control_Beep),1000);    ///< report cmd reply message
                    }
                    /********** code area end  **********/
                    break;
                default:
                    break;
            }
        }
    }

    return ret;
}

static int app_cmd_task_entry_temp()
{
    int ret = -1;
    tag_app_Response_Smoke_Control_Beep Response_Smoke_Control_Beep;
    tag_app_Smoke_Control_Beep *Smoke_Control_Beep;
    int8_t msgid;
    
    while(1)
    {
        if(osal_semp_pend(s_rcv_sync,cn_osal_timeout_forever))
        {
            msgid = s_rcv_buffer[0] & 0x000000FF;
            switch (msgid)
            {
                 case cn_app_Smoke_Control_Beep:
                    Smoke_Control_Beep = (tag_app_Smoke_Control_Beep *)s_rcv_buffer;
                    printf("Smoke_Control_Beep:msgid:%d mid:%d", Smoke_Control_Beep->messageId, ntohs(Smoke_Control_Beep->mid));
                    /********** code area for cmd from IoT cloud  **********/
                    if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'N')
                    {
                        E53_SF1_Beep_Set(ON);
                        E53_SF1_LED_Set(ON);
                        BeepCloudControl = true;
                        BeepStatus = ON;
                        if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'N')
                        {
                            E53_SF1_Beep_Set(ON);
                            E53_SF1_LED_Set(ON);
                            BeepCloudControl = true;
                            BeepStatus = ON;
                            Response_Smoke_Control_Beep.messageId = cn_app_response_Smoke_Control_Beep;
                            Response_Smoke_Control_Beep.mid = Smoke_Control_Beep->mid;
                            Response_Smoke_Control_Beep.errcode = 0;
                            Response_Smoke_Control_Beep.Beep_State = 1;
                        }
                        oc_lwm2m_report((char *)&Response_Smoke_Control_Beep,sizeof(Response_Smoke_Control_Beep),1000);    ///< report cmd reply message
                    }
                    if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'F' && Smoke_Control_Beep->Beep[2] == 'F')
                    {
                        E53_SF1_Beep_Set(OFF);
                        E53_SF1_LED_Set(OFF);
                        BeepStatus = OFF;
                        BeepCloudControl = false;
                        Response_Smoke_Control_Beep.messageId = cn_app_response_Smoke_Control_Beep;
                        Response_Smoke_Control_Beep.mid = Smoke_Control_Beep->mid;
                        Response_Smoke_Control_Beep.errcode = 0;
                        Response_Smoke_Control_Beep.Beep_State = 0;
                        if (Smoke_Control_Beep->Beep[0] == 'O' && Smoke_Control_Beep->Beep[1] == 'F' && Smoke_Control_Beep->Beep[2] == 'F')
                        {
                            E53_SF1_Beep_Set(OFF);
                            E53_SF1_LED_Set(OFF);
                            BeepStatus = OFF;
                            BeepCloudControl = false;
                            Response_Smoke_Control_Beep.messageId = cn_app_response_Smoke_Control_Beep;
                            Response_Smoke_Control_Beep.mid = Smoke_Control_Beep->mid;
                            Response_Smoke_Control_Beep.errcode = 0;
                            Response_Smoke_Control_Beep.Beep_State = 0;
                        }
                        oc_lwm2m_report((char *)&Response_Smoke_Control_Beep,sizeof(Response_Smoke_Control_Beep),1000);    ///< report cmd reply message
                    }
                    /********** code area end  **********/
                    break;
                default:
                    break;
            }
        }
    }

    return ret;
}


static int app_report_task_entry()
{
    int ret = -1;

    oc_config_param_t      oc_param;
    tag_app_Smoke Smoke;
    tag_app_Threshold Thres;
    (void) memset(&oc_param,0,sizeof(oc_param));

    oc_param.app_server.address = CONFIG_APP_SERVERIP;
    oc_param.app_server.port = CONFIG_APP_SERVERPORT;
    oc_param.app_server.ep_id = CONFIG_APP_ENDPOIINT_ID;
    oc_param.boot_mode = en_oc_boot_strap_mode_factory;
    oc_param.rcv_func = app_msg_deal;

    ret = oc_lwm2m_config(&oc_param);
    if (0 != ret)
    {
        return ret;
    }

    while(1) //--TODO ,you could add your own code here
    {
        Smoke.messageId = cn_app_Smoke;
        Smoke.Smoke_Value = htons((int)ppm);
        oc_lwm2m_report( (char *)&Smoke, sizeof(Smoke), 1000);
        Thres.messageId=cn_app_Threshold;
        Thres.Threshold=htons(SmokeThreshold);
        oc_lwm2m_report( (char *)&Thres, sizeof(Thres),1000);
        osal_task_sleep(2*1000);
    }
    return ret;
}

static void key_scan()
{
    while(1)
    {
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET)
    {
        
        BeepCount = 0;
        hal_spi_flash_erase_write(&BeepCount,sizeof(BeepCount),0);
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
        while (HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET);
    }

    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET)
    {
        BeepEnable = !BeepEnable;
        if(BeepEnable)
        {
            LogoFlag = 1;
        }
        
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
        while (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET);
    }
        osal_task_sleep(2*50);
    }
}

void Information_show(void)
{
    lcd_show_chinese(75,10,1,32,3,1); //"姓名"
    lcd_show_chinese(10,50,2,32,4,1);//"烟雾浓度"
    //LCD_ShowString(10, 85, 180, 16, 32, "Threshold:");
    lcd_show_chinese(10,85,0,32,2,1);//"烟雾浓度"
    LCD_ShowString(10, 120, 190, 16, 32, "Beep Enable :");
    LCD_ShowString(10, 160, 190, 16, 32, "Beep Count:");
    LCD_ShowString(10, 200, 190, 16, 32, "Beep State:");   
}

static int app_collect_task_entry()
{
    osal_task_sleep(1*1000);
    LCD_Init();
    LCD_Clear(BLACK);
    Information_show();
    
    while (1)
    {
        E53_SF1_Read_Data(&ppm);
       
        LCD_ShowNum(150, 50,(int)ppm, 5, 32);//烟雾浓度ppm
        LCD_ShowNum(175, 85, SmokeThreshold, 3, 32); 
        LCD_ShowNum(200, 120, BeepEnable, 1, 32);
        LCD_ShowNum(185, 160, BeepCount, 3, 32);
        LCD_ShowNum(185, 200, BeepStatus, 1, 32);
        if(BeepEnable==1)
        {
            if((int)ppm>SmokeThreshold && LogoFlag==1)
            {
                LogoFlag = 0;
                SmokeOverThreshold = 1; ///
                BeepCount++;
                hal_spi_flash_erase_write(&BeepCount,sizeof(BeepCount),0);
                LCD_Clear(WHITE);
                LCD_Show_Image(0,0,240,236);
                osal_task_sleep(2*1000);

                LCD_Clear(BLACK);
                Information_show();
            }
            if(!((int)ppm>SmokeThreshold))
            {
                LogoFlag = 1;
                SmokeOverThreshold = 0;
            }
             
        }

        osal_task_sleep(2*50);
    }

    return 0;
}

static int BeepControl(void)
{
    
    while (1)
    {
        
        if(BeepEnable&&SmokeOverThreshold&&BeepCloudControl)
        {
            E53_SF1_Beep_Set(ON);
            E53_SF1_LED_Set(ON);
            BeepStatus = ON;
            
        }
        else 
        {

            E53_SF1_Beep_Set(OFF);
            E53_SF1_LED_Set(OFF);
            BeepStatus = OFF;
            
        }

        osal_task_sleep(1*250);
          
    }
    return 0;
}


#include <stimer.h>

int standard_app_demo_main()
{
    //LCD_Clear(BLACK);
    //POINT_COLOR = GREEN;
    E53_SF1_Init();
    MX_GPIO_Init();
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    hal_spi_flash_read(&BeepCount,sizeof(BeepCount),0);
    osal_semp_create(&s_rcv_sync,1,0);

    osal_task_create("app_collect",app_collect_task_entry,NULL,0x1000,NULL,3);
    osal_task_create("app_report",app_report_task_entry,NULL,0x1000,NULL,2);
    osal_task_create("app_command",app_cmd_task_entry,NULL,0x1000,NULL,3);
    osal_task_create("app_keyscan",key_scan,NULL,0x1000,NULL,3);
    osal_task_create("app_beepcontrol",BeepControl,NULL,0x1000,NULL,3);

    return 0;
}





