#ifndef TCA9548A_H
#define TCA9548A_H

#include "mbed.h"

#define TCAADDR 0x70

class TCA9548A 
{ 
public: 
/** Create BNO055 instance **/
    TCA9548A(PinName SDA, PinName SCL); 
    bool SelectCanal(int num);

private:    
    I2C _i2c;
    int currentCanal;
    char address;
    char tx[1];


};

#endif