/* ========================================
 *
 * AY2021 HW_03 
 * GROUP 04
 *
 * Authors: 
 * Beatrice Pedretti, Massimiliano Poletti
 * 
 *
 *
 * main.c contains:
 * - global variables declarations
 * - the main program:
 *      - initialization phase
 *      - control updating managing
 *      - sampling managing
 * 
 *
 * ========================================
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "tools.h"


/**
 * GLOBAL VARIABLES 
**/

// EZI2C slave buffer 
uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; 

// control variables for the buffer
uint8_t control_register_1; 
uint8_t control_register_2; 

// EZI2C flag: switched on by EZI2C_ISR_ExitCallback 
volatile uint8_t EZI2C_flag = 0;

// state 
uint8_t state = ALL_OFF;  

// sampling 
uint8_t n_samples; //number of samples to be averaged
uint32_t ldr_sum = 0; // used to sum the ldr samples
uint32_t tmp_sum = 0; // used to sum the tmp samples
uint8_t sample_index = 0; //used to count the samples

// sampling flag: switched on by ADC_sampling_isr
volatile uint8_t do_sampling = 0;


/**
 * MAIN PROGRAM
**/

int main(void)
{   
    /**
     * INITIALIZATION
    **/
    
    //enable global interrupts
    CyGlobalIntEnable;
    
    //initialize peripherals
    init_peripherals();
    
    //initialize slaveBuffer
    init_slave();
    
    //initialize the timer period
    Timer_ADC_WritePeriod(slaveBuffer[CTRL_REG_2]);
    
    for(;;)
    {
        //check if the control has to be updated
        if (EZI2C_flag) 
        {
            update_control();
            EZI2C_flag = 0;
        }
        
        switch (state)
        {                
            case ONLY_TMP: //state 01
                //do_sampling if a flag set to 1 in the ISR when the timer reaches overflow
                //since ADC_DelSig_Read32() is a blocking function (as stated in its definition), we sample here instead of inside the ISR
                if (do_sampling)
                {
                    //read sample and accumulate sample values inside tmp_sum
                    tmp_sum += read_sample(TMP_CHANNEL);
                    sample_index ++;
    
                    if (sample_index == n_samples)
                    {
                        //compute average (function is explained in tools.c)
                        compute_average (tmp_sum, MSB_TMP, LSB_TMP);  
                        reset_variables();
                    }  
                    //sampling done -> reset flag
                    do_sampling = 0;         
                }
                break;
                
            case ONLY_LDR: //state 10
                if (do_sampling)
                {
                    ldr_sum += read_sample(LDR_CHANNEL);
                    sample_index ++;
    
                    if (sample_index == n_samples)
                    {
                        compute_average (ldr_sum, MSB_LDR, LSB_LDR);  
                        reset_variables();
                    }     
                    do_sampling = 0;
                }
                break;
                
            case ALL_IN: //state 11
                if (do_sampling)
                { 
                    tmp_sum += read_sample(TMP_CHANNEL);
        
                    ldr_sum += read_sample(LDR_CHANNEL);
                    
                    sample_index ++;
        
                    if (sample_index == n_samples)
                    {
                        compute_average (tmp_sum, MSB_TMP, LSB_TMP);
                        compute_average (ldr_sum, MSB_LDR, LSB_LDR);
                        reset_variables();
                    }   
                }
                break;
        }   
    }
}

/* [] END OF FILE */
