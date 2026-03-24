#pragma once                      // Защита от повторного включения заголовочного файла
#include "Resource.h"             // Подключаем базовый класс Resource (Directory наследуется от Resource)
#include "File.h"                 // Подключаем класс File (для создания файлов при десериализации)
#include <vector>                 // Подключаем библиотеку для векторов (std::vector)
#include <memory>                 // Подключаем библиотеку для умных указателей (std::unique_ptr)
#include <ostream>                // Подключаем библиотеку для выходных потоков (std::ostream)
#include <istream>                // Подключаем библиотеку для входных потоков (std::istream)

class Directory : public Resource {    // Класс Directory наследуется от базового класса Resource
private:                          // Начало секции private (доступно только внутри класса)
    std::vector<std::unique_ptr<Resource>> children;    // Вектор умных указателей на дочерние ресурсы (файлы и поддиректории)

public:                           // Начало секции public (доступно извне класса)
    explicit Directory(const std::string& name, AccessLevel level = AccessLevel::USER);    // Конструктор с именем и уровнем доступа (explicit запрещает неявное преобразование)

    // Методы управления дочерними элементами
    void addChild(std::unique_ptr<Resource> child);    // Добавление дочернего ресурса (перемещает владение)
    std::unique_ptr<Resource> removeChild(const std::string& name);    // Удаление дочернего ресурса по имени (возвращает владение)
    Resource* findChild(const std::string& name);      // Поиск дочернего ресурса по имени (возвращает сырой указатель)
    const std::vector<std::unique_ptr<Resource>>& getChildren() const;    // Получение константной ссылки на вектор дочерних элементов (только для чтения)

    // Методы для вычисления характеристик директории
    size_t calculateTotalSize() const;    // Рекурсивный расчет общего размера всех файлов в директории
    size_t getFileCount() const;          // Рекурсивный подсчет количества файлов в директории
    size_t getDirectoryCount() const;     // Рекурсивный подсчет количества поддиректорий

    // Переопределенные виртуальные методы из класса Resource
    ResourceType getType() const override;    // Возвращает тип ресурса (DIRECTORY)
    size_t getSize() const override;          // Возвращает общий размер директории (вызывает calculateTotalSize)
    void serialize(std::ostream& os) const override;    // Сериализация директории в бинарный поток
    void deserialize(std::istream& is) override;        // Десериализация директории из бинарного потока
};