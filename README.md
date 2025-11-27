#  MSP430 Assembly Mega Project: FSM, Timers, ADC, & Signal Analysis

This project develops a complex embedded system on the **MSP430G2553**, written in Assembly. The system is built incrementally, where each of the three major assignments **depends on the preceding one**, forming a robust knowledge base.

**Features Summary:**
1. **FSM Core:** Interrupt-driven state management.
2. **Timers & Frequency:** Countdown Timer (Timer\_A0) and Frequency Counter (Timer\_A1 Capture).
3. **ADC & Signal Analysis:** Continuous sampling (ADC10/DTC), $V_{avg}$ and Offset calculation, and signal classification.



#  MSP430 Assembly Project - Assignment 1: FSM, Interrupts, and LPM

This project implements a **Finite State Machine (FSM)** on the **Texas Instruments MSP430G2553** microcontroller, written entirely in **Assembly** language (.s43). State transitions are controlled by external interrupts from push buttons, and the system utilizes a Low Power Mode (LPM) for energy conservation.

---

##  Project Goal

* Develop a software architecture based on a **Finite State Machine (FSM)** to manage the microcontroller's operations.
* Utilize **Interrupts** from the GPIO Port 2 (PBs) to trigger and control the FSM state transitions.
* Enter **Low Power Mode 0 (LPM0)** when the system is not actively executing a task (**State 0 / Idle**).

---

##  Hardware (MSP430G2553)

| Component | Connection | Purpose |
| :--- | :--- | :--- |
| **LEDs** | **Port 1 (P1OUT)** | Display the current FSM state's operation. |
| **Push-Buttons (PBs)** | **Port 2 (P2IN)** | Trigger interrupts to control state transitions. |
| PB0 (P2.0) | Input/Interrupt | Advances the FSM to the next state in the sequence. |
| PB1 (P2.1) | Input/Interrupt | Resets the FSM to the Idle state (State 0) and enters LPM0. |

---

## 💡 Finite State Machine (FSM) Structure

The FSM is implemented in the main loop within `main.s43` and is controlled by the global variable **`state`**. All state changes are driven exclusively by the **Port 2 Interrupt Service Routine (`PBs_handler` in `hal.s43`)**, which updates the `state` variable and wakes up the MCU.

### State Transitions (PBs_handler Logic):

| Button (Port 2) | Current State (`state`) | New State (`state`) | Description |
| :---: | :---: | :---: | :--- |
| **PB0 (P2.0)** | `0x00` (Idle) | `0x01` (State 1) | Start operation (move to LED counting). |
| **PB0 (P2.0)** | `0x01` → `0x02` → `0x04` | **Cyclic Transition** (Returns to `0x01`). |
| **PB1 (P2.1)** | `0x01` or `0x02` or `0x04` | `0x00` (Idle) | Stop operation and return to Low Power Mode. |

### State Descriptions (API Implementation):

| Value (`state`) | State Name | Operation Description (API) | File |
| :---: | :--- | :--- | :--- |
| `0x00` | **State 0 (Idle/LPM)** | Turns off all LEDs (`ClrLEDs`) and enters **LPM0** (`bis.w #CPUOFF+GIE,SR`). | `main.s43` |
| `0x01` | **State 1** | **LED Increment Counting:** Lights LEDs sequentially (State 1 logic). | `api.s43` |
| `0x02` | **State 2** | **LED Running Light:** Display pattern moves from left to right (State 2 logic). | `api.s43` |
| `0x04` | **State 3** | **Frequency Output:** Activates a function for controlling frequency output (State 3 logic). | `api.s43` |

---

## 📂 File Structure

| File | Description |
| :--- | :--- |
| **`main.s43`** | Contains the main program loop and the implementation of the **Finite State Machine (FSM)**. |
| **`api.s43`** | Contains the core functions (API) that represent the operation of each state (e.g., `STATE1`, `STATE2`, `STATE3`). |
| **`hal.s43`** | Hardware Abstraction Layer (HAL). Includes I/O functions (`Print1LEDs`, `ClrLEDs`), delay functions, and the **Port 2 Interrupt Service Routine (`PBs_handler`)**. |
| **`bsp.s43`** | Board Support Package (BSP). Contains peripheral component initialization (e.g., `GPIOCONFI`) and the **Interrupt Vector Table** (`PORT2_VECTOR`, `RESET_VECTOR`). |
| **`bsp.h`** | Header file defining memory addresses, registers, and constant values (e.g., LED/PB abstractions, delay constants). |

---

#  MSP430 Assembly Project - Assignment 2: Timers, LCD, and Frequency Counter

This project is the next iteration of the Finite State Machine (FSM) architecture on the **Texas Instruments MSP430G2553** microcontroller, written in **Assembly** language (.s43). This phase introduces peripherals essential for real-time applications, specifically **Timers** and **LCD Display**.

---

##  Project Objectives

This assignment expands the system's capabilities by implementing **Timer-driven** and **LCD-based** functionalities:

* **LCD Driver Implementation:** Implementing a full **LCD Driver** to display dynamic data.
* **Time-Based Operation (State 1):** Utilizing **Timer\_A0** (Compare Mode) to create an accurate **Countdown Timer**.
* **Frequency Measurement (State 2):** Utilizing **Timer\_A1** in **Capture Mode** to measure the frequency of an external signal, as specified in the lab requirements.
* **Energy Efficiency:** Maintaining the use of **Low Power Mode 0 (LPM0)** when the system is idle.

---

##  Hardware & I/O Configuration

| Component | Main Connections | Configuration / Purpose |
| :--- | :--- | :--- |
| **Microcontroller** | **MSP430G2553** | Core processing unit. |
| **16x2 LCD Screen** | **Port 1 / Port 2** | Used for data display (e.g., timer value, measured frequency). |
| **Push-Buttons (PBs)** | **Port 2 (P2IN)** | Trigger interrupts for FSM state transitions (similar to Assignment 4). |
| **External Signal Input** | **P2.4 (TA1.2)** | Input pin for the external signal used in **Timer\_A1 Capture Mode** for frequency counting. |

---

##  Finite State Machine (FSM) Overview

The FSM is controlled by the global **`state`** variable, with transitions managed by the **Port 2 Interrupt Service Routine** in `hal.s43`.

### FSM States and Timer Usage

| Value (`state`) | State Name | Primary Operation (API) | Timer Configuration |
| :---: | :--- | :--- | :--- |
| `0x00` | **State 0 (Idle/LPM)** | Clears the LCD and enters **LPM0** to conserve power. | None (Awakens via PBs interrupt) |
| `0x01` | **State 1 (Countdown Timer)** | **Implements a Minutes:Seconds countdown.** Uses **Timer\_A0** in Compare Mode to generate precise, periodic interrupts that decrement the counter value, which is then printed to the LCD. | Timer\_A0 (Compare Mode) |
| `0x02` | **State 2 (Frequency Counter)** | **Measures the input signal frequency ($100Hz \le f \le 20kHz$).** Uses **Timer\_A1** in **Capture Mode** on pin P2.4. The Capture ISR (`ISR_HENDEL1`) calculates the frequency from the measured period and updates the LCD. | Timer\_A1 (Capture Mode) |

---

##  Project File Structure

| File | Description |
| :--- | :--- |
| **`main.s43`** | **FSM Core Logic.** Contains the main program loop, FSM structure, and global variables (`state`, LCD coordinates, display strings). |
| **`api.s43`** | **State Logic & LCD Print.** Contains the implementation of the main states (`STATE1`, `STATE2`) and LCD output routines (`print_str`, `print_ch`). |
| **`hal.s43`** | **HAL/Drivers & ISRs.** Includes the **LCD Driver** functions, the **Timer Interrupt Service Routines** (`ISR_HENDEL0`, `ISR_HENDEL1`), and necessary **mathematical routines** for division (`DIV16`) and frequency calculation (`clculate`). |
| **`bsp.s43`** | **Configuration.** Contains system initialization routines (`GPIOCONFI`), and most importantly, the **Timer configurations** (`TIMEconfig`, `TIMEconfing2`) and the **Interrupt Vector Table** updates. |
| **`bsp.h`** | **Definitions.** Header file defining all register addresses, constants, and hardware pin abstractions (LCD pins, PBs). |
| **`Preparation report LAB5.pdf`** | Official assignment document detailing the **Frequency Capture** requirements. |
| **`Tutorial 5.1 - Basic Timer.pdf`** | Reference material on Basic Timer concepts. |
| **`Tutorial 6 - Advanced Timers.pdf`** | Reference material on Advanced Timers (Timer\_A/B) including **Capture/Compare** modes. |

---

#  MSP430 Assembly Project - Assignment 3: ADC, Voltage Measurement, and Signal Analysis

This phase of the project introduces the crucial capability of interfacing with the analog world using the **Analog-to-Digital Converter (ADC)**. Written entirely in **Assembly** language (.s43), this assignment focuses on continuous sampling, voltage measurement, and classification of the input signal shape.

---

##  Project Objectives

The primary goals of this assignment are:

* **ADC Integration:** Configure and utilize the **ADC10** module for continuous sampling of an external analog voltage.
* **Data Transfer (DTC):** Use the Data Transfer Controller (DTC) for automatic, CPU-independent storage of multiple ADC samples into RAM.
* **Average Voltage (Vavg):** Implement complex **Fixed-Point Arithmetic** routines (e.g., UQ12.20) to calculate the signal's **Average Voltage (Vavg)** and display it on the LCD.
* **Signal Offset/DC Bias:** Implement a routine to calculate the signal's **DC Offset** based on maximum and minimum sampled values.
* **Signal Classification:** Analyze the sampled data to classify the signal shape (e.g., PWM, Sine, Triangle) and display the result.

---

##  Hardware & I/O Configuration

| Component | Main Connections | Configuration / Purpose |
| :--- | :--- | :--- |
| **Microcontroller** | **MSP430G2553** | Core processing unit. |
| **ADC Input** | **P1.3 (A3 Channel)** | Analog input pin for connecting the external signal source. Configured as **INCH\_3** for the ADC10 module. |
| **16x2 LCD Screen** | **Port 1 / Port 2** | Used to display calculated values (`Vavg`, `Voffset`) and the classified signal shape. |
| **Push-Buttons (PBs)** | **Port 1** | Triggering interrupts for FSM state transitions. |

---

##  ADC10 Configuration Highlights

The `ADCconfig` routine in `bsp.s43` implements the following key configurations:

| Register Field | Value/Configuration | Description |
| :--- | :--- | :--- |
| **`ADC10CTL0`** | `MSC`, `ADC10ON`, `ADC10IE`, `ADC10SHT_3` | Multiple Sample Conversion, ADC ON, Interrupt Enable, 64 ADC clocks per sample. |
| **`ADC10CTL1`** | `CONSEQ_2`, `INCH_3`, `ADC10SSEL_3` | **Repeat Single Channel Mode** (`CONSEQ_2`) for continuous operation, **A3 Channel** selected, using **SMCLK** as the clock source. |
| **`ADC10DTC1`** | `100` or `255` (as suggested by files) | Data Transfer Controller configured to transfer a block of samples (`100` or `255` samples) automatically to the array pointed to by `ADC10SA` (`sample`). |

---

##  FSM States and Functionalities

The FSM is controlled by the global **`state`** variable (`state0`, `state1`, `state2`, `state3` in `main.s43`). The states are dedicated to the analysis and presentation tasks:

| State | Primary Functionality | Implementation Details |
| :---: | :--- | :--- |
| **State 0** | **Idle / LPM** | Clears the screen and enters Low Power Mode (LPM0), awaiting a button interrupt. |
| **State 1** | **Calculate Average Voltage ($\mathbf{V_{avg}}$)** | Initiates ADC sampling. Calculates $V_{avg}$ using the complex fixed-point formula. Displays the result with `STR_AVG` and `STR_VOLT` labels. |
| **State 2** | **Signal Shape Classification** | Analyzes the statistical characteristics (e.g., standard deviation, Min/Max) of the sampled data array (`sample`) to classify the input signal as **PWM**, **SINE**, or **TRIANGLE**. Displays the corresponding `STR_SHAPE`. |
| **State 3** | **Calculate Offset/DC Bias** | Scans the sampled data array to find the maximum ($V_{max}$) and minimum ($V_{min}$) ADC values. Calculates the offset as $(\mathbf{V_{max}} + \mathbf{V_{min}}) / 2$ and displays the converted voltage value on the LCD. |

---

##  Project File Structure

| File | Description |
| :--- | :--- |
| **`main.s43`** | **FSM Core & Data.** Defines the FSM loop, global variables, sample arrays (`sample`, `sample_result`), and string constants (`STR_SHAPE`, `STR_AVG`, etc.). |
| **`api.s43`** | **State Logic & LCD.** Contains the implementation of the main state functions (`STATE1`, `STATE2`, `STATE3`) and LCD output routines (`print_str`, `print_ch`). |
| **`hal.s43`** | **HAL/Drivers & ISRs.** Includes the ADC Interrupt Service Routine (`ISR_HENDLER_ADC`), LCD Driver, and crucial **mathematical routines** for fixed-point division (`DIV16`) and multiplication/calculations required for Vavg and Offset conversion. |
| **`bsp.s43`** | **Configuration.** Contains system initialization routines, including detailed **ADCconfig** for continuous sampling with DTC, and GPIO configurations. |
| **`bsp.h`** | **Definitions.** Header file defining all register addresses, constants, and hardware pin abstractions. |
| **`Preparation report LAB6.pdf`** | Official assignment document detailing the mathematical formulas for Vavg calculation and general ADC requirements. |
| **`final_lab6.docx`** | Summary report detailing the implementation choices, including the use of 255 samples and the method for calculating the **Offset/DC Bias** (Max+Min)/2. |






