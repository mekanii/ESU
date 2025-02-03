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

_Voltage Peak (Vp):_
<br>The Vp is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

> [!NOTE]
> In this analysis, the peak voltage (Vp) is calculated using a simple method that involves measuring the vertical divisions of the peak-to-peak voltage (Vpp) on the oscilloscope. To begin, one observes the oscilloscope display and counts the total number of vertical divisions that the waveform spans from the maximum positive peak to the maximum negative peak, which provides the peak-to-peak voltage (Vpp). The peak voltage (Vp) is then determined by dividing the Vpp by two, expressed mathematically as `Vp = Vpp ÷ 2`.<br><br>While this method is straightforward and convenient, it is important to recognize its limitations, particularly when dealing with asymmetric signals. In an asymmetric waveform, the positive and negative peaks may not be equal in magnitude. Consequently, simply dividing the Vpp by two assumes that the waveform is symmetrical around the zero voltage line, which is often not the case. This approach can lead to significant errors in the calculated peak voltage, as it does not account for the actual heights of the individual peaks. For instance, if the positive peak is much higher than the negative peak, the calculated Vp will not accurately reflect the true maximum voltage deviation from zero.

| Power | Vp     |
| ----: | -----: |
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

_Voltage Peak (Vp):_
<br>The Vp is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

> [!NOTE]
> In this analysis, the peak voltage (Vp) is calculated using a simple method that involves measuring the vertical divisions of the peak-to-peak voltage (Vpp) on the oscilloscope. To begin, one observes the oscilloscope display and counts the total number of vertical divisions that the waveform spans from the maximum positive peak to the maximum negative peak, which provides the peak-to-peak voltage (Vpp). The peak voltage (Vp) is then determined by dividing the Vpp by two, expressed mathematically as `Vp = Vpp ÷ 2`.<br><br>While this method is straightforward and convenient, it is important to recognize its limitations, particularly when dealing with asymmetric signals. In an asymmetric waveform, the positive and negative peaks may not be equal in magnitude. Consequently, simply dividing the Vpp by two assumes that the waveform is symmetrical around the zero voltage line, which is often not the case. This approach can lead to significant errors in the calculated peak voltage, as it does not account for the actual heights of the individual peaks. For instance, if the positive peak is much higher than the negative peak, the calculated Vp will not accurately reflect the true maximum voltage deviation from zero.

| Power | Blend 1 | Blend 2 |
| ----: | ------: | ------: |
|   10W |     70V |     80V | 0.7 , 0.8
|   20W |    100V |    100V | 1   , 1
|   30W |    120V |    120V | 1.2 , 1.2
|   40W |    130V |    140V | 1.3 , 1.4
|   50W |    150V |    160V | 1.5 , 1.6
|   60W |    160V |    170V | 1.6 , 1.7
|   70W |    180V |    180V | 1.8 , 1.8
|   80W |    190V |    190V | 1.9 , 1.9
|   90W |    195V |    195V | 1.95, 1.95
|  100W |    200V |    200V | 2   , 2

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

_Voltage Peak (Vp):_
<br>The Vp is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 1V per division (1V/div).

> [!NOTE]
> In this analysis, the peak voltage (Vp) is calculated using a simple method that involves measuring the vertical divisions of the peak-to-peak voltage (Vpp) on the oscilloscope. To begin, one observes the oscilloscope display and counts the total number of vertical divisions that the waveform spans from the maximum positive peak to the maximum negative peak, which provides the peak-to-peak voltage (Vpp). The peak voltage (Vp) is then determined by dividing the Vpp by two, expressed mathematically as `Vp = Vpp ÷ 2`.<br><br>While this method is straightforward and convenient, it is important to recognize its limitations, particularly when dealing with asymmetric signals. In an asymmetric waveform, the positive and negative peaks may not be equal in magnitude. Consequently, simply dividing the Vpp by two assumes that the waveform is symmetrical around the zero voltage line, which is often not the case. This approach can lead to significant errors in the calculated peak voltage, as it does not account for the actual heights of the individual peaks. For instance, if the positive peak is much higher than the negative peak, the calculated Vp will not accurately reflect the true maximum voltage deviation from zero.

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

_Voltage Peak (Vp):_
<br>The Vp is calculated by taking the vertical division setting on the oscilloscope and multiplying it by 100 (total measurement factor), along with the vertical scale of 500mV per division (500mV/div).

| Power | Vp     |
| ----- | ------ |
|    5W |        |
|   10W |        |
|   20W |        |
|   30W |        |
|   40W |        |
|   50W |        |
|   60W |        |