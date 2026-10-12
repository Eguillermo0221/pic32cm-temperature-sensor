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


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
#define VOLTAGE 3.3f

int main ( void )
{
//    Private variables
    unsigned char analogVal;
    double Vr, Rt, temp;
    int tmp, status;
    uint16_t adcVal;
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    SYSTICK_TimerStart();
    ADC0_Enable();
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
               
        
        
        adcVal = ADC0_ConversionResultGet();
        
        
//        Calculation
        Vr = 3.3f * adcVal / 4095.0f; // 1.7
        
        Rt = 10000.0f * Vr / (VOLTAGE - Vr); // 10625
        temp = 1.0f / (((logf(Rt/10000.0f)) / 3950.0f) + (1.0f / (273.15f + 25.0f)));
        temp = temp - 273.15f;
        
        printf("Temp = %.2f\r\n",temp);
        SYSTICK_DelayMs(500);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

