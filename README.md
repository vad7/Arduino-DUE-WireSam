# Arduino DUE WireSam library

Arduino DUE TwoWire (I2C), EEPROM-FRAM, RTC libraries. 
Using interrupts.
Can replace Wire.h with some restrictions - just replace "Wire.h" with "WireSam.h".

Using external _delay() function for RTOS support.
Support non-blocking mode.

Included modified extEERPOM and DS3232RTC libraries.

// example:
#include "WireSam.h"
#include "extEEPROM.h"
#include "DS3232RTC.h"

// Inits:
extEEPROM eepromI2C(eeprom_size_in_kbit, number_of_eeprom_chips, eeprom_page_size, eeprom_base_address, is_it_FRAM_memory);
DS3232RTC rtcI2C;
