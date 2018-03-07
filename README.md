# Arduino DUE WireSam library, extEEPROM, DS3232, DS2482 libraries

Arduino DUE libraries for I2C devices:<br>
1. Main Wire (I2C) class,<br>
2. EEPROM or FRAM memory,<br>
3. Real Time Clock (RTC) - DS3231 or DS3232,<br>
4. OneWire-I2C bridge - DS2482.<br>
<br>
Wire class is using interrupts.<br>
Can replace #include "Wire.h" with some restrictions - just replace "Wire.h" with "WireSam.h".<br>
<br>
Using external _delay() function for RTOS support.<br>
Support non-blocking mode.<br>

Included modified extEERPOM and DS3232 libraries.

Example: Test.ino<br>

Using:<br>
----------------------------<br>
#include "WireSam.h"<br>
#include "extEEPROM.h"<br>
#include "DS3232.h"<br>
#include "DS2482.h"<br>

// Init example:<br>
extEEPROM eepromI2C(eeprom_size_in_kbit, number_of_eeprom_chips, eeprom_page_size, eeprom_base_address, is_it_FRAM_memory);<br>
DS3232 rtcI2C;<br>
#define I2C_ADR_DS2482 0x18<br>
DS2482 OneWireBus(I2C_ADR_DS2482);<br>

void _delay(int ms) {<br>
	if(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) delay(ms);<br>
	else vTaskDelay(ms / portTICK_PERIOD_MS);<br>
}<br>

