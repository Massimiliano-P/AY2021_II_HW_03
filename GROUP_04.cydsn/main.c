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

#include "project.h"
#include "InterruptRoutines.h"
#include "tools.h"

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
uint8_t state = ALL_OFF;
uint8_t control_register_1;
uint8_t control_register_2;
volatile uint8_t do_sampling = 0;
uint8_t n_samples;
uint32_t ldr_sample = 0;
uint32_t tmp_sample = 0;
uint32_t ldr_sum = 0;
uint32_t tmp_sum = 0;
uint8_t sample_index = 0;



//uint8_t ADC_sleeping = 0;

int main(void)
{   
    init_peripherals();
    //initialize slaveBuffer
    init_slave();
    control_register_1 = slaveBuffer[CTRL_REG_1];
    control_register_2 = slaveBuffer[CTRL_REG_2];
    n_samples = (slaveBuffer[CTRL_REG_1] >> 2) & 0x0f;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    ISR_ADC_StartEx(ADC_sampling_isr);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        switch (state)
        {
            case ALL_OFF:
                LED_Pin_Write(LED_OFF);
                slaveBuffer[MSB_LDR]=0;
                slaveBuffer[LSB_LDR]=0;
                slaveBuffer[MSB_TMP]=0;
                slaveBuffer[LSB_TMP]=0;
                break;
                
            case ONLY_TMP:
                slaveBuffer[MSB_LDR]=0;
                slaveBuffer[LSB_LDR]=0;

                LED_Pin_Write(LED_OFF);
                if (do_sampling)
                {
                    AMux_Select(TMP_CHANNEL);
                    tmp_sample = read_sample();
                    tmp_sum += tmp_sample;
                    sample_index ++;
    
                    if (sample_index == n_samples)
                    {
                        sample_index = 0;
                        compute_average (tmp_sum, MSB_TMP, LSB_TMP);  
                        tmp_sum = 0;
                    }   
                    
                    do_sampling = 0;
                    
                }
                break;
                
            case ONLY_LDR:
                slaveBuffer[MSB_TMP]=0;
                slaveBuffer[LSB_TMP]=0;
                LED_Pin_Write(LED_OFF);
                if (do_sampling)
                {
                    AMux_Select(LDR_CHANNEL);
                    ldr_sample = read_sample();
                    ldr_sum+=ldr_sample;
                    sample_index ++;
    
                    if (sample_index == n_samples)
                    {
                        sample_index = 0;
                        compute_average (ldr_sum, MSB_LDR, LSB_LDR);  
                        ldr_sum = 0;
                    }     
                    do_sampling = 0;
                }
                break;
                
            case ALL_IN:
                LED_Pin_Write(LED_ON);
                if (do_sampling)
                {
                    AMux_Select(TMP_CHANNEL);  
                    tmp_sample = read_sample();
                    tmp_sum+=tmp_sample;
        
                    AMux_Select(LDR_CHANNEL);
                    ldr_sample = read_sample();
                    ldr_sum+=ldr_sample;
                    
                    sample_index ++;
        
                    if (sample_index == n_samples)
                    {
                        sample_index = 0;
                        compute_average (tmp_sum, MSB_TMP, LSB_TMP);
                        compute_average (ldr_sum, MSB_LDR, LSB_LDR);
                        tmp_sum = 0;
                        ldr_sum = 0;
                    }   
                }
                break;
        }   
    }
}

/* [] END OF FILE */
