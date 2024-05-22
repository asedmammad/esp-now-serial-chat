#include <ESP8266WiFi.h>
#include <espnow.h>

// Define SENDER or RECEIVER to select which device this program will run on
#define SENDER
#define MAX_TRANSMISSION_COUNT 5 // Max number of retransmissions before giving up
#define MAX_MESSAGE_LENGTH 250 // Max supporte ESP-NOW message size is 250 bytes

#ifndef SENDER
    #define RECEIVER
#endif

// TODO: Change this to your own MAC addresses
#ifdef SENDER
    uint8_t broadcastAddress[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
#else
    uint8_t broadcastAddress[] = {0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
#endif

// Define variables to store message to be sent
String messageToSend;

// Define variables to store received message
char incomingMessageCString[MAX_MESSAGE_LENGTH];

short transmissionCount = 0;

/*
 * Callback when data is sent
 *
 * @param uint8_t * mac_addr
 * @param uint8_t sendStatus
 * @return void
 */
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  // Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Message was successfully sent.");
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print("Sent to MAC: ");
    Serial.println(macStr);
  }
  else if (transmissionCount < MAX_TRANSMISSION_COUNT){
    Serial.println("Retransmitting the message...");
    transmissionCount++;
    sendMessage(messageToSend);
  }
  else{
    Serial.println("Delivery failed, could not send the message.");
  }
}

/*
 * Callback when data is received
 *
 * @param uint8_t * mac
 * @param uint8_t *incomingData
 * @param uint8_t len
 * @return void
 */
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  // Serial.print("Bytes received: ");
  // Serial.println(len);

  // Set incoming message to incomingData
  memcpy(&incomingMessageCString, incomingData, len);

  Serial.println("Received message:");
  Serial.println(incomingMessageCString);
}

/**
 * Sends a message using esp_now
 *
 * @param String message to send
 * @return void
 */
void sendMessage(String message){
  uint8_t *buffer = (uint8_t*) message.c_str();
  size_t sizeBuff = sizeof(buffer) * message.length();

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, buffer, sizeBuff);
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.setTimeout(50);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

  if (Serial.available() > 0) {
    messageToSend = Serial.readString();  // read until timeout
    messageToSend.trim();                 // remove any \r \n whitespace at the end of the String
    transmissionCount= 0;
    sendMessage(messageToSend);
  }

}
