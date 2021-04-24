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

#include "project.h"

//all defines
#define SLAVE_BUFFER_SIZE 7
#define WHO_AM_I 0xBC

#define TMP_CHANNEL 0
#define LDR_CHANNEL 1

#define ALL_OFF 0
#define ONLY_TMP 1
#define ONLY_LDR 2
#define ALL_IN 3

#define CTRL_REG_1 0
#define CTRL_REG_2 1
#define WHO_AM_I_POS 2
#define MSB_TMP 3
#define LSB_TMP 4
#define MSB_LDR 5
#define LSB_LDR 6

#define LED_ON 1
#define LED_OFF 0

#define N_SAMPLES_DEFAULT 5

//custom functions declarations
void init_peripherals (void);
void init_slave(void);
uint32_t read_sample (void);
void compute_average (uint32_t, uint8_t, uint8_t);
void reset_variables (void);

/* [] END OF FILE */
