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
#include "tools.h"

extern uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
extern uint8_t n_samples;
extern uint32_t tmp_sum;
extern uint32_t ldr_sum;
extern uint8_t sample_index;
uint8_t control_register_1;
uint8_t control_register_2;

//initialize peripherals
void init_peripherals()
{
    Timer_ADC_Start();
    ADC_DelSig_Start();
    EZI2C_Start();
    AMux_Start();
    LED_Pin_Write(LED_OFF);
}


void init_slave ()
{
    //declaration of local variable
    uint8_t slave_index;
    //the following for cycle fills slaveBuffer with 0 values
    for (slave_index=0; slave_index < SLAVE_BUFFER_SIZE; slave_index++)
    {
        slaveBuffer [slave_index] = 0;
    }
    //set slaveBuffer[2] to WHO AM I value (0xBC)
    slaveBuffer [WHO_AM_I_POS] = WHO_AM_I;
    //assign slaveBuffer to the I2C
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 5 ,slaveBuffer); 
    //initialize control_register_x variables to content of slaveBuffer (upon turning on the device it's set to 0)
    control_register_1 = slaveBuffer[CTRL_REG_1]; 
    control_register_2 = slaveBuffer[CTRL_REG_2];
}

//read sample from ADC
//this function is called in main.c since ADC_DelSig_Read32 is a blocking function
uint32_t read_sample (void)
{
    uint32_t sample;
    sample = ADC_DelSig_Read32();
    //anti windup
    if (sample <= 0) sample = 0;
    if (sample >= 65535) sample = 65535;
    return sample;
}

//this function takes as input the sum to use in order to compute the mean
//it also takes the index of the slaveBuffer that we need to update
void compute_average (uint32_t sum, uint8_t msb, uint8_t lsb)
{
    uint16_t average;
    average = (uint16_t)(sum/n_samples);
    slaveBuffer [msb]  = average>>8;
    slaveBuffer [lsb]  = average & 0xff;
}
    
void reset_variables (void)
{
    tmp_sum = 0;
    ldr_sum = 0;
    sample_index = 0;
}


/* [] END OF FILE */
