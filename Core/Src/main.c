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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "software_timer.h"
#include "led_7seg.h"
#include "lcd.h"
#include "picture.h"
#include "button.h"
#include "uart.h"
#include "ds3231.h"
#include "lab4.h"
#include "stdlib.h"
#include "stdio.h"
// #include "lab2.h"
// #include "lab3.h"
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

int IsButtonUp();
int IsButtonDown();
void TestButtonMatrix();
void TestLcd();
void test_Uart();
void updateTime_ds3231();
void displayTime();
// uint32_t ds3231_hours = 12;
// uint32_t ds3231_min = 0;
// uint32_t ds3231_sec = 0;

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
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

#define DEBUG_LED_TIMER 0

  //  timer2Init();
  //  timer2Setup(50);
  //  led7segInit();
  //  buttonInit();
  //  lcd_init();
  //  Background();

  //  initlab3();
  initLab4();

  // timer2Init();
  // lcd_init();
  // buttonInit();

  // uart_init_rs232();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // example
  //  ds3231_SetAlarm1(ALARM_SKIP, // Bỏ qua Ngày
  //                   0,          // (isDayOfWeek = false)
  //                   ALARM_SKIP, // Bỏ qua Giờ
  //                   ALARM_SKIP, // Bỏ qua Phút
  //                   0);         // Chỉ so khớp Giây = 00
  //
  //  // BẠN CŨNG CẦN BẬT NGẮT ALARM TRONG THANH GHI CONTROL (0x0E)
  //  // (Hàm ds3231_Write của bạn có vẻ bị lỗi, nên ta dùng HAL trực tiếp)
  //  uint8_t control_reg = 0x05; // 0x05 = A1IE + A2IE + INTCN
  //  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, I2C_MEMADD_SIZE_8BIT, &control_reg, 1, 100);
  //  // Xóa cờ cũ (nếu có)

  while (1)
  {
    /* USER CODE END WHILE */
    runLab4();
    /* USER CODE BEGIN 3 */

    // test uart and buffer

    // 1. Kiểm tra xem FSM đã nhận xong tin nhắn chưa

    /*if (flag_buffer == 1)
    {
      flag_buffer = 0; // Xóa cờ ngay

      char *end_ptr;
      long received_value = strtol((const char *)msg, &end_ptr, 10);

      // 1. Kiểm tra xem có phải là số hợp lệ không
      if (end_ptr != (const char *)msg && *end_ptr == '\0')
      {
        // 2. Kiểm tra xem số có nằm trong dải của uint16_t không
        if (received_value < 0 || received_value > 65535) // 65535 là giá trị max của uint16_t
        {
          // LỖI: Giá trị nằm ngoài dải cho phép
          uart_Rs232SendString("Loi: So phai nam trong dai tu 0 den 65535.\r\n");
        }
        else
        {
          // THÀNH CÔNG: An toàn để ép kiểu
          uint16_t my_number = (uint16_t)received_value;

          // Ghi chú: Hàm uart_Rs232SendNum của bạn nhận uint32_t.
          // Khi bạn truyền (uint16_t)my_number vào, C sẽ tự động
          // "thăng hạng" (promote) nó lên uint32_t một cách an toàn.

          uart_Rs232SendString("Ban da nhap so hop le: ");
          uart_Rs232SendNum(my_number); // Gửi số đi
          uart_Rs232SendString("\r\n");
        }
      }
      else
      {
        // LỖI: Chuỗi nhập vào không phải là số
        uart_Rs232SendString("Loi: Du lieu nhap vao khong phai la so.\r\n");
      }
    }
    */

    /* USER CODE END 3 */
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void displayTime()
{
  lcd_show_int_num(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
  lcd_show_int_num(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
  lcd_show_int_num(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
  lcd_show_int_num(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
  lcd_show_int_num(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
  lcd_show_int_num(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
  lcd_show_int_num(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}
void updateTime_ds3231()
{
  ds3231_Write(ADDRESS_YEAR, 23);
  ds3231_Write(ADDRESS_MONTH, 10);
  ds3231_Write(ADDRESS_DATE, 20);
  ds3231_Write(ADDRESS_DAY, 6);
  ds3231_Write(ADDRESS_HOUR, 20);
  ds3231_Write(ADDRESS_MIN, 11);
  ds3231_Write(ADDRESS_SEC, 23);
}
/*code to test the ssytem */
// Button example 1: a button is pressed 1 time (50ms).
int IsButtonDown()
{
  if (buttonCount[0] == 1)
  {
    return 1;
  }
  return 0;
}

// Button example 2:
// Check if a button is pressed 1 time (50ms),
// or if it is held down for more than 1 second, in which case it returns 1 every 100ms.
int IsButtonUp()
{
  if (buttonCount[1] == 1 || (buttonCount[1] > 200 && buttonCount[1] % 2 == 0))
  {
    return 1;
  }
  return 0;
}

void TestButtonMatrix()
{
  for (int i = 0; i < 16; i++)
  {
    if (buttonCount[i] != 0)
    {
      lcd_show_int_num(140, 105, i, 2, BRED, WHITE, 32);
    }
  }
}

void test_Uart()
{
  if (buttonGetState(BUTTON_EN))
  {
    uart_Rs232SendNum(ds3231_hours);
    uart_Rs232SendString(":");
    uart_Rs232SendNum(ds3231_min);
    uart_Rs232SendString(":");
    uart_Rs232SendNum(ds3231_sec);
    uart_Rs232SendString("\n");
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
