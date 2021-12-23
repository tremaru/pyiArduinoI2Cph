# ПРИМЕР НАСТРОЙКИ КАЛИБРОВКИ МОДУЛЯ КНОПКОЙ:

#   Подключаем библиотеку   для работы с pH-метром I2C-flash.
from pyiArduinoI2Cph import *

#   Объявляем объект sensor для работы с функциями и методами библиотеки pyiArduinoI2Cph, указывая адрес модуля на шине I2C.
pyiArduinoI2Cph sensor(0x09)

#   Устанавливаем водородный показатель 1 жидкости для калибровки модуля кнопкой от 0 до 14,000 pH.
sensor.setKnownPH (1, 4.00)

#   Устанавливаем водородный показатель 2 жидкости для калибровки модуля кнопкой от 0 до 14,000 pH.
sensor.setKnownPH (2, 9.18)
