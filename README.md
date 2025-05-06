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
Voltage \, per \, Level = \frac{Voltage \, Range}{2^{Resolution}} = \frac{3.3V - 0V}{2^{12}} = \frac{3.3V}{4096} ≈ 0.00080586V ≈ 0.80586 mV
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
- Total Period:<br>The total period of the PWM signal is the sum of the HIGH time and LOW time.
#### Configuration
##### Clock Divider
The clock divider is a crucial parameter in microcontroller and digital circuit design that determines the frequency of a clock signal by dividing the input clock frequency, which is APB_CLK (80 MHz) for ESP32.

```math
Clock \, Divider = \frac{APB \_ CLK}{Clock \, Frequency}
```

##### Resolution
The resolution in duty cycle is a crucial parameter in microcontroller and digital circuit design that determines the granularity of control over the PWM signal. It defines how finely the duty cycle can be adjusted, allowing for precise modulation of the output signal. In the case of the ESP32, the resolution is influenced by the clock frequency which ultimately affect the ability to achieve desired output levels.

```math
Resolution = \frac{Clock \, Frequency}{PWM \, Frequency}
```

##### Combination of Clock Frequency and Resolution
The combination of clock frequency and resolution plays a vital role in determining the performance of PWM signals in microcontroller applications. The clock frequency sets the base rate at which the system operates, while the resolution defines how many discrete levels the duty cycle can be divided into. A higher clock frequency allows for faster switching and more precise timing, while a higher resolution enables finer control over the duty cycle. Together, they influence the accuracy and responsiveness of the PWM output, making it essential to balance these parameters to meet the specific requirements of the application.

| Mode          | f PWM    | f Clock  | Clock Div | Resolution | 
| ------------- | -------: | -------: | --------: | ---------: |
| Pure Cut      |  400 kHz |   80 MHz |         1 |        200 |
| Blend Cut 1   |  400 kHz |   80 MHz |         1 |        200 |
| Blend Cut 2   |  400 kHz |   80 MHz |         1 |        200 |
| Coag Spray    |   25 kHz |   10 MHz |         8 |        400 |
| Coag Forced   |   20 kHz |   10 MHz |         8 |        500 |
| Coag Standard |  400 kHz |   80 MHz |         1 |        200 |

#### Output Scope
##### Cut
- Pure Cut
<br>The image illustrates the output scope for the Pure Cut operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Pure Cut process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> Pure Cut PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/PWM-pure-cut.png" alt="PWM-pure-cut">
</p>


- Blend Cut 1 & Blend Cut 2
<br>The image illustrates the output scope for the Blend Cut 1 (left) and Blend Cut 2 (right ) operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Blend Cut process, highlighting the key features and behavior of the PWM signal during this operation.

  - In the images of Blend Cut 1 and Blend Cut 2, both exhibit the same PWM frequency of 400 kHz.
  - Additionally, for duty cycle levels ranging from 10 to 100, Blend Cut 1 and Blend Cut 2 demonstrate similar waveform HIHGH state periods, indicating consistent performance across these duty cycle settings.
  - Furthermore, each pulse in both Blend Cut 1 and Blend Cut 2 is similar to a single pulse of a Pure Cut PWM signal.

> [!NOTE]
> Blend cut 1: 18 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 2 periods).<br>Blend cut 2: 17 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 3 periods).<br>Both Blend Cut 1 and Blend Cut 2 utilize a discrete resolution of 200 levels, similar to that of a Pure Cut PWM signal. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/PWM-blend-cut.png" alt="PWM-blend-cut">
</p>

##### Coag
- Spray
<br>The image illustrates the output scope for the Spray Coagulation operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Spray Coagulation process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings. We can observe that the increment of the duty cycle results in a pulse that exhibits a linear increase in HIGH time, further emphasizing the effectiveness of the PWM modulation in maintaining a predictable response across varying duty cycle levels.

> [!NOTE]
> Coag Spray PWM signal utilize a discrete resolution of 400 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

| Duty Cycle | HIGH Time |
| ----------:| ---------:|
|         10 |      1 us |
|         20 |      2 us |
|         30 |      3 us |
|         40 |      4 us |
|         50 |      5 us |
|         60 |      6 us |
|         70 |      7 us |
|         80 |      8 us |
|         90 |      9 us |
|        100 |     10 us |
|        ... |       ... |
|        200 |     20 us |
|        300 |     30 us |
|        400 |     40 us |

<p align="center">
  <img src="pic/PWM/PWM-coag-spray.png" alt="PWM-coag-spray">
</p>

- Forced
<br>The image illustrates the output scope for the Forced Coagulation operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Forced Coagulation process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings. We can observe that the increment of the duty cycle results in a pulse that exhibits a linear increase in HIGH time, further emphasizing the effectiveness of the PWM modulation in maintaining a predictable response across varying duty cycle levels.

> [!NOTE]
> Coag Forced PWM signal utilize a discrete resolution of 500 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

| Duty Cycle | HIGH Time |
| ----------:| ---------:|
|         10 |      1 us |
|         20 |      2 us |
|         30 |      3 us |
|         40 |      4 us |
|         50 |      5 us |
|         60 |      6 us |
|         70 |      7 us |
|         80 |      8 us |
|         90 |      9 us |
|        100 |     10 us |
|        ... |       ... |
|        200 |     20 us |
|        300 |     30 us |
|        400 |     40 us |
|        500 |     50 us |

<p align="center">
  <img src="pic/PWM/PWM-coag-forced.png" alt="PWM-coag-forced">
</p>


- Standard
## 4. Power Stage
### a. DAC
### b. Transformer Monopolar
- Toroidal Ferrite Core Transformer
- Air Core Axial Inductor
### c. Transformer Bipolar
- EE Ferrite Core Transformer
- EE Ferrite Core Inductor
