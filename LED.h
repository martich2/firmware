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

// the LEDs on the front pannel
typedef enum {LED1, LED2, LED3, LED4} led_t;

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
void LED_on(led_t LED, color_t the_color)
{
    switch (LED)
    {
        case LED1:
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

        case LED2:
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

        case LED3:
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

        case LED4:
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
    }
}

/* Turns off  an LED given the number of the LED, 1 - 4 */ 
void LED_off(led_t LED)
{
    switch (LED)
    {
        case LED1:
            PORTD &= ~LED1_GRN;
            PORTC &= ~LED1_YLW;
            break;
            
        case LED2:
            PORTD &= ~LED2_GRN;
            PORTD &= ~LED2_YLW;
            break;

        case LED3:
            PORTD &= ~LED3_GRN;
            PORTD &= ~LED3_YLW;
            break;

        case LED4:
            PORTD &= ~LED4_GRN;
            PORTD &= ~LED4_YLW;
            break;
    }
}

/* turn off all LEDs */
void LED_all_off(void)
{
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        LED_off((led_t)i);
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

    for (i = 0; i < 4; i++)
    {
        LED_on((led_t)i, the_color);
    }
}

#endif
