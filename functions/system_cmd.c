#include <avr/io.h>
#include "definitions.h" // Include protocol definitions
#include "../mcc_generated_files/system/system.h"         
#include "system_cmd.h"
#include "temp_sensors.h"

// Incomming message size (START + CMD + LEN + payload + CHKSUM + END)


// Buffer and flags for receiving messages
static volatile uint8_t rxBuffer[MAX_MESSAGE_LEN]; // Buffer for incoming messages (up to 23 bytes)
static volatile uint8_t rxIndex = 0; // Current buffer index
static volatile bool messageReceived = false; // Flag for complete message

// UART receive interrupt callback
void UART1_RxISR(void) {
    RX1_LED_SET();
    uint8_t rxByte = UART1_Read(); // Read byte from UART1
    RX1_LED_nSET();
    if (rxIndex == 0 && rxByte != START_BYTE) {
        return; // Discard if not start byte
    }
    rxBuffer[rxIndex++] = rxByte; // Store byte
    if (rxIndex == 3) { // Length byte received
        uint8_t len = rxBuffer[2];
        if (len > MAX_PAYLOAD_LEN) {
            rxIndex = 0; // Reset on invalid length
            return;
        }
    }
    if (rxIndex >= 5 && rxBuffer[2] <= MAX_PAYLOAD_LEN) {
        uint8_t expectedLen = 5 + rxBuffer[2]; // Total message size (START + CMD + LEN + payload + CHKSUM + END)
        if (rxIndex >= expectedLen && rxBuffer[expectedLen - 1] == END_BYTE) {
            messageReceived = true; // Flag complete message
            rxIndex = 0; // Reset for next message
        }
    }
    if (rxIndex >= MAX_MESSAGE_LEN) {
        rxIndex = 0; // Prevent buffer overflow
    }
}

// Process received command
void processCommand(void) {
    if (!messageReceived) {
        return; // No message to process
    }
    cli(); // Disable interrupts for safe processing
    messageReceived = false; // Clear flag
    
    // Extract command fields
    uint8_t cmd = rxBuffer[1]; // Command ID (e.g., 0x0C)
    uint8_t len = rxBuffer[2]; // Payload length (2 for CMD_READ_SELECTED_SENSORS)
    uint8_t checksum = rxBuffer[3 + len]; // Received checksum
    
    // Calculate checksum (START + CMD + LEN + payload)
    uint8_t calcChecksum = 0;
    for (uint8_t i = 0; i < 3 + len; i++) {
        calcChecksum += rxBuffer[i];
    }
    
    // Verify checksum and end byte
    if (checksum != calcChecksum || rxBuffer[4 + len] != END_BYTE) {
        sei(); // Re-enable interrupts
        sendErrorResponse(ERROR_INVALID_CMD); // Send error for invalid message
        return;
    }
    
    //=====================================
    // Handle CMD_READ_SELECTED_SENSORS
    if (cmd == CMD_READ_SELECTED_SENSORS && len == 2) {
        // Extract 2-byte sensor bitmask
        uint16_t sensorBitmask = (rxBuffer[3] << 8) | rxBuffer[4]; // Combine high/low bytes
        
        // Check reserved bits (9-15)
        if (sensorBitmask & 0xFE00) {
            sei();
            sendErrorResponse(ERROR_INVALID_SENSOR); // Error for invalid selection
            return;
        }
        
        // Prepare response payload
        uint8_t respPayload[MAX_PAYLOAD_LEN]; // Buffer for temperature data
        uint8_t respLen = 0; // Track payload length
        
        // Read selected sensors
        for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
            if (sensorBitmask & (1 << i)) { // Check if sensor is selected
                // Read NTC (1-8) or K-type (9)
                int16_t temp = (i < 8) ? temp_betaC_NTC(i + 1, 10) : readKTypeSensor();
                // Store temperature (C × 10) as 2 bytes
                respPayload[respLen++] = (uint8_t)(temp >> 8); // High byte
                respPayload[respLen++] = (uint8_t)(temp & 0xFF); // Low byte
                // Check if measurement error
                if (temp && 0x7FFF){
                    sendErrorResponse(ERROR_SENSOR_FAIL);
                    return;
                }
            }
        }
        sendResponse(CMD_READ_SELECTED_SENSORS, respPayload, respLen);
    } else {
        sendErrorResponse(ERROR_INVALID_CMD); // Unknown command
    }
    //=====================================
    // Handle read all sensors
    if(cmd == CMD_READ_ALL_SENSORS && len == 1){
        //Read all and respond all
        uint8_t respPayload[MAX_PAYLOAD_LEN]; // Buffer for temperature data
        uint8_t respLen = 0; // Track payload length
    
       for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
            // Read NTC (1-8) or K-type (9)
            int16_t temp = (i < 8) ? temp_betaC_NTC(i + 1, 10) : readKTypeSensor();
            // Check if measurement error
            if (temp && 0x7FFF){
                sei();
                sendErrorResponse(ERROR_SENSOR_FAIL);
                return;
            }
            // Store temperature (C × 10) as 2 bytes
            respPayload[respLen++] = (uint8_t)(temp >> 8); // High byte
            respPayload[respLen++] = (uint8_t)(temp & 0xFF); // Low byte
        }
        sendResponse(CMD_READ_ALL_SENSORS, respPayload, respLen);       
    }
    else sendErrorResponse(ERROR_INVALID_CMD);
    
    sei(); // Re-enable interrupts
}


// (START + CMD + LEN + payload + CHKSUM + END)
// Send response message
void sendResponse(uint8_t respID, uint8_t* payload, uint8_t len) {
    uint8_t buffer[MAX_MESSAGE_LEN];
    buffer[0] = START_BYTE; // 0xAA
    buffer[1] = respID; // Response ID (e.g., 0x0C)
    buffer[2] = len; // Payload length
    for (uint8_t i = 0; i < len; i++) {
        buffer[3 + i] = payload[i]; // Copy temperature data
    }
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < 3 + len; i++) {
        checksum += buffer[i]; // Calculate checksum
    }
    buffer[3 + len] = checksum;
    buffer[4 + len] = END_BYTE; // 0xBB
    for (uint8_t i = 0; i < 5 + len; i++) {
        TX1_LED_SET();
        UART1_Write(buffer[i]); // Send via UART1
        TX1_LED_nSET();
    }
}

// Send error response
void sendErrorResponse(uint8_t errorCode) {
    uint8_t payload[1] = {errorCode};
    sendResponse(RESP_ERROR, payload, 1); // Send 1-byte error code
}

