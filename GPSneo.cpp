bool GPSneo::checksum(String *toCheck,String *checksum){
	return checksum(toCheck,String("0x"+checksum).toInt());
}

bool GPSneo::checksum(String *toCheck,uint8_t *checksum){
	uint8_t checksum_new = 0;
  	for(int i=0; i<toCheck.length(); i++){
      checksum_new = checksum_new ^ toCheck[i];
  	}

  	if (checksum_new == checksum){
  		return true;
  	}
  	return false;
}
