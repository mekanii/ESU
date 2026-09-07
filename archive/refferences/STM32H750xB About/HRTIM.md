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
    <td>
        Output
    </td>
    <td>
        Main HRTIM timer outputs. They can be coupled by pairs (HRTIM_CHx1 & HRTIM_CHx2) with deadtime insertion or work independently.
    </td>
  </tr>
  <tr>
    <td></td>
    <td></td>
    <td></td>
  </tr>
</table>