#include <SoftwareSerial.h>
#include <Gpsneo.h>

Gpsneo gps(10,11);

char link[70];



void  setup()
{
Serial.begin(9600);
Serial.println("start-------------------------------");
}

void loop()
{	
	gps.Google(link);
	
}