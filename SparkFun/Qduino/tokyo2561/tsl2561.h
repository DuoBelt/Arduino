#include <stdint.h>
#include "Wire.h"
 
// i2c_tsl2561.h
// 測定時の積分時間は常に402msecとする(INTEG = 10B, scale = 1)
class tsl2561
{
  byte i2c_address;
  byte i2c_status = 0;
  
  byte device_id = 0;
  uint16_t data_ch0, data_ch1;
  double lux_value;
  bool gain16 = false; // false のときx1 , trueのときx16
  enum {reg_control = 0, reg_timing = 1, reg_thr_L = 2, reg_thr_H = 4, reg_int = 6, reg_id = 0x0a, reg_data0 = 0x0c, reg_data1 = 0x0e };
  enum {cmd_clear_int = 0xc0 };
 
public:
  tsl2561(byte addr = 0x39) {
    i2c_address = addr;
    Wire.begin();
    data_ch0 = 0;
    data_ch1 = 0;
    device_id = 0;
    lux_value = 0.0;
  }
 
  const byte get_device_id() { return device_id; }
  const double get_lux_value() { return lux_value; }
  bool init() {
    device_id = i2c_read8(reg_id | 0x80);
    if ((device_id & 0xf0) != 0x50)  // TSL2561FNが載っている。
      return false;
    return true;    
  }
 
  void start(bool use_interrupt = true, bool high_gain = false) {
    gain16 = high_gain;
    i2c_write_reg(reg_timing | 0x80, gain16 ? 0x12 : 0x2);  // timing . GAIN=x1, integrate time = 402msec. (scale = 1).
    i2c_write_reg(reg_int | 0x80, use_interrupt ? 0x10 : 0);  // level interrupt. on every conversion completion. OR disable.
    i2c_write_reg(reg_control | 0x80, 3);  // power on.
    lux_value = 0;
  }
 
  void stop() {
    i2c_write_reg(reg_control | 0x80, 0);
  }
 
  double get_data() {
    data_ch0 =  i2c_read16_swab(reg_data0 | 0x80);
    data_ch1 =  i2c_read16_swab(reg_data1 | 0x80);
    calc_lux();
    return lux_value;
  }
 
// データシートに書いてあるlux値の算出方法をコード化。
// INTEG = 402msec
// パッケージによってプログラム内の定数が変わることに注意。FNパッケージ用を記述した。
// 
  void calc_lux() {
    if (!data_ch0 || !data_ch1) {
      lux_value = 0.0;
      return;
    }
    double ch0 = (double)data_ch0 * (gain16 ? 1.0 : 16.0);
    double ch1 = (double)data_ch1 * (gain16 ? 1.0 : 16.0);
    double ratio = ch1 / ch0;
    if (ratio <= 0.125)
      lux_value = 0.0304 - 0.0272 * ratio;
    else if (ratio > 0.125 and ratio <= 0.250)
      lux_value = 0.0325 - 0.0440 * ratio;          
    else if (ratio > 0.250 and ratio <= 0.375)
      lux_value = 0.0351 - 0.0544 * ratio;          
    else if (ratio > 0.375 and ratio <= 0.5)
      lux_value = 0.0381 - 0.0624 * ratio;          
    else if (ratio > 0.50 && ratio <= 0.61)
      lux_value = 0.0224 - 0.031 * ratio;
    else if (ratio > 0.61 && ratio <= 0.80)
      lux_value = 0.0128 - 0.0153 * ratio;
    else if (ratio > 0.80 && ratio <= 1.30)
      lux_value = 0.00146 - 0.00112 * ratio;
    else
      lux_value = 0.0;
    lux_value *= ch0;      
  }
 
  byte clear_interrupt() {
    Wire.beginTransmission(i2c_address);
    Wire.write(cmd_clear_int);
    i2c_status =  Wire.endTransmission();
    return i2c_status;
  }
 
  byte i2c_write_reg(byte reg, byte data) {
    Wire.beginTransmission(i2c_address);
    Wire.write(reg);
    Wire.write(data);
    i2c_status =  Wire.endTransmission();
    return i2c_status;
  }
 
// read 16bit data from addr/pointer. 1st LSB, 2ns MSB.
  uint16_t i2c_read16_swab(byte pointer) {
    Wire.beginTransmission(i2c_address);
    Wire.write(pointer);
    i2c_status = Wire.endTransmission();
    Wire.requestFrom(i2c_address,  (byte)2);
    uint16_t lsb = (uint16_t)Wire.read();
    uint16_t msb = (uint16_t)(Wire.read() << 8);
    return msb | lsb;
  }
 
// read 8bit data from pointer.
  uint8_t i2c_read8(byte pointer) {
    Wire.beginTransmission(i2c_address);
    Wire.write(pointer);
    i2c_status = Wire.endTransmission();
    Wire.requestFrom(i2c_address,  (byte)1);
    return (uint8_t)Wire.read();
  }
};
