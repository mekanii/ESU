# 1. General Concept
<p align="center">
  <img src="pic/ESU-block-diagram.png" alt="esu-block-diagram">
</p>

# 2. Component
## 2.1. Monopolar Switch Detection (MSD)
### 2.1.1. Schematic

<p align="center">
  <img src="pic/MSD/MSD-sch.png" alt="msd-sch">
</p>

### 2.1.2. Output Scope
- Cut
- Coag
## 2.2. Return Electrode Monitor (REM)
### 2.2.1. Schematic

<p align="center">
  <img src="pic/REM/REM-sch.png" alt="rem-sch">
</p>

### 2.2.2. Output Scope
#### 2.2.2.1. Transformer
Below is the output scope at the input and output of toroidal transformer.

<p align="center">
  <img src="pic/REM/REM XF.png" alt="rem-xf">
</p>

#### 2.2.2.2 Sense
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

## 2.3. Controller
### 2.3.1. PWM Generator
#### 2.3.1.1. Using the Remote Control Module (RMT) of ESP32 for Generating PWM Signals
The Remote Control Module (RMT) on the ESP32 is a versatile hardware peripheral designed primarily for sending and receiving infrared signals. However, it can also be effectively utilized to generate precise Pulse Width Modulation (PWM) signals.
#### 2.3.1.2. Key Concepts
- Duty Cycle:<br>The duty cycle of a PWM signal is the percentage of one cycle in which the signal is high (on) versus low (off). It is typically expressed as a value between 0 and 100.
- Total Period:<br>The total period of the PWM signal is the sum of the HIGH time and LOW time.
#### 2.3.1.3. Configuration
##### a. Clock Divider
The clock divider is a crucial parameter in microcontroller and digital circuit design that determines the frequency of a clock signal by dividing the input clock frequency, which is APB_CLK (80 MHz) for ESP32.

```math
Clock \, Divider = \frac{APB \_ CLK}{Clock \, Frequency}
```

##### b. Resolution
The resolution in duty cycle is a crucial parameter in microcontroller and digital circuit design that determines the granularity of control over the PWM signal. It defines how finely the duty cycle can be adjusted, allowing for precise modulation of the output signal. In the case of the ESP32, the resolution is influenced by the clock frequency which ultimately affect the ability to achieve desired output levels.

```math
Resolution = \frac{Clock \, Frequency}{PWM \, Frequency}
```

##### c. Combination of Clock Frequency and Resolution
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

#### 2.3.1.4. Output Scope
##### a. 400kHz
- Pattern 1
<br>The image illustrates the output scope for the Pattern 1 (Pure Cut operation) at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Pure Cut process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 400kHz PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center"><img src="pic/PWM/400kHz/1/10.png" alt="pwm-400-1-10"></p>
<p align="center"><img src="pic/PWM/400kHz/1/20.png" alt="pwm-400-1-20"></p>
<p align="center"><img src="pic/PWM/400kHz/1/30.png" alt="pwm-400-1-30"></p>
<p align="center"><img src="pic/PWM/400kHz/1/40.png" alt="pwm-400-1-40"></p>
<p align="center"><img src="pic/PWM/400kHz/1/50.png" alt="pwm-400-1-50"></p>
<p align="center"><img src="pic/PWM/400kHz/1/60.png" alt="pwm-400-1-60"></p>
<p align="center"><img src="pic/PWM/400kHz/1/70.png" alt="pwm-400-1-70"></p>
<p align="center"><img src="pic/PWM/400kHz/1/80.png" alt="pwm-400-1-80"></p>
<p align="center"><img src="pic/PWM/400kHz/1/90.png" alt="pwm-400-1-90"></p>
<p align="center"><img src="pic/PWM/400kHz/1/100.png" alt="pwm-400-1-100"></p>

<!-- <p align="center">
  <img src="pic/PWM/PWM-pure-cut.png" alt="PWM-pure-cut">
</p> -->

- Pattern 2<br>
The image illustrates the output scope for the Pattern 2 (Blend Cut 1) operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Blend Cut process, highlighting the key features and behavior of the PWM signal during this operation.<br>
For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency and similar waveform HIGH state periods, indicating consistent performance across these duty cycle settings. Each pulse in Blend Cut 1 is similar to a single pulse of a Pattern 1 (Pure Cut) PWM signal.

> [!NOTE]
> Pattern 2: 18 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 2 periods).<br>This pattern utilize a discrete resolution of 200 levels, similar to that of a Pattern 1 (Pure Cut) PWM signal. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center"><img src="pic/PWM/400kHz/2/10.png" alt="pwm-400-2-10"></p>
<p align="center"><img src="pic/PWM/400kHz/2/20.png" alt="pwm-400-2-20"></p>
<p align="center"><img src="pic/PWM/400kHz/2/30.png" alt="pwm-400-2-30"></p>
<p align="center"><img src="pic/PWM/400kHz/2/40.png" alt="pwm-400-2-40"></p>
<p align="center"><img src="pic/PWM/400kHz/2/50.png" alt="pwm-400-2-50"></p>
<p align="center"><img src="pic/PWM/400kHz/2/60.png" alt="pwm-400-2-60"></p>
<p align="center"><img src="pic/PWM/400kHz/2/70.png" alt="pwm-400-2-70"></p>
<p align="center"><img src="pic/PWM/400kHz/2/80.png" alt="pwm-400-2-80"></p>
<p align="center"><img src="pic/PWM/400kHz/2/90.png" alt="pwm-400-2-90"></p>
<p align="center"><img src="pic/PWM/400kHz/2/100.png" alt="pwm-400-2-100"></p>

- Pattern 3
<br>The image illustrates the output scope for the Pattern 3 (Blend Cut 2) operation at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Blend Cut process, highlighting the key features and behavior of the PWM signal during this operation.
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency and similar waveform HIGH state periods, indicating consistent performance across these duty cycle settings. Each pulse in Blend Cut 2 is similar to a single pulse of a Pure Cut PWM signal.

> [!NOTE]
> Pattern 3: 17 pulses per 1 modulation (400 kHz for each pulse, with 0% PWM during the last 3 periods).<br>This pattern utilize a discrete resolution of 200 levels, similar to that of a Pattern 1 (Pure Cut) PWM signal. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center"><img src="pic/PWM/400kHz/3/10.png" alt="pwm-400-3-10"></p>
<p align="center"><img src="pic/PWM/400kHz/3/20.png" alt="pwm-400-3-20"></p>
<p align="center"><img src="pic/PWM/400kHz/3/30.png" alt="pwm-400-3-30"></p>
<p align="center"><img src="pic/PWM/400kHz/3/40.png" alt="pwm-400-3-40"></p>
<p align="center"><img src="pic/PWM/400kHz/3/50.png" alt="pwm-400-3-50"></p>
<p align="center"><img src="pic/PWM/400kHz/3/60.png" alt="pwm-400-3-60"></p>
<p align="center"><img src="pic/PWM/400kHz/3/70.png" alt="pwm-400-3-70"></p>
<p align="center"><img src="pic/PWM/400kHz/3/80.png" alt="pwm-400-3-80"></p>
<p align="center"><img src="pic/PWM/400kHz/3/90.png" alt="pwm-400-3-90"></p>
<p align="center"><img src="pic/PWM/400kHz/3/100.png" alt="pwm-400-3-100"></p>


<!-- <p align="center">
  <img src="pic/PWM/PWM-blend-cut.png" alt="PWM-blend-cut">
</p> -->

##### b. 25kHz
The image illustrates the output scope for the 25kHz (Spray Coagulation operation) at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Spray Coagulation process, highlighting the key features and behavior of the PWM signal during this operation.

For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings. We can observe that the increment of the duty cycle results in a pulse that exhibits a linear increase in HIGH time, further emphasizing the effectiveness of the PWM modulation in maintaining a predictable response across varying duty cycle levels.

> [!NOTE]
> 25kHz PWM signal utilize a discrete resolution of 400 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/PWM-coag-spray.png" alt="PWM-coag-spray">
</p>

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

##### c. 20kHz
The image illustrates the output scope for the 20kHz (Forced Coagulation operation) at the PWM generator. It provides a detailed view of the waveform characteristics and performance metrics associated with the Forced Coagulation process, highlighting the key features and behavior of the PWM signal during this operation.

For duty cycle levels ranging from 10 to 100, demonstrates similar PWM frequency, indicating consistent performance across these duty cycle settings. We can observe that the increment of the duty cycle results in a pulse that exhibits a linear increase in HIGH time, further emphasizing the effectiveness of the PWM modulation in maintaining a predictable response across varying duty cycle levels.

> [!NOTE]
> 20kHz PWM signal utilize a discrete resolution of 500 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<p align="center">
  <img src="pic/PWM/PWM-coag-forced.png" alt="PWM-coag-forced">
</p>

<div align="center">

| Duty Cycle |       % | HIGH Time |
| ----------:| -------:| ---------:|
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

### 2.3.2. Serial Communication
#### 2.3.2.1. 1 Parameters <[command]>
- Description: Single parameter command for basic system operations.
- Parameters:
  - command: Command identifier

| Command |	Description 	            | Example | Success Response  | Error Response  |
|:-------:|:------------------------- |:------- |:-----------------:|:---------------:|
| 0       | System status query       | 0       | 00                | N/A             |
| 1       | System reset              | 1       | 00                | 02              |
| 2       | Stop all RMT transmission |	2	      | 00                | N/A             |
| Other   | Invalid                   | 3       | N/A               | 02              |

#### 2.3.2.2. 2 Parameters <[command] [data1]>
##### command = 0
- Description: Mode selection command without duty cycle adjustment.
- Parameters:
  - command: Command identifier, must be 0
  - data1: Mode selection (0-5)

| data1 |	Auto-assigned Channel | Description       | Example |	Success Response  |	Error Response  |
|:-----:|:---------------------:|:----------------- |:------- |:-----------------:|:---------------:|
| 0     |	Channel 0             |	Pure cut          | 0 0     |	00	              | N/A             |
| 1     |	Channel 0             |	Cut pattern 1     | 0 1     |	00	              | N/A             |
| 2     |	Channel 0             |	Cut pattern 2     | 0 2     |	00	              | N/A             |
| 3     |	Channel 1             |	Coag Spray        | 0 3     |	00	              | N/A             |
| 4     |	Channel 1             |	Coag Forced       | 0 4     |	00	              | N/A             |
| 5     |	Channel 1             |	Bipolar Standard  | 0 5     |	00	              | N/A             |
| Other |	N/A	                  | Invalid           | 0 6     | N/A	              | 02              |

#### 2.3.2.3. 3 Parameters <[command] [data1] [data2]>
##### command = 1
- Description: Mode and duty cycle adjustment command.
- Parameters:
  - command: Command identifier, must be 1
  - data1: Mode selection (0-5)
  - data2: Duty cycle value (range varies by mode)

| Mode  |	Duty Cycle  |	Description         |	Example |	Success Response  |	Error Response  |
|:-----:|:-----------:|:------------------- |:------- |:-----------------:|:---------------:|
| 0     |	    0 - 200 |	Pure cut            |	1 0 100 |	00	              | 02              |
| 1     |	    0 - 200 |	Cut pattern 1       |	1 1 150 |	00	              | 02              |
| 2     |	    0 - 200 |	Cut pattern 2       |	1 2 120 |	00	              | 02              |
| 3     |	    0 - 400 |	Coag Spray          |	1 3 250 |	00	              | 02              |
| 4     |	    0 - 500 |	Coag Forced         |	1 4 300 |	00	              | 02              |
| 5     |	    0 - 200 |	Bipolar Standard    |	1 5 80  |	00	              | 02              |
| Other |	        N/A	| Invalid	            | 1 6 100 |	N/A	              | 02              |

## 2.4. Power Stage
### 2.4.1. HF Switch
#### 2.4.1.1. Schematic

<p align="center">
  <img src="pic/PS/PS2.0.png" alt="PS2.0">
</p>

#### 2.4.1.2. MOSFET Driver
We use the TC4420 as our MOSFET driver due to its high-speed performance and ability to efficiently drive N-channel MOSFETs in various applications. The TC4420 provides a peak output current of up to 6A, allowing for rapid charging and discharging of the MOSFET gate capacitance. This capability minimizes switching losses and enhances overall efficiency in power management.

Operating within a supply voltage range of 4.5V to 18V, the TC4420 can effectively drive MOSFET gates at a voltage level of 12V, ensuring that the MOSFET turns on fully for optimal performance. Its fast switching speed, with propagation delays in the nanosecond range (typically 55ns), is crucial for high-frequency applications, reducing transition times and heat generation.

#### 2.4.1.3. Utilizing Three IRFPE50 MOSFETs in Parallel for Enhanced Performance and Reliability
We use three IRFPE50 MOSFETs in parallel to enhance the overall performance and reliability of our circuit. The decision to parallel these MOSFETs is driven by several key factors:
- Increased Current Handling:<br>Each IRFPE50 MOSFET has a maximum continuous drain current rating of ID=7.8A at TC=25°C and ID=4.9A at TC=100°C. By connecting three times as many of them in parallel, we effectively triple the total current handling capability. This is particularly beneficial in applications where high current loads are expected, ensuring that the circuit can handle the demands without overheating or exceeding the MOSFET's ratings.
- Improved Thermal Management:<br>Paralleling MOSFETs helps distribute the heat generated during operation across multiple devices. This reduces the thermal stress on each individual MOSFET, allowing for better thermal management. Proper heat dissipation is crucial for maintaining performance and preventing thermal runaway, especially in high-power applications.
- Lower On-Resistance:<br>The IRFPE50 has a low on-resistance (RDS(on)) of approximately 1.2Ω. When MOSFETs are paralleled, the effective on-resistance decreases, which results in lower conduction losses. This is advantageous for improving the efficiency of the circuit, as it minimizes the voltage drop across the MOSFETs when they are in the on state.
- Redundancy and Reliability:<br>Using multiple MOSFETs in parallel provides a level of redundancy. If one MOSFET were to fail, the remaining devices can continue to operate, thereby enhancing the overall reliability of the circuit. This is particularly important in critical applications where failure could lead to significant issues.

#### 2.4.1.4. Output Scope
##### a. 400kHz
The image illustrates the output scope for the 400kHz operation at:
- MOSFET Driver Output (blue)
- MOSFET Drain Pin (yellow)
- VDS=72V

It provides a detailed view of the waveform characteristics and performance metrics associated with the Pure Cut process, highlighting the key features and behavior of the HF Switch signal during this operation.
<br>For duty cycle levels ranging from 20 to 180, demonstrates similar Waveform frequency, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 400kHz PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<!-- <p align="center">
  <img src="pic/PWM/Wave-pure-cut-driver-mosfet.png" alt="Wave-pure-cut-driver-mosfet">
</p> -->

##### b. 25KHz

##### c. 25KHz

### 2.4.2. Transformer
**Toroidal Ferrite Core Transformer**

A toroidal ferrite core transformer is a type of transformer that uses a ring-shaped (toroidal) core made from ferrite material. Ferrite is a ceramic compound consisting of iron oxide mixed with other metals, which provides high magnetic permeability and low electrical conductivity. This makes ferrite ideal for high-frequency transformer applications.

**Structure and Construction**
- Core Shape<br>
  The core is a closed loop (doughnut-shaped), which helps confine the magnetic flux within the core material, minimizing leakage and electromagnetic interference (EMI).
- Windings<br>
  Copper wire is wound around the toroidal core, usually with primary and secondary windings placed concentrically. The windings can be distributed evenly around the core to further reduce leakage inductance and stray fields.
- Ferrite Material<br>
  Ferrite cores are preferred for high-frequency operation (tens of kHz to several MHz) because they have low core losses at these frequencies compared to laminated iron cores.

**Key Advantages**
- High Efficiency<br>
  The closed magnetic path of the toroid reduces core losses and leakage flux, resulting in higher efficiency.
- Low Electromagnetic Interference (EMI)<br>
  The toroidal shape confines the magnetic field, reducing EMI and making these transformers suitable for sensitive electronic circuits.
- Compact Size<br>
  For a given power rating, toroidal transformers are generally smaller and lighter than their laminated counterparts.
- Quiet Operation<br>
  The core shape and material minimize audible hum and vibration.

**Power Handling**

The transformer’s power rating is determined by the product of output voltage and current, and the core must be sized to avoid saturation at maximum load.

**Skin Effect**

At high frequencies, the skin effect is a phenomenon in alternating current (AC) where the current density becomes highest at the surface of a conductor and decreases exponentially toward the center. This occurs because the alternating current generates a changing magnetic field, which in turn induces **eddy currents** within the conductor. These eddy currents oppose the main current flow more strongly at the center, forcing most of the AC current to flow near the surface. As a result, the effective cross-sectional area available for current flow is reduced, increasing the effective resistance of the wire at higher frequencies.

<p align="center">
  <img src="pic/skin-effect.png" alt="skin-effect">
</p>

The depth at which the current density falls to 37% of its value at the surface.

$δ = \sqrt{ \frac{ ρ } { π • f • μ } }$

Where:
- ρ = resistivity of the conductor (Ω·m)
- μ = permeability of the conductor (H/m)
- f = frequency (Hz)

For high-frequency windings, consider using multiple thinner wires in parallel (Litz wire) to reduce skin effect losses.

The calculated diameter above is based on DC or low-frequency AC. For high-frequency applications, the effective area may be less than the physical area due to skin effect.

#### 2.4.2.1. Design and Calculation
Given:
- Input voltage<br>$V_{in} = 72 \ V$
- Output voltage<br>$V_{out} = 216 \ V$
- Switching frequency<br>$f = 400 \ kHz = 400,000 \ Hz$
- Core dimensions:
  - Inner diameter<br>$ID = 23.3 \ mm$
  - Outer diameter<br>$OD = 40.4 \ mm$
  - Height<br>$H = 15.1 \ mm$
- Maximum flux density<br>$B_{max} = 350 \ Gauss$
- Output current<br>$I_{out} = 0.5 \ A$

##### a. Core Cross-Sectional Area ($A_e$)
The effective cross-sectional area of the core is calculated as:

$A_e = \frac{( OD – ID ) • H }{ 2 }$

$A_e  = \frac{( 40.4 – 23.3 ) • 15.1 }{ 2 } = 129.1 \ mm^2$

This area is important for determining how much magnetic flux the core can handle without saturating.

##### b. Primary Turns ($N_p$)
The number of primary turns is determined by:

$N_p = \frac{ V_{in} • Duty \ Cycle • 10^{10} }{ 2 • f • B_{max} • A_e }$

$N_p = \frac{ 72 • 0.5 • 10^{10} }{ 2 • 400,000 • 350 • 129.1 } = \frac{ 360,000,000,000 }{ 36,149,400,000 }$

$N_p = 9.96 ≈ 10 \ turns$

This ensures the core does not saturate at the maximum input voltage and duty cycle.

##### c. Volt per Turn
The voltage induced per turn is:

$Volt \ per \ turn = \frac{ V_{in} }{ N_p }$

$Volt \ per \ turn = \frac{ 72 }{ 10 } = 7.2 \ V/turn$

##### d. Secondary Turns ($N_s$)
The number of secondary turns is:

$N_s = \frac{ V_{out} }{ Volt \ per \ turn }$

$N_s = \frac{ 216 }{ 7.2 } = 30 \text{ turns}$

This ensures the transformer provides the correct output voltage.

##### e. Output Power and Currents ($N_s$)
$P = V_{out} • I_{out} = 216 • 0.5 = 108 \ W$

$I_{in} = \frac{ P }{ V_{in} } = \frac{ 108 }{ 72 } = 1.5 \ A$

These calculations confirm the input current required to deliver the desired output power.

For high-frequency applications (400 kHz), we need to consider:
- **Skin effect** - Current tends to flow on the surface of conductors at high frequencies
- **Current density** - Typically 1.5-3 A/mm² for transformer applications
- **Temperature rise** - Higher current density = more heat

##### f. Wire Diameter
To calculate the required wire diameter for a given current, use the formula:<br><br>
$dW = \sqrt{ \frac{ 4 • I }{ \pi • J } }$

Where:
- $dW = wire \ diameter$
- $I = current \ (A)$
- $J = current \ density \ (A/mm^2)$

Primary:
- $I_{in} = 1.5 \ A$
- $Current \ Density = 1.5 \ A/mm^2$
- $dW_p = \sqrt{ \frac{ 4 • 1.5 }{ 3.14 • 1.5 } } = \sqrt{ \frac{ 6 }{ 4.712 } } = 1.13 \ mm$

Secondary:
- $I_{out} = 0.5 \ A$
- $Current \ Density = 1.5 \ A/mm^2$
- $dW_s = \sqrt{ \frac{ 4 • 0.5 }{ 3.14 • 1.5 } } = \sqrt{ \frac{ 2 }{ 4.712 } } = 0.65 \ mm$

##### g. High-Frequency Considerations
At 400 kHz, the skin depth in copper is approximately:

$δ = \sqrt{ \frac{ ρ } { π • f • μ } } ≈ 0.1 \ mm$

Since our calculated wire diameters are larger than 2× skin depth, consider using:
1. **Litz wire** for better high-frequency performance
2. **Multiple parallel smaller wires** instead of single thick wire
3. **Stranded wire** with individual strand diameter < 0.2 mm

##### h. Wire Specification for 400 kHz Operation using Stranded wire
- Primary
  - 7 parallel 0.2 mm each
  - Total current capacity: ~2.1A
  - Total equivalent area: ~1.5 mm²
- Secondary
  - 4 parallel 0.2 mm each
  - Total current capacity: ~1.0A
  - Total equivalent area: ~0.8 mm²

##### i. Wire Length Calculation
$Length \ per \ turn = 2 • (\frac{OD - ID}{2} + H)$

$Length \ per \ turn = 2 • (\frac{40.4 - 23.3}{2} + 15.1 ) = 47.3 \ mm$

- Primary
  - Number of Wire: 7
  - Turns: 10
  - $Individual \ Length = 10 • 47.3 • 1.1 ≈ 520 \text{ mm including 10\% leads} $
  - $Total \ Length = 7 • 520 = 3640 \ mm = 3.64 \ m$
- Secondary
  - Number of Wire: 4
  - Turns: 30
  - $Individual \ Length = 30 • 47.3 • 1.1 ≈ 1561 \text{ mm including 10\% leads}$
  - $Total \ Length = 4 • 1561 = 6244 \ mm = 6.24 \ m$

$Total \ Wire \ Length \ Needed = 3.64 + 6.24 = 9.88 m$

#### 2.4.2.2 Output Scope
##### a. 400kHz
The image illustrates the output scope at output transformer with PWM frequency 400kHz
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar Waveform length, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 400kHz PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

##### b. 25kHz
The image illustrates the output scope at output transformer with PWM frequency 25kHz
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar Waveform length, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 25kHz PWM signal utilize a discrete resolution of 400 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

##### c. 20kHz
The image illustrates the output scope at output transformer with PWM frequency 20kHz
<br>For duty cycle levels ranging from 10 to 100, demonstrates similar Waveform length, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 20kHz PWM signal utilize a discrete resolution of 500 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.

<!-- <p align="center">
  <img src="pic/PWM/XF/Bipolar/12V/bipolar-xformer-output-400.png" alt="bipolar-xformer-output-400">
</p> -->

### 2.4.3 LC Filter
**Type 1 Butterworth Filter: Differential Filter**

A Type 1 Butterworth filter is a second-order low-pass filter that provides a maximally flat frequency response in the passband. The "Type 1" designation typically refers to the specific topology or configuration of the filter circuit. In the context of differential filters, this often means a balanced configuration that processes differential signals (signals with equal and opposite components).

**Differential Filter Configuration**

A differential filter is designed to handle differential signals, which are commonly used in high-speed digital circuits, audio systems, and communication equipment. The filter processes both the positive and negative components of the signal simultaneously, providing better noise immunity and common-mode rejection.

**Key Characteristics:**
- Balanced Input/Output<br>
  The filter has balanced input and output terminals, typically labeled as $V_{in+}$, $V_{in−}$, $V_{out+}$, and $V_{out−}$.
- Common-Mode Rejection<br>
  The filter can reject common-mode noise (noise that appears equally on both input lines) while amplifying the differential signal.
- Symmetrical Design<br>
  The filter components are arranged symmetrically to ensure equal processing of both signal components.

#### 2.4.3.1 Design and Calculation
##### a. Output Class-D LC Filter Frequency Response Properties
The frequency response of the second-order class-D LC output filter is critical when selecting the component values for the inductor and capacitor. The LC filter response also varies with load impedance. The load impedance determines the damping ratio of the output LC filter and is classified as:
- Overdamped
- Critically damped
- Underdamped

It is also important to understand the load impedance variations for the application and select the L and C values that suit the expected load variations. Ideally, the LC filter value is selected for a **critically damped, flat passband, and phase response**. Two considerations when selecting components for the second-order low-pass filter is:
- the cut-off frequency
- Q factor or damping ratio

<p align="center">
  <img src="pic/effect-of-q-on-frequency-response.png" alt="effect-of-q-on -frequency-response">
</p>

Texas Instrument recommends using a second-order Butterworth low-pass filter because of its flat pass-band and phase response. Texas Instrument does not recommend the use of LC filters that peak excessively, like the underdamped filter response shown in figure above. At high frequency, the peaks are generally harsh to the human ear and can also trigger the protection circuitry, such as overcurrent, of some amplifiers. However, overdamped filters result in attenuation of high-frequency signal content.

##### b. Single-Ended Equivalent, Design and Analysis
This section details the design and analysis of a Type 1 Butterworth Filter in single-ended  LC circuit equivalent.

<p align="center">
  <img src="pic/type-1-butterworth-filter.png" alt="type-1-butterworth-filter">
</p>

**Cut-off Frequency for single-ended LC Filter**

The cut-off frequency ($f_0$) of the LC filter is determined by the values of the inductor ($L_{BTL}$) and capacitor ($C_L$), using the formula:

$f_0 = \frac{ ⍵_0 }{ 2 • π } = \frac{ 1 }{ 2 • π • \sqrt{ L_{BTL} • C_L } }$

The quality factor ($Q$) describes the selectivity or sharpness of the filter’s response and is given by:

$Q = R_L • \sqrt{ \frac{ C_L }{ L_{BTL} } } $

where $R_L$ is the load resistance.

To design a filter that is critically damped with a Butterworth response, Q = 0.707 = 1/√2. By substituting Q = 1/√2 into equations for Cut-off Frequency and Quality Factor previously listed, L and C values can be derived for a critically damped system assuming the desired cut-off frequency, ωo, is known.

**Step-by-Step Calculation**
- **Start with the Quality Factor Equation**<br>
$Q = R_L • \sqrt{ \frac{ C_L }{ L_{BTL} } } = \frac{ 1 }{ \sqrt{ 2 } } $<br><br>
This equation relates the load resistance, capacitance, and inductance for a Butterworth response

- **Rearrange to Express $C_L$ in Terms of $L_{BTL}$ and $R_L$**<br>
$\sqrt{ \frac{ C_L }{ L_{BTL} } } = \frac{ 1 }{ R_L • \sqrt{ 2 } } → \frac{ C_L }{ L_{BTL} } = \frac{ 1 }{ R_L^2 • 2 }$<br><br>
$C_L = \frac{ L_{BTL} }{ R_L^2 • 2 }$<br><br>
This gives a direct relationship between the required capacitance and inductance for the chosen Q.

- **Substitute $C_L$ into the Cut-off Frequency formula**<br>
The cut-off frequency for an LC filter is:<br>
$f_0 = \frac{ 1 }{ 2 • π • \sqrt{ L_{BTL} • \frac{ L_{BTL} }{ R_L^2 • 2 } } }$<br><br>
$f_0 = \frac{ 1 }{ 2 • π • \sqrt{ \frac{ L_{BTL}^2 }{ R_L^2 • 2 } } }$<br><br>
$f_0 = \frac{ 1 }{ 2 • π • \frac{ L_{BTL} }{ R_L • \sqrt{ 2 } } }$<br><br>
$f_0 = \frac{ R_L • \sqrt{ 2 } }{ 2 • π • L_{BTL} }$<br><br>
This equation allows us to solve for the required inductance for a given cut-off frequency and load.

- **Solve for $L_{BTL}$ and $C_L$**<br>
Rearranging the above equation for $L_{BTL}$:<br>
$L_{BTL} = \frac{ R_L • \sqrt{ 2 } }{ 2 • π • f_0 }$<br><br>
And using the earlier relationship for $C_L$:<br>
$C_L = \frac{ 1 }{ 2 • π • f_0 • R_L • \sqrt{ 2 } }$<br><br>

- **Apply to a Practical Example**<br>
  - We have:
    - $R_{BTL} = 100Ω$
    - $f_0 = 400 \text{ kHz} = 400,000 \text{ Hz}$
  - For a single-ended equivalent:
    - $C_{BTL} = \frac{ C_L }{ 2 }$
    - $R_L = \frac{ R_{BTL} }{ 2 }$
  - Plugging in the values:
    - $L_{BTL} = \frac{ \frac{ R_{BTL} }{ 2 } • \sqrt{ 2 } }{ 2 • π • f_0 } = \frac{ \frac{ 100 }{ 2 } • \sqrt{ 2 } }{ 2 • 3.14 • 400,000 } = \frac{ 70.7106 }{ 2,512,000 }$<br><br>
      $L_{BTL} = 0.000028149155 \ H = 28.1492 \text{ µH}$
    - $C_{BTL} = \frac{ \frac{ 1 }{ 2 • π • f_0 • \frac{ R_{BTL} }{ 2 } • \sqrt{ 2 } } }{ 2 } = \frac{ \frac{ 1 }{ 2 • 3.14 • 400,000 • \frac{ 100 }{ 2 } • \sqrt{ 2 } } }{ 2 } = \frac{ \frac{ 1 }{ 2,512,000 • 70.7106 } }{ 2 }$<br><br>
      $C_{BTL} = 0.000000000281492 \ F = 2.81492 \ nF ≈ 2.7 \ nF$

##### c. Frequency Response
With the targeted Q of 0.707 = 1/√2 and the cut-off frequency is:

$f_0 = \frac{ 1 }{ 2 • π • \sqrt{ L_{BTL} • C_L } } = \frac{ 1 }{ 2 • π • \sqrt{ L_{BTL} • 2 • C_{BTL} } }$

$f_0 = \frac{ 1 }{ 2 • 3.14 • \sqrt{ 28.1492 • 10^{-6} • 2 • 2.7 • 10^{-9} } } = \frac{ 1 }{ 2 • 3.14 • \sqrt{ 152 • 10^{-15} } } = \frac{ 1 }{ 2 • 3.14 • 3.8988 • 10^{-7} }$

$f_0 = 408,423.2417 \text{ Hz} = 408.423 \text{ kHz}$

Using the standard inductor and capacitor values from the previous calculations, the Q factor for 100Ω load is:

$Q = R_L • \sqrt{ \frac{ C_L }{ L_{BTL} } } = \frac{ R_{BTL} }{ 2 } • \sqrt{ \frac{ 2 • C_{BTL} } { L_{BTL} } }$

$Q = \frac{ 100 }{ 2 } • \sqrt{ \frac{ 2 • 2.7 • 10^{-9} } { 28.14 • 10^{-6} } }$

$Q = 0.6925$

The peaking at cut-off frequency in dB is:

$⍵_0 = 20 log_{10} (Q)$

$⍵_0 = 20 log_{10} (0.6925) =  -3.192 \text{ dB}$

Using the transfer function below, the frequency response of the selected LC filter can be plotted to provide a complete visual evaluation of the LC filter response.<br><br>
$H_{Diff}(s) = \frac{ V_{out}(s) } { V_{in}(s) } = \frac{1}{1 + s • \frac{ L_{BTL} }{ R_L } • C_L + s^2 }$

<p align="center">
  <img src="pic/gain-vs-frequency.png" alt="gain-vs-frequency">
</p>

##### d. Multilayer Air-Core Inductor

A multilayer air-core inductor is a type of inductor that consists of multiple layers of wire wound around a non-magnetic form (bobbin or mandrel) without any ferromagnetic core material. The "air-core" designation means the magnetic flux path is primarily through air, and the "multilayer" indicates that the wire is wound in multiple concentric layers rather than a single layer.

**Key Characteristics**

Advantages:
- No Core Losses<br>
  Absence of ferromagnetic material eliminates hysteresis and eddy current losses (No ferromagnetic material means no hysteresis, No conductive core means no eddy currents).<br>
  _Source: "Inductor Design Handbook" by Colonel Wm. T. McLyman, NASA Technical Memorandum 102538_
- High Frequency Operation<br>
  Suitable for high-frequency applications (100kHz to several GHz range).<br>
  _Source: "RF Circuit Design" by Chris Bowick, Howard W. Sams & Co._
- Linear Behavior<br>
  No saturation effects, maintaining inductance over a wide current range, Linear B-H relationship (air has μr = 1).<br>
  _Source: "Magnetic Components for Power Electronics" by Alex Van den Bossche_
- Low Distortion<br>
  Minimal harmonic distortion in signal applications (at low frequencies). But, at high frequencies, parasitic effects can introduce distortion. Inter-turn capacitance can cause frequency-dependent behavior.<br>
  _Source: "High Frequency Circuit Design" by Ralph S. Carson_
- Temperature Stability<br>
  Inductance remains relatively constant with temperature changes (Temperature coefficient: Typically <100 ppm/°C)<br>
  _Source: "Temperature Effects on Inductor Performance" - IEEE Transactions on Magnetics_

Disadvantages:
- Lower Inductance<br>
  Compared to ferromagnetic cores, air-core inductors have lower inductance per turn
  - Air-core: L ∝ N² × r²
  - Ferrite-core: L ∝ N² × μr × Ae
  - Typical ratio: Ferrite cores provide 100-1000× higher inductance per turn

  _Source: "Magnetic Core Selection for Transformers and Inductors" by Colonel Wm. T. McLyman_
- Larger Size<br>
  Requires more turns and larger physical size for the same inductance
  - Air-core: Volume ∝ N² × r³
  - Ferrite-core: Volume ∝ N² × Ae × l
  - Typical: Air-core inductors are 5-20× larger for same inductance

  _Source: "Inductor Design for Switching Regulators" - Texas Instruments Application Note_
- External Field<br>
  Magnetic field extends beyond the coil, potentially causing interference

  Source: "Electromagnetic Compatibility Engineering" by Henry W. Ott

- Q factor varies with frequency and design<br>
  Q factor depends on Wire resistance, Operating frequency, Core material properties.
  - Low frequency: Air-core can have HIGHER Q than ferrite
  - High frequency: Ferrite cores often have higher Q due to lower wire resistance

  _Source: "Quality Factor of Inductors" - IEEE Transactions on Components and Packaging_

  
**Step-by-Step Calculation**

<p align="center">
  <img src="pic/air-core-inductor.png" alt="air-core-inductor">
</p>

- **Calculate Turns per Layer**<br>
  $N_{layer} = \frac{ l } { d_{wire} }$

- **Estimate Number of Layers Needed**<br>
  First, we need to estimate the outer diameter. For rough approximation:<br>
  $OD ≈ ID + 2 • w$

  The winding thickness can be estimated as:<br>
  $w ≈ Number \ of \ Layers • d_{wire}$

- **Calculate Average Diameter**<br>
  $D = \frac{ ID + OD } { 2 }$

- **Use Inductance Formula to Solve for Number of Turns**<br>
  The inductance formula for multilayer air-core inductor:<br>
  $L = \frac{ 0.007925 • N^2 • D^2} { 3D + 9l + 10w }$

  Where:<br>
  $w = \frac{ OD - ID } { 2 }$

- **Iterative Solution Process**<br>
  Since the outer diameter depends on the number of layers, and the number of layers depends on the total turns, we need an iterative approach:
  - Initial Guess<br>
    Start with an initial guess for the number of layers
  - Calculate Dimensions
    - Calculate outer diameter<br>
      $OD = ID + 2 • ( Number \ of \ Layers • d_{wire} )$
    - Calculate average diameter<br>
      $D = \frac{ ID + OD } { 2 }$
    - Calculate winding thickness<br>
      $w = \frac{ OD - ID } { 2 }$
  - Calculate Required Turns<br>
    Rearrange the inductance formula to solve for N:<br>
    $N = \sqrt{ \frac{ L • ( 3D + 9l + 10w )} { 0.007925 • D^2 }}$
  - Calculate Actual Number of Layers<br>
    $Actual \ Layers = \frac{ N } { N_{layer} }$
  - Iterate<br>
    If the calculated layers don't match the initial guess, adjust and repeat.

**Practical Calculation**<br>
**Given:**
- Wire diameter ( $d_{wire}$ ): 1 mm
- Axial length ( $l$ ): 16 mm
- Inner diameter ( $ID$ ): 8 mm
- Target inductance ( $L$ ): 29 μH

**Calculations:**
- Turns per Layer<br>
  $N_{layer} = \frac{ 16 } { 1 } = 16 \ turns \ per \ layer$

- Initial Guess<br>
  Assume 5 layers

- Calculate Dimensions<br>
  - Outer Diameter<br>
    $OD = 8 + 2 • ( 5 • 1 ) = 18 \ mm$
  - Average Diameter<br>
    $D = \frac{ 8 + 18 } { 2 } = 13 \ mm$
  - Winding Thickness<br>
    $w = \frac{ 18 - 8 } { 2 } = 5 \ mm$

- Calculate Required Turns<br>
  $N = \sqrt{ \frac{ 29 • ( ( 3 • 13 ) + ( 9 • 16 ) + ( 10 • 5 ) )} { 0.007925 • 13^2 }} ≈ 71 \ turns$

- Calculate Actual Layers<br>
  $Actual \ Layers = \frac{ 71 } { 16 } ≈ 4.44 layers$

  Since we can't have partial layers, we need 5 layers.

- Recalculate with 5 Layers
  - $OD = 18 \ mm$
  - $D = 13 \ mm$
  - $w = 5 \ mm$
  - Final calculation<br>
    $N = \sqrt{ \frac{ 29 • ( ( 3 • 13 ) + ( 9 • 16 ) + ( 10 • 5 ) )} { 0.007925 • 13^2 }} ≈ 71 \ turns$
  - Final verification<br>
    With 71 turns and 5 layers:
    - $N_{layer} = \frac{ 71 } { 5 } = 14.2 \ turns \ per \ layer$
    - $N_{layer}Actual = 14$ _(since we can't have partial turns)_
    - $N = N_{layer}Actual • Number \ of \ Layers = 14 • 5 = 70 \ turns$
  - Calculate final inductance<br>
    $L = \frac{ 0.007925 • N^2 • D^2} { 3D + 9l + 10w }$

    $L = \frac{ 0.007925 • 70^2 • 13^2} { (3 • 3) + (9 • 16) + (10 • 5) } = 28.14 \text{ µH}$

#### 2.4.3.2 Output Scope
##### a. 400kHz
The image illustrates the output scope of the 400kHz PWM signal at the output transformer combined with the LC filter. The 400kHz PWM signal exhibits three distinct pulse patterns that repeat cyclically:
- Pattern 1: 20 pulses per modulation
- Pattern 2: 18 pulses per modulation
- Pattern 3: 17 pulses per modulation

These patterns represent different modulation schemes or switching sequences that are used to achieve the desired output characteristics. The variation in pulse count per modulation allows for precise control over the output waveform while maintaining the overall 400kHz switching frequency.

For duty cycle levels ranging from 10 to 100, demonstrates similar Waveform length, indicating consistent performance across these duty cycle settings.

> [!NOTE]
> 400kHz PWM signal utilize a discrete resolution of 200 levels. Therefore, all references to **duty cycle** in this context should be understood as discrete levels rather than percentages.
>
> In the image below, there are 2 sections (left and right). The **left section** shows the scope measurement of observed ESU product, while the **right section** displays the result of RnD implementation.

- Pattern 1<br>
  <p align="center"><img src="pic/PS/400kHz/1/400kHz-1-comparison.png" alt="400-1-comparison"></p>

  <div align="center">
    <table>
      <thead>
      <tr>
      <th align="center" rowspan="2">Duty<br>Cycle</th>
      <th align="center" rowspan="2">%</th>
      <th align="center" colspan="4">Observed ESU Product</th>
      <th align="center" colspan="4">RnD Implementation</th>
      </tr>
      <tr>
      <th align="center">$V_{max}$ [V]</th>
      <th align="center">$V_{min}$ [V]</th>
      <th align="center">$V_{rms}$ [V]</th>
      <th align="center">$f$ [kHz]</th>
      <th align="center">$V_{max}$ [V]</th>
      <th align="center">$V_{min}$ [V]</th>
      <th align="center">$V_{rms}$ [V]</th>
      <th align="center">$f$ [kHz]</th>
      </tr>
      </thead>
      <tbody align="right">
      <tr>
      <td>10</td>
      <td>5</td>
      <td>48.273</td>
      <td>-42.206</td>
      <td>18.234</td>
      <td>160.31</td>
      <td>61.456</td>
      <td>-57.940</td>
      <td>30.639</td>
      <td>400.64</td>
      </tr>
      <tr>
      <td>20</td>
      <td>10</td>
      <td>51.766</td>
      <td>-62.533</td>
      <td>30.127</td>
      <td>414.49</td>
      <td>68.733</td>
      <td>-63.186</td>
      <td>34.250</td>
      <td>400.73</td>
      </tr>
      <tr>
      <td>30</td>
      <td>15</td>
      <td>61.380</td>
      <td>-78.540</td>
      <td>39.703</td>
      <td>413.35</td>
      <td>79.413</td>
      <td>-72.800</td>
      <td>43.956</td>
      <td>399.87</td>
      </tr>
      <tr>
      <td>40</td>
      <td>20</td>
      <td>71.106</td>
      <td>-88.593</td>
      <td>46.299</td>
      <td>416.97</td>
      <td>97.440</td>
      <td>-84.820</td>
      <td>57.299</td>
      <td>399.58</td>
      </tr>
      <tr>
      <td>50</td>
      <td>25</td>
      <td>86.213</td>
      <td>-100.240</td>
      <td>55.591</td>
      <td>408.79</td>
      <td>111.670</td>
      <td>-97.086</td>
      <td>67.986</td>
      <td>399.87</td>
      </tr>
      <tr>
      <td>60</td>
      <td>30</td>
      <td>94.873</td>
      <td>-110.200</td>
      <td>60.121</td>
      <td>405.41</td>
      <td>123.010</td>
      <td>-109.400</td>
      <td>78.276</td>
      <td>400.06</td>
      </tr>
      <tr>
      <td>70</td>
      <td>35</td>
      <td>98.313</td>
      <td>-114.43</td>
      <td>62.467</td>
      <td>426.80</td>
      <td>126.940</td>
      <td>-118.660</td>
      <td>84.905</td>
      <td>400.25</td>
      </tr>
      <tr>
      <td>80</td>
      <td>40</td>
      <td>103.530</td>
      <td>-120.300</td>
      <td>64.498</td>
      <td>428.11</td>
      <td>127.920</td>
      <td>-125.280</td>
      <td>87.328</td>
      <td>400.06</td>
      </tr>
      <tr>
      <td>90</td>
      <td>45</td>
      <td>104.540</td>
      <td>-124.040</td>
      <td>66.208</td>
      <td>428.77</td>
      <td>125.520</td>
      <td>-130.500</td>
      <td>87.591</td>
      <td>400.06</td>
      </tr>
      <tr>
      <td>100</td>
      <td>50</td>
      <td>109.900</td>
      <td>-130.000</td>
      <td>67.718</td>
      <td>429.55</td>
      <td><strong>120.060</strong></td>
      <td><strong>-132.130</strong></td>
      <td><strong>85.187</strong></td>
      <td><strong>399.87</strong></td>
      </tr>
      </tbody>
    </table>
     </div>

   **Analysis of Pattern 1 (LC-29u-3n3):**

   The comparison between the observed ESU product and R&D implementation reveals several key differences:

   **Frequency Performance:**
   - **Observed ESU**: Frequency varies significantly from 160.31 kHz to 429.55 kHz across duty cycles, indicating frequency instability
   - **R&D Implementation**: Maintains consistent frequency around 400 kHz (±0.5 kHz), demonstrating superior frequency stability

   **Voltage Characteristics:**
   - **Voltage Range**: R&D implementation shows higher voltage amplitudes across all duty cycles
   - **Symmetry**: Both implementations maintain good voltage symmetry between positive and negative peaks.
     - Metric used: asymmetry = (|Vmax| − |Vmin|) / (|Vmax| + |Vmin|)
      - Consistency (±): R&D stays within ±1.0%–±7.0% across discrete levels 10–100 (5%–50% duty) (mean ≈ ±4.24%), while the observed ESU product ranges ±6.7%–±12.3% over the same levels (mean ≈ ±8.63%). This is ~2× better balance in R&D.
      - Examples (asymmetry %): level 10 (5%) → Observed 6.71%, R&D 2.94%; level 50 (25%) → Observed 7.52%, R&D 6.99%; level 100 (50%) → Observed 8.38%, R&D 4.79%.
   - **Scaling**: R&D implementation provides better voltage scaling with duty cycle changes

   **Performance Improvements:** R&D implementation eliminates the frequency drift observed in the commercial product and maintains a consistent 400 kHz operation, ensuring predictable performance across all settings.

   **Key Observations:** The R&D implementation maintains 399.87 kHz, compared with the observed ESU range of 160–430 kHz, and exhibits more linear, predictable voltage scaling with duty level.
   
- Pattern 2<br>
  <p align="center"><img src="pic/PS/400kHz/2/400kHz-2-comparison.png" alt="400-2-comparison"></p>
- Pattern 3<br>
  <p align="center"><img src="pic/PS/400kHz/3/400kHz-3-comparison.png" alt="400-3-comparison"></p>

##### b. 25kHz
##### c. 20kHz

<!-- Air Core Axial Inductor
| N1  | N2  |
|----:|----:|
|  36 | 144 |
|  36 | 144 | -->