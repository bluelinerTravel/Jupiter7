
int timeout = 0;

/*Nextion Error Code: ⸮⸮⸮    , =1A => Variable Name Invalid
  nextion sends 0xFF (DEC 255) 3 times after a message
  it also sends a bunch of -1 values for no reason sometimes
  Commands:   C = B Coefficients
              Z = Battery Voltages (Low and High)
              M = Slope
              B = Intercept
              R = Relay
              P = PWM setpoint
              U = Units select
*/
void displayReadData(){

  //read the command if serial data is available at the port
    if(Serial1.available()){
      timeout = 0;
      while (timeout<1000){
        
        int character = Serial1.read();
  
        if(character <= 0x7F && character>=0x21){          // good messages
          if (char(character) == '!'){
            break;  //end of command from nextion
          }
          else {
            message += char(character); //add the character to the message
          }
        }
        if(character == 0x1A){        //check for error
          Serial.println("Nextion - Invalid Variable Name");
        }
        timeout++;              //timeout after 500 mS
        if(timeout >= 1000){
          Serial.println("timeout");
        }
        delay(1);                //timeout after 500 mS
       }
       
        if(message != ""){
         Serial.println(message);

         //for troubleshooting
         //flashRead();
         
         messageRead(message);
         message="";
        }
      }
}






void messageRead(String fMessage){
  
  String f2Message = "";

  //B Coefficients, if the message is C44, this means set C_4 to a value of 4
  if (fMessage.substring(0,1) == "C"){
      f2Message= fMessage.substring(2);   //read the value
      fMessage = fMessage.substring(1,2); //read which value
      int tempNum = fMessage.toInt();
      int tempNum2= f2Message.toInt();
  
      //The b coefficients get 2 bytes of memory each
      switch (tempNum){
          default:
            if(C_1 != tempNum2){
              C_1=tempNum2;
              flashUpdate = true;
            }
          break;
          case 2:
            if(C_2 != tempNum2){
              C_2=tempNum2;
              flashUpdate = true;
            }
          break;
          case 3:
            if(C_3 != tempNum2){
              C_3=tempNum2;
            }
          break;
          case 4:
            if(C_4 !=tempNum2){
              C_4=tempNum2;
              flashUpdate = true;
            }
          break;
          case 5:
            if(C_5 !=tempNum2){
              C_5=tempNum2;
              flashUpdate = true;
            }
          break;
          case 6:
            if(C_6 !=tempNum2){
              C_6=tempNum2;
              flashUpdate = true;
            }
          break;
      }
    }


  //Battery level values, if the value is Z124, This means set Z_1 to 24
  if (fMessage.substring(0,1) == "Z"){
      f2Message= fMessage.substring(2);   //read the value
      fMessage = fMessage.substring(1,2); //read which value
      int tempNum = fMessage.toInt();
      int tempNum2= f2Message.toFloat();
  
      //The b coefficients get 2 bytes of memory each
      switch (tempNum){
          default:
            if(Z_1 != tempNum2){
              Z_1=tempNum2;
              flashUpdate = true;
            }
          break;
          case 2:
            if(Z_2 != tempNum2){
              Z_2=tempNum2;
              flashUpdate = true;
            }
          break;
          case 3:
            if(Z_3 != tempNum2){
              Z_3=tempNum2;
              flashUpdate = true;
            }
          break;
          case 4:
            if(Z_4 !=tempNum2){
              Z_4=tempNum2;
              flashUpdate = true;
            }
          break;
      }
    }
    
  
    //Slopes, if the message is M119, this means set M_1 to a value of 19
    if (fMessage.substring(0,1)== "M"){
      f2Message= fMessage.substring(2);     //read the value
      fMessage = fMessage.substring(1,2);   //read which value
      int tempNum = fMessage.toInt();
      float tempNum2 = f2Message.toFloat();
  
      //The slopes get 4 bytes of memory each
      switch (tempNum){
          default:
            if(M_1 != tempNum2){
              M_1=tempNum2;
              flashUpdate = true;
            }
          break;
          case 2:
            if(M_2 != tempNum2){
              M_2=tempNum2;
              flashUpdate = true;
            }
          break;
          case 3:
            if(M_3 != tempNum2){
              M_3=tempNum2;
              flashUpdate = true;
            }
          break;
          case 4:
            if(M_4 !=tempNum2){
              M_4=tempNum2;
              flashUpdate = true;
            }
          break;
          case 5:
            if(M_5 !=tempNum2){
              M_5=tempNum2;
              flashUpdate = true;
            }
          break;
          case 6:
            if(M_6 !=tempNum2){
              M_6=tempNum2;
              flashUpdate = true;
            }
          break;
      }
    }


    //intercepts- If the message is B25, this means change B_2 to a value of 5
    if (fMessage.substring(0,1) == "B"){
      f2Message= fMessage.substring(2);   //read the value
      fMessage = fMessage.substring(1,2); //read which value
      int tempNum = fMessage.toInt(); // indicates which value to update
      float tempNum2= f2Message.toFloat(); // the value to store
  
      //The intercepts get 4 bytes of memory each
      switch (tempNum){
          default:
            if(B_1 != tempNum2){
              B_1=tempNum2;
              flashUpdate = true;
            }
          break;
          case 2:
            if(B_2 != tempNum2){
              B_2=tempNum2;
              flashUpdate = true;
            }
          break;
          case 3:
            if(B_3 != tempNum2){
              B_3=tempNum2;
              flashUpdate = true;
            }
          break;
          case 4:
            if(B_4 !=tempNum2){
              B_4=tempNum2;
              flashUpdate = true;
            }
          break;
      }
    }




  // if the message is R2, this means switch state of R2
  if (fMessage.substring(0,1)=="R"){

        if(fMessage == "R1" && Relay1 == false){
          Relay1 = true;
          digitalWrite(2, HIGH);
          fMessage="";
        }
        if(fMessage == "R1" && Relay1 == true){
          Relay1 = false; 
          digitalWrite(2, LOW);
          fMessage="";
        }
        if(fMessage == "R2" && Relay2 == false){
          Relay2 = true;
          digitalWrite(3, HIGH);
          fMessage="";
        }
        if(fMessage == "R2" && Relay2 == true){
          Relay2 = false; 
          digitalWrite(3, LOW);
          fMessage="";
        }
        if(fMessage == "R3" && Relay3 == false){
          Relay3 = true;
          digitalWrite(4, HIGH);
          fMessage="";
        }
        if(fMessage == "R3" && Relay3 == true){
          Relay3 = false; 
          digitalWrite(4, LOW);
          fMessage="";
        }
        if(fMessage == "R4" && Relay4 == false){
          Relay4 = true;
          digitalWrite(5, HIGH);
          fMessage="";
        }
        if(fMessage == "R4" && Relay4 == true){
          Relay4 = false; 
          digitalWrite(5, LOW);
          fMessage=""; 
        }

        if(fMessage == "R5" && Relay5 == false){
          Relay5 = true;
          digitalWrite(6, HIGH);
          fMessage="";
        }
        if(fMessage == "R5" && Relay5 == true){
          Relay5 = false; 
          digitalWrite(6, LOW);
          fMessage="";
        }
        if(fMessage == "R6" && Relay6 == false){
          Relay6 = true;
          digitalWrite(7, HIGH);
          fMessage="";
        }
        if(fMessage == "R6" && Relay6 == true){
          Relay6 = false; 
          digitalWrite(7, LOW);
          fMessage="";
        }
  }

  if (fMessage.substring(0,1)== "P"){
      f2Message= fMessage.substring(2);   //read the value
      fMessage = fMessage.substring(1,2); //read which value
      int tempNum = fMessage.toInt(); // indicates which value to update
      int tempNum2= f2Message.toInt(); // the value to store

      switch (tempNum){
        
          default:
            pwm1 = constrain(tempNum2,0,255);
            analogWrite(8,pwm1);
          break;
          case 2:
            pwm2 = constrain(tempNum2,0,255);
            analogWrite(9,pwm2);
          break;
          
      }
  }


  // if the message is T1, this means celsius is selected
  if (fMessage.substring(0,1)=="T"){
      if(fMessage="T1"){
        tempUnits=1;  //celsius
        flashUpdate = true;
      }
      else{
        tempUnits=0;  //Fahrenheit
        flashUpdate = true;
      }
  }
  

  
}
