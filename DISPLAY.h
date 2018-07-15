
#ifndef display_H_
#define display_H_

#include <stdint.h>
#include "i2c2.hpp"
#include "printf_lib.h"         //debug print
#include "utilities.h"
#include "string.h"



class DISPLAY{
    public:
        DISPLAY();

        void initDisplay();

        void CMD(uint8_t cmd);
        void DATA(uint8_t data);

        void showDisplay();
        void clearscreen();


        void setledaddr();
        void buffer2display(uint8_t *buff);
        void stringtobuffer(uint8_t page, uint8_t column, char *str);



    private:
        I2C2 i2c = I2C2::getInstance();  //singleton
        int8_t I2C_dev_addr;    //device address on I2C bus
};


#endif
