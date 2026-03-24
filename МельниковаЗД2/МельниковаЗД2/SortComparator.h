#pragma once                      // Защита от повторного включения заголовочного файла
#include "Resource.h"             // Подключаем класс Resource для работы с ресурсами
#include <functional>             // Подключаем библиотеку для функциональных объектов (std::function)

enum class SortMode {             // Перечисление режимов сортировки (enum class - типобезопасное перечисление)
    BY_NAME,                      // Режим сортировки по имени (в алфавитном порядке)
    BY_SIZE,                      // Режим сортировки по размеру (от меньшего к большему)
    BY_DATE                       // Режим сортировки по дате создания (от старых к новым)
};

class SortComparator {            // Класс-компаратор для сортировки ресурсов (содержит статические методы сравнения)
public:                           // Начало секции public (доступно извне класса)
    // Статические методы сравнения двух ресурсов
    static bool compareByName(const Resource* a, const Resource* b);    // Сравнение ресурсов по имени (лексикографически)
    static bool compareBySize(const Resource* a, const Resource* b);    // Сравнение ресурсов по размеру
    static bool compareByDate(const Resource* a, const Resource* b);    // Сравнение ресурсов по дате создания

    // Метод получения функции-компаратора в зависимости от выбранного режима сортировки
    static std::function<bool(const Resource*, const Resource*)> getComparator(SortMode mode);    // Возвращает функцию сравнения для указанного режима
};