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

#define CAST_UBYTE(x) (unsigned char)(x)

#define I2C_ADDR_AM2321 			0x5C  				//AM2321温湿度计I2C地址
#define Param_AM2321_Read  			CAST_UBYTE(0x03)	//读寄存器命令
#define REG_AM2321_Humidity_MSB 	CAST_UBYTE(0x00)   	//湿度寄存器高位
#define REG_AM2321_Humidity_LSB 	CAST_UBYTE(0x01)   	//湿度寄存器低位
#define REG_AM2321_Temperature_MSB 	CAST_UBYTE(0x02)  	//温度寄存器高位
#define REG_AM2321_Temperature_LSB 	CAST_UBYTE(0x03)  	//温度寄存器低位


AM2321::AM2321() {
	temperature = 0;
	humidity = 0;
}

void AM2321::read() {
    //发送读取温、湿度命令
    Wire.beginTransmission(I2C_ADDR_AM2321);
    Wire.write(Param_AM2321_Read);
    Wire.write(REG_AM2321_Humidity_MSB);
    Wire.write(4);
    Wire.endTransmission();

    //等待数据准备好
    delay(2);

    //回传数据
    Wire.requestFrom(I2C_ADDR_AM2321, 7);
    delayMicroseconds(30);  //等待30us
    Wire.read();
    Wire.read();
    humidity = (Wire.read()<<8) | Wire.read();
    temperature = (Wire.read()<<8) | Wire.read();
    Wire.read();
    Wire.endTransmission();

    //修正负温度
    if(temperature < 0)
    	temperature = -(temperature&0x7FFF);
}
//
// END OF FILE
//