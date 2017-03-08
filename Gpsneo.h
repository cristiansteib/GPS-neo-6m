#ifndef Gpsneo_h
	#define Gpsneo_h
	#include <SoftwareSerial.h>

	#define RX_PIN_DEFAULT 10
	#define TX_PIN_DEFAULT 11
	#define BAUDRATE_DEFAULT 9600
	#define BUFFER_SIZE 600
	#define BUFFER_1 0
	#define BUFFER_2 500


	class Gpsneo : public SoftwareSerial
	{
		char  string[BUFFER_SIZE];
		void init (int baudrate);
		bool checksum(char * string);
		bool readSerial();
		char * getDataRaw(const __FlashStringHelper * look);



	public:
		Gpsneo(void);
		~Gpsneo(void);
		void getDataGPRMC();
		void getDataGPGSA();
	};
#endif 