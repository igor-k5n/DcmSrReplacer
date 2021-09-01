# DcmSrReplacer
Консольное ПО для подмены значений в sr dcm файле.

## Сборка
CMake.

C++ 17.

Зависит от DCMTK.

## Параметры командной строки
DcmSrReplacer [in dcm template file] [in json file] [in dcm file] [out dcm file]

[in dcm template file] - dcm файл. Выступает в качестве шаблона.

[in json file] - json файл. Берутся значения для подмены линии.

[in dcm file] - dcm файл. Берутся значения для подмены основных полей исследования.

[out dcm file] - dcm файл куда будет сохранен sr.
