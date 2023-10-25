#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "LCD.h"                        /* Graphic LCD function prototypes    */

int j,hr,min,sec;
char hrVal[2];
char minVal[2];
char secVal[2];
/* Import external IRQ handlers from IRQ.c file                               */
extern __irq void T0_IRQHandler  (void);

/* Import external variables from IRQ.c file                                  */
extern unsigned char clock_1s;

int main (void) {
/* Enable and setup timer interrupt, start timer                            */
  T0MR0         = 11999;                       /* 1msec = 12000-1 at 12.0 MHz */
  T0MCR         = 3;                           /* Interrupt and Reset on MR0  */
  T0TCR         = 1;                           /* Timer0 Enable               */
  VICVectAddr4  = (unsigned long)T0_IRQHandler;/* Set Interrupt Vector        */
  VICVectCntl4  = 15;                          /* use it for Timer0 Interrupt */
  VICIntEnable  = (1  << 4);                   /* Enable Timer0 Interrupt     */
  lcd_init();
  lcd_clear();
  lcd_print ("2023HT01104");
  set_cursor (0, 1);
  lcd_print ("00:00:00");
  
 while (1) {                           /* Loop forever                       */
 if (clock_1s) {
      clock_1s = 0;
	  sec++	;
	  if (sec==60){
	  sec = 0;
	  min++;
	  if(min==60){
	  min = 0;
	  hr++ ;
	  if (hr==24){
	  hr = 0;
	  }
	  }
	  }
	sprintf(hrVal, "%02i",hr);
	sprintf(minVal, "%02i", min);
	sprintf(secVal, "%02i", sec);
	set_cursor(0,1);
	
	for (j = 0; j < 2; j++) 
	 			{
	 				lcd_putchar(hrVal[j]);
	 			} 
	set_cursor(2,1);
	lcd_print ( ":"); 
	set_cursor(3,1);
	
	for (j = 0; j < 2; j++) 
	 			{
					lcd_putchar(minVal[j]);
	 			} 
	set_cursor(5,1);
	lcd_print ( ":");
	set_cursor(6,1);
	for (j = 0; j < 2; j++) 
	 			{
	 				lcd_putchar(secVal[j]);
	 			} 
  }	  
  }
}
