#ifndef Gpsneo_h
	#define Gpsneo_h
	#include <SoftwareSerial.h>
	//  UNCOMMENT WHAT YOU ARE GOING TO USE
	//#define GPGSA

	#define RX_PIN_DEFAULT 10
	#define TX_PIN_DEFAULT 11
	#define BAUDRATE_DEFAULT 9600
	#define BUFFER_SIZE 600
	#define BUFFER_1 0
	#define BUFFER_2 500
	//#define DEBUG

	class Gpsneo : public SoftwareSerial
	{
		bool checksum(char *sentence);
	public:
		Gpsneo(void);
		Gpsneo(uint8_t rx,uint8_t tx);
		Gpsneo(uint8_t rx,uint8_t tx,long unsigned int baudrate);
		void init (long unsigned int baudrate);
		void test();

	};
#endif 