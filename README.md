# I. General Concept
<p align="center">
  <img src="pic/ESU-block-diagram.png" alt="esu-block-diagram">
</p>

# II. Component
## 1. Monopolar Switch Detection (MSD)
### a. Schematic
### b. Output Scope
- Cut
- Coag
## 2. Return Electrode Monitor (REM)
### a. Schematic
### b. Output Scope
#### Transformer
Below is the output scope at the input and output of toroidal transformer.
<p align="center">
  <img src="pic/REM/REM XF.png" alt="rem-xf">
</p>

#### Sense
> [!NOTE]
> All measurements for Return Electrode Monitoring do not utilize a physical Return Electrode; instead, a variable resistor is used. Therefore, all references to **pad resistance** in this context describe the variable resistor that simulates the Return Electrode.

Below is the output scope from the sense circuit, where the pad resistance values are 5Ω, 200Ω, 1kΩ, and High Impedance. The voltages measured by the oscilloscope are 10.385 mV, 558.91 mV, 689.31 mV, and 1.1164 V, respectively.
<p align="center">
  <img src="pic/REM/REM-sense.png" alt="rem-sense">
</p>
The ESP32's ADC provides a versatile and powerful way to read analog signals with a resolution of up to 12 bits and operates with a reference voltage of 0V to 3.3V (the default voltage range). Below is the formula to calculate the voltage per level for the ADC at 12-bit resolution.

```math
Voltage \, per \, Level = \frac{Voltage \, Range}{2^Resolution} = \frac{3.3V - 0V}{2^12} = \frac{3.3V}{4096} ≈ 0.00080586V ≈ 0.80586 mV
```
<br>
Below is the formula to calculate the ADC value based on the voltage sense and the Voltage per Level.

```math
ADC \, Value = \frac{Vsense}{Voltage \, per \, Level}
```
<br>
Using the formula above, we have the following ADC values for different pad resistances:
| Pad Resistance | Measured Voltage | ADC Value |
| -------------: | ---------------: | --------: |
|             5Ω |        10.385 mV |        19 |
|           200Ω |        558.91 mV |       694 |
|            1kΩ |        689.31 mV |       855 |
|         High-Z |        1116.4 mV |      1385 |

## 3. Controller
### a. PWM Generator
#### Using the Remote Control Module (RMT) of ESP32 for Generating PWM Signals
The Remote Control Module (RMT) on the ESP32 is a versatile hardware peripheral designed primarily for sending and receiving infrared signals. However, it can also be effectively utilized to generate precise Pulse Width Modulation (PWM) signals.
#### Key Concepts
- Duty Cycle:<br>The duty cycle of a PWM signal is the percentage of one cycle in which the signal is high (on) versus low (off). It is typically expressed as a value between 0 and 100.
- Total Period:<br>The total period of the PWM signal is the sum of the high time and low time.
#### Configuration
##### Clock Divider
The clock divider is a crucial parameter in microcontroller and digital circuit design that determines the frequency of a clock signal by dividing the input clock frequency, which is APB_CLK for ESP32.
```math
Clock \, Divider = \frac{APB \_ CLK}{Clock \, Frequency} = \frac{80 \, MHz}{80 \, MHz} = 1
```
##### Resolution
We want a 400 kHz PWM frequency, which requires a specific resolution to achieve fine control over the duty cycle. So the max resolution is calculated by the following formula:
```math
Resolution = \frac{Clock \, Frequency}{PWM \, Frequency} = \frac{80 \, MHz}{400 \, MHz} = 200
```
This means that for every PWM cycle, we can divide the clock cycles into 200 discrete steps, allowing for precise adjustments to the PWM signal's duty cycle. The value of 200 is used as the **Total Period**, and the **Duty Cycle** can vary from 0 to 200, where:
- A duty cycle of 0 corresponds to 0% (always off).
- A duty cycle of 200 corresponds to 100% (always on).

#### Output Scope
##### Cut
- Pure Cut
> [!NOTE]
> 20 pulses per 1 modulation (400 kHz for each pulse).
- Cut Blend 1
> [!NOTE]
> 18 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 2 periods).
- Cut Blend 2
> [!NOTE]
> 17 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 3 periods).

##### Coag
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
