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
    EZI2C_flag = 1;
    
}

/* [] END OF FILE */
