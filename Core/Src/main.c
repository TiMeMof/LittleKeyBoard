/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "tim_callback.h"
#include "FPM383C.h"
#include "CH9828.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//  功能描述   : OLED 7针脚4SPI
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA5（SCL）
//              D1   接PA7（SDA）
//              RES  接PB0
//              DC   接PA6
//              CS   接PA4     //CS接低电平有效(即片选上)          
//              ----------------------------------------------------------------
/* USER CODE END PTD */

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
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void Get_ADC_Value(void);
void Set_Key_Val(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define up_v 2.9
#define dn_v 0.3

uint8_t uart1_rx[100];
uint8_t uart2_rx[100];
uint8_t ch9828[100]={0};
uint8_t speed_key=0;
uint8_t page_num=0;
uint16_t set_val_dev=20;

uint32_t adc1_val;
uint32_t adc2_val;
float adc1_v,last_adc1_v;
float adc2_v,last_adc2_v;

uint8_t start_f1=0,start_f2=0,start_cnt=0;
uint32_t speed_cnt=0;
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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	
	HAL_TIM_Base_Start_IT(&htim1);
  HAL_UART_Receive_IT(&huart1,uart1_rx,28);
	HAL_UART_Receive_IT(&huart2,uart2_rx,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	OLED_Clear();
	fingerprint_matching_synchronization_cmd();	
	char charhere[100];
  while (1)
  {
		if(timcount%20==0){
			Set_Key_Val();
			if(page_num==0){
				OLED_Clear_Line(2);
				GPIO_PinState C15=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15);
				sprintf(charhere,"%d,%d",speed_key,speed_cnt);
				OLED_ShowString(0,0,(uint8_t*)&charhere);
				sprintf(charhere,"id:%d,err:%1d",fpm_data.id,fpm_data.error_code);
				OLED_ShowString(0,2,(uint8_t*)&charhere);
				sprintf(charhere,"mode:normal");
				OLED_ShowString(0,4,(uint8_t*)&charhere);
				int aaa = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15);
				sprintf(charhere,"%1.1f,%1.1f,%d",adc1_v,adc2_v,aaa);
				OLED_ShowString(0,6,(uint8_t*)&charhere);
			}
			else{
				OLED_Clear_Line(2);
				sprintf(charhere,"%d,%d",speed_key,speed_cnt);
				OLED_ShowString(0,0,(uint8_t*)&charhere);
				sprintf(charhere,"id:%d,err:%d",fpm_data.id,fpm_data.error_code);
				OLED_ShowString(0,2,(uint8_t*)&charhere);
				sprintf(charhere,"mode:ctrl");
				OLED_ShowString(0,4,(uint8_t*)&charhere);
				sprintf(charhere,"%1.1f,%1.1f",adc1_v,adc2_v);
				OLED_ShowString(0,6,(uint8_t*)&charhere);
			}
		}
		static int time_10s=0;
		if(timcount%1000==0){
			HAL_UART_Transmit(&huart1,fpm_cmd,18,0xff);
			time_10s++;
		}
		static int if_press_psw=0;
		if(fpm_data.error_code==0&&if_press_psw==0){
			if(fpm_data.id==0){
				Send_password();
				if_press_psw=1;
			}
		}
		else if(time_10s==10){
			if_press_psw=0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_CC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  /* TIM1_UP_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
}

/* USER CODE BEGIN 4 */
void Get_ADC_Value(void){
	HAL_ADC_Start(&hadc1);//打开ADC
	HAL_ADC_PollForConversion(&hadc1,50);
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	{
		adc1_val = HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);
	HAL_ADC_Start(&hadc2);//打开ADC
	HAL_ADC_PollForConversion(&hadc2,50);
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))
	{
		adc2_val = HAL_ADC_GetValue(&hadc2);
	}
	HAL_ADC_Stop(&hadc2);
	last_adc1_v = adc1_v;
	last_adc2_v = adc2_v;
	adc1_v = (float)adc1_val*3.3/4096;
	adc2_v = (float)adc2_val*3.3/4096;
}


void Set_Key_Val(void){
	Get_ADC_Value();
//	//右上角计时
//	if(adc2_v<dn_v && adc1_v<dn_v && start_f1==0)start_f1=1;
//	if(start_f1==1)start_cnt++;//每加一次20ms
//	if(start_cnt>=25){
//		OLED_Clear();
//		start_cnt=0;
//		speed_key++;
//		if(speed_key==2)speed_key=0;
//		start_f1=0;
//	}
//	//左下角计时
//	if(adc2_v>up_v && adc1_v>up_v && start_f2==0)start_f2=1;
//	if(start_f2==1)start_cnt++;
//	if(start_cnt>=25){
//		OLED_Clear();
//		start_cnt=0;
//		page_num++;
//		if(page_num==2)page_num=0;
//		start_f2=0;
//	}
	speed_cnt++;
	if(speed_key==0){
		set_val_dev=1;
	}
	else if(speed_key==1){
		set_val_dev=3;
	}
	if(speed_cnt>=set_val_dev){
		if(page_num==0){
//			if(adc2_v>up_v&&adc1_v<up_v&&adc1_v>dn_v)Send_left();
//			if(adc2_v<dn_v&&adc1_v<up_v&&adc1_v>dn_v)Send_right();
//			if(adc1_v>up_v&&adc2_v<up_v&&adc2_v>dn_v)Send_down();
//			if(adc1_v<dn_v&&adc2_v<up_v&&adc2_v>dn_v)Send_up();
			if(adc2_v>up_v&&adc1_v<up_v&&adc1_v>dn_v)Send_left_noup();
			if(adc2_v<dn_v&&adc1_v<up_v&&adc1_v>dn_v)Send_right_noup();
			if(adc1_v>up_v&&adc2_v<up_v&&adc2_v>dn_v)Send_down_noup();
			if(adc1_v<dn_v&&adc2_v<up_v&&adc2_v>dn_v)Send_up_noup();
			if(adc2_v>up_v && adc1_v>up_v)send_l_d_noup();//左下
			if(adc2_v<dn_v && adc1_v<dn_v)send_r_u_noup();//右上
			if(adc2_v>up_v && adc1_v<up_v)send_l_u_noup();//左上
			if(adc2_v<dn_v && adc1_v>dn_v)send_r_d_noup();//右下
			
			if(adc1_v>dn_v && adc1_v<up_v && adc2_v<up_v && adc2_v>dn_v)Send_release();
		}
		else if(set_val_dev==3){
			if(adc2_v>up_v&&adc1_v<up_v&&adc1_v>dn_v)Send_smaller();
			if(adc2_v<dn_v&&adc1_v<up_v&&adc1_v>dn_v)Send_bigger();
			if(adc1_v>up_v&&adc2_v<up_v&&adc2_v>dn_v)Send_page_down();
			if(adc1_v<dn_v&&adc2_v<up_v&&adc2_v>dn_v)Send_page_up();
		}
		speed_cnt=0;
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
