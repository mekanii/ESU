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
The clock divider is a crucial parameter in microcontroller and digital circuit design that determines the frequency of a clock signal by dividing the input clock frequency.
```math
Clock Divider = \frac{Input Clock Frequency}{Output Clock Frequency}
```
```math
Clock Divider = \frac{80 MHz}{80MHz} = 1
```
​
 


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
