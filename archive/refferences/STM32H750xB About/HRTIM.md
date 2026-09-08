# Datasheet - production data STM32H750VB STM32H750ZB STM32H750IB STM32H750XB
## 3 Functional Overview
### 3.29 Timers and watchdogs
#### 3.29.1 High-resolution timer (HRTIM1)

The high-resolution timer (HRTIM1) allows generating digital signals with high-accuracy timings, such as PWM or phase-shifted pulses.

It consists of 6 timers, 1 master and 5 slaves, totaling 10 high-resolution outputs, which can be coupled by pairs for deadtime insertion. It also features 5 fault inputs for protection purposes and 10 inputs to handle external events such as current limitation, zero voltage or zero current switching.

The HRTIM1 timer is made of a digital kernel clocked at 480 MHz The high-resolution is available on the 10 outputs in all operating modes: variable duty cycle, variable frequency, and constant ON time.

The slave timers can be combined to control multiswitch complex converters or operate independently to manage multiple independent converters.

The waveforms are defined by a combination of user-defined timings and external events such as analog or digital feedbacks signals.

HRTIM1 timer includes options for blanking and filtering out spurious events or faults. It also offers specific modes and features to offload the CPU: DMA requests, Burst mode controller, Push-pull and Resonant mode.

It supports many topologies including LLC, Full bridge phase shifted, buck or boost converters, either in voltage or current mode, as well as lighting application (fluorescent or LED). It can also be used as a general purpose timer, for instance to achieve high-resolution PWM-emulated DAC.


# RM0433
## 37 High-Resolution Timer (HRTIM)
### 37.1 Introduction
The high-resolution timer can generate up to 10 digital signals with highly accurate timings. It is primarily intended to drive power conversion systems such as switch mode power supplies or lighting systems, but can be of general purpose usage, whenever a very fine timing resolution is expected.

Its modular architecture allows to generate either independent or coupled waveforms. The wave-shape is defined by self-contained timings (using counters and compare units) and a broad range of external events, such as analog or digital feedbacks and synchronization signals. This allows to produce a large variety of control signal (PWM, phase-shifted, constant Ton,...) and address most of conversion topologies.

For control and monitoring purposes, the timer has also timing measure capabilities and links to built-in ADC and DAC converters. Last, it features light-load management mode and is able to handle various fault schemes for safe shut-down purposes.

### 37.2 Main features
-   Multiple timing units
    - Full-resolution available on all outputs, possibility to adjust duty-cycle, frequency and pulse width in triggered one-pulse mode
    - 6 16-bit timing units (each one with an independent counter and 4 compare units)
    - 10 outputs that can be controlled by any timing unit, up to 32 set/reset sources per channel
    - Modular architecture to address either multiple independent converters with 1 or 2 switches or few large multi-switch topologies
-   Up to 10 external events, available for any timing unit
    - Programmable polarity and edge sensitivity
    - 5 events with a fast asynchronous mode
    - 5 events with a programmable digital filter
    - Spurious events filtering with blanking and windowing modes
-   Multiple links to built-in analog peripherals
    - 4 triggers to ADC converters
    - 3 triggers to DAC converters
    - 3 comparators for analog signal conditioning
-   Versatile protection scheme
    - 5 fault inputs can be combined and associated to any timing unit
    - Programmable polarity, edge sensitivity, and programmable digital filter
    - dedicated delayed protections for resonant converters
-   Multiple HRTIM instances can be synchronized with external synchronization inputs/outputs
-   Versatile output stage
    - Full-resolution Deadtime insertion
    - Programmable output polarity
    - Chopper mode
-   Burst mode controller to handle light-load operation synchronously on multiple converters
-   7 interrupt vectors, each one with up to 14 sources
-   6 DMA requests with up to 14 sources, with a burst mode for multiple registers update

### 37.3 Functional description
#### 37.3.1 General description
The HRTIM can be partitioned into several sub entities:
-   The master timer
-   The timing units (Timer A to Timer E)
-   The output stage
-   The burst mode controller
-   An external event and fault signal conditioning logic that is shared by all timers
-   The system interface

The master timer is based on a 16-bit up counter. It can set/reset any of the 10 outputs via 4 compare units and it provides synchronization signals to the 5 timer units. Its main purpose is to have the timer units controlled by a unique source. An interleaved buck converter is a typical application example where the master timer manages the phase-shifts between the multiple units.

The timer units are working either independently or coupled with the other timers including the master timer. Each timer contains the controls for two outputs. The outputs set/reset events are triggered either by the timing units compare registers or by events coming from the master timer, from the other timers or from external events.

The output stage has several duties
-   Addition of deadtime when the 2 outputs are configured in complementary PWM mode
-   Addition of a carrier frequency on top of the modulating signal
-   Management of fault events, by asynchronously asserting the outputs to a predefined safe level

The burst mode controller can take over the control of one or multiple timers in case of light- load operation. The burst length and period can be programmed, as well as the idle state of the outputs.

The external event and fault signal conditioning logic includes:
-   The input selection MUXes (for instance for selecting a digital input or an on-chip source for a given external event channel)
-   Polarity and edge-sensitivity programming
-   Digital filtering (for 5 channels out of 10)

The system interface allows the HRTIM to interact with the rest of the MCU:
-   Interrupt requests to the CPU
-   DMA controller for automatic accesses to/from the memories, including an HRTIM specific burst mode
-   Triggers for the ADC and DAC converters

The HRTIM registers are split into 7 groups:
-   Master timer registers
-   Timer A to Timer E registers
-   Common registers for features shared by all timer units

Note: As a writing convention, references to the 5 timing units in the text and in registers are generalized using the “x” letter, where x can be any value from A to E.



#### 37.3.2 HRTIM pins and internal signals
The table here below summarizes the HRTIM inputs and outputs, both on-chip and off-chip.

<table>
  <tr>
    <th>Signal name</th>
    <th>Signal type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>
        HRTIM_CHA1,<br>HRTIM_CHA2,<br>HRTIM_CHB1,<br>HRTIM_CHB2,<br>HRTIM_CHC1,<br>HRTIM_CHC2,<br>HRTIM_CHD1,<br>HRTIM_CHD2,<br>HRTIM_CHE1,<br>HRTIM_CHE2
    </td>
    <td>Output</td>
    <td>Main HRTIM timer outputs. They can be coupled by pairs (HRTIM_CHx1 & HRTIM_CHx2) with deadtime insertion or work independently.</td>
  </tr>
  <tr>
    <td>HRTIM_FLT[5:1],<br>hrtim_in_flt[5:1]</td>
    <td>Digital input</td>
    <td>Fault inputs: immediately disable the HRTIM outputs when asserted (5 on-chip inputs and 5 off-chip HRTIM_FLTx inputs).</td>
  </tr>
  <tr>
    <td>hrtim_sys_flt</td>
    <td>Digital input</td>
    <td>System fault gathering MCU internal fault events (Clock security system, SRAM parity error, Cortex®-M7 lockup (HardFault), PVD output).</td>
  </tr>
  <tr>
    <td>hrtim_in_sync[3:1]</td>
    <td>Digital Input</td>
    <td>
        <p>Synchronization inputs to synchronize the whole HRTIM with other internal or external timer resources:</p>
        <ul>
            <li>hrtim_in_sync1: reserved</li>
            <li>hrtim_in_sync2: the source is a regular TIMx timer (via on-chip interconnect)</li>
            <li>hrtim_in_sync3: the source is an external HRTIM (via the HRTIM_SCIN input pins)</li>
        </ul>
    </td>
  </tr>
  <tr>
    <td>hrtim_out_sync[2:1]</td>
    <td>Digital&nbsp;output</td>
    <td>
        <p>The purpose of this output is to cascade or synchronize several HRTIM instances, either on-chip or off-chip:</p>
        <ul>
            <li>hrtim_out_sync1: reserved</li>
            <li>hrtim_out_sync2: the destination is an off-chip HRTIM or peripheral (via HRTIM_SCOUT output pins)</li>
        </ul>
    </td>
  </tr>
  <tr>
    <td>
        hrtim_evt1[4:1]<br>hrtim_evt2[4:1]<br>hrtim_evt3[4:1]<br>hrtim_evt4[4:1]<br>hrtim_evt5[4:1]<br>hrtim_evt6[4:1]<br>hrtim_evt7[4:1]<br>hrtim_evt8[4:1]<br>hrtim_evt9[4:1]<br>hrtim_evt10[4:1]
    </td>
    <td>Digital input</td>
    <td>
        External events. Each of the 10 events can be selected among 4 sources, either on-chip (from other built-in peripherals: comparator, ADC analog watchdog, TIMx timers, trigger outputs) or off-chip (HRTIM_EEVx input pins)
    </td>
  </tr>
  <tr>
    <td>hrtim_upd_en[3:1]</td>
    <td>Digital input</td>
    <td>HRTIM register update enable inputs (on-chip interconnect) trigger the transfer from shadow to active registers</td>
  </tr>

  <tr>
    <td>hrtim_bm_trg</td>
    <td>Digital input</td>
    <td>Burst mode trigger event (on-chip interconnect)</td>
  </tr>
  <tr>
    <td>hrtim_bm_ck[4:1]</td>
    <td>Digital input</td>
    <td>Burst mode clock (on-chip interconnect)</td>
  </tr>
  <tr>
    <td>hrtim_adc_trg[4:1]</td>
    <td>Digital output</td>
    <td>ADC start of conversion triggers</td>
  </tr>
  <tr>
    <td>hrtim_dac_trg[3:1]</td>
    <td>Digital output</td>
    <td>DAC conversion update triggers</td>
  </tr>
  <tr>
    <td>hrtim_mst_it[7:1]</td>
    <td>Digital output</td>
    <td>Interrupt requests</td>
  </tr>
  <tr>
    <td>hrtim_dma[6:1]</td>
    <td>Digital output</td>
    <td>DMA requests</td>
  </tr>
  <tr>
    <td>hrtim_pclk</td>
    <td>Digital input</td>
    <td>APB clock</td>
  </tr>
  <tr>
    <td>hrtim_ker_ck</td>
    <td>Digital input</td>
    <td>HRTIM&nbsp;kernel&nbsp;clock&nbsp;(hereafter&nbsp;mentioned&nbsp;as&nbsp;f<sub>HRTIM)</sub></td>
  </tr>
</table>

#### 37.3.3 HRTIM pins and internal signals
The HRTIM must be supplied by the tHRTIM system clock to offer a full resolution. All clocks present in the HRTIM are derived from this reference clock.

##### Definition of terms
<table>
    <tr>
        <td>f<sub>HRTIM</sub></td>
        <td>
            main HRTIM clock (hrtim_ker_ck). All subsequent clocks are derived and synchronous with this source.
        </td>
    </tr>
    <tr>
        <td>f<sub>DTG</sub></td>
        <td>
            deadtime generator clock. For convenience, only the tDTG period (tDTG = 1/fDTG) is used in this document.
        </td>
    </tr> 
    <tr>
        <td>f<sub>CHPFRQ</sub></td>
        <td>chopper stage clock source.</td>
    </tr>
    <tr>
        <td>f<sub>1STPW</sub></td>
        <td>
            clock source defining the length of the initial pulse in chopper mode. For convenience, only the t<sub>1STPW</sub> period (t<sub>1STPW</sub> = 1/f<sub>1STPW</sub>) is used in this document.
        </td>
    </tr>
    <tr>
        <td>f<sub>BRST</sub></td>
        <td>burst mode controller counter clock.</td>
    </tr>
    <tr>
        <td>f<sub>SAMPLING</sub></td>
        <td>clock needed to sample the fault or the external events inputs.</td>
    </tr>
    <tr>
        <td>f<sub>FLTS</sub></td>
        <td>clock derived from fHRTIM which is used as a source for fSAMPLING to filter fault events.</td>
    </tr>
    <tr>
        <td>f<sub>EEVS</sub></td>
        <td>clock derived from fHRTIM which is used as a source for fSAMPLING to filter external events.</td>
    </tr>
    <tr>
        <td>f<sub>pclk</sub>(hrtim_pclk)</td>
        <td>APB bus clock, needed for register read/write accesses</td>
    </tr>
</table>

##### Timer clock and prescaler
Each timer in the HRTIM has its own individual clock prescaler, which allows you to adjust the timer resolution. (See Table 304).

<div align="center">
Table 304. Timer resolution and min. PWM frequency for fHRTIM = 400 MHz
<table>
  <tr>
    <th>CKPSC[2:0]<sup>(1)</sup></th>
    <th>Prescaling ratio</th>
    <th>f<sub>COUNTER</sub></th>
    <th>Resolution</th>
    <th>Min PWM frequency</th>
  </tr>
  <tr>
    <td align="center">101</td>
    <td align="center">1</td>
    <td align="right">400 MHz</td>
    <td align="right">2.5 ns</td>
    <td align="right">6.1 kHz</td>
  <tr>
  <tr>
    <td align="center">110</td>
    <td align="center">2</td>
    <td align="right">400/2 MHz = 200 MHz</td>
    <td align="right">5 ns</td>
    <td align="right">3.05 kHz</td>
  <tr>
  <tr>
    <td align="center">111</td>
    <td align="center">4</td>
    <td align="right">400/4 MHz = 400 MHz</td>
    <td align="right">10 ns</td>
    <td align="right">1.5 kHz</td>
  <tr>
</table>
</div>
1. CKPSC[2:0] values from 000 to 100 are reserved.

The Full-resolution is available for edge positioning, PWM period adjustment and externally
triggered pulse duration.

