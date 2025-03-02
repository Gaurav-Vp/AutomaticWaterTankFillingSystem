# **Water Pump Automation using Arduino**

## **Overview**
This project automates a **water pump system** using **Arduino** and **74HC165 Shift Registers** to monitor water levels in **four tanks** and control **solenoid valves** accordingly. The system ensures that water is pumped into tanks when the levels are low and stops when they reach full capacity. It also includes an interrupt system to start and stop the process.

🔗 **Live Simulation**: [View on Wokwi](https://wokwi.com/projects/393050034647400449)

---

## **Features**
✅ **Automatic Water Pump Control**  
✅ **Monitors Water Levels for Four Tanks**  
✅ **Uses Shift Registers (74HC165) for Sensor Input**  
✅ **Controls Four Solenoid Valves**  
✅ **Interrupts for Manual Start/Stop**  
✅ **Efficient and Reliable Operation**  

---

## **Hardware Requirements**
- **Arduino Board** (Uno, Mega, etc.)
- **74HC165 Shift Register** (for water level sensors)
- **Water Level Sensors** (for each tank)
- **4 Solenoid Valves**
- **Water Pump**
- **Relays** (for switching solenoid valves and pump)
- **Push Buttons** (for manual start/stop)

---

## **Pin Configuration**
### **Solenoid Valves & Pump**
| Component             | Arduino Pin |
|----------------------|------------|
| Solenoid Valve 1    | 4          |
| Solenoid Valve 2    | 5          |
| Solenoid Valve 3    | A4         |
| Solenoid Valve 4    | A5         |
| Water Pump         | 6          |

### **Shift Register (74HC165) - Water Level Sensors**
| Tank  | 20% Sensor | 50% Sensor | 100% Sensor |
|-------|-----------|-----------|------------|
| **Tank 1** | 0 | 1 | 2 |
| **Tank 2** | 3 | 4 | 5 |
| **Tank 3** | 6 | 7 | 8 |
| **Tank 4** | 9 | 10 | 11 |

### **Interrupt Pins**
| Function       | Pin |
|--------------|----|
| Start Button | 2  |
| Stop Button  | 3  |

---

## **Software Implementation**
The **Arduino program** performs the following functions:

1. **Reads Water Levels** using the 74HC165 Shift Register.
2. **Controls Solenoid Valves** based on tank water levels:
   - Opens valve if the tank is **≤ 20%** full.
   - Closes valve when the tank reaches **100%**.
3. **Manages the Water Pump**:
   - Turns **ON** if any solenoid valve is open.
   - Turns **OFF** if all solenoid valves are closed.
4. **Interrupt-Based Control**:
   - **START Button** (Pin 2) → Enables automation.
   - **STOP Button** (Pin 3) → Stops all operations.

---

## **Code Explanation**
- `setup()`: Initializes **pins, interrupts, and shift register**.
- `loop()`: Continuously checks tank levels and updates the pump/valves.
- `checkWaterLevelStatus()`: Reads water level sensors via **74HC165 shift register**.
- `tank_1_Logic()`, `tank_2_Logic()`, etc.: Control solenoid valves based on tank levels.
- `pumpLogic()`: Ensures the pump operates only when needed.
- `updateRelayStatus()`: Activates/deactivates relays for valves and pump.
- `read_state()`: Reads **shift register inputs**.
- **Interrupt Handlers (`START_ISR()` & `STOP_ISR()`)**:
  - Start/stop automation via external buttons.

---

## **Installation & Usage**
### **1️⃣ Upload the Code**
- Open the `.ino` file in **Arduino IDE**.
- Select the correct **board** and **COM port**.
- Upload the code.

### **2️⃣ Connect the Circuit**
- Wire up the **shift register**, **sensors**, **relays**, and **pump** as per the schematic.

### **3️⃣ Operation**
- **Press "Start"** (Pin 2) to begin automation.
- **Water levels monitored**, valves & pump controlled automatically.
- **Press "Stop"** (Pin 3) to disable the system.

---

## **Simulation**
- You can **test this project** in the **Wokwi simulator**:  
  🔗 [View Simulation](https://wokwi.com/projects/393050034647400449)

---

## **Future Improvements**
🔹 Add an **LCD display** for real-time tank levels  
🔹 Use an **IoT module (ESP8266/NodeMCU)** for **remote monitoring**  
🔹 Implement a **mobile app or web dashboard**  
🔹 Include **water flow sensors** for precise monitoring  

--- 

## **License & Usage Restrictions**  
📌 **This project is NOT open source.**  
📌 Unauthorized distribution, reproduction, or modification is strictly prohibited.  
📌 **For personal or internal use only.** Commercial use requires **explicit permission**.  

📧 **For inquiries, contact: gaurav18v.com@gmail.com**  

---

## **Author**  
👤 **Gaurav Sharma**  

---
