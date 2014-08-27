/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include "customer.h";

//RFID ENABLE PIN
#define SS_PIN_0 10
#define SS_PIN_1 53
#define SS_PIN_2 48
#define SS_PIN_3 22

//COMMAND NUMBERS
int MSG_RFID = 0;
int MSG_GLASS_OFF = 1;
int MSG_GLASS_ON = 2;
int MSG_BUTTON_ON = 3;
    
#define RST_PIN 9

#define NUM_CUSTOMERS 1

Customer arrCustomer[NUM_CUSTOMERS];

MFRC522 arrRFID[4] = { MFRC522(SS_PIN_0, RST_PIN), MFRC522(SS_PIN_1, RST_PIN), MFRC522(SS_PIN_2, RST_PIN), MFRC522(SS_PIN_3, RST_PIN) };

String _rfid = "";

//WRITE BACK TO OF
void sendCommandBack(int _customerIndex, int _command, String _val)
{
      String _out;
      
      _out = String(_customerIndex) + ":" + String(_command) + ":" + _val;
      for (int i=0; i < _out.length(); i++)
      {
        Serial.print(_out[i]);
        delay(10);
      }
      Serial.println("");
}

void setup() {

  //Init Customer pins
  arrCustomer[0].setup(A0,A1,2,3,4);
   
  Serial.begin(9600);
  
  //Init SPI RFID chain
  SPI.begin();
  
  //Init RFID readers
  for (int i=0; i<NUM_CUSTOMERS; i++)
  {
    arrRFID[i].PCD_Init(); 
  }
}

void loop() {

    /*
    Message Format:
    
    [customer index: 0-3]:[command index]:[value: rfid string or glass number index, or a button type index if button message]
  
    Example:
    1:2:0 (customer 1, glass_on , glass index 0)
    
    command index values:
    
    MSG_RFID = 0;
    MSG_GLASS_OFF = 1;
    MSG_GLASS_ON = 2;
    MSG_BUTTON_ON = 3;

    buttons values:
    
    BUTTON_TWITTER = 0;
    BUTTON_FACEBOOK = 1;
    BUTTON_EMAIL = 2;
    
  */

  for (int customerIndex=0; customerIndex<NUM_CUSTOMERS; customerIndex++)
  {

    //RFID
    if (!arrCustomer[customerIndex].initRfid)
    {
        arrRFID[customerIndex].ADAM_CARD_ON();
        if ( arrRFID[customerIndex].PICC_IsNewCardPresent())
        {
           
           if ( arrRFID[customerIndex].PICC_ReadCardSerial()) {
               arrCustomer[customerIndex].initRfid = true;
               
               //Serial.println("CARD ");
               _rfid = "";
               for (byte j = 0; j < arrRFID[customerIndex].uid.size; j++) {
                  _rfid += arrRFID[customerIndex].uid.uidByte[j];
		  //Serial.print(arrRFID[customerIndex].uid.uidByte[j], HEX);
               }
               //Serial.println("");
               
               //Send Command
               sendCommandBack(customerIndex, MSG_RFID, _rfid);
	    }
        }
        arrRFID[customerIndex].ADAM_CARD_OFF();
    }

    //EACH BUTTON
    for (int i=0; i<3; i++) {
    
      if (arrCustomer[customerIndex].readButtonValue(i)) {
        /*
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(arrCustomer[customerIndex].btnStatus[i].currentValue);
        Serial.println("");
        */
        
        //Send Command
        sendCommandBack(customerIndex, MSG_BUTTON_ON, String(i));
      }
    }

    //EACH GLASS
    for (int i=0; i<2; i++) {
    
      if (arrCustomer[customerIndex].readSensorValue(i)) {
        /*
        Serial.print("Glass ");
        Serial.print(i);
        Serial.print(arrCustomer[customerIndex].glassStatus[i].currentValue);
        Serial.println("");
        */
        
        //Send Command
        if (arrCustomer[customerIndex].glassStatus[i].currentValue == 1)
        {
          sendCommandBack(customerIndex, MSG_GLASS_ON, String(i));
        }
        else
        {
          sendCommandBack(customerIndex, MSG_GLASS_OFF, String(i));
        }
      }
    }
    //Serial.println(i + " " + arrCustomer[0].btnStatus[i].currentValue);
  
  }
  
}
