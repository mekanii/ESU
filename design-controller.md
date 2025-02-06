This table is based on PWM values obtained from graph drawings. These values were visually represented and may have slight variations due to the nature of graphical representation.
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20based%20on%20graph.png" alt="pure-cut">
</p>

This table is derived from calculations based on the PWM values. It reflects the results of mathematical computations that take into account various factors such as scaling and zero crossing.
```
Scaled Value[i] = ((PWM Value[i] - Zero Crossing) x Scaling Factor ) + Zero Crossing
Scaled Value[i] = ((PWM Value[i] - 127.5) x Scaling Factor ) + 127.5
```
where PWM Value[i] is based on the 100% PWM Value from the graph
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20scaling%20calculation.png" alt="pure-cut">
</p>
 In the current table, certain rows are highlighted in color to indicate that the values in these rows are either the same as or closely approaching the corresponding values from the previous table. This visual cue helps to quickly identify areas where the calculated PWM values align well with those derived from the graphical representation.

 The proximity of values between the two tables suggests that the calculations performed in the current table effectively replicate or approximate the PWM values that were originally drawn from the graph. This is important for validating the accuracy of the calculations and ensuring that the PWM signals generated will closely match the intended design.

 The alignment of values in the colored rows reinforces the reliability of the current table's calculations, indicating that the mathematical model used is consistent with the graphical data. This consistency is crucial for applications where precise PWM control is necessary.