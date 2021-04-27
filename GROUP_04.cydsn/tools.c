/* ========================================
 *
 * AY2021 HW_03 
 * GROUP 04
 *
 * Authors: 
 * Beatrice Pedretti, Massimiliano Poletti
 * 
 *
 * tools.c contains:
 * - custom functions definitions
 *
 *
 * ========================================
*/

#include "tools.h"
#include "InterruptRoutines.h"


/**
 * INITIALIZATIONS
**/

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
    //local index for the slave buffer
    uint8_t slave_index;
    
    //fill slaveBuffer with 0 values
    for (slave_index=0; slave_index < SLAVE_BUFFER_SIZE; slave_index++)
    {
        slaveBuffer [slave_index] = 0;
    }
    
    //set WHO_AM_I byte to the slave identifier
    slaveBuffer [WHO_AM_I] = SLAVE_IDENTIFIER;
    
    //assign slaveBuffer to the I2C so that only the first two registers can be overwritten
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 5 ,slaveBuffer); 
    
    //copy the control registers bytes into other variables which are used
    //to understand when the control registers have been overwritten (see update control function below)
    control_register_1 = slaveBuffer[CTRL_REG_1]; 
    control_register_2 = slaveBuffer[CTRL_REG_2];
    
    //initialize n_samples to the value in Control Register 1 
    n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
}


/**
 * SAMPLING AND AVERAGE
**/

//read sample from ADC
//this function is called in main.c since ADC_DelSig_Read32 is a blocking function
uint32_t read_sample (uint8_t channel)
{
    uint32_t sample;
    //select the input channel
    AMux_Select(channel);
    //read sample from ADC
    sample = ADC_DelSig_Read32();
    //anti windup
    if (sample <= 0) sample = 0;
    if (sample >= 65535) sample = 65535;
    return sample;
}

//this function takes as input the samples sum to compute the mean
//it also takes the index of the slaveBuffer that we need to update
void compute_average (uint32_t sum, uint8_t msb, uint8_t lsb)
{
    uint16_t average;
    average = (uint16_t)(sum/n_samples);
    slaveBuffer [msb]  = average>>8;
    slaveBuffer [lsb]  = average & 0xff;
}


/**
 * RESETS
**/

//reset to 0 the samples sums and the sample index
void reset_variables (void)
{
    tmp_sum = 0;
    ldr_sum = 0;
    sample_index = 0;
}

//reset to 0 the tmp slave buffer bytes
void reset_temp (void)
{
    slaveBuffer[MSB_TMP]=0;
    slaveBuffer[LSB_TMP]=0;    
}

//reset to 0 the ldr slave buffer bytes
void reset_ldr (void)
{
    slaveBuffer[MSB_LDR]=0;
    slaveBuffer[LSB_LDR]=0;  
}

/**
 * FOLLOW MASTER NEW INSTRUCTIONS
**/

//this function is called by the main whenever the EZI2C callback is activated
//to update the system control according to the master new instructions.
//it is called at the beginning of the main to let the it finish its cycle before updating the control 
void update_control(void)
{
    //if control register 1 has been changed by the master...
    if (slaveBuffer[CTRL_REG_1] != control_register_1) 
    {
        //...check if the state bits changed
        if ((slaveBuffer[CTRL_REG_1] & 0b00000011) != (control_register_1 & 0b00000011))
        {
            //...update the state
            state = slaveBuffer[CTRL_REG_1] & 0b00000011; 
            //... make some useful resets
            switch (state)
            {
                case ALL_OFF:
                    LED_Pin_Write(LED_OFF);
                    reset_ldr();
                    reset_temp();
                    break;
                case ONLY_TMP:
                    LED_Pin_Write(LED_OFF);
                    reset_ldr();
                    break;
                case ONLY_LDR:
                    LED_Pin_Write(LED_OFF);
                    reset_temp();
                    break;
                case ALL_IN:
                    //switch on the LED
                    LED_Pin_Write(LED_ON); 
            }
        }
        //...update n_samples
        n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
        //...make some useful resets
        reset_variables();
        //...update the copy of control register 1
        control_register_1 = slaveBuffer[CTRL_REG_1];
    }
    
    //if value of control register 2 has been changed by the master...
    if (slaveBuffer[CTRL_REG_2] != control_register_2)
    {
        //...update the copy of control register 2
        control_register_2 = slaveBuffer[CTRL_REG_2];
        //...update the timer period
        Timer_ADC_WritePeriod(control_register_2);
        //...make some useful resets
        reset_variables();
    }
}


/* [] END OF FILE */
