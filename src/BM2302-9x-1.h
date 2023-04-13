/***********************************************************
File:               BM2302-9x-1.h
Author:             XIAO, BESTMODULES
Description:        Define classes and required variables
History:      
V1.0.0   -- initial version；2023-03-09；Arduino IDE : v1.8.16
***********************************************************/
#ifndef _BCM23M0x1_H__
#define _BCM23M0x1_H__

#include "Arduino.h"
#include <Wire.h> 
#include <SoftwareSerial.h>

#define   SUCCESS            0
#define   FAIL               1
#define   TIMEOUT_ERROR   3

#define IIC_MODE            0x01
#define UART_MODE           0x02

const uint8_t BM2302_9x_1_Addr = 0x23;              // I2C address
#define BC2302_HXT_Freq     16                // MHz

#define RF_315MHz      0x01
#define RF_433MHz      0x02
#define RF_868MHz      0x04
#define RF_915MHz      0x08

class BM2302_9x_1
{       
     public:
          BM2302_9x_1(uint16_t statusPin, HardwareSerial *theSerial);
          BM2302_9x_1(uint16_t statusPin,uint16_t rxPin,uint16_t txPin);
          BM2302_9x_1(uint16_t statusPin, TwoWire *theWire=&Wire);
          void begin(uint8_t frequencyBand);                               // While UART_MODE Initialize UART port and set buad-rate
          uint8_t getStatus();  
          uint8_t getRFStatus(void);                                     // Get RF status
          uint8_t readRFData(void);                                       // Get RF RX data
          void setRXMode(void);                                       // Set module enter RX mode
          void setPairMode(void);                                     // Set module enter pairing mode
          void sleep(void);                                         // Set module enter sleep mode
          uint16_t  getFWVer(void);                            // Get RF RX version                                            
      
       private:
          void setRFFrequencyBand(uint8_t frequencyBand);                                  // Set RF frequency band
          void setRFFrequency(uint8_t DNDK[]);                            // Write RF frequency data buffer
          void setRFFrequency(double frequency);                      // Set RF frequency and write frequency data
          void setTestMode(void);                                     // Set module enter test mode
          void exitTestMode(void);                                    // Set module exit test mode
          void writeBytes(uint8_t wbuf[], uint8_t wlen);
          uint8_t readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut = 10);
          
          uint16_t _statusPin;
          uint16_t _rxPin;
          uint16_t _txPin;
          uint8_t _ModeSlect;
          
          HardwareSerial *_hardSerial = NULL;
          SoftwareSerial *_softSerial = NULL ;
          TwoWire        *_wire = NULL;
};

#endif
