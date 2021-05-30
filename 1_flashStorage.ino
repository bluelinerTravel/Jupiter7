

//This function will write all calibration data to flash memory
void flashWrite(){

    //first you have to erase before writing...
    if(*(uint32_t *)NVMC_READY == 1) {  //check if flash is busy doing something
    //Serial.println("Erasing...");
    *(uint32_t *)NVMC_CONFIG = 0x02;  //Set to this mode for erasing
    *(uint32_t *)NVMC_ERASEPAGE = (uint32_t)(&userData);  //erase the userData page
    while(*(uint32_t *)NVMC_READY == 0) //wait for the page erase to complete
      delay(85);  //wait a little bit before switching modes
    *(uint32_t *)NVMC_CONFIG = 0x00;  //Set the mode back to locked
    //Serial.println("...Erased");
    } else {
      //lets put a try then retry again here...
    }

    if (*(uint32_t *)NVMC_READY == 1) { //Check if flash is busy
      // Write into FLASH
      //Serial.println("Flashing...");
      *(uint32_t *)NVMC_CONFIG = 0x01;  //set to this mode for writing
  
      //write the current values back to flash
      *(uint32_t *)(&userData.C_1) = *(uint32_t *)(&C_1); //int
      *(uint32_t *)(&userData.C_2) = *(uint32_t *)(&C_2); //int
      *(uint32_t *)(&userData.C_3) = *(uint32_t *)(&C_3); //int
      *(uint32_t *)(&userData.C_4) = *(uint32_t *)(&C_4); //int
      *(uint32_t *)(&userData.C_5) = *(uint32_t *)(&C_5); //int
      *(uint32_t *)(&userData.C_6) = *(uint32_t *)(&C_6); //int
      *(uint32_t *)(&userData.Z_1) = *(uint32_t *)(&Z_1); //float
      *(uint32_t *)(&userData.Z_2) = *(uint32_t *)(&Z_2); //float
      *(uint32_t *)(&userData.Z_3) = *(uint32_t *)(&Z_3); //float
      *(uint32_t *)(&userData.Z_4) = *(uint32_t *)(&Z_4); //float
      *(uint32_t *)(&userData.M_1) = *(uint32_t *)(&M_1); //float
      *(uint32_t *)(&userData.M_2) = *(uint32_t *)(&M_2); //float
      *(uint32_t *)(&userData.M_3) = *(uint32_t *)(&M_3); //float
      *(uint32_t *)(&userData.M_4) = *(uint32_t *)(&M_4); //float
      *(uint32_t *)(&userData.M_5) = *(uint32_t *)(&M_5); //float
      *(uint32_t *)(&userData.M_6) = *(uint32_t *)(&M_6); //float
      *(uint32_t *)(&userData.B_1) = *(uint32_t *)(&B_1); //float
      *(uint32_t *)(&userData.B_2) = *(uint32_t *)(&B_2); //float
      *(uint32_t *)(&userData.B_3) = *(uint32_t *)(&B_3); //float
      *(uint32_t *)(&userData.B_4) = *(uint32_t *)(&B_4); //float
      *(uint32_t *)(&userData.tempUnits) = *(uint32_t *)(&tempUnits); //int
  
      while(*(uint32_t *)NVMC_READY == 0) //check if flash is done writing
        delayMicroseconds(50);
      *(uint32_t *)NVMC_CONFIG = 0x00;  //set the mode back to locked
      //Serial.println("...Flashed");
     } 
     else {
       //lets put a try then try again here...
     }

}


//This function will retreive all the calibration info from flash memory
void flashRead(){

  //each flash address uses 4 bytes
  C_1 = readFlashInteger(0);
  C_2 = readFlashInteger(1);
  C_3 = readFlashInteger(2);
  C_4 = readFlashInteger(3);
  C_5 = readFlashInteger(4);
  C_6 = readFlashInteger(5);
  Z_1 = readFlashFloat(6);
  Z_2 = readFlashFloat(7);
  Z_3 = readFlashFloat(8);
  Z_4 = readFlashFloat(9);
  M_1 = readFlashFloat(10);
  M_2 = readFlashFloat(11);
  M_3 = readFlashFloat(12);
  M_4 = readFlashFloat(13);
  M_5 = readFlashFloat(14);
  M_6 = readFlashFloat(15);
  B_1 = readFlashFloat(16);
  B_2 = readFlashFloat(17);
  B_3 = readFlashFloat(18);
  B_4 = readFlashFloat(19);
  tempUnits = readFlashInteger(20);

}

//This function returns an integer from flash at the specified address
int readFlashInteger(uint8_t addr){
  byte byteArray[4];
  uint8_t *ptr;
  int val;
  //get the base location of the value
  //each address has 4 bytes, so multiply addres by 4
  ptr = ((uint8_t *)(&userData)) + (addr*4); 
  //get the bytes into an array
  for(int i=0; i<4; i++){
    byteArray[i]=(byte)*(ptr+i);
  }
  memcpy(&val, byteArray, 4); //transform the byte array to an integer value
  return val;
}

//This function returns a float from flash at the specified address
float readFlashFloat(uint8_t addr){
  byte byteArray[4];
  uint8_t *ptr;
  float val;
  ptr = ((uint8_t *)(&userData)) + (addr*4); //get the base location of the value
  //get the bytes into an array
  for(int i=0; i<4; i++){
    byteArray[i]=(byte)*(ptr+i);
  }
  memcpy(&val, byteArray, 4); //transform the byte array to a float value
  return val;
}
