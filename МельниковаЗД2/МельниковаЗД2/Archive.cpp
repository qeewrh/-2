#pragma execution_character_set("utf-8")
#include "Archive.h"           // Подключаем заголовочный файл класса Archive
#include "File.h"              // Подключаем заголовочный файл класса File
#include "Logger.h"            // Подключаем заголовочный файл класса Logger (синглтон для логирования)
#include "Exceptions.h"        // Подключаем заголовочный файл с классами исключений
#include <sstream>             // Подключаем библиотеку для строковых потоков (std::stringstream)
#include <iomanip>             // Подключаем библиотеку для форматирования вывода (std::setprecision, std::fixed)
#include <algorithm>           // Подключаем библиотеку алгоритмов (для std::find_if и др.)
#include <fstream>             // Подключаем библиотеку для работы с файлами (std::ofstream, std::ifstream)
#include <regex>               // Подключаем библиотеку для регулярных выражений (std::regex)
#include <iostream>            // Подключаем библиотеку для ввода-вывода (std::cout)

Archive::Archive() : currentUserLevel(AccessLevel::USER) {    // Конструктор класса Archive, инициализируем уровень доступа пользователя как USER
    root = std::make_unique<Directory>("root", AccessLevel::ADMIN);    // Создаем корневую директорию с именем "root" и уровнем доступа ADMIN
}

void Archive::setUserLevel(AccessLevel level) {    // Метод установки уровня доступа пользователя
    currentUserLevel = level;    // Сохраняем новый уровень доступа
    Logger::getInstance()->log("Set user level", true, "Level: " + std::to_string(static_cast<int>(level)));    // Логируем операцию смены уровня
}

AccessLevel Archive::getUserLevel() const {    // Метод получения текущего уровня доступа пользователя
    return currentUserLevel;    // Возвращаем сохраненный уровень доступа
}

Directory* Archive::navigateToPath(const std::string& path) {    // Метод навигации по пути к директории
    if (path.empty() || path == "/" || path == "root") {    // Если путь пустой, корневой или "root"
        return root.get();    // Возвращаем указатель на корневую директорию
    }

    Directory* current = root.get();    // Начинаем с корневой директории
    std::stringstream ss(path);    // Создаем строковый поток из пути
    std::string segment;    // Переменная для хранения сегмента пути

    while (std::getline(ss, segment, '/')) {    // Разбиваем путь по разделителю '/'
        if (segment.empty()) continue;    // Пропускаем пустые сегменты

        Resource* child = current->findChild(segment);    // Ищем дочерний ресурс с именем сегмента
        if (!child || child->getType() != ResourceType::DIRECTORY) {    // Если ресурс не найден или это не директория
            throw FileSystemException("Path not found: " + path);    // Выбрасываем исключение о ненайденном пути
        }
        current = static_cast<Directory*>(child);    // Переходим к найденной директории
    }
    return current;    // Возвращаем указатель на найденную директорию
}

void Archive::createDirectory(const std::string& path, const std::string& name) {    // Метод создания директории
    try {    // Начало блока try для перехвата исключений
        Directory* parent = navigateToPath(path);    // Находим родительскую директорию

        if (parent->findChild(name)) {    // Проверяем, существует ли уже ресурс с таким именем
            throw ValidationException("Directory already exists: " + name);    // Если существует - выбрасываем исключение
        }

        auto newDir = std::make_unique<Directory>(name);    // Создаем умный указатель на новую директорию
        parent->addChild(std::move(newDir));    // Перемещаем новую директорию в родительскую

        Logger::getInstance()->log("Create directory", true, path + "/" + name);    // Логируем успешное создание
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Create directory", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::createFile(const std::string& path, const std::string& name,
    const std::string& extension, size_t size) {    // Метод создания файла
    try {    // Начало блока try для перехвата исключений
        Directory* parent = navigateToPath(path);    // Находим родительскую директорию
        std::string fullName = name + "." + extension;    // Формируем полное имя файла (имя.расширение)

        if (parent->findChild(fullName)) {    // Проверяем, существует ли уже файл с таким именем
            throw ValidationException("File already exists: " + fullName);    // Если существует - выбрасываем исключение
        }

        auto newFile = std::make_unique<File>(fullName, extension, size);    // Создаем умный указатель на новый файл
        parent->addChild(std::move(newFile));    // Перемещаем новый файл в родительскую директорию

        Logger::getInstance()->log("Create file", true, path + "/" + fullName);    // Логируем успешное создание
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Create file", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::removeResource(const std::string& path) {    // Метод удаления ресурса
    try {    // Начало блока try для перехвата исключений
        size_t lastSlash = path.find_last_of('/');    // Находим позицию последнего слеша в пути
        std::string parentPath = (lastSlash == std::string::npos) ? "" : path.substr(0, lastSlash);    // Извлекаем путь родителя (если слеша нет - пустая строка)
        std::string resourceName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);    // Извлекаем имя ресурса (после последнего слеша)

        Directory* parent = navigateToPath(parentPath);    // Находим родительскую директорию
        Resource* resource = parent->findChild(resourceName);    // Ищем ресурс по имени

        if (!resource) {    // Если ресурс не найден
            throw FileSystemException("Resource not found: " + path);    // Выбрасываем исключение
        }

        if (currentUserLevel < resource->getAccessLevel()) {    // Проверяем, достаточно ли прав у пользователя
            throw AccessDeniedException("Insufficient permissions to delete: " + path);    // Если прав недостаточно - исключение
        }

        parent->removeChild(resourceName);    // Удаляем ресурс из родительской директории

        Logger::getInstance()->log("Remove resource", true, path);    // Логируем успешное удаление
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Remove resource", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::moveResource(const std::string& srcPath, const std::string& dstPath) {    // Метод перемещения ресурса
    try {    // Начало блока try для перехвата исключений
        size_t lastSlash = srcPath.find_last_of('/');    // Находим позицию последнего слеша в исходном пути
        std::string srcParentPath = (lastSlash == std::string::npos) ? "" : srcPath.substr(0, lastSlash);    // Извлекаем путь родителя источника
        std::string resourceName = (lastSlash == std::string::npos) ? srcPath : srcPath.substr(lastSlash + 1);    // Извлекаем имя перемещаемого ресурса

        Directory* srcParent = navigateToPath(srcParentPath);    // Находим родительскую директорию источника
        std::unique_ptr<Resource> resource = srcParent->removeChild(resourceName);    // Удаляем ресурс из родителя и получаем владение

        if (!resource) {    // Если ресурс не найден
            throw FileSystemException("Resource not found: " + srcPath);    // Выбрасываем исключение
        }

        Directory* dstParent = navigateToPath(dstPath);    // Находим целевую директорию

        if (dstParent->findChild(resource->getName())) {    // Проверяем, нет ли уже ресурса с таким именем в цели
            throw ValidationException("Resource already exists at destination: " + resource->getName());    // Если есть - исключение
        }

        dstParent->addChild(std::move(resource));    // Перемещаем ресурс в целевую директорию

        Logger::getInstance()->log("Move resource", true, srcPath + " -> " + dstPath);    // Логируем успешное перемещение
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Move resource", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::copyResource(const std::string& srcPath, const std::string& dstPath) {    // Метод копирования ресурса
    Logger::getInstance()->log("Copy resource", true, srcPath + " -> " + dstPath);    // Логируем операцию копирования
}

void Archive::printTree(std::ostream& os) const {    // Метод печати дерева архива
    os << "\nArchive Structure:\n";    // Выводим заголовок
    os << "==================\n";    // Выводим разделитель
    printTreeRecursive(root.get(), 0, os);    // Рекурсивно выводим структуру начиная с корня
}

void Archive::printTreeRecursive(const Directory* dir, int depth, std::ostream& os) const {    // Рекурсивный метод печати дерева
    std::string indent(depth * 2, ' ');    // Создаем строку отступов (2 пробела на уровень глубины)

    for (const auto& child : dir->getChildren()) {    // Проходим по всем дочерним элементам
        os << indent << "├── ";    // Выводим отступ и символ ветки

        if (child->getType() == ResourceType::FILE) {    // Если дочерний элемент - файл
            const File* file = static_cast<const File*>(child.get());    // Приводим к типу File
            os << "[FILE] " << child->getName()    // Выводим метку [FILE] и имя
                << " (" << file->getSize() << " bytes)";    // Выводим размер в байтах
        }
        else {    // Если дочерний элемент - директория
            os << "[DIR] " << child->getName();    // Выводим метку [DIR] и имя
        }
        os << " [Created: " << child->getCreationDate().toString() << "]";    // Выводим дату создания
        os << " [Access: " << static_cast<int>(child->getAccessLevel()) << "]";    // Выводим уровень доступа
        os << "\n";    // Переход на новую строку

        if (child->getType() == ResourceType::DIRECTORY) {    // Если это директория
            printTreeRecursive(static_cast<const Directory*>(child.get()), depth + 1, os);    // Рекурсивно выводим её содержимое
        }
    }
}

void Archive::printStatistics() const {    // Метод вывода статистики
    std::cout << "\n=== Archive Statistics ===\n";    // Выводим заголовок статистики
    std::cout << "Total files: " << getTotalFileCount() << "\n";    // Выводим общее количество файлов
    std::cout << "Total directories: " << getTotalDirectoryCount() << "\n";    // Выводим общее количество директорий
    std::cout << "Total size: " << getTotalSize() << " bytes\n";    // Выводим общий размер
    std::cout << "Average file size: " << std::fixed << std::setprecision(2)    // Форматируем вывод с 2 знаками после запятой
        << getAverageFileSize() << " bytes\n";    // Выводим средний размер файла
}

size_t Archive::getTotalFileCount() const {    // Метод получения общего количества файлов
    return root->getFileCount();    // Возвращаем количество файлов в корневой директории
}

size_t Archive::getTotalDirectoryCount() const {    // Метод получения общего количества директорий
    return root->getDirectoryCount();    // Возвращаем количество директорий в корневой директории
}

size_t Archive::getTotalSize() const {    // Метод получения общего размера
    return root->calculateTotalSize();    // Возвращаем общий размер всех файлов
}

double Archive::getAverageFileSize() const {    // Метод получения среднего размера файла
    size_t fileCount = getTotalFileCount();    // Получаем общее количество файлов
    if (fileCount == 0) return 0.0;    // Если файлов нет, возвращаем 0
    return static_cast<double>(getTotalSize()) / fileCount;    // Делим общий размер на количество файлов
}

void Archive::saveToFile(const std::string& filename) const {    // Метод сохранения архива в файл
    try {    // Начало блока try для перехвата исключений
        std::ofstream file(filename, std::ios::binary);    // Открываем файл в бинарном режиме для записи
        if (!file.is_open()) {    // Если файл не открылся
            throw FileSystemException("Cannot open file for writing: " + filename);    // Выбрасываем исключение
        }

        uint32_t magic = 0x41524348;    // Магическое число "ARCH" в hex (0x41='A', 0x52='R', 0x43='C', 0x48='H')
        file.write(reinterpret_cast<const char*>(&magic), sizeof(magic));    // Записываем магическое число для проверки целостности

        int version = 1;    // Версия формата файла
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));    // Записываем версию

        root->serialize(file);    // Сериализуем корневую директорию (рекурсивно все содержимое)

        file.close();    // Закрываем файл
        Logger::getInstance()->log("Save archive", true, filename);    // Логируем успешное сохранение
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Save archive", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::loadFromFile(const std::string& filename) {    // Метод загрузки архива из файла
    try {    // Начало блока try для перехвата исключений
        std::ifstream file(filename, std::ios::binary);    // Открываем файл в бинарном режиме для чтения
        if (!file.is_open()) {    // Если файл не открылся
            throw FileSystemException("Cannot open file for reading: " + filename);    // Выбрасываем исключение
        }

        uint32_t magic;    // Переменная для хранения магического числа
        file.read(reinterpret_cast<char*>(&magic), sizeof(magic));    // Читаем магическое число из файла

        if (magic != 0x41524348) {    // Проверяем, совпадает ли магическое число с ожидаемым
            throw SerializationException("Invalid magic number. File may be corrupted.");    // Если нет - файл поврежден
        }

        int version;    // Переменная для хранения версии
        file.read(reinterpret_cast<char*>(&version), sizeof(version));    // Читаем версию из файла

        if (version != 1) {    // Проверяем, поддерживается ли эта версия
            throw SerializationException("Unsupported file version: " + std::to_string(version));    // Если нет - исключение
        }

        auto newRoot = std::make_unique<Directory>("root");    // Создаем новую корневую директорию
        newRoot->deserialize(file);    // Десериализуем данные из файла в новую корневую директорию

        root = std::move(newRoot);    // Заменяем старую корневую директорию новой (перемещаем)

        file.close();    // Закрываем файл
        Logger::getInstance()->log("Load archive", true, filename);    // Логируем успешную загрузку
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Load archive", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::exportToCSV(const std::string& filename) const {    // Метод экспорта данных в CSV файл
    try {    // Начало блока try для перехвата исключений
        std::ofstream file(filename);    // Открываем файл для записи (текстовый режим)
        if (!file.is_open()) {    // Если файл не открылся
            throw FileSystemException("Cannot open CSV file: " + filename);    // Выбрасываем исключение
        }

        file << "Type,Name,Size,Bytes,Creation Date,Access Level\n";    // Записываем заголовки столбцов CSV

        std::vector<const Resource*> allResources;    // Вектор для хранения всех ресурсов
        collectAllResources(root.get(), allResources);    // Собираем все ресурсы из архива

        for (const auto* res : allResources) {    // Проходим по всем ресурсам
            std::string type = (res->getType() == ResourceType::FILE) ? "File" : "Directory";    // Определяем тип ресурса
            std::string size = (res->getType() == ResourceType::FILE)    // Если файл - выводим размер
                ? std::to_string(res->getSize()) : "-";    // Если директория - ставим прочерк

            file << type << ","    // Записываем тип
                << "\"" << res->getName() << "\","    // Записываем имя в кавычках
                << size << ","    // Записываем размер
                << res->getSize() << ","    // Записываем размер в байтах
                << res->getCreationDate().toString() << ","    // Записываем дату создания
                << static_cast<int>(res->getAccessLevel()) << "\n";    // Записываем уровень доступа
        }

        file.close();    // Закрываем файл
        Logger::getInstance()->log("Export to CSV", true, filename);    // Логируем успешный экспорт
    }
    catch (const std::exception& e) {    // Блок catch для перехвата любых исключений
        Logger::getInstance()->log("Export to CSV", false, e.what());    // Логируем ошибку
        throw;    // Пробрасываем исключение дальше
    }
}

void Archive::collectAllResources(const Directory* dir, std::vector<const Resource*>& result) const {    // Рекурсивный сбор всех ресурсов
    for (const auto& child : dir->getChildren()) {    // Проходим по всем дочерним элементам
        result.push_back(child.get());    // Добавляем текущий ресурс в вектор
        if (child->getType() == ResourceType::DIRECTORY) {    // Если это директория
            collectAllResources(static_cast<const Directory*>(child.get()), result);    // Рекурсивно собираем её содержимое
        }
    }
}

std::vector<const Resource*> Archive::searchByMask(const std::string& nameMask,
    const std::string& extension) {    // Метод поиска ресурсов по маске имени и расширению
    std::vector<const Resource*> results;    // Вектор для результатов поиска
    std::vector<const Resource*> allResources;    // Вектор для всех ресурсов
    collectAllResources(root.get(), allResources);    // Собираем все ресурсы

    std::regex namePattern(nameMask);    // Компилируем регулярное выражение из маски имени

    for (const auto* res : allResources) {    // Проходим по всем ресурсам
        if (std::regex_search(res->getName(), namePattern)) {    // Проверяем, соответствует ли имя маске
            if (extension.empty()) {    // Если расширение не указано
                results.push_back(res);    // Добавляем ресурс в результаты
            }
            else if (res->getType() == ResourceType::FILE) {    // Если ресурс - файл
                const File* file = static_cast<const File*>(res);    // Приводим к типу File
                if (file->getExtension() == extension) {    // Проверяем расширение файла
                    results.push_back(res);    // Добавляем в результаты
                }
            }
        }
    }

    return results;    // Возвращаем вектор результатов
}

std::vector<const Resource*> Archive::filterByDateRange(const Date& start, const Date& end) {    // Метод фильтрации по диапазону дат
    std::vector<const Resource*> results;    // Вектор для результатов
    std::vector<const Resource*> allResources;    // Вектор для всех ресурсов
    collectAllResources(root.get(), allResources);    // Собираем все ресурсы

    for (const auto* res : allResources) {    // Проходим по всем ресурсам
        if (Date::isInRange(res->getCreationDate(), start, end)) {    // Проверяем, попадает ли дата создания в диапазон
            results.push_back(res);    // Добавляем ресурс в результаты
        }
    }

    return results;    // Возвращаем вектор результатов
}

std::vector<const File*> Archive::searchByContent(const std::string& directoryPath,
    const std::string& searchString) {    // Метод поиска по содержимому файлов
    std::vector<const File*> results;    // Вектор для результатов
    Directory* dir = navigateToPath(directoryPath);    // Находим директорию для поиска

    std::vector<const File*> files;    // Вектор для файлов
    collectFiles(dir, files);    // Собираем все файлы в директории и поддиректориях

    for (const auto* file : files) {    // Проходим по всем файлам
        if (file->getContent().find(searchString) != std::string::npos) {    // Ищем строку в содержимом файла
            results.push_back(file);    // Если найдено, добавляем файл в результаты
        }
    }

    return results;    // Возвращаем вектор результатов
}

void Archive::collectFiles(const Directory* dir, std::vector<const File*>& result) const {    // Рекурсивный сбор всех файлов
    for (const auto& child : dir->getChildren()) {    // Проходим по всем дочерним элементам
        if (child->getType() == ResourceType::FILE) {    // Если это файл
            result.push_back(static_cast<const File*>(child.get()));    // Добавляем его в вектор
        }
        else {    // Если это директория
            collectFiles(static_cast<const Directory*>(child.get()), result);    // Рекурсивно собираем файлы в ней
        }
    }
}

void Archive::setSortMode(SortMode mode) {    // Метод установки режима сортировки
    sortResources(root.get());    // Сортируем ресурсы в корневой директории
}

void Archive::sortResources(Directory* dir) {    // Метод сортировки ресурсов в директории
    auto& children = const_cast<std::vector<std::unique_ptr<Resource>>&>(dir->getChildren());    // Получаем ссылку на вектор детей (снимаем const)
    // Здесь будет реализация сортировки с использованием функторов из SortComparator
}