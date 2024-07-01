//  Библиотека для работы с Trema модулем pH-метр, I2C-flash для Arduino: https://iarduino.ru/shop/Sensory-Datchiki/datchik-kislotnosti-zhidkosti-ph-metr-flash-i2c.html
//  Версия: 1.1.1
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/file/559.html
//  Подробное описание функций бибилиотеки доступно по ссылке: https://wiki.iarduino.ru/page/ph-i2c/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел.
//  Порт на Rasbperry Pi: Андрей Трошин.
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_pH_h                                                                                       //
#define iarduino_I2C_pH_h                                                                                       //
                                                                                                                //
/*
#if defined(ARDUINO) && (ARDUINO >= 100)                                                                        //
#include        <Arduino.h>                                                                                     //
#else                                                                                                           //
#include        <WProgram.h>                                                                                    //
#endif                                                                                                          //
*/
                                                                                                                //
#include        "iarduino_I2C_PI.h"                                                                             //  Подключаем файл iarduino_I2C_pH_I2C.h - для работы с шиной I2C      (используя функции структуры iI2C)
                                                                                                                //
#define         DEF_CHIP_ID_FLASH       0x3C                                                                    //  ID линейки чипов - константа для всех чипов серии Flash (позволяет идентифицировать принадлежность чипа к серии).
#define         DEF_CHIP_ID_METRO       0xC3                                                                    //  ID линейки чипов - константа для всех чипов серии Metro (позволяет идентифицировать принадлежность чипа к серии).
#define         DEF_MODEL_PH            0x1A                                                                    //  Идентификатор модели - константа.
//              Адреса регистров модуля:                                                                        //
#define         REG_FLAGS_0             0x00                                                                    //  Адрес регистра флагов управления для чтения.
#define         REG_BITS_0              0x01                                                                    //  Адрес регистра битов  управления для чтения и записи.
#define         REG_FLAGS_1             0x02                                                                    //  Адрес регистра флагов управления для чтения.
#define         REG_BITS_1              0x03                                                                    //  Адрес регистра битов  управления для чтения и записи.
#define         REG_MODEL               0x04                                                                    //  Адрес регистра содержащего номер типа модуля.
#define         REG_VERSION             0x05                                                                    //  Адрес регистра содержащего версию прошивки.
#define         REG_ADDRESS             0x06                                                                    //  Адрес регистра содержащего текущий адрес модуля на шине I2C. Если адрес указан с флагом IF-PIN-ADDRES то адрес установится (и запишется в регистр) только при наличии 1 на входе PIN_ADDRES.
#define         REG_CHIP_ID             0x07                                                                    //  Адрес регистра содержащего ID линейки чипов «Flash». По данному ID можно определить принадлежность чипа к линейки «Flash».
#define         REG_PH_KNOWN_PH         0x0A                                                                    //  Адрес регистра получающего известный водородный показатель 1 или 2 калибровочной жидкости «pH» в тысячных долях (два байта).
#define         REG_PH_KNOWN_PH_1       0x0C                                                                    //  Адрес регистра содержащего значение которое копируется в «REG_PH_KNOWN_PH_1» при запуске 1 стадии калибровки вызванной нажатием кнопки (два байта).
#define         REG_PH_KNOWN_PH_2       0x0E                                                                    //  Адрес регистра содержащего значение которое копируется в «REG_PH_KNOWN_PH_2» при запуске 2 стадии калибровки вызванной нажатием кнопки (два байта).
#define         REG_PH_CALIBRATION      0x10                                                                    //  Адрес регистра содержащего биты запуска калибровки модуля.
#define         REG_PH_Ky               0x11                                                                    //  Адрес регистра содержащего коэффициент усиления усилителя в тысячных долях (два байта).
#define         REG_PH_Ustp             0x13                                                                    //  Адрес регистра содержащего шаг смещения напряжения датчика в сотых долях мВ соответствующий изменению кислотности на 1 pH (два байта).
#define         REG_PH_pHn              0x15                                                                    //  Адрес регистра содержащего кислотность жидкости при которой отсутствует смещение напряжения датчиком (два байта).
#define         REG_PH_Uin              0x17                                                                    //  Адрес регистра содержащего измеряемое напряжение на входе датчика в десятитысячных долях В (два байта).
#define         REG_PH_Uout             0x19                                                                    //  Адрес регистра содержащего измеряемое напряжение на выходе усилителя в десятитысячных долях В (два байта).
#define         REG_PH_Un               0x1B                                                                    //  Адрес регистра содержащего нейтральное напряжение на выходе усилителя в десятитысячных долях В (два байта). Напряжение каким оно должно быть при отсутствии смещения напряжения датчиком.
#define         REG_PH_pH               0x1D                                                                    //  Адрес регистра содержащего кислотность жидкости в тысячных долях pH (два байта).
#define         REG_PH_ERROR            0x1F                                                                    //  Адрес регистра содержащего флаги ошибок.
//              Позиция битов и флагов:                                                                         //
#define         PH_FLG_STATUS_2         0x80                                                                    //  Флаг  регистра REG_PH_CALIBRATION информирует о выполнении 2 стадии калибровки.
#define         PH_FLG_STATUS_1         0x40                                                                    //  Флаг  регистра REG_PH_CALIBRATION информирует о выполнении 1 стадии калибровки.
#define         PH_BIT_CALC_2           0x02                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 2 стадию калибровки модуля.
#define         PH_BIT_CALC_1           0x01                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 1 стадию калибровки модуля.
#define         PH_CODE_CALC_SAVE       0x24                                                                    //  Код   разрешения записи калибровочных данных.
#define         PH_FLG_STAB_ERR         0x02                                                                    //  Флаг  регистра REG_PH_ERROR указывает на ненормализованные показания pH. Устанавливается если показания pH не стабильны.
#define         PH_FLG_CALC_ERR         0x01                                                                    //  Флаг  регистра REG_PH_ERROR указывает на провал калибровки. Флаг устанавливается аппаратно, если калибровка провалена, а её результат игнорируется и не сохраняется. Флаг сбрасывается аппаратно при старте калибровки.

class iarduino_I2C_pH: public iarduino_I2C {
    public:
    /** Конструктор класса **/
        iarduino_I2C_pH                         (uint8_t address=0){
                                                    if(address>0x7F){ address>>=1; }
                            valAddrTemp         =   address;
        }
    /** Пользовательские функции **/
        bool                begin               (void               );
        bool                reset               (void               );
        bool                changeAddress       (uint8_t            );
        uint8_t             getAddress          (void               ){ return valAddr;  }
        uint8_t             getVersion          (void               ){ return valVers;  }
        bool                getPullI2C          (void               );
        bool                setPullI2C          (bool=true          );
        bool                setCalibration      (uint8_t, float     );
        uint8_t             getCalibration      (void               );
        bool                getResultCalib      (void               );
        bool                getStability        (void               );
        float               getKnownPH          (uint8_t            );
        bool                setKnownPH          (uint8_t, float     );
        float               getKy               (void               );
        bool                setKy               (float              );
        float               getVstp             (void               );
        bool                setVstp             (float              );
        float               getVin              (void               );
        float               getVout             (void               );
        float               getVn               (void               );
        float               getPHn              (void               );
        bool                setPHn              (float              );
        float               getPH               (void               );
	void changeBus(std::string);
    private:
    /** Внутренние переменные **/
        uint8_t             valAddrTemp         =   0;
        uint8_t             valAddr             =   0;
        uint8_t             valVers             =   0;
        uint8_t             data[4];
    /** Внутренние функции **/
        bool                _readBytes          (uint8_t, uint8_t               );
        bool                _writeBytes         (uint8_t, uint8_t, uint8_t=0    );
};

#endif
