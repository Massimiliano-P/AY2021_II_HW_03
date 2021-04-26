/* ========================================
 *
 * AY2021 HW_03 
 * GROUP 04
 *
 * Authors: 
 * Beatrice Pedretti, Massimiliano Poletti
 * 
 * INFO
 * tools.h is a header file that contains useful constants and all custom functions' declarations
 * tools.c contains the definitions of these functions
 *
 * ========================================
*/

#include "project.h"

//slave buffer constants
#define SLAVE_BUFFER_SIZE 7
#define WHO_AM_I 0xBC

//slave buffer registers
#define CTRL_REG_1 0
#define CTRL_REG_2 1
#define WHO_AM_I_POS 2
#define MSB_TMP 3
#define LSB_TMP 4
#define MSB_LDR 5
#define LSB_LDR 6

//multiplexer
#define TMP_CHANNEL 0
#define LDR_CHANNEL 1

//states
#define ALL_OFF 0 //no sensor is read
#define ONLY_TMP 1  //only the temperature sensor is read
#define ONLY_LDR 2 //only the light sensor is read
#define ALL_IN 3 //both the sensors are read

//LED
#define LED_ON 1
#define LED_OFF 0

//sampling
#define N_SAMPLES_DEFAULT 5

//main.c global variables
extern uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
extern uint8_t control_register_1; 
extern uint8_t control_register_2;
extern uint8_t state;
extern uint8_t n_samples;
extern uint32_t ldr_sum;
extern uint32_t tmp_sum ;
extern uint8_t sample_index;
extern volatile uint8_t do_sampling;
extern volatile uint8_t EZI2C_flag;

//custom functions declarations
void init_peripherals (void);
void init_slave(void);
uint32_t read_sample (uint8_t channel);
void compute_average (uint32_t, uint8_t, uint8_t);
void reset_variables (void);
void reset_temp (void);
void reset_ldr (void);
void update_control(void);



/* [] END OF FILE */
