/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"

#define configCHECK_FOR_STACK_OVERFLOW 2
#define configUSE_MALLOC_FAILED_HOOK 1

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
#define VOLTAGE 3.3f

uint16_t g_setTemp = 27;
uint16_t g_hysteresis = 2;

volatile bool g_updateLed;

volatile float g_temp;

volatile static enum _Status{
                MODE_OFF, 
                MODE_ON   
} currStatus = MODE_ON;


void updateStatusLed( void );

void calculateTemp(uint16_t adcVal)
{
    double Vr, Rt;
    
    Vr = 3.3f * adcVal / 4095.0f; 
    
    if(Vr > 3.29f)
    {
        Vr = 3.29f;
    }
    
    Rt = 10000.0f * Vr / (VOLTAGE - Vr); // Convert to resistance 
    g_temp = 1.0f / (((logf(Rt/10000.0f)) / 3950.0f) + (1.0f / (273.15f + 25.0f))); // Convert to temp in C
    g_temp -= 273.15f;

}

void vBlinkTask(void *pvParameters)
{
    while(1)
    {
    LED0_Toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vSensorTask (void *pvParameters)
{
    uint16_t adcVal;
    
    while(1)
    {
        adcVal = ADC0_ConversionResultGet();
        
        // Calculation
        calculateTemp(adcVal);
        
        //Delay
        vTaskDelay(pdMS_TO_TICKS(100));
    }
        
}

void vControlTask (void *pvParameters)
{
    while (1)
    {
        if(currStatus == MODE_OFF && g_temp > g_setTemp + g_hysteresis)
        {
            currStatus = MODE_ON;
            g_updateLed = true;
        }
        else if(currStatus == MODE_ON && g_temp < g_setTemp - g_hysteresis)
        {
            currStatus = MODE_OFF;
            g_updateLed = true;
        }
        //printf("controlTask = %d\r\n",currStatus);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vLedTask (void *pvParameters)
{
    while(1)
    {
        if (g_updateLed) 
        {
            updateStatusLed();
            g_updateLed = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
        
}

void vUartTask (void *pvParameters)
{
    
    while (1) 
    {
        printf("Temp: %f Mode:%s\r\n",g_temp, currStatus? "ON" : "OFF");
        
        //LED0_Toggle();
        //printf("Testing \r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
        
}

int main ( void )
{
//    Private variables
    
    
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    //SYSTICK_TimerStart();
    ADC0_Enable();
    
    g_updateLed = true;
    
    
    //FreeRTOS
    

    xTaskCreate(vUartTask, "uart", 256, NULL, 1, NULL);
    xTaskCreate(vSensorTask, "Sensor", 128, NULL, 1, NULL);
    xTaskCreate(vControlTask, "Control", 128, NULL, 1, NULL);
    xTaskCreate(vLedTask, "Led", 128, NULL, 1, NULL);


    vTaskStartScheduler();
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

void updateStatusLed( void )
{
    
    switch (currStatus)
    {
        case MODE_OFF:
            LED0_Set();
            break;
        case MODE_ON:
            LED0_Clear();
            break;
        default:
            break;
    }
}




/*******************************************************************************
 End of File
*/

