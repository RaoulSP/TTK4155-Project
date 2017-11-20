
#include "pid.h"
#include <avr/io.h>
#include "../lib/interrupt_flags.h"


/*! \brief Initialization of PID controller parameters.
 *
 *  Initialize the variables used by the PID algorithm.
 *
 *  \param p_factor  Proportional term.
 *  \param i_factor  Integral term.
 *  \param d_factor  Derivate term.
 *  \param pid  Struct with PID status.
 */
void pid_Init(double p_factor, double i_factor, double d_factor, struct PID_DATA *pid)
{
	// Start values for PID controller
	pid->sumError         = 0;
	pid->lastProcessValue = 0;
	// Tuning constants for PID loop
	pid->P_Factor = p_factor;
	pid->I_Factor = i_factor;
	pid->D_Factor = d_factor;
	// Limits to avoid overflow
	pid->maxError    = MAX_INT / (pid->P_Factor + 1);
	pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
	
	// Set up timer, enable timer/counter compare match interrupt
	TCCR3A = (1 << WGM31) | (1 << WGM30);				//Compare match mode
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); //clock source to be used by the Timer/Counter clkI/O/8
	TIMSK3 = (1 << OCIE3A);								//Interrupt on compare match

	OCR3AH = 0b01001110;
	OCR3AL = 0b00100000; //Sets the value for the compare match to 20000
}

/*! \brief PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *  \param setPoint  Desired value.
 *  \param processValue  Measured value.
 *  \param pid_st  PID status struct.
 */
int16_t pid_Controller(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st)
{
	int16_t errors;
	int32_t ret, temp;
	double i_term, p_term, d_term;
	errors = setPoint - processValue;

	// Calculate Pterm and limit error overflow
	if (errors > pid_st->maxError) {
		p_term = MAX_INT;
	} else if (errors < -pid_st->maxError) {
		p_term = -MAX_INT;
	} else {
		p_term = pid_st->P_Factor * (double)errors;
	}

	// Calculate Iterm and limit integral runaway
	temp = pid_st->sumError + errors;
	if (temp > pid_st->maxSumError) {
		i_term           = MAX_I_TERM;
		pid_st->sumError = pid_st->maxSumError;
	} else if (temp < -pid_st->maxSumError) {
		i_term           = -MAX_I_TERM;
		pid_st->sumError = -pid_st->maxSumError;
	} else {
		pid_st->sumError = temp;
		i_term           = pid_st->I_Factor *(double) pid_st->sumError;
	}

	// Calculate Dterm
	d_term = pid_st->D_Factor * (double)(pid_st->lastProcessValue - processValue);

	pid_st->lastProcessValue = processValue;

	ret = (p_term + i_term + d_term);// * SCALING_FACTOR;
	if (ret > MAX_INT) {
		ret = MAX_INT;
	} else if (ret < -MAX_INT) {
		ret = -MAX_INT;
	}
	//printf("I term: %d\r\n",i_term);
	return -((int16_t)ret);
}

/*! \brief Resets the integrator.
 *
 *  Calling this function will reset the integrator in the PID regulator.
 */
void pid_Reset_Integrator(pidData_t *pid_st)
{
	pid_st->sumError = 0;
}


ISR(TIMER3_COMPA_vect){ //PID timer - called with interval 0.01s
	pid_timer = 1;
}