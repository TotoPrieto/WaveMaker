# 🌊 WaveMaker

**WaveMaker** is a wave generator developed at the **LEICI Laboratory** of the **Facultad de Ingenieria - UNLP**, designed to simulate wave motion inside a small water tank. It was created to test various wave amplitudes and speeds, with the final goal of supporting the study of energy harvesting systems placed inside the tank (a piston-based generator – not part of this project).

## 🛠️ Technical Description

The system uses a vertical linear actuator driven by a stepper motor, moving a wedge inside the tank to generate waves. One limit switch defines the zero position (home), while the second acts as a safety limit in case of unexpected movement or step miscounts.

### 🧩 Components

- Arduino UNO  
- CNC Shield V3 for Arduino  
- A4988 Stepper Motor Driver  
- NEMA17 Stepper Motor, 1.8º (SC42STH47AC-504AF)  
- 24V 3A Power Supply  
- 2 Limit Switches  
- Power and Reset Buttons

### 💻 Software

The firmware was developed using **PlatformIO**, programmed in **C++** with the **Arduino** library.

Included features:

- Homing routine using limit switches  
- Stepper control for vertical movement along Z-axis  
- Error handling and movement boundaries  

## 🧱 3D Modeling

The mechanical structure was designed using **AutoCAD** and **Blender**. Below are 3D rendered images of the model:

### Side View  
![Side View](images/side-view.png)

### Front View  
![Front View](images/front-view.png)

### Bottom View  
![Bottom View](images/bottom-view.png)

## ⚙️ Installation

1. Clone this repository locally.
2. Open the project using [PlatformIO](https://platformio.org/).
3. Connect the Arduino UNO via USB.
4. Compile and upload the firmware.

## 📁 Project Structure
