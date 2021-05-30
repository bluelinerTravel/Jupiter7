


  // 2/3x gain   +/- 6.144V    1 bit = 3mV      
  // 1x gain     +/- 4.096V    1 bit = 2mV    
  // 2x gain     +/- 2.048V    1 bit = 1mV 
  // 4x gain     +/- 1.024V    1 bit = 0.5mV 
  // 8x gain     +/- 0.512V    1 bit = 0.25mV 
  // 16x gain    +/- 0.256V    1 bit = 0.125mV  




void readADC(){

//ADC0  AIN0=TEMP4, AIN1=TEMP3, AIN2=TEMP1, AIN3=TEMP2, GAIN=1
//ADC1  AIN0=LEVEL3, AIN1=LEVEL2, AIN2=BATTERY2, AIN3=LEVEL1, GAIN=2/3
//ADC2  AIN0=CURRENT1+, AIN1=CURRENT1-, AIN2=CURRENT2+, AIN3=CURRENT1-, GAIN=16
//ADC3  AIN0=BATTERY1, AIN1=PRES1, AIN2=TEMP6, AIN3=TEMP5, GAIN1

  
  //Read ADS1015
  temp4Cnts[sampleCnt] = ADC_ZERO.readADC_SingleEnded(0);
  temp3Cnts[sampleCnt] = ADC_ZERO.readADC_SingleEnded(1);
  temp1Cnts[sampleCnt] = ADC_ZERO.readADC_SingleEnded(2);
  temp2Cnts[sampleCnt] = ADC_ZERO.readADC_SingleEnded(3);
  level3Cnts[sampleCnt] = ADC_ONE.readADC_SingleEnded(0);
  level2Cnts[sampleCnt] = ADC_ONE.readADC_SingleEnded(1);
  b2Cnts[sampleCnt] = ADC_ONE.readADC_SingleEnded(2);
  level1Cnts[sampleCnt] = ADC_ONE.readADC_SingleEnded(3);
  current1Cnts[sampleCnt] = ADC_TWO.readADC_Differential_0_1();
  current2Cnts[sampleCnt] = ADC_TWO.readADC_Differential_2_3();
  b1Cnts[sampleCnt] = ADC_THREE.readADC_SingleEnded(0);
  pres1Cnts[sampleCnt] = ADC_THREE.readADC_SingleEnded(1);
  temp6Cnts[sampleCnt] = ADC_THREE.readADC_SingleEnded(2);
  temp5Cnts[sampleCnt] = ADC_THREE.readADC_SingleEnded(3);
    

  //Ignore bad values from ads1015
  if(temp4Cnts[sampleCnt]>2048) temp4Cnts[sampleCnt]= 0;
  if(temp3Cnts[sampleCnt]>2048) temp3Cnts[sampleCnt]= 0;
  if(temp1Cnts[sampleCnt]>2048) temp1Cnts[sampleCnt]= 0;
  if(temp2Cnts[sampleCnt]>2048) temp2Cnts[sampleCnt]= 0;
  if(level3Cnts[sampleCnt]>2048) level3Cnts[sampleCnt]= 0;
  if(level2Cnts[sampleCnt]>2048) level2Cnts[sampleCnt]= 0;
  if(b2Cnts[sampleCnt]>2048) b2Cnts[sampleCnt]= 0;
  if(level1Cnts[sampleCnt]>2048) level1Cnts[sampleCnt]= 0;
  if(current1Cnts[sampleCnt]>2048) current1Cnts[sampleCnt]= 0;
  if(current1Cnts[sampleCnt]>2048) current1Cnts[sampleCnt]= 0;
  if(b1Cnts[sampleCnt]>2048) b1Cnts[sampleCnt]= 0;
  if(pres1Cnts[sampleCnt]>2048) pres1Cnts[sampleCnt]= 0;
  if(temp6Cnts[sampleCnt]>2048) temp6Cnts[sampleCnt]= 0;
  if(temp5Cnts[sampleCnt]>2048) temp5Cnts[sampleCnt]= 0;



  
//for troubleshooting...
//  for(int i=0; i<NUMSAMPLES; i++){
//    Serial.print(current1Cnts[i]);Serial.print(",\t");
//  }
//  Serial.println("");
  

  //run a counter to cycle through samples in an array
  sampleCnt++;
  if(sampleCnt >= NUMSAMPLES){
    sampleCnt=0;
  }

    //reset
    Temp1=0;
    Temp2=0;
    Temp3=0;
    Temp4=0;
    Temp5=0;
    Temp6=0;
    B1Volts=0;
    B2Volts=0;
    Level1=0;
    Level2=0;
    Level3=0;
    Pres1=0;
    Current1=0;
    Current2=0;
    

  //Get the sum of each array for calculating the average of the samples
  uint8_t i;
  for (i=0; i<NUMSAMPLES;i++){
    //get sum
    Temp1 += temp1Cnts[i];
    Temp2 += temp2Cnts[i];
    Temp3 += temp3Cnts[i];
    Temp4 += temp4Cnts[i];
    Temp5 += temp5Cnts[i];
    Temp6 += temp6Cnts[i];
    B1Volts += b1Cnts[i];
    B2Volts += b2Cnts[i];
    Level1 += level1Cnts[i];
    Level2 += level2Cnts[i];
    Level3 += level3Cnts[i];
    Pres1 += pres1Cnts[i];
    Current1 += current1Cnts[i];
    Current2 += current2Cnts[i];
    
  }
  
    //Calculate averages
    Temp1 /= NUMSAMPLES;
    Temp2 /= NUMSAMPLES;
    Temp3 /= NUMSAMPLES;
    Temp4 /= NUMSAMPLES;
    Temp5 /= NUMSAMPLES;
    Temp6 /= NUMSAMPLES;
    B1Volts /= NUMSAMPLES;
    B2Volts /= NUMSAMPLES;
    Level1 /= NUMSAMPLES;
    Level2 /= NUMSAMPLES;
    Level3 /= NUMSAMPLES;
    Pres1 /= NUMSAMPLES;
    Current1 /= NUMSAMPLES;
    Current2 /= NUMSAMPLES;
    

}


void getVolts(){

  
  //voltage divider, R1=100k R2=10k, Vout=Cnts*0.002   (0.002V per count)
  //Vin=Vout/(R2/(R1+R2))
  
  B1Volts = (B1Volts*0.002)/0.09091;

  B2Volts = (B2Volts*0.003)/0.09091; //*0.003 because we have to use gain 2/3
                                     // so that we can measure float resistance
                                     // type level sensor

  B1Pct = ((B1Volts-Z_1)/(Z_2-Z_1))*100;
  B2Pct = ((B2Volts-Z_3)/(Z_4-Z_3))*100;
  B1Pct = constrain(B1Pct,0,100); //keep the value between 0 and 100 percent
  B2Pct = constrain(B2Pct,0,100);
  
}


void getLevels(){

  //Level=counts*slope+intercept

  Level1 = M_1*Level1;
  Level1 += B_1;
  if(Level1<0){
    Level1=0;   //no negative readings
  }

  Level2 = M_2*Level2;
  Level2 += B_2;
  if(Level2<0){
    Level2=0;   //no negative readings
  }

  Level3 = M_3*Level3;
  Level3 += B_3;
  if(Level3<0){
    Level3=0;   //no negative readings
  }

  Pres1 = M_4*Pres1;
  Pres1 += B_4;
  if(Pres1<0){
    Pres1=0;   //no negative readings
  }

}

void getCurrent(){

  //counts*slope+intercept
  //16x gain    +/- 0.256V    1 bit = 0.125mV  

  Current1 = M_5*Current1;
  Current2 = M_6*Current2;


}


void getTemps(){


  Temp1 = Temp1*0.002;
  Temp1 = 10000*((VIN/Temp1)-1); //Get R1 ... VIN is 
  Temp1 /= THERMISTORNOMINAL; 
  Temp1 = log(Temp1);
  Temp1 /= C_1;
  Temp1 += 1.0 / (273.15 + 25);
  Temp1 = 1.0 / Temp1;
  Temp1 -= 273.15;
  if(tempUnits==0){
    Temp1 = (Temp1*1.8)+32; //convert Kelvin to degF
  }

  Temp2 = Temp2*0.002;
  Temp2 = 10000*((VIN/Temp2)-1); //Get R1
  Temp2 /= THERMISTORNOMINAL;
  Temp2 = log(Temp2);
  Temp2 /= C_2;
  Temp2 += 1.0 / (273.15 + 25);
  Temp2 = 1.0 / Temp2;
  Temp2 -= 273.15;
  if(tempUnits==0){
    Temp2 = (Temp2*1.8)+32; //convert to degF
  }

  Temp3 = Temp3*0.002;
  Temp3 = 10000*((VIN/Temp3)-1); //Get R1 
  Temp3 /= THERMISTORNOMINAL;
  Temp3 = log(Temp3);
  Temp3 /= C_3;
  Temp3 += 1.0 / (273.15 + 25);
  Temp3 = 1.0 / Temp3;
  Temp3 -= 273.15;
  if(tempUnits==0){
    Temp3 = (Temp3*1.8)+32; //convert to degF
  }

  Temp4 = Temp4*0.002;
  Temp4 = 10000*((VIN/Temp4)-1); //Get R1
  Temp4 /= THERMISTORNOMINAL;
  Temp4 = log(Temp4);
  Temp4 /= C_4;
  Temp4 += 1.0 / (273.15 + 25);
  Temp4 = 1.0 / Temp4;
  Temp4 -= 273.15;
  if(tempUnits==0){
    Temp4 = (Temp4*1.8)+32; //convert to degF
  }

  Temp5 = Temp5*0.002;
  Temp5 = 10000*((VIN/Temp5)-1); //Get R1
  Temp5 /= THERMISTORNOMINAL;
  Temp5 = log(Temp5);
  Temp5 /= C_5;
  Temp5 += 1.0 / (273.15 + 25);
  Temp5 = 1.0 / Temp5;
  Temp5 -= 273.15;
  if(tempUnits==0){
    Temp5 = (Temp5*1.8)+32; //convert to degF
  }
  
  Temp6 = Temp6*0.002;
  Temp6 = 10000*((VIN/Temp6)-1); //Get R1
  Temp6 /= THERMISTORNOMINAL;
  Temp6 = log(Temp6);
  Temp6 /= C_6;
  Temp6 += 1.0 / (273.15 + 25);
  Temp6 = 1.0 / Temp6;
  Temp6 -= 273.15;
  if(tempUnits==0){
    Temp6 = (Temp6*1.8)+32; //convert to degF
  }

}
