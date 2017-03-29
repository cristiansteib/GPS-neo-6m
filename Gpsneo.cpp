
#include "Arduino.h"
#include "Gpsneo.h"
#include <avr/pgmspace.h> 


const char string_0[] PROGMEM = "http://www.google.com/maps/place/";
const char* const string_table[] PROGMEM = {string_0};
/*************************************************************************************************
*   MANEJO DE STRINGS
*************************************************************************************************/
int indexOf(const char *string,const __FlashStringHelper *search,uint8_t offset=0){
	int position = int(strstr_P(&string[offset],(const char*)search));
	if (position <= 0){
		return -1;
	}
	return position-int(string);
}


int indexOf(const char *string,const char *search, uint8_t offset=0){
	int position = int (strstr(&string[offset],search));
	if (position <= 0){
		return -1;
	}
	return position-int(string);
}

char * substring( char *destination, char *string,uint8_t start, uint8_t end){
	if (start<0 || end <0)
		return NULL;
	strncpy(destination,&string[start],end-start);
	destination[end-start]='\0';
	return destination;
}


char * split(char *destination,char *string,const char *separator,uint8_t position){
	int previous,last=-1;
	for (int i=0; i<=position;++i)
	{
		previous=last;
		last=indexOf(string,separator,previous+1);	
	}
	if (previous==-1)
		return NULL; //exceed the limit
	if (last==-1)
		last=strlen(string);//the last item in the list. 

	strncpy(destination,&string[previous+1],(last-previous)-1);
	destination[last-previous]='\0';
	return destination;

}
//*********************************************************************************************
//*********************************************************************************************


/*************************************************************************************************
*   Funciones utiles
*************************************************************************************************/
void intToHex(int number,char *destination){
  //maximum 4 digit
	//convert a number to hex in char
  char  hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','F'};
  char destInverted[4];
  //char dest[4];

  int i=0;
  while (number!=0){
    destInverted[i]=hex[number%16];
    number=number/16;
    i++;
  }

if (i==2){
destination[0]=destInverted[1];
destination[1]=destInverted[0];
destination[2]='\0';
  }else{
  	destination[0]=destInverted[0];
	destination[1]='\0';
  }
  
return;
}

/************************************************************************************
*
* 				CONSTRUCTORES
*
***********************************************************************************/
Gpsneo::Gpsneo(void) : SoftwareSerial(RX_PIN_DEFAULT,TX_PIN_DEFAULT)
{  
	init(BAUDRATE_DEFAULT);
}

Gpsneo::Gpsneo(uint8_t rx, uint8_t tx) : SoftwareSerial(rx,tx)
{  
	init(BAUDRATE_DEFAULT);
}

Gpsneo::Gpsneo(uint8_t rx, uint8_t tx,long unsigned int baudrate) : SoftwareSerial(rx,tx)
{  
	init(baudrate);
}

//*********************************************************************************************
//*********************************************************************************************

void Gpsneo::init(long unsigned int baudrate){
	this->begin(baudrate);
}


bool Gpsneo::checksum(char * sentence){
	//  52 microseconds.
	int end=indexOf(sentence,F("*"));
	if (end==-1)
		return false;
	unsigned int checksum = 0x00;
	int i;
	//Start in 1 to remove $ 
	for ( i = 1; i < end; i++)
	{
		checksum = checksum ^ sentence[i];
	}
	end++;
	char check[4]="";
	char checksumHEX[4];

	i=(strlen(sentence)-end);
	strncpy(check,&sentence[end],i);
	check[i-1]='\0';
	intToHex(checksum,checksumHEX);
	checksumHEX[i-1]='\0';

	if (strcmp(checksumHEX,check)==0)
		return true;
	return false;
}

bool Gpsneo::readSerial(char * buffer){	
	int length=0;
	_timeout = millis()+2000;
	while (!this->available() && millis()<_timeout){}
	if (millis()>=_timeout)
		return false; // if not receive a response from the gps, return false
	_timeout = millis()+600; // 600ms for read data from serial port. 
	while (  millis()<_timeout && length<BUFFER_SERIAL_SIZE){
		if (this->available()){
			buffer[length]=char(this->read());	
			length++;
		}
	}
	buffer[length]='\0';
	return true;
}


char *  Gpsneo::getDataRaw(const __FlashStringHelper * look,char * bufferParser){
	/*parameter look can be = 
   GPBOD - Bearing, origin to destination
   GPBWC - Bearing and distance to waypoint, great circle
   GPGGA - Global Positioning System Fix Data
   GPGLL - Geographic position, latitude / longitude
   GPGSA - GPS DOP and active satellites 
   GPGSV - GPS Satellites in view
   GPHDT - Heading, True
   GPR00 - List of waypoints in currently active route
   GPRMA - Recommended minimum specific Loran-C data
   GPRMB - Recommended minimum navigation info
   GPRMC - Recommended minimum specific GPS/Transit data
   GPRTE - Routes
   GPTRF - Transit Fix Data
   GPSTN - Multiple Data ID
   GPVBW - Dual Ground / Water Speed
   GPVTG - Track made good and ground speed
   GPWPL - Waypoint location
   GPXTE - Cross-track error, Measured
   GPZDA - Date & Time
	*/
	char bufferSerial[BUFFER_SERIAL_SIZE];
	int end=-1;
	int start=-1;
	bool fail=true;
	uint8_t attemps=0;
	bool error;
	while (fail==true && attemps<2){
		if (readSerial(bufferSerial)==false){
			break;
		}
		attemps++;
		start=indexOf(bufferSerial,look);
		if (start>-1){
			end=indexOf(bufferSerial,"\n",start+1);
			if (end>0){
		 		substring(bufferParser,bufferSerial,start,end);
		 		return bufferParser;
		 	}else{fail=true;}
		}else{fail=true;}
	}

	return NULL;

}



void Gpsneo::test(){
		char x[BUFFER_PARSER_SIZE]="";
		char s[70]="$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62";
		long int start;
		long int endd;
		//getDataRaw(F("$GPRMC"),x);

		start=micros();
		checksum(s);
		endd=micros();
		Serial.println(endd-start);
	
	//Serial.println(x);
	}