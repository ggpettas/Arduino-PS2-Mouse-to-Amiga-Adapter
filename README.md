# Arduino-PS2-Mouse-to-Amiga-Adapter
Connect a PS2 (or compatible USB Mouse) to an Amiga

Inline code for faster execution, remove arduino calls, fix glitch when change direction of mouse
OPTIMIZATION & FIXes:
  1. INLINE EXECUTION: Functions are marked 'inline' and use direct PORT register 
     manipulation instead of slow digitalwrite() calls for maximum speed.
  2. REMOVAL OF ARDUINO CALLS: Replaced the standard abs() function with a fast 
     inline ternary operator to eliminate function call overhead and dependencies.
  3. DIRECTION-CHANGE GLITCH FIX: State variables (QX/QY) are updated BEFORE driving 
     the physical pins. This keeps pins and states perfectly synchronized, eliminating 
     the 1-pixel reverse-direction jitter when reversing mouse travel.

## You will need:

  - Arduino (Nano/Uno/Mini/...). I used Pro Mini Atmega328P compatible
  - PS2 Mouse (or USB mouse that can be used with an usb-to-ps2 adapter)
  - Female DSub9 (to plug into the Amiga)
  
## Connect:
```
  PS2 Mouse         Arduino Pin
  CLOCK              D11 
  DATA               D12
  5V                 5V
  GND                GND
  
  DSub9 Female Pin   Arduino Pin
  1                   D2
  2                   D3
  3                   D4
  4                   D5
  5                   D8
  
  6                   D6
  7                   5V
  8                   GND 
  9                   D7
```

## Firmware
Upload the INO to the Arduino 

## Usage
Double-Check ! Connections !
Plug into Amiga and use your PS2 Mouse on the Workbench ...


## Thanks to:

  - Kristopher Chambers (kristopher) for the PS2Mouse library:
    http://github.com/kristopher/PS2-Mouse-Arduino/
  
  
  - Spark (cssvb94) for code from AmigaUSBMouseJoystick:
    https://github.com/cssvb94/AmigaUSBMouseJoystick
    
  
  


 
  


