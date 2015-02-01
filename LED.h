/* Christopher Martin 
 * Winter 2015
 * Common LED functions 
 */
#ifndef _LED_H_
#define _LED_H_
/*
 * Port status LED functions
 */
//TODO: define led port and change code so port is a macro
#define LED1_GRN (1 << PD6)
#define LED1_YLW (1 << PC2)
#define LED2_GRN (1 << PD5)
#define LED2_YLW (1 << PD0)
#define LED3_GRN (1 << PD4)
#define LED3_YLW (1 << PD1)
#define LED4_GRN (1 << PD3)
#define LED4_YLW (1 << PD2)

// LED states
typedef enum {GREEN, YELLOW} color_t;

/*
 * Set up the data direction registers for the LEDs
 */
void LED_init(void)
{
    DDRD |= LED1_GRN | LED2_GRN | LED3_GRN | LED4_GRN;
    DDRD |= LED2_YLW | LED3_YLW | LED4_YLW;
    DDRC |= LED1_YLW;
}

/* Turns on an LED given the number of the LED, 1 - 4, 
 * and the color, green or yellow.  LEDs are active low.
 */
void LED_on(uint8_t LED, color_t the_color)
{
    switch (LED)
    {
        case 1:
            if (the_color == GREEN)
            {
                PORTD &= ~LED1_GRN;
                PORTC |= LED1_YLW;
            }
            else if (the_color == YELLOW)
            {
                PORTD |= LED1_GRN;
                PORTC &= ~LED1_YLW;
            }
        break;

        case 2:
            if (the_color == GREEN)
            {
                PORTD &= ~LED2_GRN;
                PORTD |= LED2_YLW;
            }
            else if (the_color == YELLOW)
            {
                PORTD |= LED2_GRN;
                PORTD &= ~LED2_YLW;
            }
        break;

        case 3:
            if (the_color == GREEN)
            {
                PORTD &= ~LED3_GRN;
                PORTD |= LED3_YLW;
            }
            else if (the_color == YELLOW)
            {
                PORTD |= LED3_GRN;
                PORTD &= ~LED3_YLW;
            }
        break;

        case 4:
            if (the_color == GREEN)
            {
                PORTD &= ~LED4_GRN;
                PORTD |= LED4_YLW;
            }
            else if (the_color == YELLOW)
            {
                PORTD |= LED4_GRN;
                PORTD &= ~LED4_YLW;
            }
        break;

        default: break;
    }
}

/* Turns off  an LED given the number of the LED, 1 - 4 */ 
void LED_off(uint8_t LED)
{
    switch (LED)
    {
        case 1:
            PORTD &= ~LED1_GRN;
            PORTC &= ~LED1_YLW;
            break;
            
        case 2:
            PORTD &= ~LED2_GRN;
            PORTD &= ~LED2_YLW;
            break;

        case 3:
            PORTD &= ~LED3_GRN;
            PORTD &= ~LED3_YLW;
            break;

        case 4:
            PORTD &= ~LED4_GRN;
            PORTD &= ~LED4_YLW;
            break;

        default: break;
    }
}

/* turn off all LEDs */
void LED_all_off(void)
{
    uint8_t i;

    for (i = 1; i < 5; i++)
    {
        LED_off(i);
    }
}

/* Changes color on all the LEDs from Green to Yellow or Yellow to Green. */
void LED_all_toggle(void)
{
    PORTD ^= LED1_GRN | LED2_GRN | LED3_GRN | LED4_GRN | LED2_YLW | LED3_YLW | LED4_YLW;
    PORTC ^= LED1_YLW;
}

/* Set all the LEDs to the same color */
void LED_all_color(color_t the_color)
{
    uint8_t i;

    for (i = 1; i < 5; i++)
    {
        LED_on(i, the_color);
    }
}

#endif
