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
#include "InterruptRoutines.h"


//initialize peripherals
void init_peripherals()
{
    ADC_DelSig_Start();
    AMux_Start();
    EZI2C_Start();
    LED_Pin_Write(LED_OFF);
    Timer_ADC_Start();
    ISR_ADC_StartEx(ADC_sampling_isr);
}


void init_slave ()
{
    //declaration of local variable
    uint8_t slave_index;
    
    //the following for cycle initializes slaveBuffer with 0 values
    for (slave_index=0; slave_index < SLAVE_BUFFER_SIZE; slave_index++)
    {
        slaveBuffer [slave_index] = 0;
    }
    
    //set slaveBuffer[2] to WHO AM I value (0xBC)
    slaveBuffer [WHO_AM_I_POS] = WHO_AM_I;
    
    //assign slaveBuffer to the I2C, only the first two registers can be overwritten
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 5 ,slaveBuffer); 
    
    //copy slaveBuffer[0] and slaveBuffer[1] into control_register_x variables
    //which are used by EZI2C_ISR_ExitCallback to verify if the control registers have been overwritten
    control_register_1 = slaveBuffer[CTRL_REG_1]; 
    control_register_2 = slaveBuffer[CTRL_REG_2];
    
    //initialize n_samples to value in Control Register 1 
    n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
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

void reset_temp (void)
{
    slaveBuffer[MSB_TMP]=0;
    slaveBuffer[LSB_TMP]=0;    
}

void reset_ldr (void)
{
    slaveBuffer[MSB_LDR]=0;
    slaveBuffer[LSB_LDR]=0;  
}


/* [] END OF FILE */
