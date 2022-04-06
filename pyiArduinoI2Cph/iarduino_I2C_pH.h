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
//              Позиция битов и флагов:                                                                         //
#define         PH_FLG_STATUS_2         0x80                                                                    //  Флаг  регистра REG_PH_CALIBRATION информирует о выполнении 2 стадии калибровки.
#define         PH_FLG_STATUS_1         0x40                                                                    //  Флаг  регистра REG_PH_CALIBRATION информирует о выполнении 1 стадии калибровки.
#define         PH_BIT_CALC_2           0x02                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 2 стадию калибровки модуля.
#define         PH_BIT_CALC_1           0x01                                                                    //  Бит   регистра REG_PH_CALIBRATION указывает модулю выполнить 1 стадию калибровки модуля.
                                                                                                                //
class iarduino_I2C_pH{                                                                                          //
    public:                                                                                                     //
    /** Конструктор класса **/                                                                                  //
        iarduino_I2C_pH                         (uint8_t address=0){                                            //  Конструктор класса                                                      (Параметр: адрес модуля на шине I2C, если не указан (=0), то адрес будет определён).
                                                    if(address>0x7F){ address>>=1; }                            //  Корректируем адрес, если он указан с учётом бита RW.
                            valAddrTemp         =   address;                                                    //  Сохраняем переданный адрес модуля.
                            objI2C              =   new iarduino_I2C;                                           //  Переопределяем указатель objI2C на объект производного класса iarduino_I2C.
        }                                                                                                       //
    /** Пользовательские функции **/                                                                            //
        bool                begin               (void               );                                          //  Объявляем  функцию инициализации модуля                                 (Параметр:  отсутствует).
        bool                reset               (void               );                                          //  Объявляем  функцию перезагрузки модуля                                  (Параметр:  отсутствует).
        bool                changeAddress       (uint8_t            );                                          //  Объявляем  функцию смены адреса модуля на шине I2C                      (Параметр:  новый адрес модуля).
        uint8_t             getAddress          (void               ){ return valAddr;  }                       //  Определяем функцию возвращающую текущий адрес модуля на шине I2C        (Параметр:  отсутствует).
        uint8_t             getVersion          (void               ){ return valVers;  }                       //  Определяем функцию возвращающую текущую версию прошивки модуля          (Параметр:  отсутствует).
        bool                getPullI2C          (void               );                                          //  Объявляем  функцию возвращающую флаг наличия подтяжки шины I2C          (Параметр:  отсутствует).
        bool                setPullI2C          (bool=true          );                                          //  Объявляем  функцию установки    подтяжки шины I2C                       (Параметр:  флаг подтяжки).
        bool                setCalibration      (uint8_t, float     );                                          //  Объявляем  функцию запуска      калибровки модуля                       (Параметры: стадия 1 или 2, pH от 0 до 14,000).
        uint8_t             getCalibration      (void               );                                          //  Объявляем  функцию получения    текущей стадии калибровки модуля        (Параметр:  отсутствует).
        float               getKnownPH          (uint8_t            );                                          //  Объявляем  функцию получения    pH для калибровки модуля с кнопки       (Параметр:  стадия 1 или 2).
        bool                setKnownPH          (uint8_t, float     );                                          //  Объявляем  функцию установки    pH для калибровки модуля с кнопки       (Параметр:  стадия 1 или 2, pH от 0 до 14,000).
        float               getKy               (void               );                                          //  Объявляем  функцию получения    коэффициента усиления усилителя         (Параметр:  отсутствует).
        bool                setKy               (float              );                                          //  Объявляем  функцию установки    коэффициента усиления усилителя         (Параметр:  коэффициент усиления от 1 до 65,535).
        float               getVstp             (void               );                                          //  Объявляем  функцию получения    шага смещения напряжения датчика на 1pH (Параметр:  отсутствует).
        bool                setVstp             (float              );                                          //  Объявляем  функцию установки    шага смещения напряжения датчика на 1pH (Параметр:  напряжение от 0,01 мВ до 655,35 мВ).
        float               getVin              (void               );                                          //  Объявляем  функцию получения    напряжения на входе датчика             (Параметр:  отсутствует).
        float               getVout             (void               );                                          //  Объявляем  функцию получения    напряжения на выходе усилителя          (Параметр:  отсутствует).
        float               getVn               (void               );                                          //  Объявляем  функцию получения    нейтрального напр. на выходе усилителя  (Параметр:  отсутствует).
        float               getPHn              (void               );                                          //  Объявляем  функцию получения    нейтрального pH для датчика             (Параметр:  отсутствует).
        bool                setPHn              (float              );                                          //  Объявляем  функцию установки    нейтрального pH для датчика             (Параметр:  pH от 0 до 14,000).
        float               getPH               (void               );                                          //  Объявляем  функцию получения    водородного показателя жидкости         (Параметр:  отсутствует).
    private:                                                                                                    //
    /** Внутренние переменные **/                                                                               //
        uint8_t             valAddrTemp         =   0;                                                          //  Определяем переменную для хранения адреса модуля на шине I2C который был указан, но не был проверен.
        uint8_t             valAddr             =   0;                                                          //  Определяем переменную для хранения адреса модуля на шине I2C.
        uint8_t             valVers             =   0;                                                          //  Определяем переменную для хранения версии прошивки модуля.
        uint8_t             data[4];                                                                            //  Объявляем  массив     для хранения получаемых/передаваемых данных.
        iarduino_I2C_BASE*  objI2C;                                                                             //  Объявляем  указатель  на  объект полиморфного класса iarduino_I2C_BASE, но в конструкторе данного класса этому указателю будет присвоена ссылка на производный класс iarduino_I2C.
    /** Внутренние функции **/                                                                                  //
        bool                _readBytes          (uint8_t, uint8_t               );                              //  Объявляем  функцию чтения данных в  массив  data                                (Параметры: номер первого регистра, количество байт).
        bool                _writeBytes         (uint8_t, uint8_t, uint8_t=0    );                              //  Объявляем  функцию записи данных из массива data                                (Параметры: номер первого регистра, количество байт, номер первого элемента массива data).
};                                                                                                              //
                                                                                                                //
#endif                                                                                                          //
