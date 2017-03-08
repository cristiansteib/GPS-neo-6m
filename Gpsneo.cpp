
#include "Arduino.h"
#include "Gpsneo.h"
#include <avr/pgmspace.h> 


// --------------------SUBSTRING 

void substring(char * dest,char *string,int start,int end){
	if (start<0 || end<0){
		Serial.println("no");
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
		if (pos!=NULL)
		return pos;
	return -1;

}

int  indexOf(char * string,char * search,int offset){

	if (strlen(search) > offset)
		return -1; //if the offset is greater than the string length,return -1 
	
	int ind = int (strstr(&string[offset],search));
	int pos = int  (ind - int(string));
	if (pos!=NULL && pos>0)
		return pos;
	return -1;
}


int  indexOf(char * string,char * search){
	int pos = int(strstr(string,search)-string);
		if (pos!=NULL)
		return pos;
	return -1;

}

//-----------------------------------


Gpsneo::Gpsneo(void) : SoftwareSerial(RX_PIN_DEFAULT,TX_PIN_DEFAULT){  
  init(BAUDRATE_DEFAULT);
}

Gpsneo::~Gpsneo(void){
	Serial.println("destructor");

}

void Gpsneo::init(int baudrate){
	this->begin(baudrate);
	for (int i = 0; i < BUFFER_SIZE ; ++i)
	{
		string[i]='\0';
	}
}

void Gpsneo::readSerial(){	
	int max_lenght=BUFFER_2-1;int length=0;
	long unsigned int timeout;
	while (!this->available()){}
	timeout = millis()+600;
	while (  millis()<timeout && length<max_lenght){
		if (this->available()){
		string[length]=char(this->read());	
		length++;
		}
	}
	string[BUFFER_2 -1]='\0';
}



void Gpsneo::getData(){

	int end=-1;
	readSerial();
	int start=indexOf(string,F("GPRMC"));
	if (start!=-1){
		end=indexOf(string,"\n",start+1);
	 	substring(&string[BUFFER_2],&string[BUFFER_1],start,end);
	 	Serial.println(&string[BUFFER_2]);	
	}
	free (string);
}