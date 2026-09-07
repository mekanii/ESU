#include "main.h"

extern HRTIM_HandleTypeDef hhrtim1;

// Definisi Konstanta HRTIM DLL Speed Equivalent (~2.5 GHz)
#define HRTIM_PER_400KHZ   6250U   // 2.5 GHz / 400 kHz
#define HRTIM_PER_100KHZ  25000U   // 2.5 GHz / 100 kHz

/**
 * @brief Menghentikan seluruh output PWM secara instan (Keamanan ESU)
 */
void stop_all_outputs(void)
{
  // Disable Burst Mode Controller jika sedang aktif
  HAL_HRTIM_BurstModeStop(&hhrtim1);

  // Stop Timer A & Timer B Output
  HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1);
  HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TB1);
  
  HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMER_A);
  HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMER_B);
}

/**
 * @brief Mengkonfigurasi dan mengaktifkan Burst Mode Controller (BMC)
 * @param clock_source: HRTIM_BURSTMODECTRLCLK_TIMA atau HRTIM_BURSTMODECTRLCLK_TIMB
 * @param pulse_on: Jumlah pulsa ON
 * @param pulse_total: Total periode pulsa (ON + OFF)
 */
static void setup_burst_mode(uint32_t clock_source, uint16_t pulse_on, uint16_t pulse_total)
{
  HRTIM_BurstModeCfgTypeDef sBurstModeConfig = {0};

  sBurstModeConfig.Mode = HRTIM_BURSTMODE_CONT;               // Berjalan terus menerus
  sBurstModeConfig.ClockSource = clock_source;                // Menghitung pulsa dari timer terkait
  sBurstModeConfig.Prescaler = HRTIM_BURSTMODEPRESCALER_DIV1;
  sBurstModeConfig.Period = pulse_total;                      // Total siklus pulsa (ON + OFF)
  sBurstModeConfig.Compare = pulse_on;                        // Jumlah pulsa ON

  HAL_HRTIM_BurstModeConfig(&hhrtim1, &sBurstModeConfig);
  HAL_HRTIM_BurstModeStart(&hhrtim1);
}

/**
 * @brief Menjalankan Generator Sinyal ESU berdasarkan mode yang dipilih
 * @param signalType: 0 (400k Cont), 1 (400k 18/2), 2 (400k 17/3), 3 (100k 1/3), 4 (100k 1/4)
 */
void start(uint8_t signalType) 
{
  // 1. Selalu hentikan sinyal aktif sebelumnya demi keamanan alat ESU
  stop_all_outputs();

  if (signalType == 0 || signalType == 1 || signalType == 2) 
  {
    /* ========================================================
       SETUP CHANNEL 0: Pin 1 (HRTIM_OUTPUT_TA1 - 400 kHz)
       ======================================================== */
    
    uint32_t period = HRTIM_PER_400KHZ;
    uint32_t duty_cycle_50pct = period / 2; // Duty cycle carrier 50%

    // Set frekuensi & duty cycle carrier pada Timer A
    __HAL_HRTIM_SET_PERIOD(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, period);
    __HAL_HRTIM_SET_COMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, duty_cycle_50pct);

    if (signalType == 0) 
    {
      // Mode 1: 400 kHz Continuous (Tanpa Modulasi Burst)
      // Cukup jalankan Timer A biasa
    } 
    else if (signalType == 1) 
    {
      // Mode 2: 400 kHz, 18 pulse ON, 2 pulse OFF
      setup_burst_mode(HRTIM_BURSTMODECTRLCLK_TIMA, 18, 20);
    } 
    else if (signalType == 2) 
    {
      // Mode 3: 400 kHz, 17 pulse ON, 3 pulse OFF
      setup_burst_mode(HRTIM_BURSTMODECTRLCLK_TIMA, 17, 20);
    }

    // Start Timer A & Pin TA1 Output
    HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMER_A);
    HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1);

  } 
  else if (signalType == 3 || signalType == 4) 
  {
    /* ========================================================
       SETUP CHANNEL 1: Pin 2 (HRTIM_OUTPUT_TB1 - 100 kHz)
       ======================================================== */

    uint32_t period = HRTIM_PER_100KHZ;
    uint32_t duty_cycle_50pct = period / 2; // Duty cycle carrier 50%

    // Set frekuensi & duty cycle carrier pada Timer B
    __HAL_HRTIM_SET_PERIOD(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, period);
    __HAL_HRTIM_SET_COMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, duty_cycle_50pct);

    if (signalType == 3) 
    {
      // Mode 4: 100 kHz, 1 pulse ON, 3 pulse OFF (Total = 4 pulsa)
      setup_burst_mode(HRTIM_BURSTMODECTRLCLK_TIMB, 1, 4);
    } 
    else if (signalType == 4) 
    {
      // Mode 5: 100 kHz, 1 pulse ON, 4 pulse OFF (Total = 5 pulsa)
      setup_burst_mode(HRTIM_BURSTMODECTRLCLK_TIMB, 1, 5);
    }

    // Start Timer B & Pin TB1 Output
    HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMER_B);
    HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB1);
  }
}