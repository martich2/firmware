/* Christopher Martin 
 * Winter 2015
 * Common switching functions 
 */
#ifndef _SWITCHING_H_
#define _SWITCHING_H_

// sets probe to Hi-Z/disconnect
#define PROBE_1_OFF (1 << 4)
#define PROBE_2_OFF (1 << 5)
#define PROBE_3_OFF (1 << 6)
#define PROBE_4_OFF (1 << 7)

// unsets probe from Hi-Z/disconnect to connected
#define PROBE_1_ON ~PROBE_1_OFF 
#define PROBE_2_ON ~PROBE_2_OFF 
#define PROBE_3_ON ~PROBE_3_OFF 
#define PROBE_4_ON ~PROBE_4_OFF 


// possible channels to connect probes to.
typedef enum {CH_A, CH_B} channel_t;

/* 
 * The state a probe can be in. 
 * HI_Z   - Hi-Z/disconnected from any channel.
 * CNTD_2_CH_A - probe is connected to channel A.
 * CNTD_2_CH_B - probe is connected to channel B.
 */
typedef enum {HI_Z, CNTD_2_CH_A, CNTD_2_CH_B} probe_state_t;

// probe names to map to probe state array.
typedef enum {PROBE_1 = 0, PROBE_2 = 1, PROBE_3 = 2, PROBE_4 = 3, NO_PROBE} probe_t;

// Currnet state of the probes. Is a probe off, connected to channel A or channel B?
probe_state_t probe_status[4] = {HI_Z, HI_Z, HI_Z, HI_Z};

// What are Channel A and B connected to?
probe_t ch_a_status = NO_PROBE;
probe_t ch_b_status = NO_PROBE;

// TODO: make hader and c files
void deactivate_old_probe(channel_t channel);

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
    deactivate_old_probe(channel);

    switch (probe)
    {
        case PROBE_1:
            PORTB &= PROBE_1_ON;
            if (channel == CH_A)
            {
                //TODO: should these be hard set values for hte upper nibble?
                PORTC &= ~(1 << 4);
                PORTC &= ~(1 << 6);
                ch_a_status = PROBE_1;
            }
            else if (channel == CH_B)
            {
                PORTC |= (1 << 4);
                PORTC |= (1 << 7);
                ch_b_status = PROBE_1;
            }
        break;

        case PROBE_2:
            PORTB &= PROBE_2_ON;
            if (channel == CH_A)
            {
                PORTC |= (1 << 4);
                PORTC &= ~(1 << 6);
                ch_a_status = PROBE_2;
            }
            else if (channel == CH_B)
            {
                PORTC &= ~(1 << 4);
                PORTC |= (1 << 7);
                ch_b_status = PROBE_2;
            }
        break;

        case PROBE_3:
            PORTB &= PROBE_3_ON;
            if (channel == CH_A)
            {
                PORTC &= ~(1 << 5);
                PORTC |= (1 << 6);
                ch_a_status = PROBE_3;
            }
            else if (channel == CH_B)
            {
                PORTC |= (1 << 5);
                PORTC &= ~(1 << 7);
                ch_b_status = PROBE_3;
            }
        break;

        case PROBE_4:
            PORTB &= PROBE_4_ON;
            if (channel == CH_A)
            {
                PORTC |= (1 << 5);
                PORTC |= (1 << 6);
                ch_a_status = PROBE_4;
            }
            else if (channel == CH_B)
            {
                PORTC &= ~(1 << 5);
                PORTC &= ~(1 << 7);
                ch_b_status = PROBE_4;
            }
        break;

        case NO_PROBE: break;
    }

    // clear B if it was previously used for the current probe.
    if ((channel == CH_A) && (ch_b_status == ch_a_status))
        ch_b_status = NO_PROBE;

    // clear A if it was previously used for current probe.
    if ((channel == CH_B) && (ch_a_status == ch_b_status))
        ch_a_status = NO_PROBE;
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
        case NO_PROBE: break;
    }

    // have to update channel status when disconnecting probes.
    if (ch_a_status == probe)
        ch_a_status = NO_PROBE;
    else if (ch_b_status == probe)
        ch_b_status = NO_PROBE;
}

/*
 * Sets all probes to Hi-Z effectively turning off all probes
 */
void all_probes_off(void)
{
    // this is faster than a for loop calling probe_off
    PORTB |= 0xF0;

    ch_a_status = NO_PROBE;
    ch_b_status = NO_PROBE;
}

/*
 * If a probe is connected to a given channel, disconnect it.
 * Deactive old probe connection to prevent undesired switching connections.
 */
void deactivate_old_probe(channel_t channel)
{
    if (channel == CH_A)
    {
        probe_off(ch_a_status);
        ch_a_status = NO_PROBE;
    }
    else if (channel == CH_B)
    {
        probe_off(ch_b_status);
        ch_b_status = NO_PROBE;
    }
}

/*
 * Call the correct function with proper parameters based on the received command from host PC. 
 * Command letters A - L are the current protocol. Z is for debug.
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
        case 'K': probe_off(PROBE_3); break;
        case 'L': probe_off(PROBE_4); break;

        // for debug TODO: remove in release
        case 'Z': all_probes_off(); break;
        default: break;
    }
}

#endif
