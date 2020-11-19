/*************************************************************************************************
 * Programmer:      Shristi Saraff
 *************************************************************************************************/

#include "commands.h"

extern int pixelinfo_flag;
extern TaskHandle_t test;
extern TaskHandle_t stepper;
extern TaskHandle_t process;

/************************************** FUNCTIONS **************************************/
void process_commands(void *pvParameters) {
    while (1) {
        command_t command;
        xQueueReceive(queue, &command, portMAX_DELAY);

        switch (command.command_id) {
            case C0_ID:
            case C1_ID:
            case C2_ID:
            case C3_ID:
            case C4_ID:
            case C5_ID:
            case C6_ID:
            case C7_ID:
            case C8_ID:
            case RESET_ID:
                reset();
                break;
            case READ_ID:
                // check_rgb_color
            default:
                break;
        }
        vTaskDelay(10);
    }
}

void reset() {
    printf("Stopping tasks\n");
    pixelinfo_flag = 0;
    tcs34725_stop();
    drv8825_stop();
    sg90_stop();
    if (test != NULL) {
        vTaskDelete(test);
    }
    if (stepper != NULL) {
        vTaskDelete(stepper);
    }
    if (process != NULL) {
        vTaskDelete(process);
    }
}