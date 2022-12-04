/*
 * main.c
 *
 *  Created on: Sep 17, 2022
 *      Author: Omar Alshamy
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char arr[6]={0,0,0,0,0,0};
unsigned char i=0;

int main(void)
{
    DDRD |=0x0C;
    PORTD &=0xF3;

    DDRB |=0x04;
    PORTB &=0xFB;

	SREG|= (1<<7);  //Enable global interrupts in MC by setting the I-Bit.

    DDRC |=0x0F;  	// Configure pins as output from PC0 --> PC3

    PORTC &=0xF0;  	/* Set pins  in PORTC with value 0 to enable the BJT NPN transistor to
                            7-segment common to the ground(common Anode)*/

    DDRA |=0x3F;  	// Configure pins as output from PA0 --> PA5

    PORTA &=0xC0;   /* Set pins  in PORTA with value 0 to disable 6 7-segment*/

   TIMER1_COMPA_Init();
   INT0_Init();              // Enable external INT0
   INT1_Init(); 			// Enable external INT1
   INT2_Init();				// Enable external INT2

   while (1)
    {

	   for(i=0;i<6;i++)
	   {
		   PORTA =(1<<i);
		   PORTC =arr[i];
		   _delay_ms(2);
	   }


    }
}
/*
 * For System clock=8Mhz and prescale F_CPU/1024.
 * Timer frequency will be around 1Mhz, Ttimer = 1024us
 * So we just need 980 counts to get 1024us period.
 * In case we need 25% duty cycle we need to make the timer count 245 counts
 */
void TIMER1_COMPA_Init(void)
{
   TCNT1=0;            /* Set timer1 initial count to zero */

   OCR1A=977;          /* Set the Compare value to 977 */

   TIMSK=(1<<OCIE1A);  /* Enable Timer1 Compare A Interrupt */

                       /* Configure timer control register TCCR1B
                       * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
   	                   * 2. Prescaler = F_CPU/1024 CS10=1 CS11=0 CS12=1 */
   TCCR1B =(1<<CS10)|(1<<WGM12)|(1<<CS12);
                        /* Configure timer control register TCCR1A
                         * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
                          * 2. FOC1A=1 FOC1B=0
                          * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)*/
   TCCR1A |=(1<<FOC1A);

}


/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	MCUCR |= (1<<ISC01) ;               // Trigger INT0 with the falling edge
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

/* External INT1 enable and configuration function */
void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin
	MCUCR |= (1<<ISC11);// Trigger INT1 with the raising edge
	MCUCR |= (1<<ISC10);
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
}

/* External INT2 enable and configuration function */
void INT2_Init(void)
{
	DDRB   &= (~(1<<PB2));   // Configure INT2/PB2 as input pin
	MCUCSR &= ~(1<<ISC2);     // Trigger INT2 with the falling edge
	GICR   |= (1<<INT2);	 // Enable external interrupt pin INT2
	SREG   |= (1<<7);        // Enable interrupts by setting I-bit
}


ISR(INT0_vect)
{
	 arr[0]=0;
	 arr[1]=0;
	 arr[2]=0;
	 arr[3]=0;
	 arr[4]=0;
	 arr[5]=0;
}

ISR(INT1_vect)
{
	TIMSK=(0<<OCIE1A);
}

ISR(INT2_vect)
{
	TIMSK=(1<<OCIE1A);
}

/* Interrupt Service Routine for timer1 compare mode */
ISR(TIMER1_COMPA_vect)
{
	SREG|= (1<<7);
     arr[0]++; // 1sec

	   // check if overflow occurs at the 7-segment as its maximum count is 9
	   		if(arr[0] == 10)
	   		{
              arr[0]=0;// increment 7-segment every second
              arr[1]++;
              if(arr[1] == 6)
                  {
                  arr[1]=0;
              	  arr[2]++;
              	  if(arr[2] == 10)
              	      {
              		  arr[2]=0;
              		  arr[3]++;
              		 if(arr[3] == 6)
              		         {
              			     arr[3]=0;
              			     arr[4]++;
              			   if(arr[4] == 10)
              			          {
              				        arr[4]=0;
              				        arr[5]++;
              				        if(arr[5]==10)
              				            {
              				        	arr[5]=0;
              				        	arr[0]=0;
              				            }
              			          }
              		         }
              	      }
                  }
	   		}



   // increment sec
	 // increment min
	// check if overflow occurs at the 7-segment as its maximum count is 9

	// increment 7-segment every second




}
