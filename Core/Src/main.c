/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RX_BUFF_SIZE 128
#define TX_BUFF_SIZE 64

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

__IO uint8_t KEY0_PRESSED = 0;
__IO uint8_t KEY1_PRESSED = 0;
__IO uint8_t KEY2_PRESSED = 0;
__IO uint8_t KEY3_PRESSED = 0;
uint8_t ENROLL_ID = 1;
uint8_t DELETE_ID = 1;

uint8_t TX_BUFFER[TX_BUFF_SIZE] = { 0 };
__IO uint8_t RX_BUFFER[RX_BUFF_SIZE] = { 0 };

int menu_main();
int menu_enroll();
int menu_verify();
int menu_delete();
int (*menu)() = &menu_main; 

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  // 使能串口IDLE中断
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER, RX_BUFF_SIZE);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim6);
	OLED_Init();                           		 //OLED初始
	OLED_Clear();                        			 //清屏

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
			if (KEY1_PRESSED == 1)
			{
					menu = menu_main;
			}
			if (KEY3_PRESSED == 1)
			{
					menu = menu_enroll;
			}
			if (KEY2_PRESSED == 1)
			{
					menu = menu_verify;
			}
			if (KEY0_PRESSED == 1)
			{
					menu = menu_delete;
			}
			if (menu != NULL)
			{
					KEY0_PRESSED = 0;
					KEY1_PRESSED = 0;
					KEY2_PRESSED = 0;
					KEY3_PRESSED = 0;
					menu();
			}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// 计算 BCC 异或校验码的函数
uint8_t calculateBCC(uint8_t *data, int length)
{
    uint8_t bcc = 0;
    for (int i = 0; i < length; i++)
    {
        // 对每个字节进行异或操作
        bcc ^= data[i];
    }
    return bcc;
}

int menu_main()
{
    menu = NULL;
    OLED_Clear();
    OLED_ShowCHinese(24, 0, 8, 0);
    OLED_ShowCHinese(40, 0, 9, 0);
    OLED_ShowCHinese(56, 0, 10, 0);
    OLED_ShowCHinese(72, 0, 11, 0);
    OLED_ShowCHinese(88, 0, 12, 0);

    OLED_ShowCHinese(32, 2, 0, 0);
    OLED_ShowCHinese(48, 2, 1, 0);
    OLED_ShowCHinese(64, 2, 6, 0);
    OLED_ShowCHinese(80, 2, 7, 0);

    OLED_ShowCHinese(32, 4, 4, 0);
    OLED_ShowCHinese(48, 4, 5, 0);
    OLED_ShowCHinese(64, 4, 6, 0);
    OLED_ShowCHinese(80, 4, 7, 0);

    OLED_ShowCHinese(32, 6, 2, 0);
    OLED_ShowCHinese(48, 6, 3, 0);
    OLED_ShowCHinese(64, 6, 6, 0);
    OLED_ShowCHinese(80, 6, 7, 0);
    return 0;
}

int menu_enroll()
{
    OLED_Clear();
    OLED_ShowCHinese(0, 0, 13, 0);
    OLED_ShowCHinese(16, 0, 14, 0);
    OLED_ShowCHinese(32, 0, 15, 0);
    OLED_ShowCHinese(48, 0, 16, 0);

    OLED_ShowCHinese(64, 0, 0, 0);
    OLED_ShowCHinese(80, 0, 1, 0);
    OLED_ShowCHinese(96, 0, 6, 0);
    OLED_ShowCHinese(112, 0, 7, 0);

    OLED_ShowCHinese(32, 2, 17, 0);
    OLED_ShowCHinese(48, 2, 18, 0);
    OLED_ShowCHinese(64, 2, 19, 0);
    OLED_ShowNum(80, 2, ENROLL_ID, 2, 16, 0);
    HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_SET);

    while (1)
    {
        if (KEY3_PRESSED == 1)
        {
            KEY3_PRESSED = 0;

            OLED_Clear();
            OLED_ShowCHinese(40, 0, 0, 0);
            OLED_ShowCHinese(56, 0, 1, 0);
            OLED_ShowCHinese(72, 0, 24, 0);

            OLED_ShowCHinese(24, 2, 8, 0);
            OLED_ShowCHinese(40, 2, 20, 0);
            OLED_ShowCHinese(56, 2, 21, 0);
            OLED_ShowCHinese(72, 2, 22, 0);
            OLED_ShowCHinese(88, 2, 23, 0);

            uint8_t values[] = { 0xef, 0xaa, 0x1d, 0x00, 0x23, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x3e };

            // 赋值操作
            for (int i = 0; i < 41; i++)
            {
                TX_BUFFER[i] = values[i];
            }

            TX_BUFFER[6] = ENROLL_ID;

            // 调用函数计算 BCC 校验码
            TX_BUFFER[40] = calculateBCC(&TX_BUFFER[2], 38);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *)TX_BUFFER, 41);
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER, RX_BUFF_SIZE);
            while (1)
            {
                if ((RX_BUFFER[2] == 0X00) && (RX_BUFFER[5] == 0X1D)&& (RX_BUFFER[6] == 0X0A))
                {
                    // 播放人脸已录入语音
										HAL_GPIO_WritePin(IO3_GPIO_Port, IO3_Pin, GPIO_PIN_RESET);
										__HAL_TIM_SET_COUNTER(&htim6, 0);
										HAL_TIM_Base_Start_IT(&htim6);
									
										// 关闭FM225的电源
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
                    OLED_Clear();
										OLED_ShowCHinese(24, 0, 6, 0); // OLED显示’人‘
                    OLED_ShowCHinese(40, 0, 7, 0);	// OLED显示’脸‘
                    OLED_ShowCHinese(56, 0, 29, 0);	// OLED显示’已‘
                    OLED_ShowCHinese(72, 0, 30, 0);	// OLED显示’录‘
                    OLED_ShowCHinese(88, 0, 31, 0);// OLED显示’入‘

                    while (1)
                    {
                        if (KEY3_PRESSED == 1)
                        {
                            KEY3_PRESSED = 0;
                        }
                        if (KEY2_PRESSED == 1)
                        {
                            KEY2_PRESSED = 0;
                        }

                        if (KEY0_PRESSED == 1)
                        {
                            KEY0_PRESSED = 0;
                        }
                        if (KEY1_PRESSED == 1)
                        {
                            KEY1_PRESSED = 0;
														HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
														menu = menu_main;
														return 0;
                        }
                    }
                }
                else if ((RX_BUFFER[2] == 0X00) && (RX_BUFFER[5] == 0X1D) && (RX_BUFFER[6] == 0X00) && (RX_BUFFER[8] != 0X00))
                {
										// 播放录入成功语音
										HAL_GPIO_WritePin(IO1_GPIO_Port, IO1_Pin, GPIO_PIN_RESET);
										__HAL_TIM_SET_COUNTER(&htim6, 0);
										HAL_TIM_Base_Start_IT(&htim6);
									
										// 关闭FM225的电源
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
										OLED_Clear();
                    OLED_ShowCHinese(32, 0, 30, 0);
                    OLED_ShowCHinese(48, 0, 31, 0);
                    OLED_ShowCHinese(64, 0, 25, 0);
                    OLED_ShowCHinese(80, 0, 26, 0);

                    while (1)
                    {
                        if (KEY3_PRESSED == 1)
                        {
                            KEY3_PRESSED = 0;
                        }
                        if (KEY2_PRESSED == 1)
                        {
                            KEY2_PRESSED = 0;
                        }

                        if (KEY0_PRESSED == 1)
                        {
                            KEY0_PRESSED = 0;
                        }
                        if (KEY1_PRESSED == 1)
                        {
                            KEY1_PRESSED = 0;
														HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
														menu = menu_main;
                            return 0;
                        }
                    }
                }
                else if ((RX_BUFFER[2] == 0X00) && (RX_BUFFER[5] == 0X1D)&& (RX_BUFFER[6] == 0X0D))
                {
                    // 播放录入失败语音
										HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_RESET);
										__HAL_TIM_SET_COUNTER(&htim6, 0);
										HAL_TIM_Base_Start_IT(&htim6);
									
										// 关闭FM225的电源
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
                    OLED_Clear();
                    OLED_ShowCHinese(32, 0, 30, 0);
                    OLED_ShowCHinese(48, 0, 31, 0);
                    OLED_ShowCHinese(64, 0, 27, 0);
                    OLED_ShowCHinese(80, 0, 28, 0);

                    while (1)
                    {
                        if (KEY3_PRESSED == 1)
                        {
                            KEY3_PRESSED = 0;
                        }
                        if (KEY2_PRESSED == 1)
                        {
                            KEY2_PRESSED = 0;
                        }

                        if (KEY0_PRESSED == 1)
                        {
                            KEY0_PRESSED = 0;
                        }
                        if (KEY1_PRESSED == 1)
                        {
                            KEY1_PRESSED = 0;
														HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
														menu = menu_main;
                            return 0;
                        }
                    }
                }
                if (KEY2_PRESSED == 1)
                {
                    KEY2_PRESSED = 0;
                }
                if (KEY3_PRESSED == 1)
                {
                    KEY3_PRESSED = 0;
                }
                if (KEY0_PRESSED == 1)
                {
                    KEY0_PRESSED = 0;
                }
                if (KEY1_PRESSED == 1)
                {
										KEY1_PRESSED = 0;
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
										menu = menu_main;
										return 0;
                }
            }
        }
        if (KEY2_PRESSED == 1)
        {
            KEY2_PRESSED = 0;
            if (ENROLL_ID < 99)
            {
                ENROLL_ID++;
                OLED_ShowNum(80, 2, ENROLL_ID, 2, 16, 0);
            }

        }
        if (KEY0_PRESSED == 1)
        {
            KEY0_PRESSED = 0;
            if (ENROLL_ID > 1)
            {
                ENROLL_ID--;
                OLED_ShowNum(80, 2, ENROLL_ID, 2, 16, 0);
            }
        }
        if (KEY1_PRESSED == 1)
        {

            KEY1_PRESSED = 0;
						HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
            menu = menu_main;
            return 0;
        }
    }

}
int menu_verify()
{
    while (1)
    {
        HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_SET);
        OLED_Clear();
        OLED_ShowCHinese(24, 0, 8, 0);
        OLED_ShowCHinese(40, 0, 20, 0);
        OLED_ShowCHinese(56, 0, 21, 0);
        OLED_ShowCHinese(72, 0, 22, 0);
        OLED_ShowCHinese(88, 0, 23, 0);
				HAL_Delay(10);
        // 赋值操作
        TX_BUFFER[0] = 0XEF;
        TX_BUFFER[1] = 0XAA;
        TX_BUFFER[2] = 0X12;
        TX_BUFFER[3] = 0X00;
        TX_BUFFER[4] = 0X02;
        TX_BUFFER[5] = 0X01;
        TX_BUFFER[6] = 0X10;
        TX_BUFFER[7] = 0X01;
        HAL_UART_Transmit_DMA(&huart1, TX_BUFFER, 8);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER, RX_BUFF_SIZE);
        while (1)
        {
            if (RX_BUFFER[2] == 0X00 && RX_BUFFER[5] == 0X12 && RX_BUFFER[6] == 0X00 && RX_BUFFER[8] != 0X00)
            {
								// 播放验证成功语音
								HAL_GPIO_WritePin(IO5_GPIO_Port, IO5_Pin, GPIO_PIN_RESET);
								__HAL_TIM_SET_COUNTER(&htim6, 0);
								HAL_TIM_Base_Start_IT(&htim6);
								
							
								uint8_t id = RX_BUFFER[8];
								OLED_ShowNum(72, 2, RX_BUFFER[8], 2, 16, 0);
                HAL_GPIO_WritePin(IO6_GPIO_Port, IO6_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
                OLED_Clear();
                OLED_ShowCHinese(32, 0, 4, 0);
                OLED_ShowCHinese(48, 0, 5, 0);
                OLED_ShowCHinese(64, 0, 25, 0);
                OLED_ShowCHinese(80, 0, 26, 0);

                OLED_ShowCHinese(24, 2, 39, 0);
                OLED_ShowCHinese(40, 2, 40, 0);
                OLED_ShowCHinese(56, 2, 41, 0);
                OLED_ShowNum(72, 2, id, 2, 16, 0);
                OLED_ShowCHinese(88, 2, 42, 0);

            }
            if (RX_BUFFER[2] == 0X00 && RX_BUFFER[5] == 0X12 && RX_BUFFER[6] == 0X0D)
            {
								// 播放验证失败语音
								HAL_GPIO_WritePin(IO6_GPIO_Port, IO6_Pin, GPIO_PIN_RESET);
								__HAL_TIM_SET_COUNTER(&htim6, 0);
								HAL_TIM_Base_Start_IT(&htim6);
							
                HAL_GPIO_WritePin(IO7_GPIO_Port, IO7_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
                OLED_Clear();
                OLED_ShowCHinese(32, 0, 4, 0);
                OLED_ShowCHinese(48, 0, 5, 0);
                OLED_ShowCHinese(64, 0, 27, 0);
                OLED_ShowCHinese(80, 0, 28, 0);

            }
            if (KEY2_PRESSED == 1)
            {
                KEY2_PRESSED = 0;
            }

            if (KEY0_PRESSED == 1)
            {
                KEY0_PRESSED = 0;
            }
            if (KEY3_PRESSED == 1)
            {
                KEY3_PRESSED = 0;
            }
            if (KEY1_PRESSED == 1)
            {
								KEY1_PRESSED = 0;
								HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
								menu = menu_main;
								return 0;
            }
        }
    }
}
int menu_delete()
{

    OLED_Clear();
    OLED_ShowCHinese(0, 0, 13, 0);
    OLED_ShowCHinese(16, 0, 14, 0);
    OLED_ShowCHinese(32, 0, 15, 0);
    OLED_ShowCHinese(48, 0, 16, 0);

    OLED_ShowCHinese(64, 0, 2, 0);
    OLED_ShowCHinese(80, 0, 3, 0);
    OLED_ShowCHinese(96, 0, 6, 0);
    OLED_ShowCHinese(112, 0, 7, 0);

    OLED_ShowCHinese(24, 2, 39, 0);
    OLED_ShowCHinese(40, 2, 40, 0);
    OLED_ShowCHinese(56, 2, 41, 0);
    OLED_ShowNum(72, 2, DELETE_ID, 2, 16, 0);
    OLED_ShowCHinese(88, 2, 42, 0);
    HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_SET);

    while (1)
    {
        if (KEY0_PRESSED == 1)
        {
            KEY0_PRESSED = 0;
						// 播放删除成功语音
						HAL_GPIO_WritePin(IO7_GPIO_Port, IO7_Pin, GPIO_PIN_RESET);
						__HAL_TIM_SET_COUNTER(&htim6, 0);
						HAL_TIM_Base_Start_IT(&htim6);
					
            if (DELETE_ID == 0)
            {
                OLED_Clear();
                OLED_ShowCHinese(16, 0, 37, 0);
                OLED_ShowCHinese(32, 0, 38, 0);
                OLED_ShowCHinese(48, 0, 2, 0);
                OLED_ShowCHinese(64, 0, 3, 0);
                OLED_ShowCHinese(80, 0, 25, 0);
                OLED_ShowCHinese(96, 0, 26, 0);

                TX_BUFFER[0] = 0XEF;
                TX_BUFFER[1] = 0XAA;
                TX_BUFFER[2] = 0X21;
                TX_BUFFER[3] = 0X00;
                TX_BUFFER[4] = 0X00;
                TX_BUFFER[5] = 0X21;

                HAL_UART_Transmit_DMA(&huart1, TX_BUFFER, 6);
                HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER,RX_BUFF_SIZE); // 继续接收数据

            } else
            {
                OLED_Clear();
                OLED_ShowCHinese(32, 0, 2, 0);
                OLED_ShowCHinese(48, 0, 3, 0);
                OLED_ShowCHinese(64, 0, 25, 0);
                OLED_ShowCHinese(80, 0, 26, 0);

                TX_BUFFER[0] = 0XEF;
                TX_BUFFER[1] = 0XAA;
                TX_BUFFER[2] = 0X20;
                TX_BUFFER[3] = 0X00;
                TX_BUFFER[4] = 0X02;
                TX_BUFFER[5] = 0X00;
                TX_BUFFER[6] = DELETE_ID;


                // 调用函数计算 BCC 校验码
                TX_BUFFER[7] = calculateBCC(&TX_BUFFER[2], 5);
                HAL_UART_Transmit_DMA(&huart1, TX_BUFFER, 8);
                HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER,RX_BUFF_SIZE); // 继续接收数据
            }
            while (1)
            {
	
								if (RX_BUFFER[2] == 0X00 && RX_BUFFER[5] == 0X20 && RX_BUFFER[6] == 0X00)
								{
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
								}
								
								if (RX_BUFFER[2] == 0X00 && RX_BUFFER[5] == 0X21 && RX_BUFFER[6] == 0X00)
								{
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
								}
								
                if (KEY3_PRESSED == 1)
                {
                    KEY3_PRESSED = 0;
                }

                if (KEY2_PRESSED == 1)
                {
                    KEY2_PRESSED = 0;
                }

                if (KEY0_PRESSED == 1)
                {
                    KEY0_PRESSED = 0;
                }
                if (KEY1_PRESSED == 1)
                {
                    KEY1_PRESSED = 0;
										HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
                    menu = menu_main;
										return 0;
                }
            }
        }
        if (KEY3_PRESSED == 1)
        {
            KEY3_PRESSED = 0;
            if (DELETE_ID < 99)
            {
                DELETE_ID++;
                OLED_ShowNum(72, 2, DELETE_ID, 2, 16, 0);
            }
        }
        if (KEY2_PRESSED == 1)
        {
            KEY2_PRESSED = 0;
            if (DELETE_ID > 0)
            {
                DELETE_ID--;
                OLED_ShowNum(72, 2, DELETE_ID, 2, 16, 0);
            }

        }
        if (KEY1_PRESSED == 1)
        {
            KEY1_PRESSED = 0;
						HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
            menu = menu_main;
            return 0;
        }
    }
}

/* 定时器中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    /* 停止定时器 */
    HAL_TIM_Base_Stop_IT(&htim2);
    
    /* 读取按键状态 */
    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET) {
      KEY0_PRESSED = 1;
    }
		if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
      KEY1_PRESSED = 1;
    }
		if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) {
      KEY2_PRESSED = 1;
    }
		if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET) {
      KEY3_PRESSED = 1;
    }
  }
	if (htim->Instance == TIM6) {
		/* 停止定时器 */
    HAL_TIM_Base_Stop_IT(&htim6);
		
		/* 复位按键状态：高电平 */
		HAL_GPIO_WritePin(IO1_GPIO_Port, IO1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO3_GPIO_Port, IO3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO4_GPIO_Port, IO4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO5_GPIO_Port, IO5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO6_GPIO_Port, IO6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO7_GPIO_Port, IO7_Pin, GPIO_PIN_SET);
	}
}
// 外部中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 检查是否为有效按键引脚
    if (GPIO_Pin == KEY0_Pin || 
        GPIO_Pin == KEY1_Pin || 
        GPIO_Pin == KEY2_Pin || 
        GPIO_Pin == KEY3_Pin)
    {
        // 清除定时器计数并启动定时器
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        HAL_TIM_Base_Start_IT(&htim2);
    }
		
    if (GPIO_Pin == LD2402_IO_Pin
            && HAL_GPIO_ReadPin(LD2402_IO_GPIO_Port, LD2402_IO_Pin) == 0)
    {

        HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_RESET);
        OLED_Clear();
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };
        GPIO_InitStruct.Pin = LD2402_IO_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(LD2402_IO_GPIO_Port, &GPIO_InitStruct);
    }
    if (GPIO_Pin == LD2402_IO_Pin
            && HAL_GPIO_ReadPin(LD2402_IO_GPIO_Port, LD2402_IO_Pin) == 1)
    {
        menu = menu_main;
        HAL_GPIO_WritePin(FM225_CTL_GPIO_Port, FM225_CTL_Pin, GPIO_PIN_SET);
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };
        GPIO_InitStruct.Pin = LD2402_IO_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(LD2402_IO_GPIO_Port, &GPIO_InitStruct);
    }

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
				memset((uint8_t *)RX_BUFFER, 0X00, sizeof(RX_BUFFER));
        HAL_UART_DMAStop(&huart1);  // 停止DMA传输
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)RX_BUFFER, RX_BUFF_SIZE); // 继续接收数据
    }
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
#ifdef USE_FULL_ASSERT
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
