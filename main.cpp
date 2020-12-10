	

#include "mbed.h"
#include "BNO055.h"
 
BNO055 imu(I2C_SDA,I2C_SCL);
DigitalOut led(LED1);
 
int main() {
    printf("BNO055 Hello World\r\n\r\n");
    led = 1;
// Reset the BNO055
    imu.reset();
// Check that the BNO055 is connected and flash LED if not
    if (!imu.check())
        while (true){
            led = !led;
            ThisThread::sleep_for(100ms);
            }
// Display sensor information
    printf("BNO055 found\r\n\r\n");
    printf("Chip          ID: %0d\r\n",imu.ID.id);
    printf("Accelerometer ID: %0d\r\n",imu.ID.accel);
    printf("Gyroscope     ID: %0d\r\n",imu.ID.gyro);
    printf("Magnetometer  ID: %0d\r\n\r\n",imu.ID.mag);
    printf("Firmware version v%d.%0d\r\n",imu.ID.sw[0],imu.ID.sw[1]);
    printf("Bootloader version v%d\r\n\r\n",imu.ID.bootload);
// Display chip serial number
    for (int i = 0; i<4; i++){
        printf("%0d.%0d.%0d.%0d\r\n",imu.ID.serial[i*4],imu.ID.serial[i*4+1],imu.ID.serial[i*4+2],imu.ID.serial[i*4+3]);
    }
    printf("\r\n");
    while (true) {
        imu.setmode(OPERATION_MODE_NDOF);
        imu.get_calib();
        imu.get_angles();
        imu.get_temp();
        imu.get_accel();

        // printf("Temp: %d\n", imu.temperature);
        angles ang = imu.euler;
        printf("{ \"id\": %d, \"calib_state\": %0d, \"euler_roll\": %d, \"euler_pitch\": %d, \"euler_yaw\": %d }\r\n",imu.ID.id, imu.calib,imu.euler.rawroll,imu.euler.rawpitch,imu.euler.rawyaw);
        // printf("{ accel_x: %d, accel_y: %d, accel_z %d }\r\n",imu.accel.rawx,imu.accel.rawy,imu.accel.rawz);
        ThisThread::sleep_for(10ms);
    }
}