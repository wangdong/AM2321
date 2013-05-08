// 
// AM2321 Temperature & Humidity Sensor library for Arduino
//
// The MIT License (MIT)
//
// Copyright (c) 2013 Wang Dong
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "AM2321.h"
#include <Wire.h>

#define I2C_ADDR_AM2321                 (0xB8 >> 1)          //AM2321温湿度计I2C地址
#define PARAM_AM2321_READ                0x03                //读寄存器命令
#define REG_AM2321_HUMIDITY_MSB          0x00                //湿度寄存器高位
#define REG_AM2321_Humidity_LSB          0x01                //湿度寄存器低位
#define REG_AM2321_Temperature_MSB       0x02                //温度寄存器高位
#define REG_AM2321_Temperature_LSB       0x03                //温度寄存器低位
#define REG_AM2321_DEVICE_ID_BIT_24_31   0x0B                //32位设备ID高8位


static
unsigned short crc16(unsigned char *ptr, unsigned char len) {
    unsigned short crc = 0xFFFF; 
    unsigned char  i   = 0;
    while(len--) {
        crc ^= *ptr++; 
        for(i = 0 ; i < 8 ; i++) {
            if(crc & 0x01) {
                crc >>= 1;
                crc  ^= 0xA001; 
            }
            else {
                crc >> = 1;
            } 
        }
    }
    return crc; 
}


AM2321::AM2321() {
	temperature = 0;
	humidity    = 0;
}

bool AM2321::available() {
    return !(temperature == 0 && humidity == 0);
}

bool AM2321::read() {
    enum { len = 8 };
    unsigned char buf[len] = {0};

    //唤醒
    Wire.beginTransmission(I2C_ADDR_AM2321);
    Wire.endTransmission();

    //发送读取温、湿度命令
    Wire.beginTransmission(I2C_ADDR_AM2321);
    Wire.write(PARAM_AM2321_READ);
    Wire.write(REG_AM2321_HUMIDITY_MSB);
    Wire.write(0x04);
    Wire.endTransmission();

    //等待数据准备好
    delayMicroseconds(2000); //>1.5ms

    //回传数据
    Wire.requestFrom(I2C_ADDR_AM2321, len);
    
    int i = 0;
    buf[i++] = Wire.read(); //功能码
    buf[i++] = Wire.read(); //数据长度
    buf[i++] = Wire.read(); //湿度高字节
    buf[i++] = Wire.read(); //湿度低字节
    buf[i++] = Wire.read(); //温度高字节
    buf[i++] = Wire.read(); //温度低字节

    unsigned short crc = 0;
    crc  = Wire.read();     //CRC 校验码低字节
    crc |= Wire.read() << 8;//CRC 校验码高字节

    if (crc == crc16(buf, i)) {
        humidity     = buf[2] << 8;
        humidity    += buf[3];
        temperature  = buf[4] << 8;
        temperature += buf[5];

        return true;
    }
    
    return false;
}
//
// END OF FILE
//