#pragma execution_character_set("utf-8")
#include "SortComparator.h"        // Подключаем заголовочный файл класса SortComparator

bool SortComparator::compareByName(const Resource* a, const Resource* b) {    // Статический метод сравнения двух ресурсов по имени
    return a->getName() < b->getName();    // Сравниваем имена ресурсов в лексикографическом порядке, возвращаем true если имя a меньше имени b
}

bool SortComparator::compareBySize(const Resource* a, const Resource* b) {    // Статический метод сравнения двух ресурсов по размеру
    return a->getSize() < b->getSize();    // Сравниваем размеры ресурсов, возвращаем true если размер a меньше размера b
}

bool SortComparator::compareByDate(const Resource* a, const Resource* b) {    // Статический метод сравнения двух ресурсов по дате создания
    return a->getCreationDate() < b->getCreationDate();    // Сравниваем даты создания, возвращаем true если дата a меньше даты b
}

std::function<bool(const Resource*, const Resource*)> SortComparator::getComparator(SortMode mode) {    // Метод получения функции-компаратора в зависимости от режима сортировки
    switch (mode) {                // Переключаемся в зависимости от переданного режима
    case SortMode::BY_NAME:        // Если режим сортировки по имени
        return compareByName;      // Возвращаем функцию сравнения по имени
    case SortMode::BY_SIZE:        // Если режим сортировки по размеру
        return compareBySize;      // Возвращаем функцию сравнения по размеру
    case SortMode::BY_DATE:        // Если режим сортировки по дате
        return compareByDate;      // Возвращаем функцию сравнения по дате
    default:                       // Для всех остальных случаев (включая недопустимые значения)
        return compareByName;      // По умолчанию возвращаем функцию сравнения по имени
    }
}