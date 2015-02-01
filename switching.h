/* Christopher Martin 
 * Winter 2015
 * Common switching functions 
 */
#ifndef _SWITCHING_H_
#define _SWITCHING_H_

// possible channels to connect probes to.
typedef enum {CHANNEL_A, CHANNEL_B} channel_t;

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
probe_state_t probe_status[4] = {CNTD_2_CH_A, PROBE_OFF. PROBE_OFF, CNTD_2_CH_B};

/*
 * Sets the correct GPIO pins so the switches connect the given probe to 
 * the given channel. 
 */
void connect(probe_t probe, channel_t channel)
{
}

/*
 * Set a probe to Hi-Z effectively turning it off.
 */
void probe_off(probe_t)
{
}

/*
 * Sets all probes to Hi-Z effectively turning off all probes
 */
void all_probes_off(void)
{
    uint8_t i;

    for (i = 0; i < sizeof(probe_status)/sizeof(probe_status[0]); i++)
    {
        probe_off((probe_t)i);
        probe_status[i] = PROBE_OFF;
    }
}

/*
 * Call the correct function with proper parameters based on the received command from host PC. 
 * Command letters A - L are the current protocol
 */
void function_lookup(char cmd)
{
    switch (cmd)
    {
        case A: connect(PROBE_1, CH_A); break;
        case B: connect(PROBE_2, CH_A); break;
        case C: connect(PROBE_3, CH_A); break;
        case D: connect(PROBE_4, CH_A); break;

        case E: connect(PROBE_1, CH_B); break;
        case F: connect(PROBE_2, CH_B); break;
        case G: connect(PROBE_3, CH_B); break;
        case H: connect(PROBE_4, CH_B); break;

        case I: probe_off(PROBE_1); break;
        case J: probe_off(PROBE_2); break;
        case k: probe_off(PROBE_3); break;
        case L: probe_off(PROBE_4); break;
        default: break;
    }
}

#endif
