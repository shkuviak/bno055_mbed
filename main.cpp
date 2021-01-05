	

#include "mbed.h"
#include "BNO055.h"
#include "TCA9548A.h"
#include <string> 
 
TCA9548A tca(I2C_SDA, I2C_SCL);
BNO055 imu(I2C_SDA,I2C_SCL);
DigitalOut led(LED1);
 
int main() {
    // Init - Scan BNO devices
    int availableBNO[8];
    int bnoCount = 0;

    printf("BNO055 Hello World\r\n\r\n");
    led = 1;

    // // Reset the BNO055
    // imu.reset();
    // // Check that the BNO055 is connected and flash LED if not
    // if (!imu.check())
    //     while (true){
    //         led = !led;
    //         ThisThread::sleep_for(100ms);
    //         }

    
    // Scan for BNO devices
    for(int i = 0; i < 8; i++) {
        tca.SelectCanal(i);

        ThisThread::sleep_for(100ms);
        printf("check channel %d\r\n", 1 << i);

        imu.reset();
        if(imu.check()){
            availableBNO[bnoCount] = i;
            bnoCount += 1;                 

            // Display sensor information
            printf("BNO055 found on channel %d\r\n\r\n", i);
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
        }
    }

    printf("Found %d BNO connected!\r\n", bnoCount);
  

    while (true) {
        // Prepare data to be sent to PC
        string to_send("[");
        char buffer[100];

        // Loop though all bno and collect data
        for(int i = 0; i < bnoCount; i++) {
            // Change chanel
            tca.SelectCanal(availableBNO[i]);

            // Get IMU infos on this channel
            imu.setmode(OPERATION_MODE_NDOF);
            imu.get_calib();
            imu.get_angles();
            imu.get_temp();
            imu.get_accel();

            angles ang = imu.euler;
            //ThisThread::sleep_for(10ms);

            // Format string
            sprintf(buffer, "{ \"id\": %d, \"calib_state\": %0d, \"euler_roll\": %d, \"euler_pitch\": %d, \"euler_yaw\": %d }" , availableBNO[i], imu.calib,imu.euler.rawroll,imu.euler.rawpitch,imu.euler.rawyaw);
            to_send += buffer;
            if(i+1 < bnoCount) to_send += ',';
        }

        // Send all data
        printf("%s]\r\n", to_send.c_str());
    }
}