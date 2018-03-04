// Test WireSam, extEEPROM, DS3232RTC library
// https://github.com/vad7/Arduino-DUE-WireSam


#include "Arduino.h"

#include <WireSam.h>
#include <extEEPROM.h>
#include <DS3232.h>

#define UART_SPEED        250000 //115200      // Скорость отладочного порта
#define I2C_SPEED         twiClock400kHz // Частота работы шины I2C
#define I2C_NUM_INIT      3           // Число попыток инициализации шины
#define I2C_TIME_WAIT     2000        // Время ожидания захвата мютекса шины I2C мсек
#define I2C_ADR_RTC       0x68        // Адрес чипа rtc на шине I2C
#define I2C_ADR_DS2482    0x18        // Адрес чипа OneWire на шине I2C

#define PIN_SPI_CS_W5XXX   10       // ++ ETH-CS   сигнал CS управление сетевым чипом
#define PIN_SPI_CS_SD      4        // ++ SD-CS    сигнал CS управление SD картой

#define I2C_ADR_EEPROM    0x50         // Адрес чипа eeprom на шине I2C
#define I2C_SIZE_EEPROM   kbits_256    // Объем чипа eeprom в килобитах
#define I2C_PAGE_EEPROM   32           // Размер страницы для чтения eeprom байты
#define I2C_FRAM_MEMORY   0		  // 1 - FRAM память


extEEPROM eepromI2C(I2C_SIZE_EEPROM,1,I2C_PAGE_EEPROM,I2C_ADR_EEPROM,I2C_FRAM_MEMORY);   // I2C eeprom Размер в килобитах, число чипов, страница в байтах, адрес на шине
DS3232 rtcI2C;                                                        //[V]* Часы 3231 на шине I2C

 __attribute__((always_inline)) inline void SPI_switchAllOFF()  // Все выключить
{
 // _delay(1);
  digitalWriteDirect(PIN_SPI_CS_SD,HIGH);  
  digitalWriteDirect(PIN_SPI_CS_W5XXX,HIGH);
  #ifdef SPI_FLASH
  digitalWriteDirect(PIN_SPI_CS_FLASH,HIGH);
  #endif
}

__attribute__((always_inline))  inline void digitalWriteDirect(int pin, boolean val)
{
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

void _delay(int ms)
{
	delay(ms);
}

static char _buf[16+1];
// Получить причину последнего сброса контроллера
char* ResetCause( void )
{
  const uint32_t resetCause = rstc_get_reset_cause(RSTC);
 // strcpy(_buf,"");
  switch ( resetCause )
  {
  case RSTC_GENERAL_RESET:  strcpy(_buf, "General" );  break;
  case RSTC_BACKUP_RESET:   strcpy(_buf, "Backup" );   break;
  case RSTC_WATCHDOG_RESET: strcpy(_buf, "Watchdog" ); break;
  case RSTC_SOFTWARE_RESET: strcpy(_buf, "Software" ); break;
  case RSTC_USER_RESET:     strcpy(_buf, "User" );     break;
  default:                  strcpy(_buf, "Unknown" );  break;
  }
  return _buf;
}


void setup() {
/*
	pinMode(87,INPUT_PULLUP);                   // SD Pin 87
	pinMode(77,INPUT_PULLUP);                   // Eth Pin 77  
	pinMode(PIN_SPI_CS_SD,INPUT_PULLUP);        // сигнал CS управление SD картой
	pinMode(PIN_SPI_CS_W5XXX,INPUT_PULLUP);     // сигнал CS управление сетевым чипом
	SPI_switchAllOFF();                          // Выключить все устройства на SPI
*/
// Борьба с зависшими устройствами на шине  I2C (в первую очередь часы) не удачный сброс
// https://forum.arduino.cc/index.php?topic=288573.0  
	pinMode(21, OUTPUT);
	for (int i = 0; i < 8; i++) {
    	digitalWriteDirect(21, HIGH); delayMicroseconds(3);
		digitalWriteDirect(21, LOW);  delayMicroseconds(3);
	}
	pinMode(21, INPUT);

    Serial.begin(UART_SPEED);
    Serial.print("Reset: ");
    Serial.println(ResetCause());


	delay(3000);

	
	uint8_t st;
	uint32_t M;
	uint8_t buffer[1024]; uint16_t i;

    st = eepromI2C.begin(I2C_SPEED);  // Инициализация памяти
	if(st) { Serial.print("I2C init: "); Serial.println(st); }



	_delay(100);


//	Serial.print("t_"); Serial.print(micros());
	st = eepromI2C.read(1, buffer, 1);
//	M = micros(); Serial.print("-"); Serial.println(M);
	if(st) {	Serial.print("\nError read 1, 1: "); Serial.println(st); }
//	else Serial.println(buffer[0], HEX);


	//_delay(2);


	st = eepromI2C.read(15, buffer, 15);
	if(st) { 	Serial.print("r16 st = "); Serial.println(st); }


	st = eepromI2C.read(16, buffer, 16);
	if(st) { 	Serial.print("r16 st = "); Serial.println(st); }

	st = eepromI2C.read(16, buffer, 16);
	if(st) { 	Serial.print("r16 st = "); Serial.println(st); }


	st = eepromI2C.read(17, buffer, 17);
	if(st) { 	Serial.print("r16 st = "); Serial.println(st); }

	st = eepromI2C.read(17, buffer, 17);
	if(st) { 	Serial.print("r16 st = "); Serial.println(st); }


	st = eepromI2C.read(32, buffer, 32);
	if(st) { 	Serial.print("r200 st = "); Serial.println(st); }


	st = eepromI2C.read(64, buffer, 64);
	if(st) { 	Serial.print("r37 st = "); Serial.println(st);}


	st = eepromI2C.read(100, buffer, 1);
	if(st) { 	Serial.print("r2 st = "); Serial.println(st);}


    Wire.beginTransmission(8);
    st = Wire.endTransmission();
    if(st) { Serial.print("Err device 8: "); Serial.println(st); }


	delay(1100);

	Serial.print("t_"); Serial.print(micros());
	st = eepromI2C.read(2, buffer, 32);
	M = micros(); Serial.print("-"); Serial.println(M);
	Serial.print("Read 2, 32: "); Serial.println(st);
	if(st == 0) {
   		for(uint j = 0; j < 32; j++) {
	   		Serial.print(buffer[j], HEX); Serial.print(" ");
   		}
   		Serial.print("\n");
	}


	delay(1500);
/*/

	buffer[0] = 0x11;  buffer[1] = 0x22; buffer[3] = 0x23; buffer[3] = 0x33; buffer[4] = 0x44;
//	Serial.print("t_"); Serial.print(micros());
   	st = eepromI2C.write(30, buffer, 5);
//	M = micros(); Serial.print("-"); Serial.println(M);
	//Serial.print("Write- 5: "); Serial.println(st);

	//delay(1600);


	if(st == 0) {
		//Serial.print("t_"); Serial.print(micros());
		st = eepromI2C.read(30, buffer, 5);
		//M = micros(); Serial.print("-"); Serial.println(M);
		Serial.print("Read- 5: "); Serial.println(st);
		if(st == 0) {
			for(uint j = 0; j < 5; j++) {
				Serial.print(buffer[j], HEX); Serial.print(" ");
			}
			Serial.print("\n");
		}
	}



	delay(1700);

*/

/*	Write skip

 	uint8_t buffer2[1024];
	uint16_t buffer16[320];

	delay(1000);

   	for(i = 0; i < 320; i++) {
   		buffer16[i] = (0x44 - i) * 256 + 0x22 + i ;
   	}

	//Serial.print("t_"); Serial.print(micros());
   	st = eepromI2C.write(0, (byte *)buffer16, 640);
	//M = micros(); Serial.print("-"); Serial.println(M);
	if(st) { Serial.print("Error write 0, 640: "); Serial.println(st); }


	if(st == 0) {
		//Serial.print("t_"); Serial.print(micros());
		st = eepromI2C.read(0, buffer2, 640);
		//M = micros(); Serial.print("-"); Serial.println(M);
		if(st) { Serial.print("Error read 0, 640: "); Serial.println(st); }
		if(st == 0) {
			for(i = 0; i < 10; i++) {
				for(uint j = 0; j < 64; j++) {
					if(((byte *)buffer16)[i * 64 + j] != buffer2[i * 64 + j]) {
						Serial.print("\nError at: ");
						Serial.print(i * 64 + j);
						Serial.print(" must: ");
						Serial.print(buffer[i * 64 + j], HEX);
						Serial.print(" != ");
					}
					Serial.print(buffer2[i * 64 + j], HEX); Serial.print(" ");
				}
				Serial.print("\n");
			}
		}
	}


	delay(500);

*/


	uint32_t eeprom_size = 4096;

	Serial.println("Erasing all eeprom...");
	memset(buffer, 0xFF, sizeof(buffer));
	for(uint16_t i = 0; i < eeprom_size / sizeof(buffer); i++) {
		if(st = eepromI2C.write(i * sizeof(buffer), buffer, sizeof(buffer))) {
		   Serial.print("\nError write at: ");
		   Serial.println(i * sizeof(buffer));
		   break;
		}
	}



	//dump memory
	    Serial.println("Dump EEPROM from 0x0000:");
		for(uint16_t _i = 0; _i < eeprom_size / 64; _i++) {
			uint8_t _st;
			if((_st = eepromI2C.read(_i * 64, (byte*)buffer, 64))) {
			   Serial.print("\nRead EEPROM error at: ");
			   Serial.println(_i * 64);
			   break;
			}
			for(uint8_t _j = 0; _j < 64; _j++) {
				_st =  buffer[_j];
				if(_st <= 0x0F) Serial.print('0');
				Serial.print(_st, HEX); Serial.print(' ');
			}
			Serial.println();
			WDT_Restart(WDT);
		}
	////dump memory


//*/

	delay(500);

   uint8_t eepStatus=0;
   /*
   for(uint8_t i=0; i<I2C_NUM_INIT; i++ )
   {
	   if ((eepStatus=eepromI2C.begin(I2C_SPEED))>0)    // переходим на 400 кгц  OK==0 все остальное ошибки и пытаемся инициализировать
	   {
		   Serial.print("ERROR SET SPEED I2C: "); Serial.println(eepStatus);
	   } else  {
		   Serial.print("I2C SPEED: "); Serial.println(I2C_SPEED/1000); break;
	   }  // Все хорошо
   }
   if (eepStatus!=0)  // если шина не инициализирована делаем попытку запустится на малой частоте один раз!
   {
	  if ((eepStatus=eepromI2C.begin(twiClock100kHz))>0) {
		  Serial.print("ERROR SET SPEED 100kHz: "); Serial.println(eepStatus);
	  } else Serial.print("SPEED 100k set");
	}
	*/

	 // Сканирование шины i2c
	if (eepStatus==0)   // есть инициализация
	{
		byte error, address;
		const byte start_address = 8;       // lower addresses are reserved to prevent conflicts with other protocols
		const byte end_address = 119;       // higher addresses unlock other modes, like 10-bit addressing

		for(address = start_address; address < end_address; address++ )
		 {
			  Wire.beginTransmission(address);
			  error = Wire.endTransmission();
			  if ((error==0)) //||(error==4))
			  {
			  Serial.print("=I2C device found at address: "); Serial.println(address);
			   switch (address)
					{
					case I2C_ADR_DS2482:  Serial.print(" - OneWire DS2482-100\n");                     break; // 0x18 есть варианты
					case I2C_ADR_EEPROM:  Serial.print(" - EEPROM AT24CXXX\n");                      break; // 0x50 возможны варианты
					case I2C_ADR_RTC   :  Serial.print(" - RTC DS3231\n");                             break; // 0x68

					default            :  Serial.print(" - Unknow\n");                                 break; // не определенный тип
					}
			  delay(100);
			  }
		//      else if (error==4) journal.jprintf("Unknown error at address %s\n",byteToHex(address)); // заставить рабоать не удалось
		  } // for
	} //  (eepStatus==0)


	Serial.println();
	Serial.print("DS3231 Temp: ");
	Serial.println(rtcI2C.temperature());

	tmElements_t tm;
	st = rtcI2C.read(tm);
	if(st) {
		Serial.print("Error read RTC: "); Serial.println(st);
	} else {
		Serial.print(tm.Day); Serial.print("."); Serial.print(tm.Month); Serial.print("."); Serial.print(tm.Year); Serial.print("  ");
		Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute); Serial.print(":"); Serial.print(tm.Second); Serial.print("\n");

		Serial.println("Set time");
		st = rtcI2C.setTime(23, 59, 59);
		if(st) {
			Serial.print("Error set RTC: "); Serial.println(st);
		} else {
			Serial.println("Read time");
			st = rtcI2C.read(tm);
			if(st) {
				Serial.print("Error read RTC: "); Serial.println(st);
			} else {
				Serial.print(tm.Day); Serial.print("."); Serial.print(tm.Month); Serial.print("."); Serial.print(tm.Year); Serial.print("  ");
				Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute); Serial.print(":"); Serial.print(tm.Second); Serial.print("\n");
			}
			Serial.println("Set  date");

			st = rtcI2C.setDate(31, 12, 2017);
			if(st) {
				Serial.print("Error set date RTC: "); Serial.println(st);
			}
			Serial.println("Read date");
			st = rtcI2C.read(tm);
			if(st) {
				Serial.print("Error read RTC: "); Serial.println(st);
			} else {
				Serial.print(tm.Day); Serial.print("."); Serial.print(tm.Month); Serial.print("."); Serial.print(tm.Year); Serial.print("  ");
				Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute); Serial.print(":"); Serial.print(tm.Second); Serial.print("\n");
			}

		}

	}

}


// The loop function is called in an endless loop
void loop()
{
    tmElements_t tm;
    byte st;

    Serial.print("Time: ");
    st = rtcI2C.read(tm);
    if(st) { Serial.print(" Error read RTC: "); Serial.println(st); }
	Serial.print(tm.Day); Serial.print("."); Serial.print(tm.Month); Serial.print("."); Serial.print(tm.Year); Serial.print(" ");
	Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute); Serial.print(":"); Serial.print(tm.Second);
	Serial.print(" Temp * 100 = ");
    Serial.println(rtcI2C.temperature());
	_delay(1000);
}
