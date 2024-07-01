#ifndef iarduino_I2C_h
#define iarduino_I2C_h
#define delay(A) usleep(A*1000)
#define delayMicroseconds(A) usleep(A)
#define millis() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <chrono>
#include <string>

class iarduino_I2C {
        public:
                void begin(uint32_t speed);
                uint8_t readByte(uint8_t adr, uint8_t reg);
                uint8_t readByte(uint8_t adr);
                bool writeByte(uint8_t adr, uint8_t reg, uint8_t data);
                bool writeByte(uint8_t adr, uint8_t data);
		bool readBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum);
                bool readBytes(uint8_t addr, uint8_t *data, uint8_t sum);
                bool writeBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t sum);
                bool writeBytes(uint8_t addr, uint8_t *data, uint8_t sum);
                bool checkAddress(uint8_t addr);
                void changeBus(std::string bus);

        private:
		std::string filename = "/dev/i2c-1";
                int file_i2c;
};
#endif
