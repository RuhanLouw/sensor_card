


// Buffer for receiving response
static volatile uint8_t rxBuffer[MAX_MESSAGE_LEN];
static volatile uint8_t rxIndex = 0;
static volatile bool responseReceived = false;

// UART receive ISR for master
void UART1_RxISR(void) {
    uint8_t rxByte = UART1_Read(); // Read byte from UART1
    if (rxIndex Concentrations of 0 && rxByte != START_BYTE) {
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
        uint8_t expectedLen = 5 + rxBuffer[2]; // Total message size
        if (rxIndex >= expectedLen && rxBuffer[expectedLen - 1] == END_BYTE) {
            responseReceived = true; // Flag complete response
            rxIndex = 0; // Reset for next
        }
    }
    if (rxIndex >= MAX_MESSAGE_LEN) {
        rxIndex = 0; // Prevent overflow
    }
}

// Send command to sensor PCB
void sendSelectedSensorsCommand(uint16_t sensorBitmask) {
    uint8_t buffer[7]; // Fixed 7-byte command
    buffer[0] = START_BYTE; // 0xAA
    buffer[1] = CMD_READ_SELECTED_SENSORS; // 0x0C
    buffer[2] = 0x02; // Payload length: 2 bytes
    buffer[3] = sensorBitmask >> 8; // High byte
    buffer[4] = sensorBitmask & 0xFF; // Low byte
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < 5; i++) {
        checksum += buffer[i]; // Calculate checksum
    }
    buffer[5] = checksum;
    buffer[6] = END_BYTE; // 0xBB
    for (uint8_t i = 0; i < 7; i++) {
        UART1_Write(buffer[i]); // Send command
    }
}

// Process received response
void processResponse(float* temperatures, uint8_t* sensorCount) {
    if (!responseReceived) {
        return; // No response to process
    }
    cli(); // Disable interrupts
    responseReceived = false;
    
    // Extract response fields
    uint8_t cmd = rxBuffer[1]; // Command ID
    uint8_t len = rxBuffer[2]; // Payload length
    uint8_t checksum = rxBuffer[3 + len]; // Received checksum
    
    // Calculate checksum
    uint8_t calcChecksum = 0;
    for (uint8_t i = 0; i < 3 + len; i++) {
        calcChecksum += rxBuffer[i];
    }
    
    // Verify checksum and end byte
    if (checksum != calcChecksum || rxBuffer[4 + len] != END_BYTE || cmd != CMD_READ_SELECTED_SENSORS) {
        sei();
        return; // Invalid response
    }
    
    // Decode temperature data (2 bytes per sensor, int16_t)
    *sensorCount = len / 2; // Number of sensors (2 bytes each)
    for (uint8_t i = 0; i < *sensorCount; i++) {
        // Combine high and low bytes into int16_t
        int16_t temp = ((int16_t)rxBuffer[3 + 2*i] << 8) | rxBuffer[4 + 2*i];
        // Convert to °C (divide by 10)
        temperatures[i] = (float)temp / 10.0;
    }
    sei(); // Re-enable interrupts
}

// Main function
int main(void) {
    SYSTEM_Initialize(); // Initialize MCC modules
    UART1_SetRxISR(UART1_RxISR); // Set UART receive ISR
    sei(); // Enable global interrupts
    
    // Example: Request NTC 1 and K-type
    uint16_t sensorBitmask = SENSOR_NTC_1 | SENSOR_KTYPE; // 0x0101
    float temperatures[SENSOR_COUNT]; // Store decoded temperatures
    uint8_t sensorCount; // Number of temperatures received
    
    while (1) {
        sendSelectedSensorsCommand(sensorBitmask); // Send command
        _delay_ms(RESPONSE_TIMEOUT_MS); // Wait for response
        processResponse(temperatures, &sensorCount); // Decode response
        // Example: Use temperatures[0] (NTC 1), temperatures[1] (K-type)
    }
}



