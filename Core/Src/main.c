/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "fonts.h"
#include "testimg.h"
#include "Adafruit_AMG88xx.h"
#include "bilinear.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RESIZE_X 112
#define RESIZE_Y 112
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void HC595_send(uint8_t ld);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct button but_a;
struct button but_b;
struct button but_c;
struct button but_d;
struct button but_ctr;

uint8_t screen = 0;
uint8_t menu = 0;
uint8_t test = 0;
uint8_t  but = 0;
uint8_t led = 0;
uint8_t  tLed = 0;
float pixels[64];
float tMin;
float tMax;
uint16_t kurX = 2;
uint16_t kurY = 2;
int16_t pixelsRaw[64];

char ch[64] = {0};
char menuEnter[1] = {'>'};


int16_t pixelsRawResize[RESIZE_X*RESIZE_Y];
int16_t pixelsTemp[160*32];
uint8_t pressBut[6][5]= {
		"     ",
		"Left ",
		"Up   ",
		"Dwon ",
		"Right",
		"Enter"
};

uint8_t lostBut = 0;

uint8_t tempKur[8][8] = {
		{63,62,61,60,59,58,57,56},
		{55,54,53,52,51,50,49,48},
		{47,46,45,44,43,42,41,40},
		{39,38,37,36,35,34,33,32},
		{31,30,29,28,27,26,25,24},
		{23,22,21,20,19,18,17,16},
		{15,14,13,12,11,10,9 ,8 },
		{7 ,6 ,5 ,4 ,3 ,2 ,1 ,0 }
};


uint8_t led_595[8] = {
						0b00000000,
						0b00000001,
						0b00000011,
						0b00000111,
						0b00001111,
						0b00011111,
						0b00111111,
						0b01111111
};

const uint16_t scaleColors[] = {0x480F,
                               0x400F,  0x4010,  0x3810,  0x3810,  0x3010,
                               0x2810,  0x2810,  0x2010,  0x2010,  0x1810,
                               0x1811,  0x1011,  0x0811,  0x0811,  0x0011,
                               0x0011,  0x0031,  0x0051,  0x0072,  0x00B2,
                               0x00D2,  0x00F2,  0x0132,  0x0152,  0x0192,
                               0x01B2,  0x01F3,  0x0213,  0x0253,  0x0273,
                               0x02B3,  0x02D3,  0x0313,  0x0333,  0x0373,
                               0x03B4,  0x03D4,  0x0414,  0x0454,  0x0474,
                               0x04B4,  0x04F4,  0x0534,  0x0554,  0x0574,
                               0x0573,  0x0573,  0x0572,  0x0571,  0x0591,
                               0x0590,  0x058F,  0x058E,  0x05AE,  0x05AD,
                               0x05AC,  0x05AB,  0x05CB,  0x05CA,  0x05C9,
                               0x05C8,  0x05E8,  0x05E7,  0x05E6,  0x05E5,
                               0x0604,  0x0604,  0x0603,  0x0602,  0x0621,
                               0x0620,  0x0620,  0x0E20,  0x0E40,  0x1640,
                               0x1E40,  0x2640,  0x2E60,  0x3660,  0x3E60,
                               0x4660,  0x4E60,  0x5680,  0x5E80,  0x6680,
                               0x6E80,  0x76A0,  0x7EA0,  0x86A0,  0x8EA0,
                               0x96C0,  0x9EC0,  0xA6C0,  0xAEC0,  0xB6E0,
                               0xBEE0,  0xC6E0,  0xCEE0,  0xD700,  0xDEE0,
                               0xDEA0,  0xDE80,  0xE640,  0xE600,  0xE5C0,
                               0xE580,  0xE540,  0xE500,  0xE4C0,  0xE480,
                               0xEC40,  0xEC00,  0xEBC0,  0xEB80,  0xEB40,
                               0xEB00,  0xEAC0,  0xEA80,  0xEA40,  0xF200,
                               0xF1C0,  0xF180,  0xF140,  0xF0E0,  0xF0A0,
                               0xF060, 0xF040, 0xF020, 0xF800,
                             };

const uint16_t camColors[] = {0x480F,
                              0x400F, 0x400F, 0x400F, 0x4010, 0x3810, 0x3810, 0x3810, 0x3810, 0x3010, 0x3010,
                              0x3010, 0x2810, 0x2810, 0x2810, 0x2810, 0x2010, 0x2010, 0x2010, 0x1810, 0x1810,
                              0x1811, 0x1811, 0x1011, 0x1011, 0x1011, 0x0811, 0x0811, 0x0811, 0x0011, 0x0011,
                              0x0011, 0x0011, 0x0011, 0x0031, 0x0031, 0x0051, 0x0072, 0x0072, 0x0092, 0x00B2,
                              0x00B2, 0x00D2, 0x00F2, 0x00F2, 0x0112, 0x0132, 0x0152, 0x0152, 0x0172, 0x0192,
                              0x0192, 0x01B2, 0x01D2, 0x01F3, 0x01F3, 0x0213, 0x0233, 0x0253, 0x0253, 0x0273,
                              0x0293, 0x02B3, 0x02D3, 0x02D3, 0x02F3, 0x0313, 0x0333, 0x0333, 0x0353, 0x0373,
                              0x0394, 0x03B4, 0x03D4, 0x03D4, 0x03F4, 0x0414, 0x0434, 0x0454, 0x0474, 0x0474,
                              0x0494, 0x04B4, 0x04D4, 0x04F4, 0x0514, 0x0534, 0x0534, 0x0554, 0x0554, 0x0574,
                              0x0574, 0x0573, 0x0573, 0x0573, 0x0572, 0x0572, 0x0572, 0x0571, 0x0591, 0x0591,
                              0x0590, 0x0590, 0x058F, 0x058F, 0x058F, 0x058E, 0x05AE, 0x05AE, 0x05AD, 0x05AD,
                              0x05AD, 0x05AC, 0x05AC, 0x05AB, 0x05CB, 0x05CB, 0x05CA, 0x05CA, 0x05CA, 0x05C9,
                              0x05C9, 0x05C8, 0x05E8, 0x05E8, 0x05E7, 0x05E7, 0x05E6, 0x05E6, 0x05E6, 0x05E5,
                              0x05E5, 0x0604, 0x0604, 0x0604, 0x0603, 0x0603, 0x0602, 0x0602, 0x0601, 0x0621,
                              0x0621, 0x0620, 0x0620, 0x0620, 0x0620, 0x0E20, 0x0E20, 0x0E40, 0x1640, 0x1640,
                              0x1E40, 0x1E40, 0x2640, 0x2640, 0x2E40, 0x2E60, 0x3660, 0x3660, 0x3E60, 0x3E60,
                              0x3E60, 0x4660, 0x4660, 0x4E60, 0x4E80, 0x5680, 0x5680, 0x5E80, 0x5E80, 0x6680,
                              0x6680, 0x6E80, 0x6EA0, 0x76A0, 0x76A0, 0x7EA0, 0x7EA0, 0x86A0, 0x86A0, 0x8EA0,
                              0x8EC0, 0x96C0, 0x96C0, 0x9EC0, 0x9EC0, 0xA6C0, 0xAEC0, 0xAEC0, 0xB6E0, 0xB6E0,
                              0xBEE0, 0xBEE0, 0xC6E0, 0xC6E0, 0xCEE0, 0xCEE0, 0xD6E0, 0xD700, 0xDF00, 0xDEE0,
                              0xDEC0, 0xDEA0, 0xDE80, 0xDE80, 0xE660, 0xE640, 0xE620, 0xE600, 0xE5E0, 0xE5C0,
                              0xE5A0, 0xE580, 0xE560, 0xE540, 0xE520, 0xE500, 0xE4E0, 0xE4C0, 0xE4A0, 0xE480,
                              0xE460, 0xEC40, 0xEC20, 0xEC00, 0xEBE0, 0xEBC0, 0xEBA0, 0xEB80, 0xEB60, 0xEB40,
                              0xEB20, 0xEB00, 0xEAE0, 0xEAC0, 0xEAA0, 0xEA80, 0xEA60, 0xEA40, 0xF220, 0xF200,
                              0xF1E0, 0xF1C0, 0xF1A0, 0xF180, 0xF160, 0xF140, 0xF100, 0xF0E0, 0xF0C0, 0xF0A0,
                              0xF080, 0xF060, 0xF040, 0xF020, 0xF800,
                             };
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
int i;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  // Инициализация тепловизора
  amg88xxInit();
  // Инициализация LCD
  ST7735_Init();


  if(HAL_GPIO_ReadPin(But_Enter_GPIO_Port, But_Enter_Pin) == 0) test = 1;

  if(screen == 0 && test == 0){
  HAL_GPIO_WritePin(LED_Alert_GPIO_Port, LED_Alert_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_TEST1_GPIO_Port, LED_TEST1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_TEST2_GPIO_Port, LED_TEST2_Pin, GPIO_PIN_RESET);
  }
 // HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
 // HAL_Delay(5);
 // HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
  //ST7735_Unselect();



  // Вывод стартового изображения
  ST7735_FillScreen(ST7735_WHITE);
  ST7735_DrawImage(0, 19, 128, 116, (uint16_t*)lcd_image_atom);
  HAL_Delay(1500);

  ST7735_DrawImage(0, 17, 128, 128, (uint16_t*)lcd_image_electronica);
  HAL_Delay(1500);

  ST7735_FillScreen(ST7735_BLACK);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(screen == 0 && test == 0){
	  // Считывание массива тепловизора
	  	readPixelsRaw(pixelsRaw);
	  // Зеркалируем массив для дисплея
		uint8_t revers;
		for(uint8_t f = 0; f < 32; f++){
			revers = pixelsRaw[f];
			pixelsRaw[f] = pixelsRaw[63-f];
			pixelsRaw[63-f] = revers;
		}

		for( i = 0; i<64; i++)
			pixelsRaw[i] = (pixelsRaw[i]-80)<<2;
		// Преобразуем значения тепловизора из 8x8 в 128x128
		resizeBilinearGrey(pixelsRaw, pixelsRawResize, 8, 8, RESIZE_X, RESIZE_Y);

		for( i = 0; i< RESIZE_X*RESIZE_Y; i++){
			if(pixelsRawResize[i] < 0)
				pixelsRawResize[i] = 0;
			if(pixelsRawResize[i] > 255)
				pixelsRawResize[i] = 255;

			pixelsRawResize[i] = camColors[pixelsRawResize[i]];
		}

		  // Вывод цветовой шкалы
		for(int i = 0; i < 112; i++){
		  ST7735_FillRectangle(114, i, 14, 1, scaleColors[i]);
		  ST7735_DrawPixel(112, i, ST7735_WHITE);
		  ST7735_DrawPixel(113, i, ST7735_WHITE);
		}
		ST7735_DrawImage(0, 0, RESIZE_X, RESIZE_Y, (uint16_t*)pixelsRawResize);


		for(uint8_t i = 0; i<= 128; i++){
			ST7735_DrawPixel(i, 112, ST7735_WHITE);
		}


		readPixels(pixels,64);

		tMax = pixels[0];
		tMin = pixels[0];



		for(uint8_t i = 0; i < 64; i++){
			if(pixels[i] < tMin) tMin = pixels[i];
			if(pixels[i] > tMax) tMax = pixels[i];
		}

		tLed = ((uint8_t)tMax - (uint8_t)tMin);
		led = ((uint8_t)pixels[tempKur[kurY-1][kurX-1]] - (uint8_t)tMin) / (tLed/7);
		if(led <= 0)led =1;
		if(led >= 8)led =7;
		sprintf(ch, "%i\r\n", led);
		HAL_UART_Transmit(&huart3, ch, 9, 100);

		for(uint8_t i = 0; i<= 14; i++){
			ST7735_DrawPixel((kurX*14)-14+i, (kurY*14)-7, ST7735_WHITE);
			ST7735_DrawPixel((kurX*14)-7, (kurY*14)-14+i, ST7735_WHITE);
		}

		sprintf(ch, "%.1fC", pixels[tempKur[kurY-1][kurX-1]]);
		ST7735_WriteString(22, 118, ch, Font_16x26, ST7735_WHITE, ST7735_BLACK);

		for(uint8_t i = 0; i<= 128; i++){
			ST7735_DrawPixel(i, 144, ST7735_WHITE);
		}
		sprintf(ch, "Enter to Settings");
		ST7735_WriteString(2, 150, ch, Font_7x10, ST7735_WHITE, ST7735_BLACK);
//
//		sprintf(ch, "Min=%.1fC", tMin);
//		ST7735_WriteString(0, 150, ch, Font_7x10, ST7735_WHITE, ST7735_BLACK);
//		sprintf(ch, "Max=%.1fC", tMax);
//		ST7735_WriteString(0, 160, ch, Font_7x10, ST7735_WHITE, ST7735_BLACK);






		//HAL_UART_Transmit(&huart3, ch, sizeof(ch), 100);
		if(but != 0 ){
			if(but == 1){
				kurX--;
				if(kurX <= 1) kurX = 1;
				but = 0;
			}

			if(but == 2){
				kurY--;
				if(kurY <=1) kurY = 1;
				but = 0;
			}
			if(but == 3){
				kurY++;
				if(kurY >=8) kurY = 8;
				but = 0;
			}
			if(but == 4){
				kurX++;
				if(kurX >= 8) kurX = 8;
				but = 0;
			}
			if(but == 5){
				screen = 1;
				ST7735_FillScreen(ST7735_BLACK);
				but = 0;
			}
		}
	}

		if(test ==1 ){

			if(but != 0 ){
				lostBut = but;
				if(but == 1){
					//menu;
					but = 0;
				}

				if(but == 2){
					if(menu != 0){ menu--; }else{ menu = 4; }
					but = 0;
				}
				if(but == 3){
					if(menu != 4){ menu++; }else{ menu= 0; }
					but = 0;				}
				if(but == 4){
					//kurX++;
					//if(kurX >= 8) kurX = 8;
					but = 0;
				}
				if(but == 5){
					if(menu == 0){
						HAL_GPIO_TogglePin(LED_Alert_GPIO_Port, LED_Alert_Pin);
					}
					if(menu == 1){
						if(led == 0){ led = 7; }else{ led = 0;}
					}
					if(menu == 2){
						HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
					}
					if(menu == 4){
						test = 0;
					}
					but = 0;
				}





			sprintf(ch, "Test system");
			ST7735_WriteString(2, 0, ch, Font_11x18, ST7735_WHITE, ST7735_BLACK);
			if(HAL_GPIO_ReadPin(LED_Alert_GPIO_Port, LED_Alert_Pin)){
				sprintf(ch, " Led Alert: OFF ");
			}else{
				sprintf(ch, " Led Alert: ON  ");
			}
			ST7735_WriteString(2, 20, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);
			if(led == 0){
				sprintf(ch, " Led sens: OFF ");
			}else if(led > 0 && led < 8){
				sprintf(ch, " Led sens: ON  ");
			}
			ST7735_WriteString(2, 30, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);
			if(HAL_GPIO_ReadPin(Buzzer_GPIO_Port, Buzzer_Pin)){
				sprintf(ch, " Buzzer: ON  ");
			}else{
				sprintf(ch, " Buzzer: OFF  ");
			}
			ST7735_WriteString(2, 40, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);

			sprintf(ch, " Button:%c%c%c%c%c", pressBut[lostBut][0],pressBut[lostBut][1],pressBut[lostBut][2],pressBut[lostBut][3],pressBut[lostBut][4]);
			ST7735_WriteString(2, 50, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);
			sprintf(ch, " EXIT TEST");
			ST7735_WriteString(2, 60, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);

			sprintf(ch, "menu - %i", menu);
			ST7735_WriteString(15, 80, ch, Font_7x10, ST7735_BLUE, ST7735_BLACK);

			if(menu == 0 ){
				ST7735_WriteString(2, 20, menuEnter, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			}else if(menu == 1){
				ST7735_WriteString(2, 30, menuEnter, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			}else if(menu == 2){
				ST7735_WriteString(2, 40, menuEnter, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			}else if(menu == 3){
				ST7735_WriteString(2, 50, menuEnter, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			}else if(menu == 4){
				ST7735_WriteString(2, 60, menuEnter, Font_7x10, ST7735_WHITE, ST7735_BLACK);
			}


			}
		}

		if(screen == 1){

			sprintf(ch, "Settings");
			ST7735_WriteString(20, 0, ch, Font_11x18, ST7735_YELLOW, ST7735_BLACK);

		}

		HC595_send(led);
		HAL_Delay(100);


	    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20303E5D;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LCD_RS_Pin|HC595_DS_Pin|LED_TEST1_Pin|LED_Alert_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, HC595_SR_Pin|HC595_CLK_Pin|LED_TEST2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LCD_CS_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : USER_Btn_Pin PC8 */
  GPIO_InitStruct.Pin = USER_Btn_Pin|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : But_Down_Pin But_Up_Pin But_Left_Pin */
  GPIO_InitStruct.Pin = But_Down_Pin|But_Up_Pin|But_Left_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : But_Enter_Pin But_Right_Pin */
  GPIO_InitStruct.Pin = But_Enter_Pin|But_Right_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Buzzer_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RS_Pin HC595_DS_Pin LED_TEST1_Pin */
  GPIO_InitStruct.Pin = LCD_RS_Pin|HC595_DS_Pin|LED_TEST1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Alert_Pin */
  GPIO_InitStruct.Pin = LED_Alert_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_Alert_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : HC595_SR_Pin HC595_CLK_Pin LED_TEST2_Pin */
  GPIO_InitStruct.Pin = HC595_SR_Pin|HC595_CLK_Pin|LED_TEST2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_TXD1_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HC595_send(uint8_t ld)
{
	uint8_t mask;
	for (mask=0x80; mask; mask = mask >> 1)
	{
		if (led_595[ld] & mask)
		{
			HAL_GPIO_WritePin(HC595_DS_GPIO_Port, HC595_DS_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(HC595_DS_GPIO_Port, HC595_DS_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(HC595_CLK_GPIO_Port, HC595_CLK_Pin, GPIO_PIN_SET);
		//HAL_Delay(1);
		HAL_GPIO_WritePin(HC595_CLK_GPIO_Port, HC595_CLK_Pin, GPIO_PIN_RESET);
	}
		HAL_GPIO_WritePin(HC595_SR_GPIO_Port, HC595_SR_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(HC595_SR_GPIO_Port, HC595_SR_Pin, GPIO_PIN_RESET);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
