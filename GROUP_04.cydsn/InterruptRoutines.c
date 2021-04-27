/* ========================================
 *
 * AY2021 HW_03 
 * GROUP 04
 *
 * Authors: 
 * Beatrice Pedretti, Massimiliano Poletti
 *  
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "project.h"
#include "tools.h"


//when it's time to sample a flag is switched on to read a sample
CY_ISR(ADC_sampling_isr)
{
    Timer_ADC_ReadStatusRegister();
    do_sampling=1;
}

//when you exit the EZI2C isr a flag is switched on to update the control
void EZI2C_ISR_ExitCallback(void)
{
    EZI2C_flag = 1;
    
}

/* [] END OF FILE */
