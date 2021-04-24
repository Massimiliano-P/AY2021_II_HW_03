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
#include "tools.h"

extern uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
extern uint8_t n_samples;

void init_peripherals()
{
    Timer_ADC_Start();
    ADC_DelSig_Start();
    EZI2C_Start();
    AMux_Start();
    LED_Pin_Write(LED_OFF);
    n_samples = N_SAMPLES_DEFAULT;
}

void init_slave ()
{
    slaveBuffer [CTRL_REG_1] = 0b00010100;
    slaveBuffer [CTRL_REG_2] = 0b00101000; 
    slaveBuffer [WHO_AM_I_POS] = WHO_AM_I;
    slaveBuffer [MSB_TMP] = 0;
    slaveBuffer [LSB_TMP] = 0;
    slaveBuffer [MSB_LDR] = 0;
    slaveBuffer [LSB_LDR] = 0;
    //assign slaveBuffer to the I2C
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 5 ,slaveBuffer); 
}

uint32_t read_sample (void)
{
    uint32_t sample;
    sample = ADC_DelSig_Read32();
    if (sample <= 0) sample = 0;
    if (sample >= 65535) sample = 65535;
    return sample;
}

void compute_average (uint32_t sum, uint8_t msb, uint8_t lsb)
{
    uint16_t average;
    average = (uint16_t)sum/n_samples;
    slaveBuffer [msb]  = (average>>8) & 0xff;
    slaveBuffer [lsb]  = average & 0xff;
   
}
    



/* [] END OF FILE */
