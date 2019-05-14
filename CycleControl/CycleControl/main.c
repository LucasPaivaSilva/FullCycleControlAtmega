/*
 * CycleControl.c
 *
 * Created: 5/14/2019 12:57:43 PM
 * Author : Lucas Paiva da Silva
 */ 

//IO8 conectado no Vsinc
//IO9 conectado no disparo do SCR

#define F_CPU 16000000UL	//define a frequencia do microcontrolador - 16MHz

#include <avr/io.h> 	    //defini��es do componente especificado
#include <util/delay.h>		//biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <avr/pgmspace.h>   //para o uso do PROGMEM, grava��o de dados na mem�ria flash
#include <avr/interrupt.h>  //

//Defini��es de macros para o trabalho com bits

#define	set_bit(y,bit)	(y|=(1<<bit))	//coloca em 1 o bit x da vari�vel Y
#define	clr_bit(y,bit)	(y&=~(1<<bit))	//coloca em 0 o bit x da vari�vel Y
#define cpl_bit(y,bit) 	(y^=(1<<bit))	//troca o estado l�gico do bit x da vari�vel Y
#define tst_bit(y,bit) 	(y&(1<<bit))	//retorna 0 ou 1 conforme leitura do bit

//Vari�veis
int CycleCount = 0;

#define NCycles 3

ISR(TIMER1_CAPT_vect) //interrup��o do T1
{
	//Detecta o semi ciclo e contabiliza no cyclycount
	
	if (tst_bit(TCCR1B, ICES1))
	{
		clr_bit(TCCR1B, ICES1);
		CycleCount++;
	}
	else
	{
		set_bit(TCCR1B, ICES1);
		CycleCount++;
	}
	
	//Verifica o n�mero de ciclos e dispara o SCR
	if (CycleCount >= NCycles)
	{
		set_bit(PORTB, PB1);
		CycleCount = 0;
		_delay_ms(2);
		clr_bit(PORTB, PB1);
	}
	
}

int main(void)
{
	DDRB  = 0b11111110;	
	PORTB = 0b11111001;	
	
	TCNT1 = 0;
	TCCR1B = (1<<CS10) | (1<<CS12) | (1<<ICES1);
	TIMSK1 = (1<<ICIE1);
	sei();
	
    while (1) 
    {
    }
}

