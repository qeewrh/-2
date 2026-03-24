#pragma execution_character_set("utf-8")
#include "Resource.h"              // Подключаем заголовочный файл класса Resource
#include "Exceptions.h"            // Подключаем заголовочный файл с классами исключений
#include <regex>                   // Подключаем библиотеку для регулярных выражений (std::regex, std::regex_match)

Resource::Resource(const std::string& name, AccessLevel level)    // Конструктор базового класса Resource
    : name(name), accessLevel(level) {    // Инициализируем имя и уровень доступа переданными значениями
    if (!validateName(name)) {     // Проверяем имя на валидность с помощью статического метода validateName
        throw ValidationException("Invalid resource name: " + name);    // Если имя невалидно - выбрасываем исключение
    }
}

std::string Resource::getName() const {    // Геттер для получения имени ресурса
    return name;                   // Возвращаем имя ресурса
}

void Resource::setName(const std::string& newName) {    // Сеттер для установки имени ресурса
    if (!validateName(newName)) {  // Проверяем новое имя на валидность
        throw ValidationException("Invalid resource name: " + newName);    // Если имя невалидно - выбрасываем исключение
    }
    name = newName;                // Устанавливаем новое имя
}

Date Resource::getCreationDate() const {    // Геттер для получения даты создания ресурса
    return creationDate;           // Возвращаем дату создания
}

AccessLevel Resource::getAccessLevel() const {    // Геттер для получения уровня доступа ресурса
    return accessLevel;            // Возвращаем уровень доступа
}

void Resource::setAccessLevel(AccessLevel level) {    // Сеттер для установки уровня доступа ресурса
    accessLevel = level;           // Устанавливаем новый уровень доступа
}

bool Resource::validateName(const std::string& name) {    // Статический метод валидации имени ресурса
    std::regex pattern(R"(^[^\\/:*?\"<>|]+$)");    // Создаем регулярное выражение: имя не должно содержать запрещенные символы \ / : * ? " < > |
    return std::regex_match(name, pattern);    // Проверяем, соответствует ли имя паттерну, возвращаем true если соответствует
}