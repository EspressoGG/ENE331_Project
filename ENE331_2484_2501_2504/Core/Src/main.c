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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t lastTime_1;
uint32_t Settime_1 = 60;

uint32_t lastTime_2;
uint32_t Settime_2 = 500;

uint32_t counter = 0;

uint8_t state = 0;
uint8_t LUT[4] = {
	    0b001,  // State 0 → PA1 ON
	    0b010,  // State 1 → PA2 ON
	    0b100,  // State 2 → PA3 ON
	    0b111   // State 3 → All ON
};


void GPIOA_Config(void) {
	// Enable GPIOA Clock (if not already done)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// PA1–PA3: Set as general-purpose output
	GPIOA->MODER &= ~((0x3 << (1 * 2)) | (0x3 << (2 * 2)) | (0x3 << (3 * 2)));
	GPIOA->MODER |=  ((0x1 << (1 * 2)) | (0x1 << (2 * 2)) | (0x1 << (3 * 2))); // '01' = output



	// OTYPER: All push-pull
	GPIOA->OTYPER &= ~((1 << 1) | (1 << 2) | (1 << 3) );

	// No pull-up/pull-down
	GPIOA->PUPDR &= ~((0x3 << (1 * 2)) | (0x3 << (2 * 2)) | (0x3 << (3 * 2))  );

}
unsigned char Read_PA0(void) {
	return (GPIOA->IDR & (1 << 0)) ? 1 : 0;
}

void GPIOB_Config(void) {
	// Enable GPIOB Clock
	RCC -> AHB1ENR |= (0x01 << 1);

	// Config PB10 as Output
	GPIOB -> MODER |= (0x01 << (10* 2));

    // Set output type
    // ALL GPIO (PB10) is  Output push-pull
	GPIOB-> OTYPER &= (0x00);

	// ALL GPIO (PBB10) Speed is High speed Output
	GPIOB -> OSPEEDR &= ~(0x00);

	// Disable PB10  pull-up
	GPIOB->PUPDR |= (0x00 << (10 * 2));
}



void GPIO_Toggle_Config(void) {
    // Enable GPIOA Clock
    RCC -> AHB1ENR |= (0x01 << 0);  // Enable GPIOA clock

    // Set PA6 as output
    GPIOA -> MODER &= ~(0x03 << (6 * 2));  // Clear MODER6
    GPIOA -> MODER |= (0x01 << (6 * 2));   // Set PA6 as output ('01')

    // Set output type
    GPIOA -> OTYPER &= ~(0x01 << 6);  // Push-pull for PA6


    // Disable pull-up/pull-down
    GPIOA -> PUPDR &= ~(0x03 << (6 * 2));   // No pull-up/pull-down for PA6
}


//	// Enable GPIOC Clock
//	RCC -> AHB1ENR |= (0x01 << 2);
//	// Set PC13 as output
//	GPIOC -> MODER &= ~(0x03 << 26); 	// Clear bit 27:26 bit
//	GPIOC -> MODER |= (0x01 << 26);		// Set bit 27:26 bit to b01
//	// Set output type
//	GPIOC -> OTYPER &= ~(0x01 << 13);
//	// Set GPIO Speed (optional)
//	GPIOC -> OSPEEDR &= ~(0x03 << 26);
//	// Disable pull-up/pull-down
//	GPIOC -> PUPDR &= ~(0x03 << 26);


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	if(htim -> Instance == TIM2) {
		counter++;
		GPIOB -> ODR ^= (1 << 10);
	}
}


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */

  GPIOA_Config();
  GPIOB_Config();
  GPIO_Toggle_Config();
  HAL_TIM_Base_Start(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


	  // 20 us base time from TIM2
	     if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) == SET) {
	         __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
	         counter++;
	     }
	        // Task 1
  	        if((counter - lastTime_1) >= Settime_1) {
	            // Your program task
	        	GPIOA-> ODR ^= (1 << 6); // Toggle PA6
//	        	GPIOC -> ODR ^= (1 << 13);
	        	for(uint32_t i=0; i<=6000; i++) {
	        		__NOP();
	        	}
	            lastTime_1 = counter;
		    }

	        // Task 2
	        if((counter - lastTime_2) >= Settime_2) {
	            // Your program task
	        	 if ((Read_PA0() == 0 )) {
	        		// On falling edge (button press)
	        		GPIOA->ODR &= ~(0x0E);               // Clear PA1–PA3
	        		GPIOA->ODR |= (LUT[state] << 1);     // Update LED pattern

	        		state += 1;

	        		if (state == 4){
	        			state = 0;}
	        		     	             }
	            lastTime_2 = counter;
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1440-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
