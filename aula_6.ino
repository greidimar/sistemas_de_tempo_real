// #include <Arduino.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"

// SemaphoreHandle_t semA;
// SemaphoreHandle_t semB;
// SemaphoreHandle_t semC;

// static void print_block(const char *tag) {
//   for (int i = 0; i < 5; i++) {
//     Serial.printf("[%s] line=%d\n", tag, i);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }

// void TaskA(void *pv) {
//   (void)pv;

//   while (true) {
//     if (xSemaphoreTake(semA, portMAX_DELAY) == pdTRUE) {
//       print_block("semA");
//       xSemaphoreGive(semB);
//     }
//   }
// }

// void TaskB(void *pv) {
//   (void)pv;

//   while (true) {
//     if (xSemaphoreTake(semB, portMAX_DELAY) == pdTRUE) {
//        print_block("semB");
//        xSemaphoreGive(semC);
//     }
//   }
// }

// void TaskC(void *pv) {
//   (void)pv;

//   while (true) {
//     if (xSemaphoreTake(semC, portMAX_DELAY) == pdTRUE) {
//        print_block("semC");
//        xSemaphoreGive(semA);
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   semA = xSemaphoreCreateBinary();
//   semB = xSemaphoreCreateBinary();
//   semC = xSemaphoreCreateBinary();

//   if (!semA || !semB || !semC) {
//     Serial.println("Falha ao criar semaforos!");
//     while (true) {}
//   }

//   xTaskCreate(TaskA, "TaskA", 2048, nullptr, 3, nullptr);
//   xTaskCreate(TaskB, "TaskB", 2048, nullptr, 2, nullptr);
//   xTaskCreate(TaskC, "TaskC", 2048, nullptr, 1, nullptr);

//   xSemaphoreGive(semA);
// }

// void loop() {}



//Ctrl + ;     comenta um bloco de código

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mtx;

static void print_block(const char *tag) {
  for (int i = 0; i < 5; i++) {
    Serial.printf("[%s] line=%d\n", tag, i);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void TaskA(void *pv) {
  (void)pv;
  while (true) {
    //vTaskDelay(pdMS_TO_TICKS(5));
     if (xSemaphoreTake(mtx, portMAX_DELAY) == pdTRUE) {
       print_block("semA");
       xSemaphoreGive(mtx);
     }
  }
}

void TaskB(void *pv) {
  (void)pv;
  while (true) {
    //vTaskDelay(pdMS_TO_TICKS(5));
    if (xSemaphoreTake(mtx, portMAX_DELAY) == pdTRUE) {
       print_block("semB");
       xSemaphoreGive(mtx);
     }
  }
}

void setup() {
  Serial.begin(115200);

  mtx = xSemaphoreCreateMutex();
  if (!mtx) {
    Serial.println("Falha ao criar mutex!");
    while (true) { delay(1000); }
  }

  xTaskCreate(TaskA, "A", 2048, nullptr, 2, nullptr);
  xTaskCreate(TaskB, "B", 2048, nullptr, 1, nullptr);
}

void loop() {}
