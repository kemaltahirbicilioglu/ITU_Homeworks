# Computer Organization – Project Repository (2023-2024)

This repository contains the code implementations and assignment descriptions for the **2023-2024 Computer Organization** course projects.

## 📌 How to Use

- Each project is organized into separate folders.
- Inside each folder, you will find:
  - **Assignment Description:** The details of the project.
  - **Code:** The Verilog implementation.

## 🛠 Project 1 Overview

### Implemented Files:
- **`Register.v`** → Implements a basic **register**, one of the fundamental components of the basic computer. It serves as a foundational building block for later components.  
- **`InstructionRegister.v`** → Implements the **instruction register**, which will hold instructions. Since the bus is **8-bit**, separate cases exist for **LSB** and **MSB** loading.  
- **`RegisterFile.v`** → A component containing multiple registers that can later be used as scratch registers. The activation timing of each register is specified in the assignment PDF. **FunSel** values match those of the registers.  
- **`AddressRegisterFile.v`** → A component that holds address-related registers such as **PC, AR, SP**. The assignment PDF specifies when each register should be active.  
- **`ArithmeticLogicUnit.v`** → Executes arithmetic operations based on **FunSel**. The key point is that **operation results** are computed on the **rising edge of the clock**, while **flags are updated asynchronously**. This behavior was verified through the provided simulation tests.  
- **`ArithmeticLogicUnitSystem.v`** → Integrates all components. **Multiplexers** are implemented to select the appropriate input wires. Essentially, previous components act like **class definitions**, and this file creates their **instances** and establishes necessary connections.  

## ⚠ Important Notes

- The **example submission** contains a detailed project report.  
- Understanding the **simulation files** is crucial for smooth progress in the Verilog project.  
- Writing custom simulation files helps debug **wire connections** and verify correct/incorrect behaviors.  

---

## 🛠 Project 2 Overview

## ⚠ Before Starting

Before working on **Project 2**, ensure that all errors from **Project 1** have been fixed.  
Since **Project 2** focuses entirely on executing instructions within the system built in **Project 1**, it's essential to verify that no components are misconnected in the first project.  

## 📂 Provided Files

- **`CPUsystem.v`** → Implements the complete CPU system, handling instruction execution.  
- **`comporg_project2.pdf`** → Contains quick-access **tables** detailing which operation is performed for each selection and which registers are used.  

## 🔑 Key Points of the Project

- **Disable Units:** These components **disable unintended active components** from the previous clock cycle, ensuring that only the required components are enabled in the current cycle.  
- **Clock Cycle Tracking:** The **sequential counter** is fed into a **decoder**, and the output is taken from **T**, allowing easy tracking of the current clock cycle.  
- **Instruction Fetch & Decode:** Executed at **T0** and **T1**.  
- **Instruction Types:**  
  - **Type 1:** Instructions without a **destination register**. The exact operations performed in each clock cycle are detailed in the comments.  
  - **Type 2:** Instructions with a **destination register**, following a structured execution approach:  
    - At **T2-T3**, the **source register** is determined.  
    - At **T4**, the **source registers** are transferred to **scratch registers**.  
    - At **T4**, ALU operations are applied based on the instruction, with **FunSel** and **destination registers** selected accordingly.  
    - Results are written to the necessary registers efficiently, avoiding redundant code for each Type 2 instruction.  
    - For Type 2 instructions **without two sources**, a **one-cycle delay** is applied in T3, ensuring all Type 2 instructions follow a unified execution structure.  

---

Feel free to explore and use the materials as needed! 🚀  
