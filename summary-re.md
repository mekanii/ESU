## Performance Test
### A. Monopolar
#### A.1 Cut
##### A.1.1 Pure Cut
- Input
- Output
<br>The image illustrates the output scope for the Pure Cut operation at the power stage output. It provides a detailed view of the waveform characteristics and performance metrics associated with the pure cut process, highlighting the key features and behavior of the signal during this operation.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20pure%20cut.png" alt="pure-cut">
</p>

_Signal Frequency:_
```
T = 5µs ÷ 2.5 wave
T = 2µs

f = 1 ÷ 2µs
f = 500kHz
```

_Maximum Voltage Peak (Vp-max)_
<br>The Vp-max is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

<table>
  <thead>
    <tr>
      <th width="200px">Power</th>
      <th width="200px">Vp-max</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>5W</td>
      <td>50V</td>
    </tr>
    <tr>
      <td>10W</td>
      <td>75V</td>
    </tr>
    <tr>
      <td>20W</td>
      <td>100V</td>
    </tr>
    <tr>
      <td>30W</td>
      <td>115V</td>
    </tr>
    <tr>
      <td>40W</td>
      <td>130V</td>
    </tr>
    <tr>
      <td>50W</td>
      <td>150V</td>
    </tr>
    <tr>
      <td>60W</td>
      <td>160V</td>
    </tr>
    <tr>
      <td>70W</td>
      <td>165V</td>
    </tr>
    <tr>
      <td>80W</td>
      <td>175V</td>
    </tr>
    <tr>
      <td>90W</td>
      <td>180V</td>
    </tr>
    <tr>
      <td>100W</td>
      <td>185V</td>
    </tr>
  </tbody>
</table>

<!-- | Power | Vp-max |
| ----- | ------ |
| 0.5
| 0.75
| 1
| 1.15
| 1.3
| 1.5
| 1.6
| 1.65
| 1.75
| 1.8
| 1.85 -->

##### A.1.2 Blend Cut
- Input
- Output
<br>The image displays two output scopes for Blend Cut operations at the power stage output:
  - Left Scope:<br>This represents the output for Blend Cut 1. It illustrates the waveform characteristics and performance metrics associated with the first cut blend operation.
  - Right Scope:<br>This represents the output for Blend Cut 2. It showcases the waveform details and performance metrics related to the second cut blend operation, allowing for a comparative analysis between the two blends.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20cut%20blend.png" alt="cut-blend">
</p>

_Signal Frequency:_
```
T = 10µs ÷ 5
T = 2µs

f = 1 ÷ 2µs
f = 500kHz
```

_Modulation Frequency:_
```
T = 10µs * 4.2 DIV
T = 42µs

f = 1 ÷ 42µs
f = 23.8kHz
```

_Blend Cut 1 Modulation Duty Cycle:_
```
Duty Cycle = 3.4 DIV ÷ 4.2 DIV x 100%
Duty Cycle = 80.95%
```

_Blend Cut 2 Modulation Duty Cycle:_
```
Duty Cycle = 3.0 DIV ÷ 4.2 DIV x 100%
Duty Cycle = 71.42%
```

_Maximum Voltage Peak (Vp-max)_
<br>The Vp-max is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

| Power | Blend 1 | Blend 2 |
| ----- | ------- | ------- |
|   10W |  70V    |  80V    | 0.7 , 0.8
|   20W | 100V    | 100V    | 1   , 1
|   30W | 120V    | 120V    | 1.2 , 1.2
|   40W | 130V    | 140V    | 1.3 , 1.4
|   50W | 150V    | 160V    | 1.5 , 1.6
|   60W | 160V    | 170V    | 1.6 , 1.7
|   70W | 180V    | 180V    | 1.8 , 1.8
|   80W | 190V    | 190V    | 1.9 , 1.9
|   90W | 195V    | 195V    | 1.95, 1.95
|  100W | 200V    | 200V    | 2   , 2

#### A.2 Coag
##### A.2.1 Spray & Forced
- Input
- Output
<br>The image displays two output scopes for Coagulation operations at the power stage output:
  - Left Scope:<br>This represents the output for the Spray Coagulation operation. It illustrates the waveform characteristics and performance metrics associated with the coag spray process, highlighting its unique features.
  - Right Scope:<br>This represents the output for the Forced Coagulation operation. It showcases the waveform details and performance metrics related to the coag forced process, allowing for a comparative analysis between the two coagulation methods.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20coag%20spray%20and%20forced.png" alt="coag-spray-forced">
</p>

_Spray Coag Modulation Frequency:_
```
T = 20µs * 2 DIV
T = 40µs

f = 1 ÷ 40µs
f = 25kHz
```

_Forced Coag Modulation Frequency:_
```
T = 20µs * 2.2 DIV
T = 44µs

f = 1 ÷ 44µs
f = 22.72kHz
```

_Maximum Voltage Peak (Vp-max):_
<br>The Vp-max is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

| Power | Spray  | Forced |
| ----- | ------ | ------ |
|    5W |        |        |
|   10W |        |        |
|   20W |        |        |
|   30W |        |        |
|   40W |        |        |
|   50W |        |        |
|   60W |        |        |

### B. Bipolar Standard Coag
- Input
- Output
<br>The image illustrates the output scope for the Bipolar Standard Coagulation operation at the power stage output. It provides a detailed view of the waveform characteristics and performance metrics associated with the bipolar coagulation process, highlighting the key features and behavior of the signal during this operation.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/bipolar%20coag%20standard.png" alt="coag-standard">
</p>

_Maximum Voltage Peak (Vp-max)_
<br>The Vp-max is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 500mV per division (500mV/div).

| Power | Vp-max |
| ----- | ------ |
|    5W |        |
|   10W |        |
|   20W |        |
|   30W |        |
|   40W |        |
|   50W |        |
|   60W |        |