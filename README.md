# Arduino DUE WireSam, extEEPROM, DS3232 libraries

Arduino DUE TwoWire (I2C), EEPROM-FRAM, RTC libraries.<br>
Using interrupts.<br>
Can replace Wire.h with some restrictions - just replace "Wire.h" with "WireSam.h".<br>

Using external _delay() function for RTOS support.<br>
Support non-blocking mode.<br>

Included modified extEERPOM and DS3232RTC libraries.

Example: Test.ino

Using:<br>
#include "WireSam.h"<br>
#include "extEEPROM.h"<br>
#include "DS3232RTC.h"<br>

// Inits:
extEEPROM eepromI2C(eeprom_size_in_kbit, number_of_eeprom_chips, eeprom_page_size, eeprom_base_address, is_it_FRAM_memory);<br>
DS3232RTC rtcI2C;<br>


