#include "DualVNH5019MotorShield.h"

const byte STOP  = 0x0;
const byte LEFT  = 0x1;
const byte RIGHT = 0x2;
const byte FWD   = 0x4;
const byte REV   = 0x8;
const byte BRK   = 0xA;

const int MAX_SPEED = 50;

byte inByte = 0xF;

DualVNH5019MotorShield md;

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}

void setup()
{
  // start serial port at 115200 bps and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  establishContact();  // send a byte to establish contact until controller responds
  md.init(); 
}

void loop()
{
  int m1Current = 0;
  int m2Current = 0;
  
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) 
  {
    // get incoming byte:
    inByte = Serial.read();
    
    // get motor currents
    m1Current = getM1CurrentMilliamps();
    m2Current = getM2CurrentMilliamps();
    
    switch(inByte)
    {
      case STOP:
        
        md.setSpeeds(0, 0); 
        Serial.println("M1,M2 Stopped");
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        break;
      case LEFT:
        Serial.println("M1 Fwd, M2 Rev");
        for (int i = 0; i <= MAX_SPEED; i++)
        { 
           md.setSpeeds(-i, i);
           stopIfFault();
           delay(20);
        }
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        break;
      case RIGHT: 
        Serial.println("M1 Rev, M2 Fwd");       
        for (int i = 0; i <= MAX_SPEED; i++)
        { 
           md.setSpeeds(i, -i);
           stopIfFault();
           delay(20);
        }
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        break;
      case FWD: 
        Serial.println("M1 Fwd, M2 Fwd");
        for (int i = 0; i >= -MAX_SPEED; i--)
        { 
           md.setSpeeds(i, i);
           stopIfFault();
           delay(20);
        }
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        break;
      case REV: 
        Serial.println("M1 Rev, M2 Rev");
        for (int i = 0; i <= MAX_SPEED; i++)
        { 
           md.setSpeeds(i, i);
           stopIfFault();
           delay(20);
        }
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        break;
      case BRK:
        Serial.println("M1,M2 Braked");
        md.setBrakes(MAX_SPEED/2, MAX_SPEED/2);
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());        
    }
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("Motor controller ready");   // send an initial string
    delay(1000);
  }
}


