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


#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    
    #define TMP_CHANNEL 0
    #define LDR_CHANNEL 1
    #define N_SAMPLES 5

    volatile uint8 do_average;
    int32 sample;
    
    CY_ISR_PROTO(ADC_sampling_isr);
    
#endif


/* [] END OF FILE */
