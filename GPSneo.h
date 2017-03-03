#ifndef GPSneo_h
#define GPSneo_h
#include "Arduino.h" // 

class GPSneo
{
private:
		bool checksum(String *toCheck,String *checksum);
		bool checksum(String *toCheck,uint8_t *checksum);
	public:
		GPSneo();
	
};