#pragma once                      // Защита от повторного включения заголовочного файла
#include <string>                 // Подключаем библиотеку для работы со строками (std::string)
#include <memory>                 // Подключаем библиотеку для умных указателей (std::unique_ptr)
#include <vector>                 // Подключаем библиотеку для векторов (std::vector)
#include <ostream>                // Подключаем библиотеку для выходных потоков (std::ostream)
#include <istream>                // Подключаем библиотеку для входных потоков (std::istream)
#include "Date.h"                 // Подключаем класс Date для работы с датой и временем

enum class AccessLevel {          // Перечисление уровней доступа (enum class - типобезопасное перечисление)
    GUEST = 0,                    // Гость - минимальные права доступа
    USER = 1,                     // Пользователь - средний уровень доступа
    ADMIN = 2                     // Администратор - полный доступ ко всем ресурсам
};

enum class ResourceType {         // Перечисление типов ресурсов
    FILE,                         // Тип "Файл"
    DIRECTORY                     // Тип "Директория"
};

class Resource {                  // Базовый абстрактный класс для всех ресурсов (файлов и директорий)
protected:                        // Начало секции protected (доступно наследникам, но не извне)
    std::string name;             // Имя ресурса (файла или директории)
    Date creationDate;            // Дата и время создания ресурса
    AccessLevel accessLevel;      // Уровень доступа к ресурсу (GUEST, USER, ADMIN)

public:                           // Начало секции public (доступно извне класса)
    Resource(const std::string& name, AccessLevel level = AccessLevel::USER);    // Конструктор с именем и уровнем доступа (по умолчанию USER)
    virtual ~Resource() = default;    // Виртуальный деструктор по умолчанию (для корректного удаления через указатель на базовый класс)

    // Геттеры и сеттеры для имени
    std::string getName() const;       // Геттер для получения имени ресурса
    void setName(const std::string& newName);    // Сеттер для установки имени ресурса (с валидацией)

    // Геттеры и сеттеры для даты и уровня доступа
    Date getCreationDate() const;      // Геттер для получения даты создания
    AccessLevel getAccessLevel() const; // Геттер для получения уровня доступа
    void setAccessLevel(AccessLevel level);    // Сеттер для установки уровня доступа

    // Чисто виртуальные методы (абстрактные) - должны быть переопределены в наследниках
    virtual ResourceType getType() const = 0;    // Получение типа ресурса (FILE или DIRECTORY)
    virtual size_t getSize() const = 0;          // Получение размера ресурса (для файла - его размер, для директории - суммарный размер)
    virtual void serialize(std::ostream& os) const = 0;    // Сериализация ресурса в бинарный поток
    virtual void deserialize(std::istream& is) = 0;        // Десериализация ресурса из бинарного потока

    static bool validateName(const std::string& name);     // Статический метод валидации имени (проверка на запрещенные символы)
};