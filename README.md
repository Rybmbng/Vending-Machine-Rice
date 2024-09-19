# Vending-Machine-Rice
This Arduino Mega based rice vending machine is an automated solution for rice distribution, utilizing several integrated electronic components and sensors to produce an efficient and easy to use system. Following are the main components and their functions:

Arduino Mega: Acts as the main brain that controls the entire vending machine process, from reading input to activating the rice dispensing mechanism.

Keypad: Used as a user input tool to select the amount of rice or enter a certain code.

HX711: Load cell sensor to measure the weight of rice dispensed. This provides high accuracy in mass measurement.

MFRC522 (RFID): Used to read RFID cards as a user identification method, such as membership or payment cards.

Liquid Crystal I2C: This LCD screen functions to display information to the user, such as menu options, transaction status, and weight of rice issued.

Servo Motor: Controls the opening and closing mechanism of the valve or rice flow cover to release the desired amount of rice.


This vending machine allows users to select and take the desired amount of rice with high precision, utilizing a load cell to measure the weight dispensed. This system is suitable for implementation in public environments such as supermarkets, cooperatives, or social institutions, to facilitate automatic and measurable distribution of rice.

First, install the required libraries. Open Arduino IDE, then:

Go to Tools > Manage Libraries.
Find and install the following libraries:

`1.Keypads`

`2.LiquidCrystal_I2C`

`3.HX711`

`4.MFRC522`

`5.Servos`

Connect the components to the Arduino Mega according to the schematic diagram in the schematics folder in this repository.

Upload code to Arduino:

`1.Open the vending_machine_beras.ino file from this repository in the Arduino IDE.`

`2.Connect the Arduino Mega to the computer using a USB cable.`

`3.Select the Arduino Mega board and appropriate port from the Tools menu.`

`4.Click the Upload button to start the process of uploading the code to the Arduino device.`



Connection Diagram

![image](https://github.com/user-attachments/assets/eb73c954-bc17-4f25-b6ac-ce31fe1f9308)

