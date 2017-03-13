
#include "Arduino.h"
#include "Gpsneo.h"
#include <avr/pgmspace.h> 


const char string_0[] PROGMEM = "http://www.google.com/maps/place/";
const char* const string_table[] PROGMEM = {string_0};

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


//--------------------------------------------Constructors

Gpsneo::Gpsneo(void) : SoftwareSerial(RX_PIN_DEFAULT,TX_PIN_DEFAULT)
{  
	init(BAUDRATE_DEFAULT);
}

Gpsneo::Gpsneo(uint8_t rx, uint8_t tx) : SoftwareSerial(rx,tx)
{  
	init(BAUDRATE_DEFAULT);
}

Gpsneo::Gpsneo(uint8_t rx, uint8_t tx,long baudrate) : SoftwareSerial(rx,tx)
{  
	init(baudrate);
}
//---------------------------------------------------------------
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
	buffer[length-1]='\0';
	//buffer[BUFFER_2 -1]='\0';
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


void Gpsneo::getDataGPRMC(char * latitude, char * latitudHemisphere ,char * longitude,char * longitudeMeridian){
	char x[2];
	x[0]='z';
	x[1]='\0';
	getDataGPRMC(&x[0],&x[0],latitude,latitudHemisphere,longitude,longitudeMeridian,&x[0],&x[0],&x[0],&x[0],&x[0]);
	return;
	
}

void Gpsneo::getDataGPRMC(char *time,char * status,char * latitude,char *latitudHemisphere, char * longitude, char * longitudeMeridian,char * speedKnots,char * trackAngle,char * date,char * magneticVaration,char * magneticVarationOrientation){
	#ifdef DEBUG
		Serial.println(F("getDataGPRMC"));
	#endif
	char buffer[BUFFER_SIZE];
	char * string;
	string = &buffer[BUFFER_2];

	string=getDataRaw(F("GPRMC"),buffer);	
	#ifdef DEBUG
		Serial.println(string);
	#endif
	if (string!=NULL){
		if (checksum(string)){
			if (time[0]!='z')
				split(string,",",1,time);
			if (status[0]!='z')
				split(string,",",2,status);
			if (latitude[0]!='z')
				split(string,",",3,latitude);
			if (latitudHemisphere[0]!='z')
				split(string,",",4,latitudHemisphere);
			if (longitude[0]!='z')
				split(string,",",5,longitude);
			if (longitudeMeridian[0]!='z')
				split(string,",",6,longitudeMeridian);
			if (speedKnots[0]!='z')
				split(string,",",7,speedKnots);
			if (trackAngle[0]!='z')
				split(string,",",8,trackAngle);
			if (date[0]!='z')
				split(string,",",9,date);
			if (magneticVaration[0]!='z')
				split(string,",",10,magneticVaration);
			if (magneticVarationOrientation[0]!='z')
				split(string,",",11,magneticVarationOrientation);
		}
	}else{
		//string es null, entonces el contenido de los punteros tiene que ser NULL
		time[0]=NULL;status[0]=NULL;
		latitude[0]=NULL;latitudHemisphere[0]=NULL;
		longitude[0]=NULL;longitudeMeridian[0]=NULL;
		speedKnots[0]=NULL;
	}
	#ifdef DEBUG
		Serial.println(F("EndgetDataGPRMC"));
	#endif
	return;

}


void Gpsneo::convertLatitude(char * latitude,char * returnValue){
	/*
	 El formato de la latitud es de ddmm.mmmm
	 hay que convertirlo en dd.dddd
	*/
	#ifdef DEBUG
		Serial.println(F("convLat"));
	#endif
	int dot = indexOf(latitude,F("."));	//busco la posicion del punto
	if (dot!=-1){
		char div[10]="";
		substring(div,latitude,dot-2,strlen(latitude));	
		float  mm=atof(div);
  		mm = mm/60;
		substring(div,latitude,dot-4,2);
		float  dd=atof(div);
		dd=dd+mm;	
		dtostrf(dd, 3, 7, returnValue);
	#ifdef DEBUG
		Serial.println(F("EndconvLat"));
	#endif
		return;
	}
	#ifdef DEBUG
		Serial.println(F("EndconvLat"));
	#endif
	strcpy(returnValue,"-1");
	return;
}

void Gpsneo::convertLongitude(char * longitude,char * returnValue){
	/*
	 El formato de la latitud es de dddmm.mmmm
	 hay que convertirlo en dd.dddd
	*/
	#ifdef DEBUG
		Serial.println(F("convLon"));
	#endif
	int dot = indexOf(longitude,F("."));	//busco la posicion del punto
	if (dot!=-1){
		char div[10]="";
		substring(div,longitude,dot-2,strlen(longitude));	
		float  mm=atof(div);
  		mm = mm/60;
		substring(div,longitude,dot-5,3);
		float  dd=atof(div);
		dd=dd+mm;	
		dtostrf(dd, 3, 7, returnValue);
	#ifdef DEBUG
		Serial.println(F("EndconvLon"));
	#endif
		return;
	}
	strcpy(returnValue,"-1");
	#ifdef DEBUG
		Serial.println(F("EndconvLon"));
	#endif
	return;
}

void Gpsneo::Google(char *link){
	/*
		Return a link of google Maps. 

		http://www.google.com/maps/place/Latitud,Longitud
	*/
	#ifdef DEBUG
		Serial.println(F("Google"));
	#endif
	char lat[17]="";
	char latH[3]="";
	char lon[17]="";
	char lonM[3]="";
	strcpy_P(link, (char*)pgm_read_word(&(string_table[0])));
	getDataGPRMC(lat,latH,lon,lonM);
	convertLatitude(lat,lat);
	convertLongitude(lon,lon);
	if (latH[0]=='S')
		strcat(link,"-");
	strcat(link,lat);
	strcat(link,",");
	if (lonM[0]=='W')
		strcat(link,"-");
	strcat(link,lon);
	#ifdef DEBUG
		Serial.println(F("EndGoogle"));
	#endif
	return;
}

void Gpsneo::getDataGPGSA(){
	//TODO
	#ifdef GPGSA
		char buffer[BUFFER_SIZE];
		char * string;
		string = &buffer[BUFFER_2];

		string=getDataRaw(F("GPGSA"),buffer);

		if (checksum(string)){
			Serial.println(F("checksum OK---> "));
		//	Serial.println(string);
		}
	#endif
	return;
}