# distutils: language = c++
# cython: language_level = 3

from iarduino_I2C_pH cimport iarduino_I2C_pH
#from time import sleep

DEF_CHIP_ID_FLASH   =   0x3C                                                                    //  ID линейки чипов - константа для всех чипов серии Flash (позволяет идентифицировать принадлежность чипа к серии).
DEF_CHIP_ID_METRO   =   0xC3                                                                    //  ID линейки чипов - константа для всех чипов серии Metro (позволяет идентифицировать принадлежность чипа к серии).
DEF_MODEL_PH        =   0x1A                                                                    //  Идентификатор модели - константа.

# Адреса регистров модуля:                                                                        //
REG_FLAGS_0         =   0x00                                                                    //  Адрес регистра флагов управления для чтения.
REG_BITS_0          =   0x01                                                                    //  Адрес регистра битов  управления для чтения и записи.
REG_FLAGS_1         =   0x02                                                                    //  Адрес регистра флагов управления для чтения.
REG_BITS_1          =   0x03                                                                    //  Адрес регистра битов  управления для чтения и записи.
REG_MODEL           =   0x04                                                                    //  Адрес регистра содержащего номер типа модуля.
REG_VERSION         =   0x05                                                                    //  Адрес регистра содержащего версию прошивки.
REG_ADDRESS         =   0x06                                                                    //  Адрес регистра содержащего текущий адрес модуля на шине I2C. Если адрес указан с флагом IF-PIN-ADDRES то адрес установится (и запишется в регистр) только при наличии 1 на входе PIN_ADDRES.
REG_CHIP_ID         =   0x07                                                                    //  Адрес регистра содержащего ID линейки чипов «Flash». По данному ID можно определить принадлежность чипа к линейки «Flash».
REG_PH_KNOWN_PH     =   0x0A                                                                    //  Адрес регистра получающего известный водородный показатель 1 или 2 калибровочной жидкости «pH» в тысячных долях (два байта).
REG_PH_KNOWN_PH_1   =   0x0C                                                                    //  Адрес регистра содержащего значение которое копируется в «REG_PH_KNOWN_PH_1» при запуске 1 стадии калибровки вызванной нажатием кнопки (два байта).
REG_PH_KNOWN_PH_2   =   0x0E                                                                    //  Адрес регистра содержащего значение которое копируется в «REG_PH_KNOWN_PH_2» при запуске 2 стадии калибровки вызванной нажатием кнопки (два байта).
REG_PH_CALIBRATION  =   0x10                                                                    //  Адрес регистра содержащего биты запуска калибровки модуля.
REG_PH_Ky           =   0x11                                                                    //  Адрес регистра содержащего коэффициент усиления усилителя в тысячных долях (два байта).
REG_PH_Ustp         =   0x13                                                                    //  Адрес регистра содержащего шаг смещения напряжения датчика в сотых долях мВ соответствующий изменению кислотности на 1 pH (два байта).
REG_PH_pHn          =   0x15                                                                    //  Адрес регистра содержащего кислотность жидкости при которой отсутствует смещение напряжения датчиком (два байта).
REG_PH_Uin          =   0x17                                                                    //  Адрес регистра содержащего измеряемое напряжение на входе датчика в десятитысячных долях В (два байта).
REG_PH_Uout         =   0x19                                                                    //  Адрес регистра содержащего измеряемое напряжение на выходе усилителя в десятитысячных долях В (два байта).
REG_PH_Un           =   0x1B                                                                    //  Адрес регистра содержащего нейтральное напряжение на выходе усилителя в десятитысячных долях В (два байта). Напряжение каким оно должно быть при отсутствии смещения напряжения датчиком.
REG_PH_pH           =   0x1D                                                                    //  Адрес регистра содержащего кислотность жидкости в тысячных долях pH (два байта).

# Позиция битов и флагов:                                                                         //
PH_BIT_CALC_2       =   0x02                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 2 стадию калибровки модуля.
PH_BIT_CALC_1       =   0x01                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 1 стадию калибровки модуля.

NO_BEGIN = 1

cdef class pyiArduinoI2Cph:
    cdef iarduino_I2C_pH c_module

    def __cinit__(self, address=None, auto=None):

        if address is not None:

            self.c_module = iarduino_I2C_pH(address)

            if auto is None:
                #sleep(.5)
                if not self.c_module.begin():

                    print("ошибка инициализации модуля.\n"
                          "Проверьте подключение и адрес модуля,"
                          " возможно не включен интерфейс I2C.\n"
                          " Запустите raspi-config и включите интерфейс"
                          ", инструкция по включению:"
                          " https://wiki.iarduino.ru/page/raspberry-i2c-spi/")

        else:

            self.c_module = iarduino_I2C_pH()

            if auto is None:
                #sleep(.5)
                if not self.c_module.begin():

                    print("ошибка инициализации модуля.\n"
                          "Проверьте подключение и адрес модуля, "
                          " возможно не включен интерфейс I2C.\n"
                          " Запустите raspi-config и включите интерфейс"
                          ", инструкция по включению:"
                          " https://wiki.iarduino.ru/page/raspberry-i2c-spi/")

    def begin(self):
        return self.c_module.begin()

    def changeAddress(self, unsigned char newAddr):
        return self.c_module.changeAddress(newAddr)

    def reset(self):
        return self.c_module.reset()

    def getAddress(self):
        return self.c_module.getAddress()

    def getVersion(self):
        return self.c_module.getVersion()

    def getPullI2C(self):
        return self.c_module.getPullI2C()

    def setPullI2C(self, flag):
        if flag is None:
            return self.c_module.setPullI2C(True)
        if flag is not None:
            return self.c_module.setPullI2C(flag)


    def setCalibration(self, num, pH):
        return self.c_module.setCalibration(num, pH)

    def getKnownPH(self, num):
        return self.c_module.getKnownPH(num)

    def setKnownPH(self, num, pH):
        return self.c_module.setKnownPH(num, pH)

    def getKy(self):
        return self.c_module.getKy()

    def setKy(self, Ky):
        return self.c_module.setKy(Ky)

    def getVstp(self):
        return self.c_module.getVstp()

    def setVstp(self, Vstp):
        return self.c_module.getVstp(Vstp)

    def getVin(self):
        return self.c_module.getVin()

    def getVout(self):
        return self.c_module.getVout()

    def getVn(self):
        return self.c_module.getVn()

    def getPHn(self):
        return self.c_module.getPHn()

    def setPHn(self, PHn):
        return self.c_module.setPHn(PHn)

    def getPH(self):
        return self.c_module.getPH()
