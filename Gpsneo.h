#ifndef Gpsneo_h
	#define Gpsneo_h
	#include <SoftwareSerial.h>
	//  UNCOMMENT WHAT YOU ARE GOING TO USE
	//#define GPGSA

	#define RX_PIN_DEFAULT 10
	#define TX_PIN_DEFAULT 11
	#define BAUDRATE_DEFAULT 9600

	#define BUFFER_SERIAL_SIZE 500
	#define BUFFER_PARSER_SIZE 100

	//#define DEBUG

	class Gpsneo : public SoftwareSerial
	{
		long unsigned int _timeout; //used for restrict the time.

		bool checksum(char *sentence);
		bool readSerial(char *buffer);
		char *getDataRaw(const __FlashStringHelper * look,char * bufferParser);
	public:
		Gpsneo(void);
		Gpsneo(uint8_t rx,uint8_t tx);
		Gpsneo(uint8_t rx,uint8_t tx,long unsigned int baudrate);
		void init (long unsigned int baudrate);
		void test();

	};
#endif 