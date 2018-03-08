/** Pin Defines
*	Mega
*	MISO =	50 = PB3
	MOSI =	51 = PB2
	SCK =	52 = PB1
	SS/CS =	53 = PB0
*	Uno
	MISO =	12 = PB4
	MOSI =	11 = PB3
	SCK =	13 = PB5
	SS/CS =	10 = PB2
	
SPI			Mega	Uno		MEGA_PIN	UNO_PIN
________________________________________________________________________
CE			PB4		PB1		10			9	// CE output Pin 10 on Mega
CSN			PB0		PB2		53			10	// CSN output SS
SCK			PB1		PB5		52			13	// SCK output
MOSI		PB2		PB3		51			11	// MOSI output
MISO		PB3		PB4		50			12	// MISO input
________________________________________________________________________
*/

#include <avr/io.h>
#include "nrf24.h"



/* set direction ports */
void nrf24_setupPins()
{
    set_bit(DDRB,CE); // CE output		10 on Mega
    set_bit(DDRB,CSN); // CSN output	53
    set_bit(DDRB,SCK); // SCK output	52
    set_bit(DDRB,MOSI); // MOSI output	51
    clr_bit(DDRB,MISO); // MISO input	50
}

void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,CE);
    }
    else
    {
        clr_bit(PORTB,CE);
    }
}

void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,CSN);
    }
    else
    {
        clr_bit(PORTB,CSN);
    }
}

void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,SCK);
    }
    else
    {
        clr_bit(PORTB,SCK);
    }
}

void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,MOSI);
    }
    else
    {
        clr_bit(PORTB,MOSI);
    }
}

uint8_t nrf24_miso_digitalRead()
{
    return check_bit(PINB,MISO);
}

