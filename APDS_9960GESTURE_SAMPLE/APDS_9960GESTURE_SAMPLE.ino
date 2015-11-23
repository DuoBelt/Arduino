//AKIZUKI AE-APDS9960 GESTURE SENSOR SAMPLE PROGRAM 
//APDS9960 SCL = A5
//APDS9960 SDA = A4
//APDS9960 GND = GND
//APDS9960 VDD = 3.3V
//APDS9960 LED = 3.3V(47uF REQUIRE)
//USE 3.3V PWR or USE LEVEL SHIFTER
//NO INTERRUPT USE (THIS PROGRAM FORMED "DIRECT REGISTER ACCESS")
#include <Wire.h>

#define APDS9960_ADR  0x39
#define APDS9960_ID   0xAB

String        SERIAL_STRING;
unsigned char DATA_U,DATA_D,DATA_L,DATA_R;
unsigned char OLD_U,OLD_D,OLD_L,OLD_R;
unsigned char work;
unsigned char U_PEAK_END_FLAG,D_PEAK_END_FLAG,L_PEAK_END_FLAG,R_PEAK_END_FLAG;
unsigned char STATUS_UD,STATUS_LR;
unsigned char OLD_STATUS_UD,OLD_STATUS_LR;
unsigned char DISP_FLAG;
unsigned char NOISE_LEVEL = 2;
unsigned char DECIDE_FLAG;
unsigned int  PHASE_COUNTER;
unsigned int  U_PEAK,D_PEAK,L_PEAK,R_PEAK;

//***************************************************
unsigned char I2C_READ(unsigned char REG_ADR)
{
  Wire.beginTransmission(APDS9960_ADR);
  Wire.write(REG_ADR);
  Wire.endTransmission(false);
  Wire.requestFrom(APDS9960_ADR,1);
  return  Wire.read();
}

void I2C_WRITE(unsigned char REG_ADR, unsigned char DATA)
{
  Wire.beginTransmission(APDS9960_ADR);
  Wire.write(REG_ADR);
  Wire.write(DATA);
  Wire.endTransmission(false);
}
//************************************************************
void setup()
{
  Wire.begin();                 //I2C BEGIN
  Serial.begin(9600);          //SET BAUD RATE 9600
  work = I2C_READ(0x92);        //READ REGISTER 0x92 (ADPS9960's ID)
  if(work == APDS9960_ID) 
  {
    Serial.println("Found APDS-9960 : ID = 0xAB on Resister Address 0x92");
    Serial.println("GESTURE SENSOR START");
  }
  else Serial.println("APDS-9960 NOT FOUND");
  //
  I2C_WRITE(0x80,B01000101); //POWER ON<0>, GESTURE ENABLE<6>, PROXIMITY DETECT ENALBE<2>,AEN=0
  I2C_WRITE(0x90,B00110000); //Gesture LED Drive Strength 300%(max)
  I2C_WRITE(0xA3,B01100100); //Reserve0, Gain x8(11), LED Drive 100mA(00), Wait Time see under number
                             //111=39.2mS 110=30.8mS 101=22.4mS 100=14.0mS 011=8.4mS 010=5.6mS 001=2.8ms 000=0mS
  I2C_WRITE(0xA4,70);        //U MINUS OFFSET
  I2C_WRITE(0xA5,0);         //D MINUS OFFSET
  I2C_WRITE(0xA7,10);        //L MINUS OFFSET
  I2C_WRITE(0xA9,34);        //R MINUS OFFSET
  I2C_WRITE(0xAB,B00000001); //GIEN off<1>(INTERRUPT DISABLE), GMODE ON<0>
  RESET_VARIABLE();
}
//---------------------------------------------------------------------
void loop()
{
  work = I2C_READ(0xAE);  //READ GESTUR FIFO LEVEL REGISTER
  if(work != 0)           //IF FIFO HAS SOME DATA
  {
    DATA_U = I2C_READ(0xFC);
    DATA_D = I2C_READ(0xFD);
    DATA_L = I2C_READ(0xFE);
    DATA_R = I2C_READ(0xFF);
    if((DATA_U > NOISE_LEVEL) && (DATA_D > NOISE_LEVEL)&& (DATA_L> NOISE_LEVEL) && (DATA_R > NOISE_LEVEL)) //NOISE CANCEL
    {
      DATA_SYORI();       // 
      PHASE_COUNTER++;    //
      DISP_FLAG = 1;      //
    }
    else 
    {
      if(DISP_FLAG)
      {
        DISP_FLAG = 0;
        DISP_DIR();
      }
      RESET_VARIABLE();
    }
  }
}
//***************************************************
void RESET_VARIABLE(void)
{
  PHASE_COUNTER = 0;
  U_PEAK=0;
  D_PEAK=0;
  L_PEAK=0;
  R_PEAK=0;
  OLD_U = 0;
  OLD_D = 0;
  OLD_L = 0;
  OLD_R = 0;
  U_PEAK_END_FLAG = 0;
  D_PEAK_END_FLAG = 0;
  L_PEAK_END_FLAG = 0;
  R_PEAK_END_FLAG = 0;
  STATUS_UD = 0;
  STATUS_LR = 0;
  OLD_STATUS_UD = 0;
  OLD_STATUS_LR = 0;
  SERIAL_STRING = "";
  DISP_FLAG = 0;
  DECIDE_FLAG = 0;
}
//----------------------------------------------------------------------
void DISP_DIR(void)
{
  if(!(SERIAL_STRING == "")) Serial.println(SERIAL_STRING);
}
//---------------------------------------------------------------------------------------
void DATA_SYORI(void)
{
  if (DATA_U > OLD_U)                //IF NEW_DATA > OLD_DATA_BUFFER(APROACH TO PEAK)
  {
    OLD_U = DATA_U;                  //SAVE NEW_DATA TO OLD_DATA_BUFFER
    U_PEAK = PHASE_COUNTER;          //PEAK_PHASE RENEWAL
    U_PEAK_END_FLAG = 0;             //STILL PEAK or APROACH TO PEAK
  }
  else
  {
    U_PEAK_END_FLAG = 1;             //PEAK WAS GONE
  }
  //**************************
  if (DATA_D > OLD_D)
  {
    OLD_D = DATA_D;
    D_PEAK = PHASE_COUNTER;
    D_PEAK_END_FLAG = 0;
  }
  else
  {
    D_PEAK_END_FLAG = 1;
  }
  //**************************
  if (DATA_L > OLD_L)
  {
    OLD_L = DATA_L;
    L_PEAK = PHASE_COUNTER;
    L_PEAK_END_FLAG = 0;
  }
  else
  {
    L_PEAK_END_FLAG = 1;
  }
  //*************************
  if (DATA_R > OLD_R)
  {
    OLD_R = DATA_R;
    R_PEAK = PHASE_COUNTER;
    R_PEAK_END_FLAG = 0;
  }
  else
  {
    R_PEAK_END_FLAG = 1;
  }
  //**************************
  if(U_PEAK_END_FLAG && D_PEAK_END_FLAG && L_PEAK_END_FLAG && R_PEAK_END_FLAG) //IF ALL PEAK WAS GONE
  {
    DECIDE_FLAG = 0;
    if ((U_PEAK > D_PEAK) & (U_PEAK >= L_PEAK) & (U_PEAK >= R_PEAK))           //U_PEAK WAS LAST
    {
      SERIAL_STRING = "DOWN";
      DECIDE_FLAG = 1;
    }
    if ((D_PEAK > U_PEAK) & (D_PEAK >= L_PEAK) & (D_PEAK >= R_PEAK))           //D_PEAK WAS LAST
    {
      SERIAL_STRING = "UP";
      DECIDE_FLAG = 1;
    }
    if ((L_PEAK >= U_PEAK) & (L_PEAK >= D_PEAK) & (L_PEAK > R_PEAK))           //L_PEAK WAS LAST
    {
      SERIAL_STRING = "RIGHT";
      DECIDE_FLAG =1;
    }
    if ((R_PEAK >= U_PEAK) & (R_PEAK >= D_PEAK) & (R_PEAK > L_PEAK))           //R_PEAK WAS LAST
    {
      SERIAL_STRING = "LEFT";
      DECIDE_FLAG = 1;
    }
    if (!DECIDE_FLAG)SERIAL_STRING = "NONE";                                   //CAN'T DECIDE
 }
}

//END
 
