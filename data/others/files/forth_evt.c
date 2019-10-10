#include "espressif/esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "forth_evt.h"

static QueueHandle_t event_queue;

void init_event_queue() {
    event_queue = xQueueCreate(12, sizeof(struct forth_event));
}

void forth_add_event_isr(struct forth_event *event) {
    xQueueSendToBackFromISR(event_queue, event, 0); // queued by copy, not reference
}

int forth_wait_event(int timeout_ms, void* event) {
    return (xQueueReceive(event_queue, event, timeout_ms / portTICK_PERIOD_MS) == pdTRUE) ? 1 : 0;
}
