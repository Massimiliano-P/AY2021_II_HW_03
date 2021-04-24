/* ========================================
 *
 * AY2021 HW_03 
 * GROUP 04
 *
 * Authors: 
 * Beatrice Pedretti, Massimiliano Poletti
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


//simple ISR that sets a flag to 1 when it's time to sample (set by user, 4ms default)
CY_ISR(ADC_sampling_isr)
{
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
}

void EZI2C_ISR_ExitCallback(void)
{
    //if value of slaveBuffer changed (updated in BCP)...
    if (slaveBuffer[CTRL_REG_1] != control_register_1) 
    {
        //...set new control register
        control_register_1 = slaveBuffer[CTRL_REG_1];
        //...new state
        state = control_register_1 & 0b00000011; 
        //...update n_samples
        n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
        //reset everything because the state changes here
        reset_variables();
    }
    
    if (slaveBuffer[CTRL_REG_2] != control_register_2)
    {
        control_register_2 = slaveBuffer[CTRL_REG_2];
        Timer_ADC_WritePeriod(control_register_2);
        reset_variables();
    }
}

/* [] END OF FILE */
