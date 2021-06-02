
/*

Jupiter 7 Control Module
REV 1.1

Overview:
The Jupiter 7 control module acquires data using four ADS1015
ADCs through i2c. The voltage readings are converted to units
using calibraton info. The calibration info is stored in non- 
volitile flash memory. After the readings are converted, they
are sent to the nextion display through serial port. The nextion
display sends calibration information and control information
back. When receiving commands, updated calibration info is
written to flash memory, or the outputs are controlled. 

Select Board:
Select Tools > Board > Board Manager and ensure Arduino MBED OS boards is installed

Select Port:
The port will change when uploading and when running

Power:
The nextion screen may flash if the USB power is insufficient
to power the screen. Either remove the arduino nano 33 BLE and
program separately, or program the display while the unit
is powered on.


*/


//Ensure the following libraries are installed from the tools > library manager menu
#include <arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Arduino_LSM9DS1.h>



//::::::::::flash storage:::::::::::::::::::::::::::::::::::::

// nRF52 NVMC registers for FLASH storage, non-volitle memory
// BASE is used to define where the other registers are located
// READY is used to check if EEPROM is OK to write to
// CONFIG is used to set the mode to write or erase
// ERASEPAGE is used to erase all the data... required before writing
  #define NVMC_BASE       (0x4001E000U)
  #define NVMC_READY      (NVMC_BASE + 0x400U)
//#define NVMC_READYNEXT  (NVMC_BASE + 0x408U)
  #define NVMC_CONFIG     (NVMC_BASE + 0x504U)
  #define NVMC_ERASEPAGE  (NVMC_BASE + 0x508U)
//#define NVMC_ERASEALL   (NVMC_BASE + 0x50CU)
//#define NVMC_ERASEUICR  (NVMC_BASE + 0x514U)
//#define NVMC_ERASEPAGEPARTIAL   (NVMC_BASE + 0X518U)
//#define NVMC_ERASEPAGEPARTIALCFG  (NVMC_BASE + 0X51CU)
//#define NVMC_ICACHECNF  (NVMC_BASE + 0x540U)
//#define NVMC_IHIT       (NVMC_BASE + 0x548U)
//#define NVMC_IMISS      (NMVC_BASE + 0x54cU)

//Create a structure that contains all the values 
//to be written to FLASH
//this is important so that these values are assigned an area in flash memory
typedef struct flash_mem {
    uint32_t C_1;
    uint32_t C_2;
    uint32_t C_3;
    uint32_t C_4;
    uint32_t C_5;
    uint32_t C_6;
    uint32_t Z_1;
    uint32_t Z_2;
    uint32_t Z_3;
    uint32_t Z_4;
    uint32_t M_1;
    uint32_t M_2;
    uint32_t M_3;
    uint32_t M_4;
    uint32_t M_5;
    uint32_t M_6;
    uint32_t B_1;
    uint32_t B_2;
    uint32_t B_3;
    uint32_t B_4;
    uint32_t tempUnits;
} flash_mem_t; //total of 26.... flash_mem_t is the struct


//define default values... 
//IMPORTANT: these values are written when program is uploaded
//This also asigns the structure to a specific spot in memory that we can find later
//define floats as hex since these float values are stored in uint32 memory slots
//The values are in the same order as the flash_mem structure above
const flash_mem_t userData __attribute__ ((section("FLASH"), aligned (0x1000))) = {
  3950,3950,3950,3950,3950,3950,0x413e6666,0x4149999a,0x413e6666,0x4149999a,0x3f800000,0x3f800000,0x3f800000,0x3f800000,0x3d4ccccd,0x3d4ccccd,0,0,0,0,0
}; 

//add a page of memory after the user data to tell the compiler how to bound the memory space
//later, we have to make sure we use this data or the compiler won't add it and will overwrite
//critical program space memory when trying to write the userdata to flash
const uint32_t padding __attribute__ ((section("FLASH"), aligned (0x1000), packed)) = 0x09090909;

//create a variable to dump data to so that the compiler knows the 'padding' variable is used.
uint32_t dump;

//This is used to indicate when to re-write values to flash memory
boolean flashUpdate = false;

//Define the addresses for each of the ADS1015 ADCs
Adafruit_ADS1015 ADC_ZERO(0x48);
Adafruit_ADS1015 ADC_ONE(0x49);
Adafruit_ADS1015 ADC_TWO(0x4A);
Adafruit_ADS1015 ADC_THREE(0x4B);


#define THERMISTORNOMINAL 10000 //The system was designed to read 10K NTC thermistors, but the resistance can be changed here
#define NUMSAMPLES 10   //This is the number of samples to average for every measurement to provide a more stable reading
#define VIN 4.991   //define this based on the voltage regulator output

int sampleCnt = 0;  //a counter for the current sample count
  
//Temperatures
int16_t temp1Cnts[NUMSAMPLES];
int16_t temp2Cnts[NUMSAMPLES];
int16_t temp3Cnts[NUMSAMPLES];
int16_t temp4Cnts[NUMSAMPLES];
int16_t temp5Cnts[NUMSAMPLES];
int16_t temp6Cnts[NUMSAMPLES];
float Temp1=0;
float Temp2=0;
float Temp3=0;
float Temp4=0;
float Temp5=0;
float Temp6=0;
int C_1 = 3950;
int C_2 = 3950;
int C_3 = 3950;
int C_4 = 3950;
int C_5 = 3950;
int C_6 = 3950;

int tempUnits = 0;

// Voltages
int16_t b1Cnts[NUMSAMPLES];
int16_t b2Cnts[NUMSAMPLES];
float B1Volts = 0;
float B1Pct = 0;
float B2Volts = 0;
float B2Pct = 0;
float Z_1 = 11;    //battery1 zero
float Z_2 = 13;    //battery1 full
float Z_3 = 11;    //battery2 zero
float Z_4 = 13;    //battery2 full
  
// Tank Levels and Pressure measurement
// *Note* Pressure 1 can be used to measure any 0-5V signal
int16_t level1Cnts[NUMSAMPLES];
int16_t level2Cnts[NUMSAMPLES];
int16_t level3Cnts[NUMSAMPLES];
int16_t pres1Cnts[NUMSAMPLES];
float Level1 = 0;
float Level2 = 0;
float Level3 = 0;
float Pres1 = 0;
float M_1 = 1;  //Level 1 Slope
float M_2 = 1;  //Level 2 Slope
float M_3 = 1;  //Level 3 Slope
float M_4 = 1;  //Pressure 1 Slope
float B_1 = 0;  //Level 1 Intercept
float B_2 = 0;  //Level 2 Intercept
float B_3 = 0;  //Level 3 Intercept
float B_4 = 0;  //Pressure 1 Intercept


// Current Measurements
int16_t current1Cnts[NUMSAMPLES];
int16_t current2Cnts[NUMSAMPLES];
int16_t current3Cnts[NUMSAMPLES];
int16_t current4Cnts[NUMSAMPLES];
float Current1 = 0;
float Current2 = 0;
float M_5 = 1;
float M_6 = 1;

//LSM9DS1 IMU
//currently unused but can be used to determine heading and level
float TiltX = 0;
float TiltY = 0;
int Heading = 0;

//Used to control each relay and PWM output
bool Relay1 = false;
bool Relay2 = false;
bool Relay3 = false;
bool Relay4 = false;
bool Relay5 = false;
bool Relay6 = false;
int pwm1 = 0;
int pwm2 = 0;

//Used to store and parse the messages received from the Nextion display
String message;
String pMessage;





void setup() {

  //read the variable stored after user data to tell the compiler that this variable is to be included
  //and to bound the page used for user data
  dump = (uint32_t) (&padding);

  //Get all parameters stored in flash (calibration info)
  flashRead();

  //wait for USB to reconnect... problems with  USB power vs. VIN power causing power surge on USB port
  delay(2000);

  //Relay outputs
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  //pinMode(8, OUTPUT);
  //pinMode(9, OUTPUT);
  //pinMode(10,INPUT_PULLUP); //If using these inputs, use input pullup. 
  //pinMode(11,INPUT_PULLUP); //Then all you have to do is ground the pin to toggle the event

  //This opens the port to talk to the nextion display through the display connector
  Serial1.begin(9600);
  //This opens the port to talk to the computer through the USB port
  Serial.begin(9600);

//  Use this to capture information during startup during troubleshooting
//  while (!Serial) {
//    ;
//  }


  // ADS1015 gain information:
  // 2/3x gain   +/- 6.144V    1 bit = 3mV      
  // 1x gain     +/- 4.096V    1 bit = 2mV    
  // 2x gain     +/- 2.048V    1 bit = 1mV 
  // 4x gain     +/- 1.024V    1 bit = 0.5mV 
  // 8x gain     +/- 0.512V    1 bit = 0.25mV 
  // 16x gain    +/- 0.256V    1 bit = 0.125mV  

  //Set the gain for each ADS1015
  //Note the current measurements are set up for differential and use a high gain
  //Also note the Gain_twothirds is used because the resistive tank level sensors
  //are over 4.096V.
  ADC_ZERO.setGain(GAIN_ONE); //AIN0=TEMP4, AIN1=TEMP3, AIN2=TEMP1, AIN3=TEMP2, GAIN=1
  ADC_ONE.setGain(GAIN_TWOTHIRDS); //AIN0=LEVEL3, AIN1=LEVEL2, AIN2=BATTERY2, AIN3=LEVEL1, GAIN=2/3
  ADC_TWO.setGain(GAIN_SIXTEEN); //ADC2  AIN0=CURRENT1+, AIN1=CURRENT1-, AIN2=CURRENT2+, AIN3=CURRENT1-, GAIN=16
  ADC_THREE.setGain(GAIN_ONE); //ADC3  AIN0=BATTERY1, AIN1=PRES1, AIN2=TEMP6, AIN3=TEMP5, GAIN1

  //Start communication with each ADS1015
  ADC_ZERO.begin();
  ADC_ONE.begin();
  ADC_TWO.begin();
  ADC_THREE.begin();

  //Clear the serial port
  Serial1.read();
  delay(500);
  Serial1.read();
  Serial1.flush();

  //Initialize the IMU for tilt and heading measurements
  //Currently not used
  //IMU.begin();

}


int updateTimer = 0;
int adcTimer = 0;


void loop() {
  
  displayReadData();
  
  if(flashUpdate == true){
       //a flash update is pending. 
       //Wait until we have all of the data so we don't write too often.
       updateTimer++;
       if(updateTimer>10){
        flashWrite();
        flashUpdate = false;
        updateTimer = 0;
        Serial.println("Flash Updated");
       }
  }

  //Run the loop at a high rate. Only read the ADC once every 10 cycles
  adcTimer++;
  if(adcTimer>=10){
    readADC();
    getTemps();
    getLevels();
    getVolts();
    getCurrent();
    adcTimer=0;
  }

  displayWriteData();

}
