#pragma once                      // Защита от повторного включения заголовочного файла
#include "Directory.h"            // Подключаем заголовочный файл класса Directory (для работы с директориями)
#include "SortComparator.h"       // Подключаем заголовочный файл класса SortComparator (для сортировки)
#include <functional>             // Подключаем библиотеку для функциональных объектов (std::function)
#include <vector>                 // Подключаем библиотеку для векторов (std::vector)
#include <fstream>                // Подключаем библиотеку для файловых потоков (std::ofstream, std::ifstream)
#include <iostream>               // Подключаем библиотеку для ввода-вывода (std::cout, std::ostream)

class Archive {                   // Объявление класса Archive - основного класса управления архивом
private:                          // Начало секции private (доступно только внутри класса)
    std::unique_ptr<Directory> root;    // Умный указатель на корневую директорию (владеет объектом Directory)
    AccessLevel currentUserLevel;       // Текущий уровень доступа пользователя (GUEST, USER, ADMIN)

    void printTreeRecursive(const Directory* dir, int depth, std::ostream& os) const;    // Рекурсивный метод вывода дерева (приватный, вызывается из printTree)
    void collectAllResources(const Directory* dir, std::vector<const Resource*>& result) const;    // Рекурсивный сбор всех ресурсов (для поиска и фильтрации)
    void collectFiles(const Directory* dir, std::vector<const File*>& result) const;    // Рекурсивный сбор всех файлов (для поиска по содержимому)

public:                           // Начало секции public (доступно извне класса)
    Archive();                    // Конструктор класса Archive (создает корневую директорию)

    // Методы управления структурой архива
    void createDirectory(const std::string& path, const std::string& name);    // Создание директории по указанному пути
    void createFile(const std::string& path, const std::string& name,          // Создание файла по указанному пути
        const std::string& extension, size_t size = 0);                        // с именем, расширением и размером
    void removeResource(const std::string& path);                              // Удаление ресурса по указанному пути
    void moveResource(const std::string& srcPath, const std::string& dstPath); // Перемещение ресурса из источника в назначение
    void copyResource(const std::string& srcPath, const std::string& dstPath); // Копирование ресурса (глубокое копирование)

    // Методы поиска и аналитики
    std::vector<const Resource*> searchByMask(const std::string& nameMask,     // Поиск ресурсов по маске имени
        const std::string& extension = "");                                    // и опционально по расширению
    std::vector<const Resource*> filterByDateRange(const Date& start, const Date& end);    // Фильтрация ресурсов по диапазону дат
    std::vector<const File*> searchByContent(const std::string& directoryPath, // Поиск по содержимому файлов
        const std::string& searchString);                                      // в указанной директории

    // Методы визуализации и экспорта
    void printTree(std::ostream& os = std::cout) const;    // Вывод структуры архива в виде дерева (по умолчанию в std::cout)
    void printStatistics() const;                          // Вывод статистики архива (количество файлов, папок, размер)
    void exportToCSV(const std::string& filename) const;   // Экспорт данных архива в CSV файл

    // Методы сериализации (сохранение и загрузка)
    void saveToFile(const std::string& filename) const;    // Сохранение архива в бинарный файл
    void loadFromFile(const std::string& filename);        // Загрузка архива из бинарного файла

    // Методы сортировки
    void setSortMode(SortMode mode);                       // Установка режима сортировки
    void sortResources(Directory* dir);                    // Сортировка ресурсов в указанной директории

    // Методы управления уровнем доступа
    void setUserLevel(AccessLevel level);                  // Установка уровня доступа пользователя
    AccessLevel getUserLevel() const;                      // Получение текущего уровня доступа пользователя

    // Метод навигации по пути
    Directory* navigateToPath(const std::string& path);    // Навигация к директории по указанному пути

    // Методы получения статистики
    size_t getTotalFileCount() const;      // Получение общего количества файлов в архиве
    size_t getTotalDirectoryCount() const; // Получение общего количества директорий в архиве
    size_t getTotalSize() const;           // Получение общего размера всех файлов в архиве
    double getAverageFileSize() const;     // Получение среднего размера файла в архиве
};