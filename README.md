# msp430-assembly




#  MSP430 Assembly Project - Assignment 4: FSM, Interrupts, and LPM

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

#  MSP430 Assembly Project - Assignment 5: Timers, LCD, and Frequency Counter

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




