/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint8 i;
    
    DataBuffer[0] = 0xA0; 
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    
    for(;;)
    {
        if (do_average == 1){
            do_average = 0;
            for (i=0; i<N_SAMPLES; i++){
            }
        }
    }
}

/* [] END OF FILE */
