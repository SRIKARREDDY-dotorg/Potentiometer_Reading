#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "LCD.h"

#define MAX_ADC_10_BIT_VALUE					(1023.0)

#define VREF_VOLTAGE									(3.3)

#define CCLK                          (48000000)

static int adcStatus = 0;
static int adcReading = 0;
static float voltage = 0.0f;
char buff[5];

void delayMicroseconds(unsigned int us) 
{
    // Calculate the number of iterations required for the given delay
    unsigned int iterations = us * (CCLK / 1000000);

    // Use a for loop to introduce the delay
    for (unsigned int i = 0; i < iterations; i++) 
	  {
        // This loop will take some time to execute, contributing to the delay
        // You may need to optimize or add more instructions if it's too short.
    }
}

void adc_Init(void)
{
	//COnfigure the ADC Pin
	//potentiometer is on AD0.0; "PINSEL1" & bit 15:14;
	PINSEL1 = (0x01 << 14);
	//Enable the ADC power control pin
	PCONP |= (1<<12);
	//Configure the ADC channel
	// 7:0 pin sel; 15:8 clk ; 16 BURST; 21 ADC status.
	AD0CR = 0x01; //select Pin AD0.0
	AD0CR |= (0x02 << 8); //CLKDIV div by 2 => (4.5MHz / (2 +1))
	AD0CR |= (0x01 << 16);//BURST on
	AD0CR |= (0x01 << 21);//ADC operational.
}

float convertReading(int measure)
{
	float reading = 0.0f;
	reading = ((measure/MAX_ADC_10_BIT_VALUE)*VREF_VOLTAGE);
	return reading;
}

int main(void)
{
	lcd_init();
	lcd_clear();
	adc_Init();

	//Print BITS ID
	lcd_print("2023HT01047");
	set_cursor(0,1);
	lcd_print("Volt : ");
	while(1)
	{
		//check ADC completion status
		adcStatus = AD0DR0 & (0x80000000);
		if(adcStatus)
		{
			//read the ADC 10bit binary fraction
			adcReading = (AD0DR0>>6) & 0x3FF;
			//convert it to float
			voltage = convertReading(adcReading);
			//convert it from float to char to display on LCD
			sprintf(buff, "%.2f", voltage);
			//Print on LCD
			set_cursor(8,1);
			lcd_print(&buff);
			lcd_print ("V");
		}
		delayMicroseconds(2);
	}
	return 0;
}
