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

#define SLAVE_BUFFER_SIZE 7
#define WHO_AM_I 0xBC

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];

volatile uint8_t do_sampling = 0;
uint8_t sample_index = 0;
uint32_t sample_sum = 0;
uint32_t sample_avg = 0;

int main(void)
{
    slaveBuffer [0] = 0b00010100;
    slaveBuffer [1] = 0b00101000;
    slaveBuffer [2] = WHO_AM_I;
    slaveBuffer [3] = 0;
    slaveBuffer [4] = 0;
    slaveBuffer [5] = 0;
    slaveBuffer [6] = 0;
  
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        if (do_sampling){
            AMux_Select(LDR_CHANNEL);
            sample = ADC_DelSig_Read32();
            if (sample < 0) sample = 0;
            if (sample > 65535) sample = 65535;
            sample_index ++;
            sample_sum+=sample;
            if (sample_index == N_SAMPLES){
                sample_index = 0;
                sample_avg = sample_sum/N_SAMPLES;
                slaveBuffer [5]  = (sample_avg>>8) & 0xff;
                slaveBuffer [6]  = sample_avg & 0xff;
                sample_sum = 0;                        
             }
        }

            
    }
}

/* [] END OF FILE */
