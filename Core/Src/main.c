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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "dxlora.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char key_flag = 0;
uint8_t flag = 0;
// 页面枚举
typedef enum
{
  PAGE_HOME,      // 主页
  PAGE_SETTINGS,  // 设置页
  PAGE_MAX        // 页面总数
} PageType;

// 当前页面
PageType current_page = PAGE_HOME;
void Show_HomePage(void);
void Show_SettingsPage(void);
uint8_t rx_buf[64];  // 扩大缓冲区
uint8_t rx_data[6];
uint8_t rx_len = 0;  // 记录接收长度

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
	// 定义接收缓冲区
 HAL_UART_Receive_IT(&huart3, rx_buf, sizeof(rx_buf));
__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  // 启用空闲中断

	DXLORA_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_Clear();
	Show_HomePage();
	flag = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
switch(flag)
{
	case 1: backdata();/*func_读取数值并且赋值给data，再通过串口发送回去*/flag =0;break;//
	case 2:/*func_进入休眠模式（可唤醒）*/flag =0;break;//
	case 3:flag =0;break;
	case 4:flag =0;break;
}
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// 主页显示函数
void Show_HomePage(void)
{
  // 清屏
  OLED_Clear();

  // 显示主页内容
  OLED_ShowString(1, 1, "Home Page",5);
  OLED_ShowString(1, 7, "2-> setting",5);
}

// 设置页显示函数
void Show_SettingsPage(void)
{
  // 清屏
  OLED_Clear();

  // 显示设置页内容
  OLED_ShowString(1, 1, "Settings Page",5);
	

	
	
  OLED_ShowString(1, 7, "1-> Home",5);
}

// 按键处理函数
// 按键处理函数
void Key1_Handler(void)
{
  if (current_page == PAGE_SETTINGS) // 如果当前是设置页
  {
    current_page = PAGE_HOME;       // 切换到主页
    Show_HomePage();               // 显示主页
  }
}

void Key2_Handler(void)
{
  if (current_page == PAGE_HOME)    // 如果当前是主页
  {
    current_page = PAGE_SETTINGS;   // 切换到设置页
    Show_SettingsPage();            // 显示设置页
  }
}

// EXTI中断回调函数
void Key3_Handler(void)
{
	if (current_page == PAGE_SETTINGS){
		send("01");
	}
}
void Key4_Handler(void)
{
 if (current_page == PAGE_SETTINGS)
{send("02");}
}
// EXTI中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case GPIO_PIN_8:
      Key1_Handler(); // 处理按键1
      break;
    case GPIO_PIN_9:
      Key2_Handler(); // 处理按键2
      break;
    case GPIO_PIN_10:
      Key3_Handler(); // 处理按键3
      break;
    case GPIO_PIN_11:
      Key4_Handler(); // 处理按键4
      break;
    default:
      break;
  }
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//    if (huart == &huart3) {
//        OLED_Clear();
//			OLED_ShowString(1,1,(char*)rx_buf[6],6);
//      HAL_UART_Receive_IT(&huart3, rx_buf, 1);
//    }
//}
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
