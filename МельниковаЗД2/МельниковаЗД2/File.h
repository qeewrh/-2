#pragma once                      // Защита от повторного включения заголовочного файла
#include "Resource.h"             // Подключаем базовый класс Resource (File наследуется от Resource)
#include <string>                 // Подключаем библиотеку для работы со строками (std::string)

class File : public Resource {    // Класс File наследуется от базового класса Resource
private:                          // Начало секции private (доступно только внутри класса)
    std::string extension;        // Расширение файла (например, "txt", "cpp", "exe")
    size_t size;                  // Размер файла в байтах
    std::string content;          // Содержимое файла (для поиска по содержимому)

public:                           // Начало секции public (доступно извне класса)
    File(const std::string& name, const std::string& ext, size_t sz = 0,    // Конструктор файла с параметрами:
        AccessLevel level = AccessLevel::USER);    // имя, расширение, размер (по умолчанию 0), уровень доступа (по умолчанию USER)

    // Геттеры и сеттеры для расширения
    std::string getExtension() const;              // Геттер для получения расширения файла
    void setExtension(const std::string& ext);     // Сеттер для установки расширения файла

    // Методы для работы с размером
    size_t getSize() const override;               // Переопределенный метод получения размера файла
    void setSize(size_t sz);                       // Сеттер для установки размера файла

    // Методы для работы с содержимым
    std::string getContent() const;                // Геттер для получения содержимого файла
    void setContent(const std::string& cont);      // Сеттер для установки содержимого файла (автоматически обновляет размер)

    // Переопределенные виртуальные методы из класса Resource
    ResourceType getType() const override;         // Возвращает тип ресурса (FILE)
    void serialize(std::ostream& os) const override;    // Сериализация файла в бинарный поток
    void deserialize(std::istream& is) override;        // Десериализация файла из бинарного потока
};