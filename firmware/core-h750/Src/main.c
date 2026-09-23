/* USER CODE BEGIN Header */
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
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

HRTIM_HandleTypeDef hhrtim;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_HRTIM_Init(void);
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

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

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
	MX_HRTIM_Init();

	/* USER CODE BEGIN 2 */
	HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1);
	HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TB1);

	HAL_HRTIM_WaveformCounterStart(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(500);
	}
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
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

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 			= RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM 			= 5;
	RCC_OscInitStruct.PLL.PLLN 			= 192;
	RCC_OscInitStruct.PLL.PLLP 			= 2;
	RCC_OscInitStruct.PLL.PLLQ 			= 2;
	RCC_OscInitStruct.PLL.PLLR 			= 2;
	RCC_OscInitStruct.PLL.PLLRGE 		= RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL 	= RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN 		= 0;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType			= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
			| RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider	= RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider 	= RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief HRTIM Initialization Function
 * @param None
 * @retval None
 */
static void MX_HRTIM_Init(void)
{
	/* USER CODE BEGIN HRTIM_Init 0 */
	/* USER CODE END HRTIM_Init 0 */

	HRTIM_TimeBaseCfgTypeDef	pTimeBaseCfg	= {0};
	HRTIM_TimerCfgTypeDef 		pTimerCfg		= {0};
	HRTIM_CompareCfgTypeDef 	pCompareCfg		= {0};
	HRTIM_OutputCfgTypeDef 		pOutputCfg		= {0};

	/* USER CODE BEGIN HRTIM_Init 1 */
	/* USER CODE END HRTIM_Init 1 */

	hhrtim.Instance 					= HRTIM1;
	hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	hhrtim.Init.SyncOptions 			= HRTIM_SYNCOPTION_NONE;

	if (HAL_HRTIM_Init(&hhrtim) != HAL_OK)
	{
		Error_Handler();
	}

	pTimerCfg.InterruptRequests 	= HRTIM_TIM_IT_NONE;
	pTimerCfg.DMARequests			= HRTIM_TIM_DMA_NONE;
	pTimerCfg.DMASrcAddress 		= 0x0000;
	pTimerCfg.DMADstAddress 		= 0x0000;
	pTimerCfg.DMASize				= 0x1;
	pTimerCfg.HalfModeEnable		= HRTIM_HALFMODE_DISABLED;
	pTimerCfg.StartOnSync			= HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetOnSync			= HRTIM_SYNCRESET_DISABLED;
	pTimerCfg.DACSynchro			= HRTIM_DACSYNC_NONE;
	pTimerCfg.PreloadEnable			= HRTIM_PRELOAD_DISABLED;
	pTimerCfg.UpdateGating			= HRTIM_UPDATEGATING_INDEPENDENT;
	pTimerCfg.BurstMode				= HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	pTimerCfg.RepetitionUpdate		= HRTIM_UPDATEONREPETITION_DISABLED;
	pTimerCfg.PushPull				= HRTIM_TIMPUSHPULLMODE_DISABLED;
	pTimerCfg.FaultEnable			= HRTIM_TIMFAULTENABLE_NONE;
	pTimerCfg.FaultLock				= HRTIM_TIMFAULTLOCK_READWRITE;
	pTimerCfg.DeadTimeInsertion		= HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	pTimerCfg.DelayedProtectionMode	= HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	pTimerCfg.UpdateTrigger			= HRTIM_TIMUPDATETRIGGER_NONE;
	pTimerCfg.ResetTrigger			= HRTIM_TIMRESETTRIGGER_NONE;
	pTimerCfg.ResetUpdate			= HRTIM_TIMUPDATEONRESET_DISABLED;

	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &pTimerCfg) != HAL_OK)
	{
		Error_Handler();
	}

	pOutputCfg.Polarity					= HRTIM_OUTPUTPOLARITY_HIGH;
	pOutputCfg.SetSource				= HRTIM_OUTPUTSET_TIMPER;
	pOutputCfg.ResetSource				= HRTIM_OUTPUTRESET_TIMCMP1;
	pOutputCfg.IdleMode					= HRTIM_OUTPUTIDLEMODE_NONE;
	pOutputCfg.IdleLevel				= HRTIM_OUTPUTIDLELEVEL_INACTIVE;
	pOutputCfg.FaultLevel				= HRTIM_OUTPUTFAULTLEVEL_NONE;
	pOutputCfg.ChopperModeEnable		= HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	pOutputCfg.BurstModeEntryDelayed	= HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &pOutputCfg) != HAL_OK)
	{
		Error_Handler();
	}

	pTimeBaseCfg.Period 			= 1200;
	pTimeBaseCfg.RepetitionCounter	= 0x00;
	pTimeBaseCfg.PrescalerRatio		= HRTIM_PRESCALERRATIO_DIV1;
	pTimeBaseCfg.Mode				= HRTIM_MODE_CONTINUOUS;

	if (HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}

	pCompareCfg.CompareValue = 600;

	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}

	pTimeBaseCfg.Period				= 4800;
	pTimeBaseCfg.RepetitionCounter	= 0x00;
	pTimeBaseCfg.PrescalerRatio		= HRTIM_PRESCALERRATIO_DIV1;
	pTimeBaseCfg.Mode				= HRTIM_MODE_CONTINUOUS;

	if (HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &pTimeBaseCfg) != HAL_OK)
	{
		Error_Handler();
	}

	pCompareCfg.CompareValue = 2400;

	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
	{
		Error_Handler();
	}

	/* USER CODE BEGIN HRTIM_Init 2 */
	/* USER CODE END HRTIM_Init 2 */

	HAL_HRTIM_MspPostInit(&hhrtim);

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
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin : PA5 */
	GPIO_InitStruct.Pin		= GPIO_PIN_5;
	GPIO_InitStruct.Mode	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull	= GPIO_NOPULL;
	GPIO_InitStruct.Speed	= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	 */
	MPU_InitStruct.Enable 			= MPU_REGION_ENABLE;
	MPU_InitStruct.Number			= MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress		= 0x0;
	MPU_InitStruct.Size				= MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField 	= MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable 		= MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable 		= MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable 	= MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	__disable_irq();
	while (1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		for (volatile int i = 0; i < 200000; i++);
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
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
