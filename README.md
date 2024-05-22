# ESP-NOW Serial Chat using Two ESP8266 Devices

This repository contains an simple code that demonstrates how to use Espressif's ESP-NOW protocol to establish a chat application between two ESP8266 devices. This can be used to send and receive messages in serial communication softwares (serial monitor).

**Hardware Requirements**

* Two ESP8266 boards
* Serial cables connected to PCs or Android devices

**Software Requirements**

* Arduino IDE
* ESP8266 board support package

**Getting Started**

1. Clone this repository to your local machine.
2. Open the project in Arduino IDE and select the correct board (e.g., ESP8266, NodeMCU).
3. Change the sender and receiver MAC addresses to the approporiate values (You can find your board's MAC address using the included `macaddr.ino` sketch).
4. Upload the code to both devices, each with the other's MAC address in `broadcastAddress`.
5. Connect the serial cables to the boards and PCs/Mobile phones.
6. Use Arduino IDE's Serial monitor, putty, or android based serial communication softwares on one device to send messages to the other device.

**Troubleshooting**

* Check the serial monitor for any errors or issues.
* Verify that both devices are connected and configured correctly.
* Adjust the transmission count and message length as needed.

**Contributing**

Contributions are welcome! If you'd like to contribute, please fork this repository and submit a pull request with your changes.
