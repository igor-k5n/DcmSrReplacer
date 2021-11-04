# DcmSrReplacer
Консольное ПО для подмены значений в sr dcm файле.

## Сборка
CMake.

C++ 17.

Зависит от DCMTK.

## Параметры командной строки
DcmSrReplacer [in dcm template file] [in txt|json file] [in dcm file] [out dcm file] [c|m]

[in dcm template file] - dcm файл. Выступает в качестве шаблона.

[in txt|json file] - json файл. Берутся значения для подмены.

[in dcm file] - dcm файл. Берутся значения для подмены основных полей исследования.

[out dcm file] - dcm файл куда будет сохранен sr.

[c|m] - режим работы. с - на вход txt файл, подмена линии. m - на вход json файл вставка измерений.
