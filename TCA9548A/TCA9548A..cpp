#include "TCA9548A.h"
#include "mbed.h"

TCA9548A::TCA9548A(PinName SDA, PinName SCL) : _i2c(SDA,SCL){
    //Set I2C fast and bring reset line high
    _i2c.frequency(400000);
    address = TCAADDR;

    // Set default canal to canal 0
    tx[0] = 1 << 0;
    _i2c.write(address << 1,tx,1,false);
    currentCanal = 0;

    }

bool TCA9548A::SelectCanal(int num) {
    if(num > 7) return false;

    tx[0] = 1 << num;
    _i2c.write(address << 1,tx,1,false);
    currentCanal = num;

    return true;
}