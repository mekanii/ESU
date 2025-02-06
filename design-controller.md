### 1. Sine PWM (SPWM)
#### 1.1. Graph
The image below presents two graphs that illustrate the relationship between a sine wave and its PWM (Pulse Width Modulation) representation:
- Left Graph
<br>This graph displays a sine wave with a frequency of 500 kHz, sampled using a sawtooth waveform at a sampling frequency of 20 MHz. The sawtooth waveform serves as a reference for capturing the sine wave's shape, allowing for a detailed representation of its amplitude variations over time.

- Right Graph
<br>This graph shows the PWM output generated from the same 500 kHz sine wave, also sampled at 20 MHz. The PWM output is derived from the sampling of the sine wave using the sawtooth waveform, effectively converting the continuous sine wave into a discrete signal that can be used for various applications, such as motor control or audio signal generation.

The graphs include five distinct rows, each representing a different amplitude level of the sine wave:
- Amplitude 100%
<br>This row illustrates the full-scale representation of the sine wave, where the peak values reach the maximum output level.

- Amplitude 50%
<br>This row shows the sine wave scaled down to half of its maximum amplitude, demonstrating how the waveform's height is reduced while maintaining its shape.

- Amplitude 25%
<br>In this row, the sine wave is further scaled down to a quarter of its maximum amplitude, highlighting the diminishing height of the waveform.

- Amplitude 10%
<br>This row depicts the sine wave at a significantly reduced amplitude, showcasing the waveform's ability to represent very low signal levels while still retaining its sinusoidal characteristics.

- Amplitude 2%
<br>The final row illustrates the sine wave at just 2% of its maximum amplitude, emphasizing the precision of the PWM output even at minimal signal levels.

The comparison between the left and right graphs provides valuable insights into how the sine wave is transformed into a PWM signal. The varying amplitude levels across the five rows demonstrate the flexibility and accuracy of PWM in representing different signal strengths, making it a crucial technique in in various electronic applications.

<p align="center">
  <img src="pic/Graph/SPWM%20graph.png" alt="spwm-graph">
</p>
This table is based on PWM values obtained from graph drawings. These values were visually represented and may have slight variations due to the nature of graphical representation.
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20based%20on%20graph.png" alt="spwm-table">
</p>

#### 1.2. Calculation
This table is derived from calculations based on the PWM values. It reflects the results of mathematical computations that take into account various factors such as scaling and zero crossing.
```
Scaled Value[i] = ((PWM Value[i] - Zero Crossing) x Scaling Factor ) + Zero Crossing
Scaled Value[i] = ((PWM Value[i] - 127.5) x Scaling Factor ) + 127.5
```
where PWM Value[i] is based on the 100% PWM Value from the graph
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20scaling%20calculation.png" alt="spwm-calc">
</p>
 In the current table, certain rows are highlighted in color to indicate that the values in these rows are either the same as or closely approaching the corresponding values from the previous table. This visual cue helps to quickly identify areas where the calculated PWM values align well with those derived from the graphical representation.

 The proximity of values between the two tables suggests that the calculations performed in the current table effectively replicate or approximate the PWM values that were originally drawn from the graph. This is important for validating the accuracy of the calculations and ensuring that the PWM signals generated will closely match the intended design.

 The alignment of values in the colored rows reinforces the reliability of the current table's calculations, indicating that the mathematical model used is consistent with the graphical data. This consistency is crucial for applications where precise PWM control is necessary.

 #### 1.3. Code


 #### 1.4. Measurement