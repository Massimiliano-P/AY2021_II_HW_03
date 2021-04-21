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
    
    
    #define BYTE_TO_SEND 4
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1
    #define LDR_CHANNEL 1
    #define TMP_CHANNEL 0
    #define N_SAMPLES 5
    
    volatile uint8 do_average;
    int32 sample[N_SAMPLES];
    uint8_t DataBuffer [TRANSMIT_BUFFER_SIZE];
    
    CY_ISR_PROTO(ADC_sampling_isr);
    
#endif


/* [] END OF FILE */
