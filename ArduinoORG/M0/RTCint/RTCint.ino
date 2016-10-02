/********************************************************************************************************************************************************
This sketch gives a simple demonstration of how to use RTCint library.
The code sets date and time using internal structure and then prints them on serial monitor .
In this example the Time is represented in 24 hour mode
********************************************************************************************************************************************************/
 
#include <RTCInt.h>// include the RTCint library
 
RTCInt rtc;             //create an RTCInt type object
/*setup*/
void setup() 
{
  SerialUSB.begin(115200);//initialize the Serial communication
  rtc.begin(TIME_H24);  //init RTC in 24 hour mode
   
  //time settings
  CHour(15,0);  //setting hour
  rtc.setMinute(43);   //setting minute
  rtc.setSecond(0);    //setting second
 
  rtc.setDay(13);      //setting day
  rtc.setMonth(8);     //setting month
  rtc.setYear(15);     //setting year
 
}
/*loop*/
void loop() 
{
 rtc.getDate();       //getting date in local structure (local_date)
 rtc.getTime();       //getting time in local structure(local_time)
 
 //printing date in format YYYY/MM/DD
 SerialUSB.print(rtc.local_date.year+2000); //year
 SerialUSB.print('/');
 SerialUSB.print(rtc.local_date.month);     //month
 SerialUSB.print('/');
 SerialUSB.print(rtc.local_date.day);       //day
 SerialUSB.print(' ');
 
 //printing time
 SerialUSB.print(rtc.local_time.hour);      //hour
 SerialUSB.print(':');
 SerialUSB.print(rtc.local_time.minute);    //minute
 SerialUSB.print(':');
 SerialUSB.println(rtc.local_time.second);  //second
 
 delay(1000);          //wait 1 second
} 
