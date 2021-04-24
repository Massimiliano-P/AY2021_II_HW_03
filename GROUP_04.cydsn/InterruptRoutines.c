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
extern uint8_t control_register_1;
extern uint8_t control_register_2;
extern uint8_t state;
extern uint8_t n_samples;
extern uint32_t ldr_sum;
extern uint32_t tmp_sum ;
extern uint8_t sample_index;

CY_ISR(ADC_sampling_isr)
{
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
}

void EZI2C_ISR_ExitCallback(void)
{
    if (slaveBuffer[CTRL_REG_1] != control_register_1) 
    {
        control_register_1 = slaveBuffer[CTRL_REG_1];
        state = control_register_1 & 0b00000011; 
        n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
        ldr_sum = 0;
        tmp_sum = 0;
        sample_index = 0;
    }
    
    if (slaveBuffer[CTRL_REG_2] != control_register_2)
    {
        control_register_2 = slaveBuffer[CTRL_REG_2];
        Timer_ADC_WritePeriod(control_register_2);
    }
}

/* [] END OF FILE */
