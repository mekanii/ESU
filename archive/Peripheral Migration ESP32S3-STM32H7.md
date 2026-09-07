VAR             ESP32S3     PHR                                     STM32H7     PHR
SENS_CUT        IO.4        Digital Input                           PA0         EXTI Line 0 (Pull-Up)
SENS_COAG       IO.5        Digital Input                           PA1         EXTI Line 1 (Pull-Up)
OPTO            IO.6        Digital Input                           PA2         EXTI Line 2 (Pull-Up)
REM             IO.7                                                PA3         ADC1_INP15 (Single-ended)
MSD2            IO.15       Digital Input                           PB0         GPIO Input (Pull-Up)
MSD1            IO.16       Digital Input                           PB1         GPIO Input (Pull-Up)
CTL_RLY         IO.8        Digital Output                          PC4         GPIO Output (Push-Pull)
PWM_0           IO.1        RMT_TX_CHANNEL_0 80MHz (clk_div=1)      PA8         HRTIM1_CHA1
PWM_1           IO.2        RMT_TX_CHANNEL_1 20MHz (clk_div=4)      PA9         HRTIM1_CHB1
PWM_EN          IO.38       Digital Output                          PD2	        GPIO Output (Push-Pull)
CTL_BUZZ        IO.36       LEDC                                    PB8         TIM4_CH3

PA0     22      SENS_CUT
PA1     23      SENS_COAG
PA2     24      MSD1
PA3     25      MSD2
PA4     28      OPTO
PA5     29      CTL_RLY1
PA6     30      CTL_RLY2
PA7     31      PWM_EN
PA8     67      PWM0
PA10    69      PWM1

PC2_C   17      GND
PC3_C   18      REM

PD11    58      QSPI_BK1_IO0
PD12    59      QSPI_BK1_IO1
PD13    60      QSPI_BK1_IO3
PE2     1       QSPI_BK1_IO2
PB2     36      QSPI_CLK
PB6     92      QSPI_CS             PB10    46

PE7     37      QSPI_BK2_IO0
PE8     38      QSPI_BK2_IO1
PE9     39      QSPI_BK2_IO2
PE19    40      QSPI_BK2_IO3
PC11    79      QSPI_CS
