//control driver for step motor in step mode

// RB0 mode. RB0=0 step mode; RB0=1 continuous mode. Implemented as interrupt
// RB1 
// RB2 step forward (step mode); speed up (continuous mode)
// RB3 step backward (step mode); speed down (continous mode)


#include <xc.h>
#define _XTAL_FREQ 4000000

// Global Variables
int mode=0; //binary variable. mode=0 step, mode1=continuous
int direction=0; //binary variable, direction of rotation in continuous mode
int speed=1; //speed of the motor when continuous rotation, 0-9 

// Functions
void init(void) ;
int step_forward(int state_motor);
int step_backward(int state_motor);
void update_rate(void);


int main(void)
{
	init();
	int state=3;  // integer between 0,1,2,3
 	          // described the current configuration of the step motor
	for(;;)
	{
		if(mode==0)  //stepwise operation
		{
			__delay_ms(200);
			if(RB2==0)
				{state=step_forward(state);}
			if(RB3==0)
				{state=step_backward(state);}
		}// end of stepwise operation


		if(mode==1)  //continuous operation
		{
			if(direction==0)  //forward
			{
				update_rate();  //wait some time wrt speed 
				state=step_forward(state);
			}
			else if (direction==1)  //backward
			{
				update_rate();  //wait some time wrt speed
				state=step_backward(state);  
			}

			//check for button presses
			if(RB2==0&speed<9)
				{speed++;__delay_ms(100);}
			if(RB3==0&speed>0)
				{speed--;__delay_ms(100);}
			if(RB1==0)
			{
				if(direction)
					direction=0;
				else
					direction=1;
				__delay_ms(200);
 			}
				
		}//end continuous operation
		
	} //end of infinite for 
} //end of main

void interrupt myISR(void)
{
	if(mode==0)
		{mode=1;}
	else if (mode==1)
		{mode=0;}
	INTCONbits.INT0IF=0;
}

void init(void)
{	
	OPTION_REG=0;  //pull ups are enabled; interrupts are on falling edge 
	OSCCON=0b01101100;
		
	ANSEL=0;
	TRISB=0b00001111;  //PORTB <0-3> input, <4-7> output
	RB4=1; //phase 1 - state 3
	RB5=0; //phase 2 - state 3
	RB6=0; //phase 3 - state 3
	RB7=1; //phase 4 - state 3
	
	// configuration of interrupt on RB0 (direction change)
	INTCONbits.INT0IF=0; //clear the interrupt flag
	INTCONbits.INT0IE=1; //enable the interrupt
	
	ei(); //enable global interrupts

}  //end init

int step_forward(int state_motor)
{
	if(state_motor==3)
	{RB4=1;RB5=0;RB6=1;RB7=0;return 0;}

	else if(state_motor==0)
	{RB4=0;RB5=1;RB6=1;RB7=0;return 1;}

	else if(state_motor==1)
	{RB4=0;RB5=1;RB6=0;RB7=1;return 2;}
	
	else if(state_motor==2)
	{RB4=1;RB5=0;RB6=0;RB7=1;return 3;}
}

int step_backward(int state_motor)
{
	if(state_motor==3)
	{RB4=0;RB5=1;RB6=0;RB7=1;return 2;}

	else if(state_motor==0)
	{RB4=1;RB5=0;RB6=0;RB7=1;return 3;}

	else if(state_motor==1)
	{RB4=1;RB5=0;RB6=1;RB7=0;return 0;}
	
	else if(state_motor==2)
	{RB4=0;RB5=1;RB6=1;RB7=0;return 1;}

}

void update_rate(void)
{
	for (int i=0;i<10-speed;i++)
		__delay_ms(20);
}	
	