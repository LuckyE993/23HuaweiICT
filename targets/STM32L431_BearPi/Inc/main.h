/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes LiteOS------------------------------------------------------------------*/

#include "los_base.h"
#include "los_config.h"
#include "los_sys.h"
#include "los_typedef.h"
#include "los_event.ph"
#include "los_event.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define E53_GPIO1_Pin GPIO_PIN_0
#define E53_GPIO1_GPIO_Port GPIOA

#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI2_IRQn

#define USER_GPIO1_Pin GPIO_PIN_12
#define USER_GPIO1_GPIO_Port GPIOB
#define WAN_RI_Pin GPIO_PIN_14
#define WAN_RI_GPIO_Port GPIOB
#define LCD_POWER_Pin GPIO_PIN_15
#define LCD_POWER_GPIO_Port GPIOB
#define LCD_WR_RS_Pin GPIO_PIN_6
#define LCD_WR_RS_GPIO_Port GPIOC
#define LCD_RESET_Pin GPIO_PIN_7
#define LCD_RESET_GPIO_Port GPIOC
#define E53_GPIO5_Pin GPIO_PIN_9
#define E53_GPIO5_GPIO_Port GPIOC
#define E53_GPIO4_Pin GPIO_PIN_8
#define E53_GPIO4_GPIO_Port GPIOA
#define USER_GPIO2_Pin GPIO_PIN_11
#define USER_GPIO2_GPIO_Port GPIOA

#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI3_IRQn

#define E53_GPIO3_Pin GPIO_PIN_8
#define E53_GPIO3_GPIO_Port GPIOB
#define E53_GPIO2_Pin GPIO_PIN_9
#define E53_GPIO2_GPIO_Port GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
