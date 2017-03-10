
#include "Arduino.h"
#include "Gpsneo.h"
#include <avr/pgmspace.h> 


// --------------------SUBSTRING 

void substring(char * dest,char *string,int start,int end){
	if (start<0 || end<0){
		return;
	}
	int offset = end-start;
	strncpy(dest,((string)+start),offset);
	dest[offset] = '\0';   /* null character manually added */
	}

//----------------------------------------------------



// ------------ SEARCH FOR A STRING 
int indexOf(char * string,const __FlashStringHelper * search){
	int pos =  int(strstr_P(string,(const char*)search)-string);
	if (pos==NULL || pos<0)
		return -1;
	
	return pos;

}

int  indexOf(char * string,char * search,int offset){

	//Serial.println("IndexOf offset->");

	//if (strlen(search) > offset)
	//	return -1; //if the offset is greater than the string length,return -1 
	
	int ind = int (strstr(&string[offset],search));
	int pos = int  (ind - int(string));
	if (pos!=NULL && pos>0)
		return pos;
	return -1;
}


int  indexOf(char * string,char * search){
	int pos = int(strstr(string,search)-string);
	if (pos==NULL || pos<0)
		return -1;
	
	return pos;
}

//-----------------------------------

//------------------------Split String

void split(char * string,char * separator,unsigned int position,char *dest){
	int last;
	int end=0;
	for (int i = 0; i <= position; ++i)
	{
		last=end;
		end=indexOf(string,separator,last+1);		
	}
	int offset=end-last;
	//char dest[offset];
	strncpy(dest,((string)+last+1),offset-1);
	dest[offset]='\0';
	return ;

}
//------------------------------------------



Gpsneo::Gpsneo(void) : SoftwareSerial(RX_PIN_DEFAULT,TX_PIN_DEFAULT)
{  
	init(BAUDRATE_DEFAULT);
}

Gpsneo::~Gpsneo(void){
	Serial.println("destructor");

}

void Gpsneo::init(int baudrate){
	this->begin(baudrate);
	/*
	for (int i = 0; i < BUFFER_SIZE ; ++i)
	{
		string[i]='\0';
	}
	*/
}


bool Gpsneo::checksum(char * string){

	int end=indexOf(string,F("*"));
	if (end<0)
		return false;
	int checksum = 0x00;
	int i;
	for ( i = 0; i < end; ++i)
	{
		checksum = checksum ^ string[i];
	}

	char check[4]="";
	i=0;
	end++;
	
	// copy the checksum of the string 
	for (end; end < strlen(string)-1 ; ++end)
	{

		check[i]=string[end];
		i++;
		if (i>4){
			return false;
		}
	}
	check[i]='\0';

	//convert integer 'checksum' to String hex text 
	String cc = String(checksum,HEX);
	cc.toUpperCase();
	if (cc = check){
		delete &cc;
		return true;
	}
	else{
		return false;
	}
}


bool Gpsneo::readSerial(char * buffer){	

	int max_lenght=BUFFER_2-1; // limit the size for reading.
	int length=0;
	long unsigned int timeout;
	timeout = millis()+2000;
	while (!this->available() && millis()<timeout){}
	if (millis()>=timeout)
		return false; // if not receive a response from the gps, return false

	timeout = millis()+600; // 600ms for read data from serial port. 
	while (  millis()<timeout && length<max_lenght){
		if (this->available()){
		buffer[length]=char(this->read());	
		length++;
		}
	}
	buffer[BUFFER_2 -1]='\0';
	return true;
}


char *  Gpsneo::getDataRaw(const __FlashStringHelper * look,char * buffer){
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

	int end=-1;
	int start=-1;
	bool fail=true;
	uint8_t attemps=0;
	bool error;
	while (fail==true && attemps<2){
		if (readSerial(buffer)==false){
			break;
		}
		attemps++;
		start=indexOf(buffer,look);
		if (start>-1){
			end=indexOf(buffer,"\n",start+1);
			if (end>0){
		 		substring(&buffer[BUFFER_2],&buffer[BUFFER_1],start,end);
		 		return &buffer[BUFFER_2];
		 	}else{fail=true;}
		}else{fail=true;}
	}
	return NULL;

}


void Gpsneo::getDataGPRMC(char *time,char * status,char * latitude,char *latitudeMeridian, char * logitude, char * logitudeMeridian,char * speedKnots){
	char buffer[BUFFER_SIZE];
	char * string;
	string = &buffer[BUFFER_2];



	string=getDataRaw(F("GPRMC"),buffer);	
	
	if (string!=NULL){
		if (checksum(string)){
			//Serial.println(F("checksum OK---> "));
			Serial.println(string);
			split(string,",",1,time);
			split(string,",",2,status);
			split(string,",",3,latitude);
			split(string,",",4,latitudeMeridian);
			split(string,",",5,logitude);
			split(string,",",6,logitudeMeridian);
			split(string,",",7,speedKnots);

		//	Serial.println(string);
		}
	}

	return;

}


void Gpsneo::getDataGPGSA(){
	char buffer[BUFFER_SIZE];
	char * string;
	string = &buffer[BUFFER_2];

	string=getDataRaw(F("GPGSA"),buffer);

	if (checksum(string)){
		Serial.println(F("checksum OK---> "));
	//	Serial.println(string);
	}

	return;
}