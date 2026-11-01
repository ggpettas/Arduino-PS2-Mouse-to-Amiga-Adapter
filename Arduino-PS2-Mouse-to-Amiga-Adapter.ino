/**
 * Using the PS2Mouse library available from
 *   http://github.com/kristopher/PS2-Mouse-Arduino/
 * Original by Kristopher Chambers <kristopher.chambers@gmail.com>
 * Updated by Jonathan Oxer <jon@oxer.com.au>
 * 
 * Using code from Spark cssvb94
 *   https://github.com/cssvb94/AmigaUSBMouseJoystick
 *   
 * 
 * Data from PS2-Mouse:
 * 
 * data0[2:1:0]  ->  middle,right, left mousebutton
 * data1         ->  x from -127 to + 127   signed int
 * data2         ->  y from -127 to + 127   signed int
 * 
 * 
 * amiga mouse-port, looking at the amiga:
 * 
 * 9 PIN D-SUB MALE
 * 
 *   _________________________
 *   \                       /
 *    \  1   2   3   4   5  /
 *     \                   /
 *      \  6   7   8   9  /
 *       \_______________/
 * 
 * 1  V-pulse               
 * 2  H-pulse
 * 3  VQ-pulse
 * 4  HQ-pulse
 * 5  BUTTON 3 (Middle)
 * 6  BUTTON 1 (Lefl)
 * 7  +5V
 * 8  GND
 * 9  BUTTON 2 (Right)
 * 
 */

#include <avr/io.h>
#include <util/delay.h>
#include <PS2Mouse.h>

// Pin Definitions (ATmega328P / Arduino Uno)
// PS/2 Pins (PORTB)
#define MOUSE_DATA_PIN   12  // PB4 (Pin 12)
#define MOUSE_CLOCK_PIN  11  // PB3 (Pin 11)

// Amiga Pulses & Buttons (PORTD & PORTB)
// V_PULSE: PD2 (Pin 2)
// H_PULSE: PD3 (Pin 3)
// VQ_PLSE: PD4 (Pin 4)
// HQ_PLSE: PD5 (Pin 5)
// LMB:     PD6 (Pin 6)
// RMB:     PD7 (Pin 7)
// MMB:     PB0 (Pin 8)

PS2Mouse mouse(MOUSE_CLOCK_PIN, MOUSE_DATA_PIN, STREAM);

uint8_t QX = 3;
uint8_t QY = 3;

#define ADELAY 150 // σε microseconds

void setup() {
    mouse.initialize();

    // Set pin directions (DDR) as outputs (1)
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    DDRB |= (1 << PB0);

    // Initial states: Pulses LOW, Buttons HIGH (since they are active LOW)
    PORTD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));
    PORTD |= (1 << PD6) | (1 << PD7); // LMB, RMB
    PORTB |= (1 << PB0);              // MMB

    _delay_ms(200);
}

// Fast horizontal movement update (Without PROGMEM & without lookup tables)
inline void AMIGAHorizontalMove() {
    if (QX >= 2)            PORTD |= (1 << PD3);  else PORTD &= ~(1 << PD3);
    if (QX == 1 || QX == 2) PORTD |= (1 << PD5);  else PORTD &= ~(1 << PD5);

    _delay_us(ADELAY);
}

// Fast vertical movement update
inline void AMIGAVerticalMove() {
    if (QY >= 2)            PORTD |= (1 << PD2);  else PORTD &= ~(1 << PD2);
    if (QY == 1 || QY == 2) PORTD |= (1 << PD4);  else PORTD &= ~(1 << PD4);

    _delay_us(ADELAY);
}

void loop() {
    int16_t data[3];
    mouse.report(data);

    // Button handling (Active Low)
    if (data[0] & 1) PORTD &= ~(1 << PD6); else PORTD |= (1 << PD6); // LMB
    if (data[0] & 2) PORTD &= ~(1 << PD7); else PORTD |= (1 << PD7); // RMB
    if (data[0] & 4) PORTB &= ~(1 << PB0); else PORTB |= (1 << PB0); // MMB

    // Calculate movement steps
    uint8_t xsteps = abs(data[1]);
    uint8_t ysteps = abs(data[2]);
    uint8_t xsign = (data[1] > 0);
    uint8_t ysign = (data[2] > 0);

    // X/Y Movement handling
    while ((xsteps | ysteps) != 0) {
        if (xsteps != 0) {
            if (xsign) {
                QX = (QX <= 0) ? 3 : (QX - 1);
                AMIGAHorizontalMove();
            } else {
                QX = (QX >= 3) ? 0 : (QX + 1);
                AMIGAHorizontalMove();
            }
            xsteps--;
        }

        if (ysteps != 0) {
            if (ysign) {
                QY = (QY >= 3) ? 0 : (QY + 1);
                AMIGAVerticalMove();
            } else {
                QY = (QY <= 0) ? 3 : (QY - 1);
                AMIGAVerticalMove();
            }
            ysteps--;
        }
    }
}
