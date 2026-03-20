// #include <Arduino.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"  //fila

//control ; comenta tudo que esta selecionado

// QueueHandle_t q;  //o q é a fila uso na Producer e Consumer para indicar a fila que estou usandp

// //passando dados de uma task para a outra por meio da fila q

// void Producer(void *pv) {
//   int v = 0;
//   while (true) {
//     if (xQueueSend(q, &v, pdMS_TO_TICKS(50)) == pdPASS) {
//       v++;
//       Serial.printf("[Producer] data sent = %d\n", v);
//     } else {
//       Serial.println("[Producer] fail to send data");
//     }
//     vTaskDelay(pdMS_TO_TICKS(300));
//   }
// }

// void Consumer(void *pv) {
//   int v;
//   while (true) {
//     if (xQueueReceive(q, &v, portMAX_DELAY) == pdPASS) {
//       Serial.printf("[Consumer] data sent = %d\n", v);
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);

//   q = xQueueCreate(1, sizeof(int));
//   if (!q) {
//     Serial.println("Falha ao criar fila!");
//     while (true) {}
//   }

//   xTaskCreate(Producer, "Producer", 2048, nullptr, 1, nullptr);
//   xTaskCreate(Consumer, "Consumer", 2048, nullptr, 1, nullptr);
// }

// void loop() {
//   //vTaskDelay(pdMS_TO_TICKS(1000));
// }




#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t sensorQueue;

struct SensorData {
  float temp;
  int hum;
  unsigned long timestamp;
};

void SensorTask(void *pv) {
    while (1) {
      SensorData data;

      data.temp = random(20, 30);
      data.hum = random(0, 100);
      data.timestamp = millis();

      xQueueSend(sensorQueue, &data, portMAX_DELAY);

	    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void ProcessingTask(void *pv) {
    SensorData received;

    while (1) {
        if (xQueueReceive(sensorQueue, &received, portMAX_DELAY)) {
          Serial.printf("Temp = %.2f | Hum = %.2f | Timesatmp = %lu\n", 
          received.temp,
          received.hum,
          received.timestamp);
        }
    }
}

void setup() {
    Serial.begin(115200);
    sensorQueue = xQueueCreate(2, sizeof(SensorData));

    xTaskCreate(SensorTask, "Sensor", 2048, NULL, 1, NULL);
    xTaskCreate(ProcessingTask, "Processing", 2048, NULL, 1, NULL);
}

void loop() {}

