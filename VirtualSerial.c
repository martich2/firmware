/*
   LUFA Library
   Copyright (C) Dean Camera, 2014.

   dean [at] fourwalledcubicle [dot] com
   www.lufa-lib.org
   */

/*
   Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

   Permission to use, copy, modify, distribute, and sell this
   software and its documentation for any purpose is hereby granted
   without fee, provided that the above copyright notice appear in
   all copies and that both that the copyright notice and this
   permission notice and warranty disclaimer appear in supporting
   documentation, and that the name of the author not be used in
   advertising or publicity pertaining to distribution of the
   software without specific, written prior permission.

   The author disclaims all warranties with regard to this
   software, including all implied warranties of merchantability
   and fitness.  In no event shall the author be liable for any
   special, indirect or consequential damages or any damages
   whatsoever resulting from loss of use, data or profits, whether
   in an action of contract, negligence or other tortious action,
   arising out of or in connection with the use or performance of
   this software.
   */

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "VirtualSerial.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
    .Config =
    {
        .ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
        .DataINEndpoint           =
        {
            .Address          = CDC_TX_EPADDR,
            .Size             = CDC_TXRX_EPSIZE,
            .Banks            = 1,
        },
        .DataOUTEndpoint =
        {
            .Address          = CDC_RX_EPADDR,
            .Size             = CDC_TXRX_EPSIZE,
            .Banks            = 1,
        },
        .NotificationEndpoint =
        {
            .Address          = CDC_NOTIFICATION_EPADDR,
            .Size             = CDC_NOTIFICATION_EPSIZE,
            .Banks            = 1,
        },
    },
};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;

void tcnt0_init(void);
// TODO move this to the LED file
void status_leds(probe_t ch_a, probe_t ch_b);

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    char buff = 0;
    SetupHardware();
    all_probes_off();
    LED_all_off();
    


    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

    //LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
    GlobalInterruptEnable();

    for (;;)
    {
        //CheckJoystickMovement();

        if (CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface) > 0 )
        {
            buff = (char)CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

            // range of valid commands
            if ((buff >= 'A') || (buff <= 'L') || (buff == 'Z'))
            {
                function_lookup(buff);
                status_leds(ch_a_status, ch_b_status);
                
                /*
                    LED_all_color(GREEN);
                    TCCR0B = (1 << CS02) | (1 << CS00); // start blinking LED
               */ 
            }
        }

        /* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
        CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

        // must happen for API to work
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    // USB status LEDs (4:1)
    LED_init();
    switches_init();

    /* Hardware Initialization */
    //Joystick_Init();
    //LEDs_Init();
    tcnt0_init();
    USB_Init();
}

/** Checks for changes in the position of the board joystick, sending strings to the host upon each change. */
#if 0
void CheckJoystickMovement(void)
{
    uint8_t     JoyStatus_LCL = Joystick_GetStatus();
    char*       ReportString  = NULL;
    static bool ActionSent    = false;

    if (JoyStatus_LCL & JOY_UP)
        ReportString = "Joystick Up\r\n";
    else if (JoyStatus_LCL & JOY_DOWN)
        ReportString = "Joystick Down\r\n";
    else if (JoyStatus_LCL & JOY_LEFT)
        ReportString = "Joystick Left\r\n";
    else if (JoyStatus_LCL & JOY_RIGHT)
        ReportString = "Joystick Right\r\n";
    else if (JoyStatus_LCL & JOY_PRESS)
        ReportString = "Joystick Pressed\r\n";
    else
        ActionSent = false;

    if ((ReportString != NULL) && (ActionSent == false))
    {
        ActionSent = true;

        /* Write the string to the virtual COM port via the created character stream */
        fputs(ReportString, &USBSerialStream);

        /* Alternatively, without the stream: */
        // CDC_Device_SendString(&VirtualSerial_CDC_Interface, ReportString);
    }
}
#endif 

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    //LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
    LED_all_off();
    LED_on(LED2, YELLOW);
    LED_on(LED3, YELLOW);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    //LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
    LED_all_off();
    LED_on(LED1, YELLOW);
}


/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

    //LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
    if (ConfigSuccess)
    {
        LED_all_off();
        LED_on(LED2, YELLOW);
        LED_on(LED4, YELLOW);
    }
    else
    {
        LED_all_off();
        LED_on(LED1, YELLOW);
        LED_on(LED3, YELLOW);
    }
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

void tcnt0_init(void)
{
    TCCR0A = 0x00;  // normal mode, OC0A & OC0B pins disconnected, TOP/MAX = FF
    TCCR0B = 0x00; //(1 << CS02) | (1 << CS00);   // 1024 prescaling, no force compare
    TIMSK0 = (1 << TOIE0);  // interrupt on overflow
}

// ((1/(16,000,000/1,024))*256*64) = 1.048576s
ISR(TIMER0_OVF_vect)
{
    static uint8_t tick = 0;
    tick++;

    if ((tick % 32) == 0)
    {
        //fputs("Hello\r\n", &USBSerialStream);
        LED_all_toggle();
    }
}

// TODO: Move the LED file
/*
 * Sets the front pannel status LEDs to the correct color based on 
 * probe to channel connection
 */
void status_leds(probe_t ch_a, probe_t ch_b)
{
   LED_all_off();

   switch (ch_a)
   {
       case PROBE_1: LED_on(LED1, GREEN); break;
       case PROBE_2: LED_on(LED2, GREEN); break;
       case PROBE_3: LED_on(LED3, GREEN); break;
       case PROBE_4: LED_on(LED4, GREEN); break;
       case NO_PROBE: break;
   }
   
   switch (ch_b)
   {
       case PROBE_1: LED_on(LED1, YELLOW); break;
       case PROBE_2: LED_on(LED2, YELLOW); break;
       case PROBE_3: LED_on(LED3, YELLOW); break;
       case PROBE_4: LED_on(LED4, YELLOW); break;
       case NO_PROBE: break;
   }
}
