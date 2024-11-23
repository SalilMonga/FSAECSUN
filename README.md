# Test to Read CAN Bus Data

Starts a FreeRTOS task to read CAN bus data.

## How to Use

If you have never worked with ESP-IDF, please follow the [Getting Started with ESP-IDF Guide](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/get-started/vscode-setup.html).

### Otherwise:

1. **Set GPIO Pins:**

   - Ensure the GPIO pins in the `twai_general_config_t` configuration are set to the appropriate pins:
     ```c
     g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_23, TWAI_MODE_NORMAL);
     ```

2. **Set the Bitrate:**

   - Verify the bitrate is correctly configured based on your CAN bus system:
     ```c
     TWAI_TIMING_CONFIG_1MBITS();
     ```

3. **Update the Identifier:**

   - Adjust the identifier to match the data you need:
     ```c
     .rx_message.identifier == 0x640;
     ```

4. **Build and Flash the Code:**
   - Use the ESP-IDF Explorer to build the project and flash the code to the ESP device.
