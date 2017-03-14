# GPS Library Arduino
Using Software Serial library.
[Website](https://cristiansteib.github.io/Gps-neo-6m) - More info, and you can leave a comment.  
### Protocol NMEA
 NMEA consists of sentences, the first word of which, called a data type, defines the interpretation of the rest of the sentence. 
 There are many sentences in the NMEA standard for all kinds of devices that may be used in a Marine environment. Someones are this:
 - GPRMC (Recommended Minimum data) // (at the moment this is implemented.)
 - GPGGA (Global positioning system fix data)
 - GPGSA (GNSS DOP and Active Satellites)
 - GPGLL (Latitude and longitude, with time of position fix and status)

#### Tested with 
  - GPS-NEO-6M


## Capabilities of the library
#### Constructors
they can call the class wih differents parameters as necessary.
|Constructors|Notes|
|-----------|-----|
|Gpsneo()| The class initialize with the default values|
|Gpsneo ( rx ,tx )| Parameters of the Software Serial Port|
|Gpsneo ( rx, tx, baudrate)|Parameters of the Software Serial Port|
Example of constructors: 
```c++
Gpsneo gps(); //default parameters
Gpsneo gps(10,11); // set rx and tx pin
Gpsneo gps(10,11,9600); //set rx,tx and baudrate
```

|Public Methods| Notes|
|----------------------|------|
|getDataGPRMC (parameters)| See below |
|Google (char *link)| Return in *link the complete link for google maps with latitude and longitude|
|convertLatitude (char *latitude)| Convert latitude ddmm.mmmm in dd.dddd|
|convertLongitude (char *destination)| Convert longitude dddmm.mmm in dddd.ddd|
