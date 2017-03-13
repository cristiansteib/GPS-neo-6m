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
		void init (int baudrate);
		bool checksum(char * string);
		bool readSerial(char * buffer);
		char * getDataRaw(const __FlashStringHelper * look,char * buffer);



	public:
		Gpsneo(void);
		Gpsneo(uint8_t rx,uint8_t tx);
		Gpsneo(uint8_t rx,uint8_t tx, long baudrate);
		~Gpsneo(void);
		void getDataGPRMC(char *time,char * status,char * latitude,char *latitudHemisphere, char * longitude, char * longitudeMeridian,char * speedKnots,char * trackAngle,char * date,char * magneticVaration, char * magneticVarationOrientation);
		void getDataGPRMC(char * latitude, char * latitudHemisphere ,char * longitude,char * longitudeMeridian);
		void convertLongitude(char * longitude,char * returnValue); //convierte los datos de lectura que vienen por defecto , a grados. 
		void convertLatitude(char * longitude,char * returnValue);
		void Google(char *link);
		void getDataGPGSA();
	};
#endif 