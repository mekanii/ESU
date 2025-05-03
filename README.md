# I. General Concept
<p align="center">
  <img src="pic/ESU-block-diagram.png" alt="esu-block-diagram">
</p>

# II. Component
## 1. Monopolar Switch Detection (MSD)
### a. Schematic
### b. Sine Wave
### c. Output Scope
- Cut
- Coag
## 2. Return Electrode Monitor (REM)
### a. Schematic
### b. Sine Wave
### c. Output Scope
## 3. Controller
### a. PWM Generator
#### Using the Remote Control Module (RMT) of ESP32 for Generating PWM Signals
The Remote Control Module (RMT) on the ESP32 is a versatile hardware peripheral designed primarily for sending and receiving infrared signals. However, it can also be effectively utilized to generate precise Pulse Width Modulation (PWM) signals.
#### Key Concepts
- Duty Cycle:<br>The duty cycle of a PWM signal is the percentage of one cycle in which the signal is high (on) versus low (off). It is typically expressed as a value between 0 and 100.
- Total Period:<br>The total period of the PWM signal is the sum of the high time and low time.
#### Configuration
- Clock Divider<br>
The clock divider is a crucial parameter in microcontroller and digital circuit design that determines the frequency of a clock signal by dividing the input clock frequency, which is APB_CLK for ESP32.
```math
Clock Divider = \frac{APB_CLK}{Clock Frequency} = \frac{80 MHz}{80 MHz} = 1
```

​- Resolution<br>
We want a 400 kHz PWM frequency, which requires a specific resolution to achieve fine control over the duty cycle. So the max resolution is calculated by the following formula:
```math
Resolution = \frac{Clock Frequency}{PWM Frequency} = \frac{80 MHz}{400 kHz} = 200
```
This means that for every PWM cycle, we can divide the clock cycles into 200 discrete steps, allowing for precise adjustments to the PWM signal's duty cycle. The value of 200 is used as the **Total Period**, and the **Duty Cycle** can vary from 0 to 200, where:
  - A duty cycle of 0 corresponds to 0% (always off).
  - A duty cycle of 200 corresponds to 100% (always on).

- Cut
  - Pure Cut
  - Cut Blend 1
  - Cut Blend 2
- Coag
  - Spray
  - Forced
  - Standard
## 4. Power Stage
### a. DAC
### b. Transformer Monopolar
- Toroidal Ferrite Core Transformer
- Air Core Axial Inductor
### c. Transformer Bipolar
- EE Ferrite Core Transformer
- EE Ferrite Core Inductor
