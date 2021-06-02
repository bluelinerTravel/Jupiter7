

void nextionWriteNumber(String A, int B){
  String printLine = A + ".val=";
  Serial1.print(printLine);
  Serial1.print(B);
  //Serial.print(char(176)); //degrees character
  //"°"
  Serial1.write(0xff);  //every command is followed by 3 0xff characters
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void nextionWriteString(String A, float B, char C, int D){
  String printLine = A + ".txt=";
  Serial1.print(printLine);
  Serial1.print("\"");
  Serial1.print(B,D); //to the 1st decimal point
  if(C!='0'){
  Serial1.print(C);
  }
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}






int j = 0;

void displayWriteData(){


//make sure all variables are set to global on Nextion so that they will work on 
//all pages

//if you send a variable to nextion that doesn't exist in nextion
//the serial response will be an error and flood the serial port
//If this happens, the nextion wont read anymore
  
//send values in batches so that we aren't filling up the serial port

//Don't forget to check that each page has all of the variables listed below and are
//set to global to avoid communication issues

//Make sure if a value sent is a string, make sure the global variable in 
//nextion is also set to string, otherwise you get invalid variable error.

switch(j){

  default:
  nextionWriteString("va0",Temp1,'°',0); //Temp1
  nextionWriteString("va1",C_1,'0',0); //Temp1 Coefficient
  nextionWriteString("va2",Temp2,'°',0); //Temp2 
  nextionWriteString("va3",C_2,'0',0); //Temp2 Coefficient
  break;
  case 1:
  nextionWriteString("va4",Temp3,'°',0); //Temp3
  nextionWriteString("va5",C_3,'0',0); //Temp3 Coefficient
  nextionWriteString("va6",Temp4,'°',0); //Temp4
  nextionWriteString("va7",C_4,'0',0); //Temp4 Coefficient
  break;
  case 2:
  nextionWriteString("va8",Temp5,'°',0); //Temp5
  nextionWriteString("va9",C_5,'0',0); //Temp5 Coefficient
  nextionWriteString("va10",Temp6,'°',0); //Temp6
  nextionWriteString("va11",C_6,'0',0); //Temp6 Coefficient
  break;
  case 3:
  nextionWriteString("va12",B1Volts,'V',2); //Battery1 Voltage
  nextionWriteString("va13",B1Pct,'%',0); //Battery1 Percent
  nextionWriteString("va14",Z_1,'0',2); //Battery1 Zero
  nextionWriteString("va15",Z_2,'0',2); //Battery1 Full
  break;
  case 4:
  nextionWriteString("va16",B2Volts,'V',2); //Battery2 Voltage
  nextionWriteString("va17",B2Pct,'%',0); //Battery2 Percent
  nextionWriteString("va18",Z_3,'0',2); //Battery2 Zero
  nextionWriteString("va19",Z_4,'0',2); //Battery2 Full
  break;
  case 5:
  nextionWriteString("va20",Level1,'%',0); //Level 1
  nextionWriteString("va21",M_1,'0',2); //Level 1 M (Mx+b)  M_1
  nextionWriteString("va22",B_1,'0',2); //Level 1 b (Mx+b)
  break;
  case 6:
  nextionWriteString("va23",Level2,'%',0); //Level 2
  nextionWriteString("va24",M_2,'0',2); //Level 2 M (Mx+b) M_2
  nextionWriteString("va25",B_2,'0',2); //Level 2 b (Mx+b)
  break;
  case 7:
  nextionWriteString("va26",Level3,'%',0); //Level 3
  nextionWriteString("va27",M_3,'0',2); //Level 3 M (Mx+b)  M_3
  nextionWriteString("va28",B_3,'0',2); //Level 3 b (Mx+b)
  break;
  case 8:  
  nextionWriteString("va29",Pres1,'0',0); //Pres 1
  nextionWriteString("va30",M_4,'0',2); //Pres 1 M (Mx+b)  M_4
  nextionWriteString("va31",B_4,'0',2); //Pres 1 b (Mx+b)
  break;
  case 9:
  nextionWriteString("va32",Current1,'A',1); //Current 1
  nextionWriteString("va33",M_5,'0',2); //Current 1 M (Mx+b)  M_5
  nextionWriteString("va34",Current2,'A',1); //Current 2
  nextionWriteString("va35",M_6,'0',2); //Current 2 M (Mx+b)  M_6
  break;
  case 10:
  nextionWriteString("va36",TiltX,'°',1);
  nextionWriteString("va37",TiltY,'°',1);
  nextionWriteNumber("va38",Heading);
  break;
  case 11:
  nextionWriteNumber("va39",Relay1); //Relay1
  nextionWriteNumber("va40",Relay2); //Relay2
  nextionWriteNumber("va41",Relay3); //Relay3
  nextionWriteNumber("va42",Relay4); //Relay4
  break;
  case 12:
  nextionWriteNumber("va43",Relay5); //Relay5
  nextionWriteNumber("va44",Relay6); //Relay6
  nextionWriteNumber("va45",pwm1); //Relay7
  nextionWriteNumber("va46",pwm2); //Relay8
  nextionWriteNumber("va47",tempUnits); //0 = Fahrenheit, 1 = Celsius
  break;
}

  //counter for switch case
  j++;
  if(j>12){
    j=0;
  }
}
