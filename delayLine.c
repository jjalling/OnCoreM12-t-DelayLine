/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 *
 * File:   delayLine.c
 * Author: Jonas Bo Jalling
 *
 */

// CONFIG
#pragma config WDTE = OFF       // Watchdog Timer (WDT disabled)
#pragma config CP = OFF         // Code Protect (Code protection off)
#pragma config MCLRE = OFF      // Master Clear Enable (GP3/MCLR pin fuction is digital I/O, MCLR internally tied to VDD)

#define _XTAL_FREQ 4000000UL

#define RxD     GP3
#define DS_CLK  GP1
#define DS_D    GP0
#define DS_LE   GP2

#define WAIT_FOR_COMMAND0   0
#define WAIT_FOR_COMMAND1   1
#define WAIT_FOR_COMMAND2   2
#define WAIT_FOR_COMMAND3   3
#define WAIT_FOR_DATA       4
#define READ_SAWTOOTH       5

#include <xc.h>

void main(void) {
    OPTION = 192; // Disable GPWU, GPPU
    OSCCAL = 0;
    TRISGPIO = 8; // RxD input, others output

    DS_CLK = 0;
    DS_LE = 0;
    DS_D = 0;
    
    unsigned char carryFlag;
    unsigned char bitCounter;
    unsigned char rxByte;
    unsigned char state = WAIT_FOR_COMMAND0;
    unsigned char sawToothByte = 0;
    unsigned char dummyByteCount;

    while (1)
    {
        // Reset
        bitCounter = 9; // 8 bytes + stop-bit
        carryFlag = 1;
        rxByte = 0;

        while (RxD)
        {
            // Wait for UART start-bit
        }
        __delay_us(52);
        while (bitCounter != 0)
        {
            __delay_us(87);
            if (RxD)
            {
                rxByte |= carryFlag;
            }
            carryFlag <<=1;    
            bitCounter--;
        }

        // Byte received - parse it
        switch (state)
        {
            case WAIT_FOR_COMMAND0:
                dummyByteCount = 0;
                if (rxByte == '@')
                    state = WAIT_FOR_COMMAND1;

                break;

            case WAIT_FOR_COMMAND1:
                if (rxByte == '@')
                    state = WAIT_FOR_COMMAND2;
                break;

            case WAIT_FOR_COMMAND2:
                if (rxByte == 'H')
                    state = WAIT_FOR_COMMAND3;
                break;

            case WAIT_FOR_COMMAND3:
                if (rxByte == 'n')
                    state = WAIT_FOR_DATA;
                break;

            case WAIT_FOR_DATA:
                dummyByteCount++;
                if (dummyByteCount == 10)
                    state = READ_SAWTOOTH;
                break;

            case READ_SAWTOOTH:
                sawToothByte = rxByte;
                state = WAIT_FOR_COMMAND0;
                break;

            default:
                state = WAIT_FOR_COMMAND0;
        }

        // We have received the correct byte, send it to the delayline
        if (sawToothByte)
        {
            DS_LE = 1;

            bitCounter = 8;
            char mask;
            while (bitCounter != 0)
            {
                mask = (sawToothByte & 0x01);
                sawToothByte >>= 1;
                if (mask)
                {
                    DS_D = 1;
                }
                else
                {
                    DS_D = 0;
                }
                DS_CLK = 1;
                DS_CLK = 0;
                bitCounter--;
            }
            DS_LE = 0;
            sawToothByte = 0;
        }
    }
}
