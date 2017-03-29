#include <SoftwareSerial.h>
#include <Gpsneo.h>

Gpsneo gps(10,11);


char time[10];
char status[3];
char latitud[11];
char latitudHemisphere[3];
char longitud[11];
char longitudMeridiano[3];
char speedKnots[10];
char trackAngle[8];
char date[10];
char magneticVariation[10];
char magneticVariationOrientation[3];


void  setup()
{
    Serial.begin(9600);
}
void loop()
{
    //Just simple do getDataGPRMC, and the method solve everything.
    gps.getDataGPRMC(time,
                    status,
                    latitud,
                    latitudHemisphere,
                    longitud,
                    longitudMeridiano,
                    speedKnots,
                    trackAngle,
                    date,
                    magneticVariation,
                    magneticVariationOrientation);

   Serial.println(time);
   Serial.println(status);
   Serial.println(latitud);
   Serial.println(latitudHemisphere);
   Serial.println(longitud);
   Serial.println(longitudMeridiano);
   Serial.println(speedKnots);
   Serial.println(trackAngle);
   Serial.println(date);
   Serial.println(magneticVariation);
   Serial.println(magneticVariationOrientation);
}