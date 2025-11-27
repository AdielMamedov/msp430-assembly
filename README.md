# msp430-assembly




# 🚀 MSP430 Assembly Project - Assignment 4: FSM, Interrupts, and LPM

This project implements a **Finite State Machine (FSM)** on the **Texas Instruments MSP430G2553** microcontroller, written entirely in **Assembly** language (.s43). State transitions are controlled by external interrupts from push buttons, and the system utilizes a Low Power Mode (LPM) for energy conservation.

---

## 🎯 Project Goal

* Develop a software architecture based on a **Finite State Machine (FSM)** to manage the microcontroller's operations.
* Utilize **Interrupts** from the GPIO Port 2 (PBs) to trigger and control the FSM state transitions.
* Enter **Low Power Mode 0 (LPM0)** when the system is not actively executing a task (**State 0 / Idle**).

---

## ⚙️ Hardware (MSP430G2553)

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
| **`Tutorial 4 - Interrupts , LPM.pdf`** | The lab preparation file providing the theoretical background on interrupts and operating modes. |
