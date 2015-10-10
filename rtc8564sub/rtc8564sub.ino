//---------------------------------------------------------------------------
// EPSON TOYOCOM RTC-8564 テストプログラム
//
// 配線：
//  Arduino Analog 4 - RTC8564 SDA
//  Arduino Analog 5 - RTC8564 SCL
//---------------------------------------------------------------------------
#include <Wire.h>

#define REG_ADDR_CONTROL1      0x00
#define REG_ADDR_CONTROL2      0x01
#define REG_ADDR_SECONDS       0x02
#define REG_ADDR_MINUTES       0x03
#define REG_ADDR_HOURS         0x04
#define REG_ADDR_DAYS          0x05
#define REG_ADDR_WEEKDAYS      0x06
#define REG_ADDR_MONTHS        0x07
#define REG_ADDR_YEARS         0x08
#define REG_ADDR_MINUTE_ALARM  0x09
#define REG_ADDR_HOUR_ALARM    0x0A
#define REG_ADDR_DAY_ALARM     0x0B
#define REG_ADDR_WEEKDAY_ALARM 0x0C
#define REG_ADDR_CLOCKOUT_FREQ 0x0D
#define REG_ADDR_TIMER_CONTROL 0x0E
#define REG_ADDR_TIMER         0x0F

#define STOP_BIT                5     // CONTROL1
#define INTERRUPT_PERIODIC      4     // CONTROL2
#define ALARM_FLAG              3     // CONTROL2
#define TIMER_FLAG              2     // CONTROL2
#define ALARM_INTERRUPT_ENABLE  1     // CONTROL2
#define TIMER_INTERRUPT_ENABLE  0     // CONTROL2
#define VOLTAGE_LOW             7     // SECONDS
#define ALARM_ENABLE            7     // MIN ALARAM - WEEKDAY ALARAM
#define CLOCK_OUT_ENABLE        7     // CLKOUT
#define CLOCK_OUT_FREQ_32768Hz  0x00  // CLKOUT
#define CLOCK_OUT_FREQ_1024Hz   0x01  // CLKOUT
#define CLOCK_OUT_FREQ_32Hz     0x02  // CLKOUT
#define CLOCK_OUT_FREQ_1Hz      0x03  // CLKOUT
#define TIMER_ENABLE            7     // TIMER CONTROL
#define TIMER_CLOCK_4096Hz      0     // TIMER CONTROL
#define TIMER_CLOCK_64Hz        1     // TIMER CONTROL
#define TIMER_CLOCK_1Hz         2     // TIMER CONTROL
#define TIMER_CLOCK_1_60Hz      3     // TIMER CONTROL

#define MINUTES_MASK   0b01111111
#define HOURS_MASK     0b00111111
#define DAYS_MASK      0b00111111
#define WEEKDAYS_MASK  0b00000111
#define MONTHS_MASK    0b00011111

#define RTC8564_ADDR  0x51  // I2C 7bit address

typedef struct {
  int  year;
  byte month;
  byte day;
  byte hour;
  byte min;
  byte sec;
  byte weekday;
} RTC8564_TIME;

void rtc8564_get_time( RTC8564_TIME *tm );
void rtc8564_sprintf( char *buf, RTC8564_TIME tm );

//---------------------------------------------------------------------------
// DECIMAL -> BCD 変換
//---------------------------------------------------------------------------
byte rtc8564_dec2bcd( byte data )
{
  return ((( data / 10) << 4) + (data % 10));
}

//---------------------------------------------------------------------------
// BCD -> DECIMAL 変換
//---------------------------------------------------------------------------
byte rtc8564_bcd2dec( byte data )
{
  return ((( data >> 4) * 10) + (data % 16));
}

//---------------------------------------------------------------------------
// CLKOUT端子の周波数設定
//
// 引数：
//   CLOCK_OUT_FREQ_32768Hz : 32.768 kHz
//   CLOCK_OUT_FREQ_1024Hz  : 1.024 kHz
//   CLOCK_OUT_FREQ_32Hz    : 32 Hz
//   CLOCK_OUT_FREQ_1Hz     : 1 Hz
//---------------------------------------------------------------------------
void rtc8564_clock_out_freq( byte freq )
{
  rtc8564_write_byte( REG_ADDR_CLOCKOUT_FREQ, freq );
}  

//----- CLKOUT端子には出力しない(消費電力減) ---------------------
void rtc8564_clock_out_enable( void )
{
  rtc8564_set_bit( REG_ADDR_CLOCKOUT_FREQ, CLOCK_OUT_ENABLE );
}

//----- CLKOUT端子に出力する -----------------------------------
void rtc8564_clock_out_disable( void )
{
  rtc8564_clear_bit( REG_ADDR_CLOCKOUT_FREQ, CLOCK_OUT_ENABLE );
} 

//---------------------------------------------------------------------------
// １バイトRTC8564に書き込む
//
// 引数１：RTC8564のレジスターアドレス
// 引数２：レジスターに書き込む値
//---------------------------------------------------------------------------
void rtc8564_write_byte(byte addr, byte data)
{
  Wire.beginTransmission(RTC8564_ADDR);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission(); 
}

//---------------------------------------------------------------------------
// １バイトRTC8564から読み出す
//
// 戻り値：読み出した値
//---------------------------------------------------------------------------
byte rtc8564_read_byte(byte addr)
{
  Wire.beginTransmission(RTC8564_ADDR);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(RTC8564_ADDR, 1);

  return Wire.read();
}

//---------------------------------------------------------------------------
// RTC8564のレジスターの特定ビットの値を調べる
//
// 戻り値　　true:ビットの値が１　false:ビットの値が０
//---------------------------------------------------------------------------
boolean rtc8564_test_bit(byte addr, byte bit_position)
{
  byte data;
  
  data = rtc8564_read_byte( addr );
  data &= (0x01 << bit_position);
  if( data == 0x00 ) {
    return false;
  } else {
    return true;
  }
} 

//---------------------------------------------------------------------------
// RTC8564のレジスターの特定ビットをセットする
//---------------------------------------------------------------------------
void rtc8564_set_bit(byte addr, byte bit_position)
{
  byte data;
  
  data = rtc8564_read_byte( addr );
  data |= (0x01 << bit_position);
  rtc8564_write_byte( addr, data ); 
} 

//---------------------------------------------------------------------------
// RTC8564のレジスターの特定ビットをクリヤする
//---------------------------------------------------------------------------
void rtc8564_clear_bit(byte addr, byte bit_position)
{
  byte data;
  
  data = rtc8564_read_byte( addr );
  data &= ~(0x01 << bit_position);
  rtc8564_write_byte( addr, data ); 
} 

//---------------------------------------------------------------------------
// RTC8564の初期化
// 
// 日時設定、アラーム無効、CLKOUT出力なし、タイマー無効
//---------------------------------------------------------------------------
void rtc8564_init(
  int  year,
  byte month,
  byte day,
  byte hour,
  byte minute,
  byte second)
{
  rtc8564_write_byte(REG_ADDR_CONTROL1,0x20); 
  rtc8564_write_byte(REG_ADDR_CONTROL2,0x00);
  rtc8564_set_time( year, month, day, hour, minute, second);
  rtc8564_alarm_disable();
  rtc8564_clock_out_disable();
  rtc8564_write_byte(REG_ADDR_TIMER_CONTROL,0x00);
  rtc8564_write_byte(REG_ADDR_TIMER,0x00);
  rtc8564_clear_bit( REG_ADDR_CONTROL1, STOP_BIT ); 
}

//---------------------------------------------------------------------------
// アラーム機能
//  ・日時週時刻はそれぞれ個別に設定可能なため
//    毎週月曜の12:00にアラーム発生といった使い方ができる　　
//---------------------------------------------------------------------------

//------ アラーム発生日の設定 -------------------------------------
void rtc8564_set_alarm_day( byte day )
{
  rtc8564_write_byte(REG_ADDR_DAY_ALARM, rtc8564_dec2bcd(day));
}

//------ アラーム発生時刻の設定 -----------------------------------
void rtc8564_set_alarm_hour( byte hour )
{
  rtc8564_write_byte(REG_ADDR_HOUR_ALARM, rtc8564_dec2bcd(hour));
}

//------ アラーム発生分の設定 -------------------------------------
void rtc8564_set_alarm_minute( byte minute )
{
  rtc8564_write_byte(REG_ADDR_MINUTE_ALARM, rtc8564_dec2bcd(minute));
}

//------ アラーム発生週の設定 -------------------------------------
void rtc8564_set_alarm_weekday( byte weekday )
{
  rtc8564_write_byte(REG_ADDR_WEEKDAY_ALARM, weekday );
}

//------ アラーム発生時にINT端子をLOWにする -----------------------
void rtc8564_alarm_interrupt_enable( void )
{
  rtc8564_set_bit( REG_ADDR_CONTROL2, ALARM_INTERRUPT_ENABLE );
}

//------ INT端子には出力しない ------------------------------------
void rtc8564_alarm_interrupt_disable( void )
{
  rtc8564_clear_bit( REG_ADDR_CONTROL2, ALARM_INTERRUPT_ENABLE );
  rtc8564_alarm_disable();
}

//------ アラームを無効にする -------------------------------------
void rtc8564_alarm_disable( void )
{
  rtc8564_set_bit( REG_ADDR_DAY_ALARM,     ALARM_ENABLE );
  rtc8564_set_bit( REG_ADDR_HOUR_ALARM,    ALARM_ENABLE );
  rtc8564_set_bit( REG_ADDR_MINUTE_ALARM,  ALARM_ENABLE );
  rtc8564_set_bit( REG_ADDR_WEEKDAY_ALARM, ALARM_ENABLE );
  rtc8564_alarm_clear();
}

//------ アラームイベント(アラーム発生フラグ)のクリヤ -------------
void rtc8564_alarm_clear( void )
{
  rtc8564_clear_bit( REG_ADDR_CONTROL2, ALARM_FLAG );
}

//------ アラームが発生したか否かの確認 ---------------------------
// これを使ってアラームイベント発生を検知するためにはloop内で呼び続ける必要がある
// 戻り値
//   true：イベント発生
//   true：イベントは発生していない
//-----------------------------------------------------------------
boolean rtc8564_alarm_test( void )
{
  if( rtc8564_test_bit(REG_ADDR_CONTROL2, ALARM_FLAG ) ) {
    return true;
  } else {
    return false;
  }
} 

//---------------------------------------------------------------------------
// タイマー設定
//　・一定周期でイベントを発生させるために使う
//　・タイマーはダウンカウンターでゼロになるとイベント発生 
//---------------------------------------------------------------------------

//----- タイマーの設定 --------------------------------------------
// ２つの引数の組み合わせでイベント発生させる
// 引数１：ダウンカウンタープリセット値
// 引数２：カウントダウンさせるためのクロック速度
//-----------------------------------------------------------------
void rtc8564_timer_set( byte count, byte clock )
{
  rtc8564_timer_disable();
  rtc8564_write_byte(REG_ADDR_TIMER_CONTROL, clock );  
  rtc8564_write_byte(REG_ADDR_TIMER, count );
  rtc8564_timer_enable();
}

//----- タイマーを有効にする --------------------------------------
void rtc8564_timer_enable( void )
{
  rtc8564_set_bit( REG_ADDR_TIMER_CONTROL, TIMER_ENABLE );
}

//----- タイマーを無効にする --------------------------------------
void rtc8564_timer_disable( void )
{
  rtc8564_clear_bit( REG_ADDR_TIMER_CONTROL, TIMER_ENABLE );
}

//----- イベント発生時にINT端子の出力をLOWにする ------------------
void rtc8564_timer_interrupt_enable( void )
{
  rtc8564_set_bit( REG_ADDR_CONTROL2, TIMER_INTERRUPT_ENABLE );
}

//----- イベント発生時にINT端子に出力しない -----------------------
void rtc8564_timer_interrupt_disable( void )
{
  rtc8564_clear_bit( REG_ADDR_CONTROL2, TIMER_INTERRUPT_ENABLE );
}

//----- イベント発生フラグをクリヤする ----------------------------
void rtc8564_timer_clear( void )
{
  rtc8564_clear_bit( REG_ADDR_CONTROL2, TIMER_FLAG );
}

//----- イベントが発生したか否かをテストする ----------------------
// これを使ってイベント発生を検知するためにはloop内で呼び続ける必要がある
// 戻り値
//   true：イベント発生
//   true：イベントは発生していない
//-----------------------------------------------------------------
boolean rtc8564_timer_test( void )
{
  if( rtc8564_test_bit(REG_ADDR_CONTROL2, TIMER_FLAG ) ) {
    return true;
  } else {
    return false;
  }
} 

//---------------------------------------------------------------------------
// RTC8564に日時を設定する
//---------------------------------------------------------------------------
void rtc8564_set_time(
  int  year,
  byte month,
  byte day,
  byte hour,
  byte minute,
  byte second)
{
  rtc8564_write_byte(REG_ADDR_SECONDS,  rtc8564_dec2bcd(second));
  rtc8564_write_byte(REG_ADDR_MINUTES,  rtc8564_dec2bcd(minute));  
  rtc8564_write_byte(REG_ADDR_HOURS,    rtc8564_dec2bcd(hour));
  rtc8564_write_byte(REG_ADDR_DAYS,     rtc8564_dec2bcd(day));
  rtc8564_write_byte(REG_ADDR_WEEKDAYS, rtc8564_calc_weekday(year,month,day));   
  rtc8564_write_byte(REG_ADDR_MONTHS,   rtc8564_dec2bcd(month));
  rtc8564_write_byte(REG_ADDR_YEARS,    rtc8564_dec2bcd((byte)(year-2000)));
} 

//---------------------------------------------------------------------------
// RTC8564から日付時刻を取得する
//
// 引数：日時構造体へのポインタ
//---------------------------------------------------------------------------
void rtc8564_get_time( RTC8564_TIME *tm )
{
  tm->year    = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_YEARS    )) + 2000;
  tm->month   = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_MONTHS   ) & MONTHS_MASK );
  tm->day     = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_DAYS     ) & DAYS_MASK   );
  tm->hour    = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_HOURS    ) & HOURS_MASK  );
  tm->min     = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_MINUTES  ) & MINUTES_MASK);
  tm->sec     = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_SECONDS  ));
  tm->weekday = rtc8564_bcd2dec( rtc8564_read_byte( REG_ADDR_WEEKDAYS ));
} 

//---------------------------------------------------------------------------
// 日時を書式整形して出力する　rtc8564_get_time()で日時を取得した後に呼び出す
//
// 引数１：文字列の先頭アドレス
// 引数２：日時が格納されている構造体　
//---------------------------------------------------------------------------
void rtc8564_sprintf( char *buf, RTC8564_TIME tm )
{
  sprintf( buf,"%04u-%02u-%02u %02u:%02u:%02u\n", 
      tm.year, tm.month, tm.day, tm.hour, tm.min, tm.sec );
}

//---------------------------------------------------------------------------
// Zellerの公式による曜日の計算　RTC8564は曜日を自動計算できない
//
// return: 0:日曜　6:土曜
//---------------------------------------------------------------------------
byte rtc8564_calc_weekday( int year, int month, int day )
{
  if( month <= 2 ) {
    month += 12;
    year--;
  }

  return (byte)((year + year/4 - year/100 + year/400 + ((13 * month + 8)/5) + day) % 7);
}

//---------------------------------------------------------------------------
// RTC8564の初期電源投入確認
//  Arduinoの電源は切るがRTC8564は常時通電の場合、これを呼び出して
//
// 戻り値  true:最初の電源投入　false:すでに電源投入初期化済み
//---------------------------------------------------------------------------
boolean rtc8564_is_power_on( void )
{
  if( rtc8564_test_bit(REG_ADDR_SECONDS, VOLTAGE_LOW ) ) {
    return true;
  } else {
    return false;
  }
}

void handler(){
  Serial.println("Hi!");  
}

//---------------------------------------------------------------------------
// arduino setup
//---------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1,handler,CHANGE);


  Wire.begin();
  delay(1000);                                  // RTC8564が安定動作するのを待つ
  if( rtc8564_is_power_on() ) {
    rtc8564_init( 2014, 7, 10, 12, 0, 0 );      // 電源初期投入時のみ時刻の設定
  }
  Serial.println("start");
//  rtc8564_clock_out_freq( CLOCK_OUT_FREQ_1Hz ); // CLKOUTの周波数を1Hzに設定
//  rtc8564_clock_out_enable();                   // CLKOUT端子に出力

//  rtc8564_set_alarm_minute( 1 );                // 毎時01分にアラームベントを発生させる　1時間ごとに発生する
//  rtc8564_alarm_interrupt_enable();             // アラームイベントでINT端子をLOWにする
  
  rtc8564_timer_set( 3, TIMER_CLOCK_1Hz );     // 10秒おきにタイマーイベントを発生させる
  rtc8564_timer_interrupt_enable();             // タイマーイベントでINT端子をLOWにする
}

//---------------------------------------------------------------------------
// arduino loop
//---------------------------------------------------------------------------
RTC8564_TIME rtc_time;

void loop()
{
  char buf[32];

  rtc8564_get_time( &rtc_time );
  rtc8564_sprintf( buf, rtc_time );
  Serial.write( buf );
  
  if( rtc8564_alarm_test()  ) {   // アラームイベント発生確認
    Serial.println("alarm");
    rtc8564_alarm_clear();
  } 

  if( rtc8564_timer_test() ) {    // タイマーイベント発生確認
    Serial.println("timer clear");
    rtc8564_timer_clear();
  } 
  
  delay(1000);
}

//--------- E N D -----------------------------------------------------------

