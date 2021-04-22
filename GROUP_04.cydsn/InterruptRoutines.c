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

extern volatile uint8_t do_sampling;

CY_ISR(ADC_sampling_isr){
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
    
}
/* [] END OF FILE */
