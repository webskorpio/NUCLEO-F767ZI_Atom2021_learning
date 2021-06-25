/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
uint32_t tt = 0;
uint8_t buttPer = 0;
extern uint8_t but;
extern struct button but_a;
extern struct button but_b;
extern struct button but_c;
extern struct button but_d;
extern struct button but_ctr;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	buttPer++;
	//Обработка кнопок
	tt = HAL_GetTick();
	if(buttPer >= 20){

		//Кнопка А
	if(HAL_GPIO_ReadPin(But_Left_GPIO_Port, But_Left_Pin) == 0 && but_a.short_state == 0 && (tt - but_a.delay) > 50){
		but_a.short_state =1;	but_a.delay = tt;
	}
	if(HAL_GPIO_ReadPin(But_Left_GPIO_Port, But_Left_Pin) == 1 && but_a.short_state == 1 && (tt - but_a.delay) >50){
		but = 1;	but_a.short_state = 0;	but_a.delay = 0;
	}

	//Кнопка B
	if(HAL_GPIO_ReadPin(But_Up_GPIO_Port, But_Up_Pin) == 0 && but_b.short_state == 0 && (tt - but_b.delay) > 50){
		but_b.short_state =1;	but_b.delay = tt;
	}
	if(HAL_GPIO_ReadPin(But_Up_GPIO_Port, But_Up_Pin) == 1 && but_b.short_state == 1 && (tt - but_b.delay) >50){
		but = 2;	but_b.short_state = 0;	but_b.delay = 0;
	}


	//Кнопка C
	if(HAL_GPIO_ReadPin(But_Down_GPIO_Port, But_Down_Pin) == 0 && but_c.short_state == 0 && (tt - but_c.delay) > 50){
		but_c.short_state =1;	but_c.delay = tt;
	}
	if(HAL_GPIO_ReadPin(But_Down_GPIO_Port, But_Down_Pin) == 1 && but_c.short_state == 1 && (tt - but_c.delay) >50){
		but = 3;	but_c.short_state = 0;	but_c.delay = 0;
	}


	//Кнопка D
	if(HAL_GPIO_ReadPin(But_Right_GPIO_Port, But_Right_Pin) == 0 && but_d.short_state == 0 && (tt - but_d.delay) > 50){
		but_d.short_state =1;	but_d.delay = tt;
	}
	if(HAL_GPIO_ReadPin(But_Right_GPIO_Port, But_Right_Pin) == 1 && but_d.short_state == 1 && (tt - but_d.delay) >50){
		but = 4;	but_d.short_state = 0;	but_d.delay = 0;
	}

	//Кнопка CTR
	if(HAL_GPIO_ReadPin(But_Enter_GPIO_Port, But_Enter_Pin) == 0 && but_ctr.short_state == 0 && (tt - but_ctr.delay) > 50){
		but_ctr.short_state =1;	but_ctr.delay = tt;
	}
	if(HAL_GPIO_ReadPin(But_Enter_GPIO_Port, But_Enter_Pin) == 1 && but_ctr.short_state == 1 && (tt - but_ctr.delay) >50){
		but = 5;	but_ctr.short_state = 0;	but_ctr.delay = 0;
	}

	buttPer = 0;
	}

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
