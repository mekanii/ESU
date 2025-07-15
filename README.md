# I. General Concept
<p align="center">
  <img src="pic/ESU-block-diagram.png" alt="esu-block-diagram">
</p>

# II. Component
## 1. Monopolar Switch Detection (MSD)
### a. Schematic

<p align="center">
  <img src="pic/MSD/MSD-sch.png" alt="msd-sch">
</p>

### b. Output Scope
- Cut
- Coag
## 2. Return Electrode Monitor (REM)
### a. Schematic

<p align="center">
  <img src="pic/REM/REM-sch.png" alt="rem-sch">
</p>

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
<br>

```math
Voltage \, per \, Level = \frac{Voltage \, Range}{2^{Resolution}} = \frac{3.3V - 0V}{2^{12}} = \frac{3.3V}{4096} ≈ 0.00080586V ≈ 0.80586 mV
```
<br>
Below is the formula to calculate the ADC value based on the voltage sense and the Voltage per Level.
<br>
<br>

```math
ADC \, Value = \frac{Vsense}{Voltage \, per \, Level}
```
<br>
Using the formula above, we have the following ADC values for different pad resistances:

<div align="center">

| Pad Resistance | Measured Voltage | ADC Value |
| -------------: | ---------------: | --------: |
|             5Ω |        10.385 mV |        19 |
|           200Ω |        558.91 mV |       694 |
|            1kΩ |        689.31 mV |       855 |
|         High-Z |        1116.4 mV |      1385 |

</div>

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

<div align="center">

| Mode          | f PWM    | f Clock  | Clock Div | Resolution | 
| ------------- | -------: | -------: | --------: | ---------: |
| Pure Cut      |  400 kHz |   80 MHz |         1 |        200 |
| Blend Cut 1   |  400 kHz |   80 MHz |         1 |        200 |
| Blend Cut 2   |  400 kHz |   80 MHz |         1 |        200 |
| Coag Spray    |   25 kHz |   10 MHz |         8 |        400 |
| Coag Forced   |   20 kHz |   10 MHz |         8 |        500 |
| Coag Standard |  400 kHz |   80 MHz |         1 |        200 |

</div>

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

<div align="center">

| Duty Cycle |       % | HIGH Time |
| ----------:| ------: | ---------:|
|         10 |   2.5 % |      1 us |
|         20 |   5.0 % |      2 us |
|         30 |   7.5 % |      3 us |
|         40 |  10.0 % |      4 us |
|         50 |  12.5 % |      5 us |
|         60 |  15.0 % |      6 us |
|         70 |  17.5 % |      7 us |
|         80 |  20.0 % |      8 us |
|         90 |  22.5 % |      9 us |
|        100 |  25.0 % |     10 us |
|        ... |     ... |       ... |
|        200 |  50.0 % |     20 us |
|        300 |  75.0 % |     30 us |
|        400 | 100.0 % |     40 us |

</div>

> [!NOTE]
> Spray Coagulation PWM signal utilize a discrete resolution of 400 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/PWM-coag-spray.png" alt="PWM-coag-spray">
</p>

- Forced
<br>The image illustrates the output scope for the Forced Coagulation operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Forced Coagulation process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings. We can observe that the increment of the duty cycle results in a pulse that exhibits a linear increase in HIGH time, further emphasizing the effectiveness of the PWM modulation in maintaining a predictable response across varying duty cycle levels.

> [!NOTE]
> Forced Coagulation PWM signal utilize a discrete resolution of 500 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<div align="center">

| Duty Cycle |       % | HIGH Time |
| ----------:| ------: | ---------:|
|         10 |   2.0 % |      1 us |
|         20 |   4.0 % |      2 us |
|         30 |   6.0 % |      3 us |
|         40 |   8.0 % |      4 us |
|         50 |  10.0 % |      5 us |
|         60 |  12.0 % |      6 us |
|         70 |  14.0 % |      7 us |
|         80 |  16.0 % |      8 us |
|         90 |  18.0 % |      9 us |
|        100 |  20.0 % |     10 us |
|        ... |     ... |       ... |
|        200 |  40.0 % |     20 us |
|        300 |  60.0 % |     30 us |
|        400 |  80.0 % |     40 us |
|        500 | 100.0 % |     50 us |

</div>

<p align="center">
  <img src="pic/PWM/PWM-coag-forced.png" alt="PWM-coag-forced">
</p>


- Standard
## 4. Power Stage
### a. DAC
#### Schematic

<p align="center">
  <img src="pic/PS/PS2.0.png" alt="PS2.0">
</p>

##### MOSFET Driver
We use the TC4420 as our MOSFET driver due to its high-speed performance and ability to efficiently drive N-channel MOSFETs in various applications. The TC4420 provides a peak output current of up to 6A, allowing for rapid charging and discharging of the MOSFET gate capacitance. This capability minimizes switching losses and enhances overall efficiency in power management.

Operating within a supply voltage range of 4.5V to 18V, the TC4420 can effectively drive MOSFET gates at a voltage level of 12V, ensuring that the MOSFET turns on fully for optimal performance. Its fast switching speed, with propagation delays in the nanosecond range (typically 55ns), is crucial for high-frequency applications, reducing transition times and heat generation.

##### Utilizing Three IRFPE50 MOSFETs in Parallel for Enhanced Performance and Reliability
We use three IRFPE50 MOSFETs in parallel to enhance the overall performance and reliability of our circuit. The decision to parallel these MOSFETs is driven by several key factors:
- Increased Current Handling:<br>Each IRFPE50 MOSFET has a maximum continuous drain current rating of ID=7.8A at TC=25°C and ID=4.9A at TC=100°C. By connecting three times as many of them in parallel, we effectively triple the total current handling capability. This is particularly beneficial in applications where high current loads are expected, ensuring that the circuit can handle the demands without overheating or exceeding the MOSFET's ratings.
- Improved Thermal Management:<br>Paralleling MOSFETs helps distribute the heat generated during operation across multiple devices. This reduces the thermal stress on each individual MOSFET, allowing for better thermal management. Proper heat dissipation is crucial for maintaining performance and preventing thermal runaway, especially in high-power applications.
- Lower On-Resistance:<br>The IRFPE50 has a low on-resistance (RDS(on)) of approximately 1.2Ω. When MOSFETs are paralleled, the effective on-resistance decreases, which results in lower conduction losses. This is advantageous for improving the efficiency of the circuit, as it minimizes the voltage drop across the MOSFETs when they are in the on state.
- Redundancy and Reliability:<br>Using multiple MOSFETs in parallel provides a level of redundancy. If one MOSFET were to fail, the remaining devices can continue to operate, thereby enhancing the overall reliability of the circuit. This is particularly important in critical applications where failure could lead to significant issues.

#### Output Scope
##### Cut
The image illustrates the output scope for the Pure Cut operation at:
- MOSFET Driver Output (blue)
- MOSFET Drain Pin (yellow)
- VDS=12V

It provides a detailed view of the waveform characteristics and performance metrics associated with the Pure Cut process, highlighting the key features and behavior of the DAC signal during this operation.
<br>For duty cycle levels ranging from 20 to 180, demonstrates similar Waveform frequency, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> Pure Cut PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/Wave-pure-cut-driver-mosfet.png" alt="Wave-pure-cut-driver-mosfet">
</p>

##### Coag
### b. Transformer
#### Monopolar
##### Design and Calculation
Air Core Transformer using EE25 Vertical Transformer Bobbin
#### Bipolar
##### Design and Calculation
Toroidal Ferrite Core Transformer
##### Output Scope
The image illustrates the output scope for per pulse operation at input transformer with VDS=12V and PWM frequency 400kHz
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar Waveform length, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 400kHz PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/XF/Bipolar/12V/bipolar-xformer-output-400.png" alt="bipolar-xformer-output-400">
</p>

### c. LC Filter
#### Monopolar
##### Design and Calculation
EE Ferrite Core Inductor
#### Bipolar
##### Design and Calculation
```math
L = \frac{ R_L • \sqrt{ 2 } }{ ⍵_0 }
```

where
```math
R_L = \frac{ R_BTL }{ 2 }
```

and
```math
⍵_0 = 2 • π • f_0
```

then
```math
L = \frac{ \frac{ R_BTL }{ 2 } • \sqrt{ 2 } }{ 2 • π • f_0 }
```

Air Core Axial Inductor
| N1  | N2  |
|----:|----:|
|  36 | 144 |
|  36 | 144 |
##### Design and Calculation
Rectangullar Air Core Inductor