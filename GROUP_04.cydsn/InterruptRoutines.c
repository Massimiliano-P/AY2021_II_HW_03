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


//simple ISR that sets a flag to 1 when it's time to sample (set by user, 4ms default)
CY_ISR(ADC_sampling_isr)
{
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
}

void EZI2C_ISR_ExitCallback(void)
{
    //if value of slaveBuffer[0] changed (updated in BCP)...
    if (slaveBuffer[CTRL_REG_1] != control_register_1) 
    {
        if ((slaveBuffer[CTRL_REG_1] & 0b00000011) != (control_register_1 & 0b00000011))
        {
            //...update the state
            state = slaveBuffer[CTRL_REG_1] & 0b00000011; 
            switch (state)
            {
                case ALL_OFF:
                    LED_Pin_Write(LED_OFF);
                    reset_ldr();
                    reset_temp();
                    break;
                case ONLY_TMP:
                    LED_Pin_Write(LED_OFF);
                    //light sensor reading reset to zero 
                    reset_ldr();
                    break;
                case ONLY_LDR:
                    LED_Pin_Write(LED_OFF);
                    //temperature sensor reading reset to 0 
                    reset_temp();
                    break;
                case ALL_IN:
                    //turn on LED
                    LED_Pin_Write(LED_ON); 
            }
        }
        //...update n_samples
        n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
        //reset 
        reset_variables();
        //...update the copy of slaveBuffer[0]
        control_register_1 = slaveBuffer[CTRL_REG_1];
    }
    
    //if value of slaveBuffer[1] changed (updated in BCP)...
    if (slaveBuffer[CTRL_REG_2] != control_register_2)
    {
        //...update the copy of slaveBuffer[1]
        control_register_2 = slaveBuffer[CTRL_REG_2];
        //...update the timer period
        Timer_ADC_WritePeriod(control_register_2);
        //reset everything because the state changes here
        reset_variables();
    }
}

/* [] END OF FILE */
