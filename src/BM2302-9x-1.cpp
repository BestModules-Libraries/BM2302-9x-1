/***********************************************************
File:           BM2302-9x-1.cpp
Author:         XIAO, BESTMODULES
Description:    UART/IIC communication with the BM2302_9x_1   
History:      
V1.0.0   -- initial version；2023-03-09；Arduino IDE : v1.8.16
***********************************************************/
#include "BM2302-9x-1.h"
/**********************************************************
Description: Constructor
Parameters:  intPin: The INT output pin is connected to the Arduino, 
                     and when receiving the data sent by the transmitter, the INT is pulled down
             *theSerial : Wire object if your board has more than one UART interface        
Return:          
Others:    
**********************************************************/
BM2302_9x_1::BM2302_9x_1(uint16_t statusPin, HardwareSerial *theSerial)
{
    _softSerial = NULL ;
    _wire = NULL;
    _hardSerial = theSerial;
    _statusPin = statusPin;
    _ModeSlect = UART_MODE;
}

/**********************************************************
Description: Constructor
Parameters:  intPin: The INT output pin is connected to the Arduino, 
                     and when receiving the data sent by the transmitter, the INT is pulled down
             *twowire : Wire object if your board has more than one IIC interface        
Return:          
Others:   
**********************************************************/
BM2302_9x_1::BM2302_9x_1(uint16_t statusPin, TwoWire *theWire)
{
    _hardSerial = NULL;
    _softSerial = NULL ;
    _wire = theWire;
    _statusPin = statusPin;
    _ModeSlect = IIC_MODE;
}

/**********************************************************
Description: Constructor
Parameters:  intPin : The INT output pin is connected to the Arduino, 
                      and when receiving the data sent by the transmitter, the INT is pulled down
             rxPin : Receiver pin of the UART
             txPin : Send signal pin of UART         
Return:          
Others:   
**********************************************************/
BM2302_9x_1::BM2302_9x_1(uint16_t statusPin,uint16_t rxPin,uint16_t txPin)
{
    _hardSerial = NULL;
    _wire = NULL;
    _statusPin = statusPin;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
    _ModeSlect = UART_MODE;
}
/**********************************************************
Description: Module Initial
Parameters:  baudRate: Set the Module  baudRate(Unique Value 19200)        
Return:          
Others:      If the _softSerial pointer is empty, the software serial port is initialized,
             otherwise the hardware serial port is initialized. 
**********************************************************/

void BM2302_9x_1::begin(uint8_t frequencyBand)
{
    float delaytime;    
    if(_ModeSlect == UART_MODE)
    {
      if(_softSerial != NULL)
      {
        pinMode(_statusPin,INPUT);
        _softSerial->begin(19200);
        delaytime = (60000/19200)+100;
        _softSerial->setTimeout((uint8_t)delaytime);
      }
      else
      {
        pinMode(_statusPin,INPUT);
        _hardSerial->begin(19200);
        delaytime = (60000/19200)+100;
        _hardSerial->setTimeout((uint8_t)delaytime);
      }
    }
   else if(_ModeSlect == IIC_MODE)
    {
      if(_wire != NULL)
      { 
        pinMode(_statusPin,INPUT);
        _wire->begin();
        _wire->setClock(100000);
      }
    } 
    setRFFrequencyBand(frequencyBand);   
}

/**********************************************************
Description: Get INT Status
Parameters:         
Return:     Returns the INT Status  0:INT output low level 
                                    1:INT output high level  
Others:     
**********************************************************/
uint8_t BM2302_9x_1::getStatus()
{
     return (digitalRead(_statusPin));
}

/**********************************************************
Description:Get RF status
Parameters:          
Return:     result:   
                   B0~B1:retain
                   B2：RF is in the pair mode
                   B3：RF is in the receiving state
                   B4：RF once completed pairing
                   B5：RF has data to read
                   B6~B7：retain     
Others:      
**********************************************************/
uint8_t BM2302_9x_1::getRFStatus(void)
{
    uint8_t sendBuf[1] = {0x81};
    uint8_t buff[1] = {0};
    writeBytes(sendBuf,1);
    if(readBytes(buff,1)== SUCCESS)
    {
     return  buff[0];
    } 
}

/**********************************************************
Description: Get the data received by RF
Parameters:          
Return:      result: data received by RF
Others:     
**********************************************************/
uint8_t BM2302_9x_1::readRFData(void)
{
    uint8_t sendBuf[1] = {0x82};
    uint8_t buff[1] = {0};
    writeBytes(sendBuf,1);
    if(readBytes(buff,1)== SUCCESS)
    {
     return  buff[0];
    } 
}


/**********************************************************
Description: Configure RF to enter RX receive mode
Parameters:        
Return:          
Others:      The command execution time should not be longer than 3ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::setRXMode(void)
{
    uint8_t sendBuf[1] = {0x01};
    writeBytes(sendBuf,1); 
}

/**********************************************************
Description: Configure RF to enter pairing mode
Parameters:         
Return:          
Others:      Store the next packet address received. RF will continue to be in RX mode.
             The command execution time should not be longer than 3ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::setPairMode(void)
{
    uint8_t sendBuf[1] = {0x02};
    writeBytes(sendBuf,1); 
}

/**********************************************************
Description: Configure RF to enter DeepSleep mode.
Parameters:         
Return:          
Others:      The command execution time should not be longer than 3ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::sleep(void)
{
    uint8_t sendBuf[1] = {0x03};
    writeBytes(sendBuf,1); 
    delay(10); 
}

/**********************************************************
Description:  Get the version information 
Parameters:          
Return:       result:0b1000000011   
Others:     
**********************************************************/
uint16_t BM2302_9x_1::getFWVer(void)
{
    uint8_t sendBuf[1] = {0x90};
    uint8_t buff[2] = {0};
    writeBytes(sendBuf,1);
    if(readBytes(buff,2)== SUCCESS)
    {
     return (buff[1]<<8+buff[0]);
    } 
}

/**********************************************************
Description: Configuring the RF Frequency Band
Parameters:  band：
                  RF_315MHz(325MHZ)
                  RF_433_92MHz(433MHz)        
Return:          
Others:      The command execution time should not be longer than 50ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::setRFFrequencyBand(uint8_t frequencyBand)
{
    uint8_t sendBuf[2] = {0x10, 0x00};
    sendBuf[1] = frequencyBand;
    writeBytes(sendBuf,2);
    delay(62);  
}

/**********************************************************
Description: Accurate configuration of RF frequency
Parameters:  DNDK[6:0]:Floor{((fRF-fIF)/(fXTAL/2))×(0.8)}×M, (315MHz：M=2, Other frequency bands：M=1)
             DNDK[7]:0(default)
             DNDK[19:8]: Floor{(((fRF-fIF)/(fXTAL/2))×(0.8)×M-D_N[6:0])×1048576}, (315MHz：M=2, Other frequency bands：M=1)
             DNDK[24:20]:0x06(default)
Return:          
Others:  fIF=200kHz, fXTAL=16MHz    
**********************************************************/
void BM2302_9x_1::setRFFrequency(uint8_t DNDK[])
{
    uint8_t sendBuf[5] = {0x40, 0x00, 0x00, 0x00, 0x00};
    sendBuf[1] = DNDK[0];
    sendBuf[2] = DNDK[1];
    sendBuf[3] = DNDK[2];
    sendBuf[4] = DNDK[3];
    writeBytes(sendBuf,5);
}

/**********************************************************
Description: Accurate configuration of RF frequency
Parameters:  frequency： unit(Mhz)
      
Return:          
Others:      
**********************************************************/
void BM2302_9x_1::setRFFrequency(double frequency)
{
    uint8_t freq_value[4],band,m;
    unsigned long DK;
    double result;

    if(frequency<360)
      m = 2;
    else
      m = 1;

    result = ((frequency-0.2)/(BC2302_HXT_Freq/2))*0.8*m;
    freq_value[0] = result;
    
    DK=(result-(double)freq_value[0])*1048576ul;
    freq_value[1] = DK&0xff;
    freq_value[2] = (DK>>8)&0xff;
    freq_value[3] = (DK>>16)&0xff|0x60;

    if(frequency<360)
      band = RF_315MHz;          //  300~360MHz
    else if(frequency<450)
      band = RF_433MHz;          //  390~450MHz
    else if(frequency<890)
      band = RF_868MHz;
    else if(frequency>900)
      band = RF_915MHz;
    else
      band = RF_433MHz;          // default

    //SetRFBand(band);
    setRFFrequency(freq_value);
}

/**********************************************************
Description: Configure RF to enter DeepSleep mode.
Parameters:         
Return:          
Others:      The command execution time should not be longer than 3ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::setTestMode(void)
{
    uint8_t sendBuf[1] = {0x0E};
    writeBytes(sendBuf,1);
}

/**********************************************************
Description: Configure RF to enter DeepSleep mode.
Parameters:         
Return:          
Others:      The command execution time should not be longer than 3ms.
             Run other commands after the command execution is complete
**********************************************************/
void BM2302_9x_1::exitTestMode(void)
{
    uint8_t sendBuf[1] = {0x0F};
    writeBytes(sendBuf,1);
}


/**********************************************************
Description: writeBytes
Parameters:  wbuf[]:Variables for storing Data to be sent
             wlen:Length of data sent  
Return:   
Others:
**********************************************************/
void BM2302_9x_1::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
 if(_ModeSlect==IIC_MODE)
 {
  if (_wire != NULL)
  {
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(BM2302_9x_1_Addr);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
  }
 }
 else if(_ModeSlect==UART_MODE)
 {
  /* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_hardSerial->available() > 0)
    {
      _hardSerial->read();
    }
    _hardSerial->write(wbuf, wlen);
  }
 }
}

/**********************************************************
Description: readBytes
Parameters:  rbuf[]:Variables for storing Data to be obtained
             rlen:Length of data to be obtained
Return:   
Others:
**********************************************************/
uint8_t BM2302_9x_1::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut)
{
  uint8_t i = 0, delayCnt = 0;
  /****** IIC MODE ******/
  if(_ModeSlect==IIC_MODE)
  {
    _wire->requestFrom(BM2302_9x_1_Addr, rlen);
    if (_wire->available() == rlen)
   {
     for (i = 0; i < rlen; i++)
     {
      rbuf[i] = _wire->read();
     }
   }
   else
   {
    return TIMEOUT_ERROR;
   }
 }
 
/****** UART MODE ******/
 else if(_ModeSlect==UART_MODE)
{
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }

  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_hardSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _hardSerial->read();
    }
  }
 }
  return SUCCESS;
}
