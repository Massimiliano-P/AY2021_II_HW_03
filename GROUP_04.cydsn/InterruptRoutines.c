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
#include "InterruptRoutines.h"
#include "project.h"

uint8 sample_index = 0;


CY_ISR(ADC_sampling_isr){
    Timer_ADC_ReadStatusRegister();
    Mux_Select(LDR_CHANNEL);
    sample [sample_index] = ADC_Read32();
    if (sample [sample_index] < 0) sample [sample_index] = 0;
    if (sample [sample_index] > 65535) sample [sample_index] = 65535;
    sample_index ++;
    if (sample_index == N_SAMPLES){
        sample_index = 0;
        do_average = 1;
    }
}
/* [] END OF FILE */
