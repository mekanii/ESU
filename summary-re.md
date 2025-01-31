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

##### A.1.2 Cut Blend
- Input
- Output
<br>The image displays two output scopes for cut blend operations at the power stage output:
  - **Left Scope**:<br>This represents the output for Cut Blend 1. It illustrates the waveform characteristics and performance metrics associated with the first cut blend operation.
  - **Right Scope**:<br>This represents the output for Cut Blend 2. It showcases the waveform details and performance metrics related to the second cut blend operation, allowing for a comparative analysis between the two blends.

> [!NOTE]
> This setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20cut%20blend.png" alt="cut-blend">
</p>

#### A.2 Coag
##### A.2.1 Spray & Forced
- Input
- Output
<br>The image displays two output scopes for coagulation operations at the power stage output:
  - **Left Scope**:<br>This represents the output for the Coag Spray operation. It illustrates the waveform characteristics and performance metrics associated with the coag spray process, highlighting its unique features.
  - **Right Scope**:<br>This represents the output for the Coag Forced operation. It showcases the waveform details and performance metrics related to the coag forced process, allowing for a comparative analysis between the two coagulation methods.

> [!NOTE]
> This setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/monopolar%20coag%20spray%20and%20forced.png" alt="coag-spray-forced">
</p>

### B. Bipolar
#### B.1 Cut
#### B.2 Coag
##### B.2.2 Standard
- Input
- Output
<br>The image illustrates the output scope for the Bipolar Coag Standard operation at the power stage output. It provides a detailed view of the waveform characteristics and performance metrics associated with the bipolar coagulation process, highlighting the key features and behavior of the signal during this operation.

> [!NOTE]
> This setup uses a passive probe with a 10x attenuation factor and a voltage divider of 1/10, resulting in a total measurement factor of 100.

<p align="center">
  <img src="pic/Reverse%20Engineering/scope%20-%20generator%20output/bipolar%20coag%20standard.png" alt="coag-standard">
</p>


#### bipolar pure cut ???
T = 10µs/DIV * 8DIV ÷ 20 = 4µs
f = 1 ÷ 4µs = 250kHz

V = 50mV/DIV * 0.6DIV * 100 = 3V