/* Christopher Martin 
 * Winter 2015
 * Common switching functions 
 */
#ifndef _SWITCHING_H_
#define _SWITCHING_H_

#define PROBE_1_OFF (1 << 4)
#define PROBE_2_OFF (1 << 5)
#define PROBE_3_OFF (1 << 6)
#define PROBE_4_OFF (1 << 7)

#define PROBE_1_ON ~PROBE_1_OFF 
#define PROBE_2_ON ~PROBE_1_OFF 
#define PROBE_3_ON ~PROBE_1_OFF 
#define PROBE_4_ON ~PROBE_1_OFF 


// possible channels to connect probes to.
typedef enum {CH_A, CH_B} channel_t;

/* 
 * The state a probe can be in. 
 * PROBE_OFF   - Hi-Z/disconnected from any channel.
 * CNTD_2_CH_A - probe is connected to channel A.
 * CNTD_2_CH_B - probe is connected to channel B.
 */
typedef enum {PROBE_OFF, CNTD_2_CH_A, CNTD_2_CH_B} probe_state_t;

// probe names to map to probe state array.
typedef enum {PROBE_1 = 0, PROBE_2 = 1, PROBE_3 = 2, PROBE_4 = 3} probe_t;

// Currnet state of the probes. Is a probe off, connected to channel A or channel B?
probe_state_t probe_status[4] = {CNTD_2_CH_A, PROBE_OFF, PROBE_OFF, CNTD_2_CH_B};

/*
 * Set up the data direction registers for controlling the switches.
 */
void switches_init(void)
{
    DDRB |= 0xF0;
    DDRC |= 0xF0;
}

/*
 * Sets the correct GPIO pins so the switches connect the given probe to 
 * the given channel. 
 */
void connect(probe_t probe, channel_t channel)
{
    switch (probe)
    {
        case PROBE_1:
            PORTB &= PROBE_1_ON;
            if (channel == CH_A)
            {
                PORTC &= ~(1 << 4);
                PORTC &= ~(1 << 6);
            }
            else if (channel == CH_B)
            {
                PORTC |= (1 << 4);
                PORTC |= (1 << 7);
            }
        break;

        case PROBE_2:
            PORTB &= PROBE_2_ON;
            if (channel == CH_A)
            {
                PORTC |= (1 << 4);
                PORTC &= ~(1 << 6);
            }
            else if (channel == CH_B)
            {
                PORTC &= ~(1 << 4);
                PORTC |= (1 << 7);
            }
        break;

        case PROBE_3:
            PORTB &= PROBE_3_ON;
            if (channel == CH_A)
            {
                PORTC &= ~(1 << 5);
                PORTC |= (1 << 6);
            }
            else if (channel == CH_B)
            {
                PORTC |= (1 << 5);
                PORTC &= ~(1 << 7);
            }
        break;

        case PROBE_4:
            PORTB &= PROBE_4_ON;
            if (channel == CH_A)
            {
                PORTC |= (1 << 5);
                PORTC |= (1 << 6);
            }
            else if (channel == CH_B)
            {
                PORTC &= ~(1 << 5);
                PORTC &= ~(1 << 7);
            }
        break;
    }
}

/*
 * Set a probe to Hi-Z effectively turning it off.
 */
void probe_off(probe_t probe)
{
    switch (probe)
    {
        case PROBE_1: PORTB |= PROBE_1_OFF; break;
        case PROBE_2: PORTB |= PROBE_2_OFF; break;
        case PROBE_3: PORTB |= PROBE_3_OFF; break;
        case PROBE_4: PORTB |= PROBE_4_OFF; break;
    }
}

/*
 * Sets all probes to Hi-Z effectively turning off all probes
 */
void all_probes_off(void)
{
    // this is faster than a for loop calling probe_off
    PORTB |= 0xF0;
}

/*
 * Call the correct function with proper parameters based on the received command from host PC. 
 * Command letters A - L are the current protocol
 */
void function_lookup(char cmd)
{
    switch (cmd)
    {
        case 'A': connect(PROBE_1, CH_A); break;
        case 'B': connect(PROBE_2, CH_A); break;
        case 'C': connect(PROBE_3, CH_A); break;
        case 'D': connect(PROBE_4, CH_A); break;

        case 'E': connect(PROBE_1, CH_B); break;
        case 'F': connect(PROBE_2, CH_B); break;
        case 'G': connect(PROBE_3, CH_B); break;
        case 'H': connect(PROBE_4, CH_B); break;

        case 'I': probe_off(PROBE_1); break;
        case 'J': probe_off(PROBE_2); break;
        case 'k': probe_off(PROBE_3); break;
        case 'L': probe_off(PROBE_4); break;
        default: break;
    }
}

#endif
