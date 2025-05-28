#include "stm32f4xx.h"
#include "mq2_ppm.h"
#include "adc.h"
#include <math.h>
#include <stdio.h>
#define VREF     5.0f
#define ADC_RES  4095.0f
#define RL       5.0f
#define RO       0.48f   //https://ai.thestempedia.com/docs/evive-iot-kit/interfacing-mq-2-gas-sensor-with-evive/    our RS=4.7Kohm  we can also take ro as 10 by default

// LPG gas calibration constants
#define A       -2.34f
#define B        2.78f
// for LPG (A=-2.347,B=2.780), CH4 (A=-3.021,B=3.742), CO (A=-3.811,B=4.818) , H2 (A=-1.45,B=3)


float MQ2_PPM_OP(void) {
	start_conversion_adc();
	uint32_t adc_val = adc_read();
	float voltage = ((float)adc_val / ADC_RES) * VREF;
	 if (voltage < 0.01f) {
	        // Avoid division by zero or invalid reading
	        return -1;
	    }
	 float Rs = RL * (VREF - voltage) / voltage;
	 float ratio = Rs / RO;
	 float ppm = powf(10, (log10f(ratio) - B) / A);
   return ppm;
}


