#include <msp430g2553.h>
#include <signal.h> 
#include <string.h>
#include <stdio.h>

#define NNN 40

unsigned n = 0;
unsigned x = 0;
double 	f = 0;
char 	str[40];
char 	*ps;

/*
unsigned temp1;
unsigned temp2;
unsigned temp3;
unsigned temp4;
*/
#define ACLK 32768.0

#pragma vector=USCIAB0RX_VECTOR
__interrupt void IntServiceRoutineUSCIRX(void) 
{
	if(UCA0RXBUF == 'a')
	{
        
		UCA0CTL1 	|= UCDORM;
		TA1CTL 		|= MC_2;
                TA1CCTL0        &= ~CCIFG;
                TA1CCTL0	|= CCIE;

	}
}



#pragma vector=USCIAB0TX_VECTOR
__interrupt void IntServiceRoutineUSCITX(void) 
{
	UCA0TXBUF = *ps;

	if(*ps == '\0')
	{
		IE2		&= ~UCA0TXIE;
		UCA0CTL1	&= ~UCDORM;

	}
	else
	{
		ps++;
	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void IntServiceRoutineTIMER1A1(void) 
{
  		
	if(TA1IV == 0x0A)
	{
                
          P1OUT &= ~(BIT0 + BIT6);
	
         
                TA1CTL		&= ~MC_2;
		TA1R = 0;
		f = (double) ACLK*NNN/x;
		TA1CCTL0	&= ~( CCIE + CCIFG);
		if(n==0)
                {
                    sprintf(str, "desligado\r\n\0");
                }
                else
                {
                    sprintf(str, "Estouro\r\n\0");
                }
                n=0;
		ps		= str;
		IE2		|= UCA0TXIE;

	}

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void IntServiceRoutineTIMER1A0(void)
{
	switch(n)
	{
		case 0:
                        TA1R = 0;
			n++;
                        break;
		case NNN:
                        TA1CTL		&= ~MC_2;
                        TA1R = 0;
			n=0;
			x = TA1CCR0;
			f = (double) ACLK*NNN/x;
                        //f = (f < 300.0)?f:300.0;
			TA1CCTL0	&= ~CCIE;
			TA1CTL 		&= ~TAIFG;
			sprintf(str, "%.3f %.3f\r\n\0", (double) f/2.0, (double) f/2.0 -60);
			ps	 = 	str;
			IE2	 |=	UCA0TXIE;
			break;
             
                default:
			n++;
			break;

	}
}

int main(void)
{
    	/* stop watchdog timer */
    	WDTCTL = 	WDTPW | WDTHOLD;

        /* 12.5pF cap- setting */
        BCSCTL3         |= XCAP_3;
        
	/* set P1DIR */
	P1DIR |= 	BIT0 + BIT6;
	P1OUT |=  	BIT0 + BIT6;
 
    	/* set P2SEL */
	P2SEL |= 	BIT0 ; 		// Timer1_A3.CCI0A function - P2.0
 
    	/* set TA1CTL */
	TA1CTL |= 	TASSEL_1 +	// Timer A clock source select: 1 - ACLK
			TAIE; 		// Timer A counter interrupt Enable
	
	/* set TA1CCTL0 */
	TA1CCTL0 |=	CAP +		// Capture Mode: ON
                     //   SCS + 
		    	CM_2;

	/* set USCI UART */
	UCA0CTL1 |=	UCSSEL_1;
	UCA0BR0   = 	27;
       	UCA0BR1   = 	0;	
	UCA0MCTL |=	UCBRS_2;

	/* configure ports (UART) */
	P1SEL 	 |=	BIT1 + BIT2;
	P1SEL2	 |=	BIT1 + BIT2;

	/* clear UCSWRST via software */
	UCA0CTL1  &= 	~UCSWRST;

	/* enable interrupt UCA0RXIE */
	IE2	 |=	UCA0RXIE; 

	_BIS_SR( LPM3_bits + GIE );

}
