cdef extern from "iarduino_I2C_pH.cpp":
    pass

cdef extern from "iarduino_I2C_pH.h":
    cdef cppclass iarduino_I2C_pH:
        iarduino_I2C_pH() except +
        iarduino_I2C_pH(unsigned char) except +
        bint begin()
        bint changeAddress(unsigned char)
        bint reset()
        unsigned char getAddress()
        unsigned char getVersion()
