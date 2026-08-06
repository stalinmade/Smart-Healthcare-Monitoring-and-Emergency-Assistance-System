# Smart Healthcare Monitoring and Emergency Assistance System

The **Smart Healthcare Monitoring and Emergency Assistance System** is an embedded systems project developed using the **LPC2129 ARM7TDMI-S microcontroller** to continuously monitor a patient's vital health parameters and generate emergency alerts when abnormal conditions are detected. The system provides an efficient and low-cost solution for real-time health monitoring, making it suitable for home healthcare, hospitals, and remote patient monitoring applications.

The project interfaces the **MAX30102 pulse oximeter sensor** using the **I²C communication protocol** to measure heart rate (BPM) and blood oxygen saturation (SpO₂). A body temperature sensor is also integrated to continuously monitor the patient's body temperature. The sensor data is collected by the LPC2129, processed using embedded firmware, and displayed on an LCD. Additionally, the measured values are transmitted through UART for debugging, logging, or communication with external devices.

To ensure reliable readings, the firmware continuously samples the sensor data and performs basic processing before calculating the final health parameters. The system compares the measured values against predefined threshold limits. Whenever an abnormal heart rate, low oxygen saturation, or high body temperature is detected, the controller immediately generates an emergency alert, enabling quick medical attention.

The firmware is written in **Embedded C** with a modular structure that simplifies maintenance and future enhancements. The project demonstrates practical implementation of embedded concepts such as GPIO control, timer configuration, UART communication, I²C protocol implementation, LCD interfacing, and sensor integration on the LPC2129 microcontroller.

This project serves as an excellent example of real-time embedded system design and can be further enhanced by integrating wireless communication modules such as GSM, Wi-Fi, or Bluetooth for remote patient monitoring and cloud-based healthcare applications.

## Key Features

* Real-time monitoring of Heart Rate (BPM), SpO₂, and Body Temperature.
* LPC2129 (ARM7TDMI-S) based embedded system.
* MAX30102 sensor interfaced using the I²C protocol.
* LCD display for live health parameter visualization.
* UART communication for debugging and data logging.
* Threshold-based emergency detection and alert generation.
* Modular Embedded C firmware for easy maintenance and scalability.
* Suitable for healthcare monitoring, embedded systems learning, and IoT applications.

**Technologies Used:** Embedded C, LPC2129, ARM7TDMI-S, MAX30102, I²C, UART, LCD Interfacing, Keil uVision, Sensor Integration.
