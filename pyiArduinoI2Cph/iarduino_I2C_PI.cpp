#include "iarduino_I2C_PI.h"
#include <stdexcept>

void iarduino_I2C::begin(uint32_t speed)
{
        if ((file_i2c = open(filename.c_str(), O_RDWR))<0) {
		return;
	}
	/*
	else {
		throw std::ios_base::failure("Cannot open I2C file");
	}
	*/
}

uint8_t iarduino_I2C::readByte(uint8_t adr, uint8_t reg)
{
        uint8_t i=0; readBytes(adr, reg, &i, 1);
        return i;
}

uint8_t iarduino_I2C::readByte(uint8_t adr)
{
        uint8_t i=0; readBytes(adr, &i, 1);
        return i;
}

bool iarduino_I2C::writeByte(uint8_t adr, uint8_t reg, uint8_t data)
{
        return writeBytes(adr, reg, &data, 1);
}

bool iarduino_I2C::writeByte(uint8_t adr, uint8_t data)
{
        return writeBytes(adr, &data, 1);
}

bool iarduino_I2C::readBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum)
{
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) return false;
        else if ((write(file_i2c, &reg, 1) != 1)) return false;
        usleep(10);
        if (read(file_i2c, data, sum) != sum) return false;
        else return true;
}

bool iarduino_I2C::readBytes(uint8_t addr, uint8_t *data, uint8_t sum)
{
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) return false;
        else if (read(file_i2c, data, sum) != sum) return false;
        else return true;
}

bool iarduino_I2C::writeBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum)
{
        uint8_t length = sum+1;
        uint8_t data_with_reg[length];
        for (int i=1; i < length; i++) {
                        data_with_reg[i] = data[i-1];
        }
        data_with_reg[0] = reg;
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
                return false;
        else if (write(file_i2c, data_with_reg,
                       length) != length) return false;
        else return true;
}

bool iarduino_I2C::writeBytes(uint8_t addr, uint8_t *data, uint8_t sum)
{
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) return false;
        else if (write(file_i2c, data, sum) !=sum) return false;
        else return true;
}

bool iarduino_I2C::checkAddress(uint8_t addr)
{
        if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) return false;
        else return true;
}

void iarduino_I2C::changeBus(std::string bus)
{
        filename = bus;
}
