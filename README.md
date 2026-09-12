# Automotive Black Box / Event Data Recorder

A PIC16F877A-based embedded automotive black box prototype that monitors vehicle operating parameters, records events in external EEPROM, and provides password-protected access to stored data.

## Features

- Real-time clock using DS1307 RTC
- Speed measurement using ADC
- Gear up/down control
- Collision/event recording
- External I2C EEPROM event logging
- Retains the 10 most recent events
- View and clear stored logs
- UART log download to a laptop
- Password-protected menu
- Three password attempts with retry delay
- Set RTC time
- Change password
- Timer1-based timing
- Switch debounce
- Developed and tested using PICSimLab

## Hardware

| Component | Purpose |
|---|---|
| PIC16F877A | Main controller |
| 20 MHz Crystal | System clock |
| 16x2 CLCD | Dashboard and user interface |
| DS1307 | Real-time clock |
| External I2C EEPROM | Event storage |
| ADC input | Simulated vehicle speed |
| Push buttons | Vehicle/user controls |
| UART | Log download |
| PICSimLab | Simulation |
| Tera Term | UART terminal |

## Technologies

**Embedded C · PIC16F877A · XC8 · MPLAB X · PICSimLab · GPIO · ADC · I2C · UART · Timer1 · DS1307 · EEPROM · CLCD**

## System Operation

### Dashboard

The dashboard displays:

```text
TIME      EV  SP
10:25:32  G1  45
```

Gear states:

```text
GN = Neutral
G1 = Gear 1
G2 = Gear 2
G3 = Gear 3
G4 = Gear 4
G5 = Gear 5
GR = Reverse
_C = Collision / Event
```

A gear change or collision event is stored with its current time and speed.

### Event Record

Each EEPROM record is exactly 10 bytes:

```text
Hour     2 bytes
Minute   2 bytes
Second   2 bytes
Gear     2 bytes
Speed    2 bytes
-----------------
Total   10 bytes
```

The compact format reduces RAM usage and directly matches the EEPROM record size.

### EEPROM Storage

Records occupy consecutive 10-byte blocks:

```text
Event 0 ->  0 - 9
Event 1 -> 10 - 19
...
Event 9 -> 90 - 99
```

Only the 10 most recent events are retained. When the storage is full, records are shifted:

```text
Before: E1 E2 E3 E4 E5 E6 E7 E8 E9 E10
New E11
After:  E2 E3 E4 E5 E6 E7 E8 E9 E10 E11
```

The implementation reads one record at a time instead of keeping all events in an MCU RAM array.

## Password Protection

The menu is protected by a four-digit password consisting of `0` and `1`.

```text
SW4 -> 0
SW5 -> 1
```

Three incorrect attempts are permitted. After three failures, a retry delay is applied.

The password can be changed from the menu by entering and re-entering the new four-digit password.

## Main Menu

After successful authentication:

```text
View Log
Clear Log
Download Log
Set Time
Change Password
```

### View Log

Two events are displayed at a time. The user can scroll through records stored in EEPROM without loading the complete log into RAM.

### Clear Log

Clears the current logical event count and returns to the menu after the confirmation message.

### Download Log

All stored records are transmitted to a laptop through UART.

Tera Term settings:

```text
Baud Rate    : 9600
Data Bits    : 8
Parity       : None
Stop Bits    : 1
Flow Control : None
```

Example:

```text
---------------------
Events Logs
---------------------
10:25:32  G1  45 km/h
10:26:14  G2  58 km/h
10:27:03  G3  72 km/h
---------------------
End of Logs
---------------------
```

The UART transmitter is enabled only during the download operation.

### Set Time

The DS1307 time can be modified through the menu.

```text
Seconds -> Minutes -> Hours
```

The entered values are converted to BCD and written back to the RTC.

## Timer1

Timer1 provides the timing base for password timeout, retry delay, and other timed operations.

For a 20 MHz oscillator:

```text
Instruction clock = 20 MHz / 4 = 5 MHz
Instruction cycle = 200 ns
```

With a 1:8 Timer1 prescaler:

```text
Timer1 tick = 1.6 us
```

For approximately 100 ms:

```text
100 ms / 1.6 us = 62,500 counts
```

Preload:

```text
65,536 - 62,500 = 3,036 = 0x0BDC
```

Therefore:

```text
TMR1H = 0x0B
TMR1L = 0xDC
```

Ten 100 ms overflows are counted to generate a one-second flag.

## Switch Mapping

### Dashboard

`SW0 = Collision` | `SW1 = Gear Up` | `SW2 = Gear Down` | `SW3 = Menu`

### Password

`SW4 = 0` | `SW5 = 1`

### Menu

`SW3 = Up` | `SW4 = Down` | `SW5 = Select` | `SW0 = Back`

### View Log

`SW3 = Up` | `SW4 = Down` | `SW0 = Back`

### Set Time

`SW3 = Decrease` | `SW4 = Increase` | `SW2 = Next Field` | `SW5 = Confirm` | `SW0 = Cancel`

## Software Architecture

The application is divided into peripheral and application modules for easier testing and maintenance.

Typical modules include:

```text
main
 ├── CLCD
 ├── Digital Keypad
 ├── ADC
 ├── I2C
 ├── External EEPROM
 ├── DS1307 RTC
 ├── UART
 └── Timer1
```

The main application manages the dashboard, authentication, menu, event logging, log viewing, log downloading, time setting, and password management.

## Important Implementation Decisions

### RAM optimization

The PIC16F877A has limited RAM. Storing ten complete event structures in an array caused memory allocation problems. The final design uses one `event_t` buffer and reads/processes EEPROM records one at a time.

### Fixed-size event records

The event fields are stored as two-character arrays rather than null-terminated strings. This keeps the record compact at exactly 10 bytes.

Consequently, individual characters are used when displaying or transmitting fields rather than passing them directly to string functions.

### Chronological log storage

When ten records already exist, the EEPROM records are shifted left and the newest event is written to the final slot. This keeps physical storage in chronological order.

### Timer-based delays

Timer1 is used for timing-sensitive delays instead of relying only on long blocking software delays.

### Menu control flow

`menu_handling()` owns the continuous menu loop. Menu functions return to the menu instead of recursively calling it.

## Debugging Highlights

### ADC initialization

The dashboard initially appeared to freeze because `read_adc()` waited for conversion completion while ADC initialization had not been performed. Adding `adc_init()` before ADC reads resolved the issue.

### Watchdog reset

Unexpected watchdog resets affected the application display. The watchdog was disabled for the current configuration.

### Switch bounce

A single press could produce multiple logical key presses, especially during password entry. A debounce delay and press/release state mechanism were added to the keypad driver.

### Password display handling

The password prompt was being cleared by subsequent display operations. Display clearing and password prompt handling were reorganized so the validation routine controls its own screen.

### Timer accuracy

Software delay loops did not provide the required reliable timing. Timer1 interrupts were used to create a repeatable timing base.

### EEPROM write parameter order

The EEPROM driver uses:

```c
write_external_eeprom(data, address);
```

Using the parameters in the wrong order caused invalid writes and display corruption.

### RAM allocation

Creating `event_t events[10]` exceeded available MCU RAM. The implementation was changed to a single reusable event buffer.

### Non-null-terminated fields

The compact two-byte event fields are not C strings. Using `puts(event.hour)` or `clcd_print(event.hour, ...)` can read beyond the field. UART and LCD output therefore handles the two characters individually.

## Software Flow

```text
START
  |
  v
Initialize peripherals
  |
  v
Dashboard
  |
  +---- Gear change ----> Store event
  |
  +---- Collision ------> Store event
  |
  +---- Menu request ---> Password
                            |
                     +------+------+
                     |             |
                   Wrong         Correct
                     |             |
                   Retry        Main Menu
                                   |
          +------------+------------+-----------+------------+
          |            |            |           |            |
       View Log    Clear Log   Download Log  Set Time   Change Password
                                   |
                                  UART
                                   |
                               Tera Term
```

## Testing Environment

```text
Microcontroller : PIC16F877A
Clock           : 20 MHz
Compiler        : Microchip XC8
IDE             : MPLAB X
Simulation      : PICSimLab
UART Terminal   : Tera Term
UART Baud Rate  : 9600
```

## Future Enhancements

Possible extensions include:

- Engine temperature monitoring
- Sudden braking detection
- More detailed collision detection
- Persistent event-count metadata after reset
- Password persistence in EEPROM
- EEPROM write verification and wear management
- RPM monitoring
- Battery/voltage monitoring
- CAN bus integration
- PC-side log visualization
- More advanced event classification

## Learning Outcomes

This project provided practical experience with:

- Embedded C
- PIC16F877A architecture
- GPIO and switch interfacing
- Switch debouncing
- CLCD interfacing
- ADC configuration
- I2C communication
- External EEPROM storage
- DS1307 RTC
- UART communication
- Timer1 interrupts
- Memory optimization
- Modular embedded firmware
- Embedded debugging
- PICSimLab simulation and testing

## Project Status

The core prototype functionality has been implemented and tested in PICSimLab, including dashboard operation, event logging, password-protected menu access, log viewing, log clearing, RTC configuration, password change, and UART log download.
