#pragma execution_character_set("utf-8")
#include "File.h"                  // Подключаем заголовочный файл класса File
#include "Exceptions.h"            // Подключаем заголовочный файл с классами исключений
#include <regex>                   // Подключаем библиотеку для регулярных выражений (std::regex, std::regex_match)
#include <ostream>                 // Подключаем библиотеку для выходных потоков (std::ostream)
#include <istream>                 // Подключаем библиотеку для входных потоков (std::istream)

File::File(const std::string& name, const std::string& ext, size_t sz, AccessLevel level)    // Конструктор класса File
    : Resource(name, level), extension(ext), size(sz) {    // Вызываем конструктор базового класса Resource, инициализируем расширение и размер
    std::regex extPattern(R"(^[a-zA-Z0-9]+$)");    // Создаем регулярное выражение: только буквы (латиница) и цифры, от начала до конца строки
    if (!std::regex_match(extension, extPattern)) {    // Проверяем расширение на соответствие паттерну
        throw ValidationException("Invalid file extension: " + extension);    // Если расширение невалидно - выбрасываем исключение
    }
}

std::string File::getExtension() const {    // Геттер для получения расширения файла
    return extension;    // Возвращаем расширение
}

void File::setExtension(const std::string& ext) {    // Сеттер для установки расширения файла
    std::regex extPattern(R"(^[a-zA-Z0-9]+$)");    // Создаем регулярное выражение для проверки расширения
    if (!std::regex_match(ext, extPattern)) {    // Проверяем новое расширение на валидность
        throw ValidationException("Invalid file extension: " + ext);    // Если невалидно - выбрасываем исключение
    }
    extension = ext;    // Устанавливаем новое расширение
}

size_t File::getSize() const {    // Геттер для получения размера файла
    return size;    // Возвращаем размер в байтах
}

void File::setSize(size_t sz) {    // Сеттер для установки размера файла
    size = sz;    // Устанавливаем новый размер
}

std::string File::getContent() const {    // Геттер для получения содержимого файла
    return content;    // Возвращаем содержимое в виде строки
}

void File::setContent(const std::string& cont) {    // Сеттер для установки содержимого файла
    content = cont;    // Устанавливаем новое содержимое
    size = content.size();    // Автоматически обновляем размер файла в соответствии с содержимым
}

ResourceType File::getType() const {    // Переопределенный метод получения типа ресурса
    return ResourceType::FILE;    // Возвращаем тип FILE (файл)
}

void File::serialize(std::ostream& os) const {    // Метод сериализации файла в бинарный поток
    size_t nameLen = name.size();    // Получаем длину имени файла
    os.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));    // Записываем длину имени в поток (sizeof(size_t) байт)
    os.write(name.c_str(), nameLen);    // Записываем само имя в поток (nameLen байт)

    size_t extLen = extension.size();    // Получаем длину расширения файла
    os.write(reinterpret_cast<const char*>(&extLen), sizeof(extLen));    // Записываем длину расширения в поток
    os.write(extension.c_str(), extLen);    // Записываем само расширение в поток

    os.write(reinterpret_cast<const char*>(&size), sizeof(size));    // Записываем размер файла в поток

    int access = static_cast<int>(accessLevel);    // Преобразуем уровень доступа (enum) в целое число
    os.write(reinterpret_cast<const char*>(&access), sizeof(access));    // Записываем уровень доступа в поток

    time_t creation = creationDate.toTimeT();    // Преобразуем дату создания в time_t (секунды с 1970 года)
    os.write(reinterpret_cast<const char*>(&creation), sizeof(creation));    // Записываем дату создания в поток
}

void File::deserialize(std::istream& is) {    // Метод десериализации файла из бинарного потока
    size_t nameLen;    // Переменная для хранения длины имени
    is.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));    // Читаем длину имени из потока
    name.resize(nameLen);    // Изменяем размер строки name на прочитанную длину
    is.read(&name[0], nameLen);    // Читаем само имя из потока (nameLen байт)

    size_t extLen;    // Переменная для хранения длины расширения
    is.read(reinterpret_cast<char*>(&extLen), sizeof(extLen));    // Читаем длину расширения из потока
    extension.resize(extLen);    // Изменяем размер строки extension на прочитанную длину
    is.read(&extension[0], extLen);    // Читаем само расширение из потока

    is.read(reinterpret_cast<char*>(&size), sizeof(size));    // Читаем размер файла из потока

    int access;    // Переменная для хранения уровня доступа
    is.read(reinterpret_cast<char*>(&access), sizeof(access));    // Читаем уровень доступа из потока
    accessLevel = static_cast<AccessLevel>(access);    // Преобразуем обратно в enum AccessLevel и сохраняем

    time_t creation;    // Переменная для хранения даты создания
    is.read(reinterpret_cast<char*>(&creation), sizeof(creation));    // Читаем дату создания из потока
    creationDate = Date(creation);    // Создаем объект Date из time_t
}