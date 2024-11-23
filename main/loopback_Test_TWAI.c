#include "driver/twai.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TWAI_TEST";

void app_main(void) {
    // TWAI configuration
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_23, TWAI_MODE_NORMAL);//TX,RX, loopback
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    g_config.alerts_enabled = TWAI_ALERT_ALL;
    // Install the TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        ESP_LOGI(TAG, "TWAI driver installed");
    } else {
        ESP_LOGE(TAG, "Failed to install TWAI driver");
        return;
    }

    // Start the TWAI driver
    if (twai_start() == ESP_OK) {
        ESP_LOGI(TAG, "TWAI driver started");
    } else {
        ESP_LOGE(TAG, "Failed to start TWAI driver");
        return;
    }

    // Create and transmit a message
    // twai_message_t tx_message = {
    //     .identifier = 0x640,
    //     .data_length_code = 8,
    //     .data = {0b10101010, 0b11001100, 0b11110000, 0b00001111, 0b00110011, 0b01010101, 0b01111000, 0b10000001},
    //     .self = 1
    // };

    // if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    //     ESP_LOGI(TAG, "Message sent successfully");
    // } else {
    //     ESP_LOGE(TAG, "Failed to send message");
    // }

    // Receive the message
    twai_message_t rx_message;
    // rx_message.identifier = 0x640;
    while (true)
    {
      uint32_t alerts;
      if (twai_read_alerts(&alerts, pdMS_TO_TICKS(1000)) == ESP_OK) {
            if (alerts & TWAI_ALERT_TX_SUCCESS) {
                ESP_LOGI(TAG, "Transmission successful");
            }
            if (alerts & TWAI_ALERT_RX_DATA) {
                ESP_LOGI(TAG, "Data received");
            }
            // Handle other alerts as needed
      }
      if (twai_receive(&rx_message, pdMS_TO_TICKS(100)) == ESP_OK){
        // printf("Message received: ID=0x%" PRIx32 "\n", rx_message.identifier);
        //     printf("Data: ");
        //     for (int i = 0; i < 2; i++) {
        //       if(rx_message.identifier == 0x640){
        //         // printf("0x%02X ", rx_message.data[i]);
        //         printf("%d ", rx_message.data[i]);
        //       }
        //     }
        //     printf("\n");
        printf("Message received: ID=0x%" PRIx32 "\n", rx_message.identifier);
        // uint8_t previous_gear_position = 0;
        if (rx_message.identifier == 0x640)
        {
          if (rx_message.data_length_code >= 2)
          {
            // Concatenate the first two bytes into a 16-bit integer
            uint16_t concatenated_value = (rx_message.data[0] << 8) | rx_message.data[1];
            printf("Concatenated 16-bit value (Engine Speed): %d\n", concatenated_value);
          }
          else
          {
            ESP_LOGE(TAG, "Not enough data to concatenate");
          }
        }
        else if (rx_message.identifier == 0x64d)
        {
          // for (int i = 0; i < rx_message.data_length_code; i++){
          //   // printf("Gear postion: ID=0x%" PRIx32 "\n", rx_message.data);
          //   printf("Gear Postion: 0x%02X\n ", rx_message.data[i]);
          // }
          if (rx_message.data_length_code >= 8)
          {
            // Extract the last 4 bits of the 7th byte for gear position
            uint8_t current_gear_position = rx_message.data[7] & 0x0F;
            // if (previous_gear_position != current_gear_position){
              printf("Gear Position: %d\n", current_gear_position);
              // previous_gear_position = current_gear_position;
            // }
          }
          else
          {
            ESP_LOGE(TAG, "Not enough data to extract gear position");
          }
        }
      }else
      {
        ESP_LOGE(TAG, "Failed to receive message");
      }
    }

    // Stop and uninstall the TWAI driver
    twai_stop();
    twai_driver_uninstall();
    ESP_LOGI(TAG, "TWAI driver stopped and uninstalled");
}
