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

```
Signal Frequency:
T = 5µs ÷ 2.5 wave
T = 2µs

f = 1 ÷ 2µs
f = 500kHz
```

Maximum Voltage Peak (Vp-max)
The Vp-max is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

| Power | Vp-max |
| ----- | ------ |
|    5W |  50V   | 0.5
|   10W |  75V   | 0.75
|   20W | 100V   | 1
|   30W | 115V   | 1.15
|   40W | 130V   | 1.3
|   50W | 150V   | 1.5
|   60W | 160V   | 1.6
|   70W | 165V   | 1.65
|   80W | 175V   | 1.75
|   90W | 180V   | 1.8
|  100W | 185V   | 1.85

##### A.1.2 Blend Cut
- Input
- Output
<br>The image displays two output scopes for Blend Cut operations at the power stage output:
  - **Left Scope**:<br>This represents the output for Blend Cut 1. It illustrates the waveform characteristics and performance metrics associated with the first cut blend operation.
  - **Right Scope**:<br>This represents the output for Blend Cut 2. It showcases the waveform details and performance metrics related to the second cut blend operation, allowing for a comparative analysis between the two blends.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20cut%20blend.png" alt="cut-blend">
</p>

```
Signal Frequency:
T = 10µs ÷ 5
T = 2µs

f = 1 ÷ 2µs
f = 500kHz

Modulation Frequency:
T = 10µs * 4.2 DIV
T = 42µs

f = 1 ÷ 42µs
f = 23.8kHz

Blend Cut 1 Modulation Duty Cycle:
Duty Cycle = 3.4 DIV ÷ 4.2 DIV x 100%
Duty Cycle = 80.95%

Blend Cut 2 Modulation Duty Cycle:
Duty Cycle = 3.0 DIV ÷ 4.2 DIV x 100%
Duty Cycle = 71.42%
```

Maximum Voltage Peak (Vp-max)
| Power | Blend 1 | Blend 2 |
| ----- | ------- | ------- |
|   10W |         |         |
|   20W |         |         |
|   30W |         |         |
|   40W |         |         |
|   50W |         |         |
|   60W |         |         |
|   70W |         |         |
|   80W |         |         |
|   90W |         |         |
|  100W |         |         |

#### A.2 Coag
##### A.2.1 Spray & Forced
- Input
- Output
<br>The image displays two output scopes for Coagulation operations at the power stage output:
  - **Left Scope**:<br>This represents the output for the Spray Coagulation operation. It illustrates the waveform characteristics and performance metrics associated with the coag spray process, highlighting its unique features.
  - **Right Scope**:<br>This represents the output for the Forced Coagulation operation. It showcases the waveform details and performance metrics related to the coag forced process, allowing for a comparative analysis between the two coagulation methods.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20coag%20spray%20and%20forced.png" alt="coag-spray-forced">
</p>

```
Spray Coag Modulation Frequency:
T = 20µs * 2 DIV
T = 40µs

f = 1 ÷ 40µs
f = 25kHz

Forced Coag Modulation Frequency:
T = 20µs * 2.2 DIV
T = 44µs

f = 1 ÷ 44µs
f = 22.72kHz
```

Maximum Voltage Peak (Vp-max)
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

Maximum Voltage Peak (Vp-max)
| Power | Vp-max |
| ----- | ------ |
|    5W |        |
|   10W |        |
|   20W |        |
|   30W |        |
|   40W |        |
|   50W |        |
|   60W |        |