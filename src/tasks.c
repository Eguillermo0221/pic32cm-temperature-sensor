#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "FreeRTOS.h"
#include "task.h"
#include "definitions.h"

void vBlinkTask(void *pvParameters)
{
while(1)
{
    //LED0_Toggle();

    vTaskDelay(pdMS_TO_TICKS(500));
    }
}
