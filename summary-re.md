## Performance Test
### A. Monopolar
#### A.1 Cut
##### A.1.1 Pure Cut
- Input
- Output
<br>The image illustrates the output scope for the Pure Cut operation at the power stage output. It provides a detailed view of the waveform characteristics and performance metrics associated with the pure cut process, highlighting the key features and behavior of the signal during this operation.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20pure%20cut.png" alt="pure-cut">
</p>

```
Signal Frequency:
T = 5µs ÷ 2.5
T = 2µs

f = 1 ÷ 2µs
f = 500kHz
```

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
T = 4.2 * 10µs
T = 42µs

f = 1 ÷ 42µs
f = 23.8kHz
```

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

### B. Bipolar Standard Coag
- Input
- Output
<br>The image illustrates the output scope for the Bipolar Standard Coagulation operation at the power stage output. It provides a detailed view of the waveform characteristics and performance metrics associated with the bipolar coagulation process, highlighting the key features and behavior of the signal during this operation.

> [!NOTE]
> The oscilloscope setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/bipolar%20coag%20standard.png" alt="coag-standard">
</p>


#### bipolar pure cut ???
T = 10µs/DIV * 8DIV ÷ 20 = 4µs
f = 1 ÷ 4µs = 250kHz

V = 50mV/DIV * 0.6DIV * 100 = 3V