/***********************************************************
File:                communication_UART.ino
Description:         1.Receives the data sent by the BMC21M0x1 and displays the received data on the serial monitor      
***********************************************************/

#include "BM2302-9x-1.h" 
BM2302_9x_1     BMC23(3,5,4); //intPin=3,rxPin=5,txPin=4,Please comment out this line of code if you don't use SW Serial
//BM2302_9x_1     BMC23(3,&Serial); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM2302_9x_1     BMC23(22,&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BM2302_9x_1     BMC23(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM2302_9x_1     BMC23(3,&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BM2302_9x_1     BMC23(3,&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino

int LED = 13;                         
uint8_t RXDATA; 
void setup() 
{
  pinMode(LED, OUTPUT);                                 
  digitalWrite(LED, LOW);                               
  Serial.begin(9600);    
  BMC23.begin(RF_433MHz);          //Set the Baud rate of the UART to 19200    
  BMC23.setPairMode();    //Enter the pairing mode, and automatically enter the RX mode after successful pairing */                                                        
  while(BMC23.getRFStatus()& 0x04)   //Determine whether it is in pairing
  {
    digitalWrite(LED, HIGH);      
  }
   digitalWrite(LED, LOW); 
}

void loop() 
{
 if(!BMC23.getStatus())//Determine whether the STATUS pin is low
 {  
   if(BMC23.getRFStatus()& 0x20) //Determine whether there is data to be read
   {
    RXDATA = BMC23.readRFData();  //Read the received data
    Serial.print("RXDATA:");          
    Serial.println(RXDATA);      //Displays the received data on the serial monitor
    digitalWrite(LED, HIGH); 
    delay(10);
    digitalWrite(LED, LOW); 
   } 
 }  
}
