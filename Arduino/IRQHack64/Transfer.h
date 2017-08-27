#ifndef _TRANSFER_

#define _TRANSFER_

#include "Arduino.h"
#include "HardwareSerial.h"
#include "TransferConstants.h"

#define PORT_MANIPULATION
#define OPENCOLLECTORSTYLE

extern unsigned int transferIndex;
extern unsigned int blockIndex;
extern unsigned char toTransmitVal1;
extern unsigned char toTransmitVal2;
extern unsigned char toTransmitVal3;
extern unsigned char toTransmitVal4;
extern unsigned char toTransmitVal5;
extern unsigned char toTransmitVal6;
extern unsigned char toTransmitVal7;
extern unsigned char transferBufferIndex;
extern unsigned char bytesPerNMI;

unsigned int GetTransferIndex();
unsigned int GetBlockIndex();
void setAddressPinsOutput();
void setPage(unsigned char value);
void InitTransfer(unsigned char);
void ResetC64();
void TransmitByteSlow(unsigned char val);
void TransmitByteBlockEnd(unsigned char val);
void ResetIndex();
unsigned char  IsMatchLast(char * container, char * val);
//inline void SetPort(unsigned char )  __attribute__((always_inline) );

inline void ResetLow() {
    #ifdef OPENCOLLECTORSTYLE
     PORTB &= ~_BV(PB1); // turn off internal resistor 
     DDRB |= _BV(PB1); // set to output       
    #else
      //digitalWrite(RESET, LOW);    
      PORTB &= ~_BV (PB1);
    #endif  
}

inline void ResetHigh() {
    #ifdef OPENCOLLECTORSTYLE
      DDRB &= ~_BV(PB1); //switch to input while port is low. 
      PORTB |= _BV(PB1); //turn on internal resistor to Vcc 
    #else
      //digitalWrite(RESET, HIGH); 
      PORTB |= _BV (PB1);
    #endif  
}

inline void ResetSetup() {
    #ifdef OPENCOLLECTORSTYLE
      ResetHigh();
    #else  
      pinMode(RESET, OUTPUT);
      digitalWrite(RESET, HIGH);
    #endif  
}


inline void NmiLow() {
    #ifdef OPENCOLLECTORSTYLE
     PORTB &= ~_BV(PB0); // turn off internal resistor 
     DDRB |= _BV(PB0); // set to output       
    #else
      //digitalWrite(NMI, LOW);    
      PORTB &= ~_BV (PB0);
    #endif
}

inline void NmiHigh() {
    #ifdef OPENCOLLECTORSTYLE
      DDRB &= ~_BV(PB0); //switch to input while port is low. 
      PORTB |= _BV(PB0); //turn on internal resistor to Vcc 
    #else
      //digitalWrite(NMI, HIGH); 
      PORTB |= _BV (PB0);
    #endif
}

inline void NmiSetup() {
    #ifdef OPENCOLLECTORSTYLE
      NmiHigh();
    #else  
      pinMode(NMI, OUTPUT);
      digitalWrite(NMI, HIGH);
    #endif  
}

inline void  TransmitByteFast(unsigned char val) {
    #ifdef PORT_MANIPULATION
    PORTD = (PIND & 0x0F) | (val & 0xF0);
    PORTC = (PINC & 0xF0) | (val & 0x0F);
    #else
    unsigned char mask = 1;
    for (int i=0;i<8;i++) {
      digitalWrite(addressPins[i], val & mask);
      mask = mask<<1;
    }    
    #endif 


    NmiLow();
    delayMicroseconds(6); //Wait for interrupt to trigger
    NmiHigh();   
    delayMicroseconds(31);  //Wait for interrupt to finish it's job  
}

/*
inline void SetPort(unsigned char value) {
    #ifdef PORT_MANIPULATION
    unsigned char portDVal = (PIND & 0x0F) | (value & 0xF0);
    unsigned char portCVal = (PINC & 0xF0) | (value & 0x0F);
    PORTD = portDVal;
    PORTC = portCVal;
    #else
    unsigned char mask = 1;
    for (int i=0;i<8;i++) {
      digitalWrite(addressPins[i], value & mask);
      mask = mask<<1;
    }    
    #endif 
}

*/

inline void SetPort(unsigned char value) {
    #ifdef PORT_MANIPULATION
    PORTD = (PIND & 0x0F) | (value & 0xF0);
    PORTC = (PINC & 0xF0) | (value & 0x0F);
    #else
    unsigned char mask = 1;
    for (int i=0;i<8;i++) {
      digitalWrite(addressPins[i], value & mask);
      mask = mask<<1;
    }    
    #endif 
}

#define PRE_WAIT 3
#define INITIAL_WAIT 17
#define INTER_WAIT 11
#define FINAL_WAIT 23
#define SINGLE_WAIT 35

inline void  TransmitByteFastNew1(unsigned char value) 
{ 
    SetPort(value);

    NmiLow();      
    delayMicroseconds(5); //Wait for interrupt to trigger
    NmiHigh();      
    
    delayMicroseconds(SINGLE_WAIT);

    transferIndex++;
    if (transferIndex == 256) 
    {
      blockIndex++;
      transferIndex = 0;
      delayMicroseconds(25); //Extra wait for the branch...
    }          
}



#define WAIT_1_CYCLE asm("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n")
#define WAIT_2_CYCLE WAIT_1_CYCLE;WAIT_1_CYCLE;
#define WAIT_3_CYCLE WAIT_2_CYCLE;WAIT_1_CYCLE;
#define WAIT_4_CYCLE WAIT_3_CYCLE;WAIT_1_CYCLE;asm("nop\n");
#define WAIT_5_CYCLE WAIT_4_CYCLE;WAIT_1_CYCLE;
#define WAIT_6_CYCLE WAIT_4_CYCLE;WAIT_2_CYCLE;
#define WAIT_7_CYCLE WAIT_4_CYCLE;WAIT_3_CYCLE;
#define WAIT_8_CYCLE WAIT_4_CYCLE;WAIT_4_CYCLE;
#define WAIT_9_CYCLE WAIT_8_CYCLE;WAIT_1_CYCLE;
#define WAIT_10_CYCLE WAIT_8_CYCLE;WAIT_2_CYCLE;
#define WAIT_11_CYCLE WAIT_8_CYCLE;WAIT_3_CYCLE;
#define WAIT_12_CYCLE WAIT_8_CYCLE;WAIT_4_CYCLE;
#define WAIT_13_CYCLE WAIT_8_CYCLE;WAIT_5_CYCLE;
#define WAIT_14_CYCLE WAIT_8_CYCLE;WAIT_6_CYCLE;
#define WAIT_15_CYCLE WAIT_8_CYCLE;WAIT_7_CYCLE;
#define WAIT_16_CYCLE WAIT_8_CYCLE;WAIT_8_CYCLE;
#define WAIT_17_CYCLE WAIT_16_CYCLE;WAIT_1_CYCLE;
#define WAIT_18_CYCLE WAIT_16_CYCLE;WAIT_2_CYCLE;
#define WAIT_19_CYCLE WAIT_16_CYCLE;WAIT_3_CYCLE;
#define WAIT_20_CYCLE WAIT_16_CYCLE;WAIT_4_CYCLE;
#define WAIT_21_CYCLE WAIT_16_CYCLE;WAIT_5_CYCLE;
#define WAIT_22_CYCLE WAIT_16_CYCLE;WAIT_6_CYCLE;
#define WAIT_23_CYCLE WAIT_16_CYCLE;WAIT_7_CYCLE;
#define WAIT_24_CYCLE WAIT_16_CYCLE;WAIT_8_CYCLE;
#define WAIT_25_CYCLE WAIT_16_CYCLE;WAIT_9_CYCLE;
#define WAIT_26_CYCLE WAIT_16_CYCLE;WAIT_10_CYCLE;
#define WAIT_27_CYCLE WAIT_16_CYCLE;WAIT_11_CYCLE;
#define WAIT_28_CYCLE WAIT_16_CYCLE;WAIT_12_CYCLE;
#define WAIT_29_CYCLE WAIT_16_CYCLE;WAIT_13_CYCLE;
#define WAIT_30_CYCLE WAIT_16_CYCLE;WAIT_14_CYCLE;
#define WAIT_31_CYCLE WAIT_16_CYCLE;WAIT_15_CYCLE;
#define WAIT_32_CYCLE WAIT_16_CYCLE;WAIT_16_CYCLE;


/*
#define _PRE_WAIT WAIT_4_CYCLE
#define _INITIAL_WAIT WAIT_22_CYCLE
#define _INTER_WAIT WAIT_10_CYCLE
#define _FINAL_WAIT WAIT_26_CYCLE

inline void  TransmitByteFastNew4(unsigned char value) 
{ 
    // value will be the 4th byte to be transmitted
    if (transferBufferIndex==0) {
        toTransmitVal1 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==1) {
        toTransmitVal2 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==2) {
        toTransmitVal3 = value; transferBufferIndex++; return;
    } else {
        transferBufferIndex = 0;
    }
    
    SetPort(toTransmitVal1);

    NmiLow();      
    _PRE_WAIT
    NmiHigh();      

    _INITIAL_WAIT
    SetPort(toTransmitVal2);
    _INTER_WAIT
    SetPort(toTransmitVal3);
    _INTER_WAIT
    SetPort(value);
    
    _FINAL_WAIT
    
    transferIndex = transferIndex + 4;
    if (transferIndex == 256) 
    {
      blockIndex++;
      transferIndex = 0;
      delayMicroseconds(25); //Extra wait for the branch...
    }  

}
*/


/*
#define _PRE_WAIT 5
#define _INITIAL_WAIT 16
#define _INTER_WAIT 11
#define _FINAL_WAIT 23
#define SINGLE_WAIT 35

inline void  TransmitByteFastNew4(unsigned char value) 
{ 
    // value will be the 4th byte to be transmitted
    if (transferBufferIndex==0) {
        toTransmitVal1 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==1) {
        toTransmitVal2 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==2) {
        toTransmitVal3 = value; transferBufferIndex++; return;
    } else {
        transferBufferIndex = 0;
    }
    
    SetPort(toTransmitVal1);

    NmiLow();      
    delayMicroseconds(_PRE_WAIT); //Wait for interrupt to trigger
    NmiHigh();      

    delayMicroseconds(_INITIAL_WAIT);
    //Delay16Micro();Delay4Micro();Delay2Micro();
    SetPort(toTransmitVal2);
    delayMicroseconds(_INTER_WAIT);
    //Delay8Micro();Delay2Micro();DelayMicro();
    SetPort(toTransmitVal3);
    delayMicroseconds(_INTER_WAIT);
    //Delay8Micro();Delay2Micro();DelayMicro();
    SetPort(value);
    
    delayMicroseconds(_FINAL_WAIT);
    //Delay16Micro();Delay4Micro();Delay2Micro();DelayMicro();

    transferIndex = transferIndex + 4;
    if (transferIndex == 256) 
    {
      blockIndex++;
      transferIndex = 0;
      delayMicroseconds(25); //Extra wait for the branch...
    }          
}



*/


#define _PRE_WAIT WAIT_5_CYCLE
#define _INITIAL_WAIT WAIT_19_CYCLE
#define _INTER_WAIT WAIT_8_CYCLE
#define _FINAL_WAIT WAIT_40_CYCLE

inline void TransmitByteFastNew8(unsigned char value) 
{ 
    // value will be the 4th byte to be transmitted
    if (transferBufferIndex==0) {
        toTransmitVal1 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==1) {
        toTransmitVal2 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==2) {
        toTransmitVal3 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==3) {
        toTransmitVal4 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==4) {
        toTransmitVal5 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==5) {
        toTransmitVal6 = value; transferBufferIndex++; return;
    } else if (transferBufferIndex==6) {
        toTransmitVal7 = value; transferBufferIndex++; return;
    }     
    else {
        transferBufferIndex = 0;
    }
    
    SetPort(toTransmitVal1);

    NmiLow();      
    _PRE_WAIT
    NmiHigh();      

    _INITIAL_WAIT
    SetPort(toTransmitVal2);
    _INTER_WAIT
    SetPort(toTransmitVal3);
    _INTER_WAIT
    SetPort(toTransmitVal4);
    _INTER_WAIT
    SetPort(toTransmitVal5);
    _INTER_WAIT
    SetPort(toTransmitVal6);
    _INTER_WAIT
    SetPort(toTransmitVal7);
    _INTER_WAIT
    SetPort(value);
    
    //_FINAL_WAIT
    delayMicroseconds(10);

    transferIndex = transferIndex + 8;
    if (transferIndex == 256) 
    {
      blockIndex++;
      transferIndex = 0;
      delayMicroseconds(25); //Extra wait for the branch...
    }          
}

inline void  TransmitByteFastNew(unsigned char value)  {
  if (bytesPerNMI == 1) {
    TransmitByteFastNew1(value);    
  } else if (bytesPerNMI == 4) {
    //TransmitByteFastNew4(value);
  } else if (bytesPerNMI == 8) {
    TransmitByteFastNew8(value);    
  }
}

inline void  TransmitByteFastStd(unsigned char val) 
{ 
    #ifdef PORT_MANIPULATION
    PORTD = (PIND & 0x0F) | (val & 0xF0);
    PORTC = (PINC & 0xF0) | (val & 0x0F);
    #else
    unsigned char mask = 1;
    for (int i=0;i<8;i++) {
      digitalWrite(addressPins[i], val & mask);
      mask = mask<<1;
    }    
    #endif 

   if (transferIndex==255) {
      NmiLow();      
      delayMicroseconds(6); //Wait for interrupt to trigger
      NmiHigh();      
      delayMicroseconds(70);  //Wait for interrupt to finish it's job
      transferIndex = 0;
      blockIndex++;
   } else {
     
    NmiLow();     
    delayMicroseconds(6); //Wait for interrupt to trigger    
    NmiHigh();
    delayMicroseconds(31);  //Wait for interrupt to finish it's job     
    transferIndex++;
   }      
}

inline void EnableCartridge() {
  digitalWrite(EXROM, LOW);
}

inline void DisableCartridge() {
  digitalWrite(EXROM, HIGH);
}

#endif

