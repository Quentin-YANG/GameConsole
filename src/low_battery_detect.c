/* This C file is to detect battery voltage and show with LED */

#include "game_console.h" 

//ADC INIT
void ADC_init(void)
{
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs

	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); //Enable ADC;ADC frequency is Fosc/64
    ADMUX|=(1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX0);//Use internal 2.56V VR, Left adjust result, ADC5

	ADCSRA|=1<<ADSC; //Free running mode, start first conversion
}

void low_battery_detect(void)
{
	uint8_t batt_vol;
	while(ADCSRA & (1<<ADSC)); //Wait until conversion complete
	batt_vol = ADCH;
	if(batt_vol < 120) BAT_LOW_LED(ON); //batt_vol<1.2V, LED ON
	else BAT_LOW_LED(OFF);
}
