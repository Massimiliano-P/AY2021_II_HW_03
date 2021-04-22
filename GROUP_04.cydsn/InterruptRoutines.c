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
#include "tools.h"

extern volatile uint8_t do_sampling;
extern uint8_t slaveBuffer[];
extern uint8_t N_SAMPLES;
extern uint8_t control_register_1;
extern uint8_t state;

CY_ISR(ADC_sampling_isr){
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
    
}

void EZI2C_ISR_ExitCallback(void){
    // Update PWM compare value if it was updated and it is lower than period value
    if (slaveBuffer[0] != control_register_1) 
    {
        control_register_1 = slaveBuffer[0];
        state = control_register_1 & 0b00000011;
        
        /*
        N_SAMPLES = (control_register_1 >> 2) & 0x0f;

        Timer_ADC_WritePeriod()*/
    }
}



/* [] END OF FILE */
