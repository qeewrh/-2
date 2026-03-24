#include "Directory.h"             // Подключаем заголовочный файл класса Directory
#include "File.h"                  // Подключаем заголовочный файл класса File
#include "Exceptions.h"            // Подключаем заголовочный файл с классами исключений
#include <algorithm>               // Подключаем библиотеку алгоритмов для std::find_if
#include <ostream>                 // Подключаем библиотеку для выходных потоков
#include <istream>                 // Подключаем библиотеку для входных потоков

Directory::Directory(const std::string& name, AccessLevel level)    // Конструктор класса Directory
    : Resource(name, level) {      // Вызываем конструктор базового класса Resource
}

void Directory::addChild(std::unique_ptr<Resource> child) {    // Метод добавления дочернего ресурса
    if (findChild(child->getName()) != nullptr) {    // Проверяем, существует ли уже ресурс с таким именем
        throw ValidationException("Resource with name " + child->getName() + " already exists");    // Если существует - исключение
    }
    children.push_back(std::move(child));    // Перемещаем умный указатель в вектор
}

std::unique_ptr<Resource> Directory::removeChild(const std::string& name) {    // Метод удаления дочернего ресурса
    auto it = std::find_if(children.begin(), children.end(),    // Ищем ресурс с заданным именем
        [&name](const std::unique_ptr<Resource>& child) {    // Лямбда-функция для сравнения
            return child->getName() == name;    // Сравниваем имя ресурса с искомым
        });

    if (it != children.end()) {    // Если ресурс найден
        std::unique_ptr<Resource> removed = std::move(*it);    // Перемещаем указатель из вектора
        children.erase(it);        // Удаляем элемент из вектора
        return removed;            // Возвращаем умный указатель на удаленный ресурс
    }
    return nullptr;                // Если ресурс не найден, возвращаем nullptr
}

Resource* Directory::findChild(const std::string& name) {    // Метод поиска дочернего ресурса
    auto it = std::find_if(children.begin(), children.end(),    // Ищем ресурс с заданным именем
        [&name](const std::unique_ptr<Resource>& child) {    // Лямбда-функция для сравнения
            return child->getName() == name;    // Сравниваем имя
        });

    if (it != children.end()) {    // Если ресурс найден
        return it->get();          // Возвращаем сырой указатель
    }
    return nullptr;                // Если ресурс не найден, возвращаем nullptr
}

const std::vector<std::unique_ptr<Resource>>& Directory::getChildren() const {    // Получение списка детей
    return children;               // Возвращаем константную ссылку на вектор
}

size_t Directory::calculateTotalSize() const {    // Рекурсивный расчет общего размера
    size_t total = 0;              // Инициализируем сумму
    for (const auto& child : children) {    // Проходим по всем дочерним элементам
        if (child->getType() == ResourceType::FILE) {    // Если это файл
            total += child->getSize();    // Добавляем его размер
        }
        else {                       // Если это папка
            total += static_cast<const Directory*>(child.get())->calculateTotalSize();    // Рекурсивно вычисляем размер вложенных файлов
        }
    }
    return total;                  // Возвращаем общий размер
}

size_t Directory::getFileCount() const {    // Рекурсивный подсчет количества файлов
    size_t count = 0;              // Инициализируем счетчик
    for (const auto& child : children) {    // Проходим по всем дочерним элементам
        if (child->getType() == ResourceType::FILE) {    // Если это файл
            count++;               // Увеличиваем счетчик
        }
        else {                       // Если это папка
            count += static_cast<const Directory*>(child.get())->getFileCount();    // Рекурсивно считаем файлы в папке
        }
    }
    return count;                  // Возвращаем количество файлов
}

size_t Directory::getDirectoryCount() const {    // Рекурсивный подсчет количества папок
    size_t count = 0;              // Инициализируем счетчик
    for (const auto& child : children) {    // Проходим по всем дочерним элементам
        if (child->getType() == ResourceType::DIRECTORY) {    // Если это папка
            count++;               // Увеличиваем счетчик
            count += static_cast<const Directory*>(child.get())->getDirectoryCount();    // Рекурсивно считаем вложенные папки
        }
    }
    return count;                  // Возвращаем количество папок
}

ResourceType Directory::getType() const {    // Переопределенный метод получения типа
    return ResourceType::DIRECTORY;    // Возвращаем тип DIRECTORY
}

size_t Directory::getSize() const {    // Переопределенный метод получения размера
    return calculateTotalSize();   // Возвращаем общий размер папки
}

void Directory::serialize(std::ostream& os) const {    // Сериализация папки
    size_t nameLen = name.size();  // Получаем длину имени
    os.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));    // Записываем длину имени
    os.write(name.c_str(), nameLen);    // Записываем имя

    int access = static_cast<int>(accessLevel);    // Преобразуем уровень доступа в int
    os.write(reinterpret_cast<const char*>(&access), sizeof(access));    // Записываем уровень доступа

    time_t creation = creationDate.toTimeT();    // Преобразуем дату в time_t
    os.write(reinterpret_cast<const char*>(&creation), sizeof(creation));    // Записываем дату

    size_t childCount = children.size();    // Получаем количество дочерних элементов
    os.write(reinterpret_cast<const char*>(&childCount), sizeof(childCount));    // Записываем количество детей

    for (const auto& child : children) {    // Проходим по всем дочерним элементам
        int type = static_cast<int>(child->getType());    // Получаем тип ресурса
        os.write(reinterpret_cast<const char*>(&type), sizeof(type));    // Записываем тип
        child->serialize(os);    // Рекурсивно сериализуем дочерний ресурс
    }
}

void Directory::deserialize(std::istream& is) {    // Десериализация папки
    size_t nameLen;                // Переменная для длины имени
    is.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));    // Читаем длину имени
    name.resize(nameLen);          // Выделяем память под имя
    is.read(&name[0], nameLen);    // Читаем имя

    int access;                    // Переменная для уровня доступа
    is.read(reinterpret_cast<char*>(&access), sizeof(access));    // Читаем уровень доступа
    accessLevel = static_cast<AccessLevel>(access);    // Преобразуем обратно в enum

    time_t creation;               // Переменная для даты
    is.read(reinterpret_cast<char*>(&creation), sizeof(creation));    // Читаем дату
    creationDate = Date(creation);    // Создаем объект Date из time_t

    size_t childCount;             // Переменная для количества детей
    is.read(reinterpret_cast<char*>(&childCount), sizeof(childCount));    // Читаем количество детей

    for (size_t i = 0; i < childCount; i++) {    // Цикл для чтения каждого дочернего элемента
        int type;                  // Переменная для типа ресурса
        is.read(reinterpret_cast<char*>(&type), sizeof(type));    // Читаем тип

        std::unique_ptr<Resource> child;    // Создаем умный указатель
        if (static_cast<ResourceType>(type) == ResourceType::FILE) {    // Если это файл
            child = std::make_unique<File>("", "");    // Создаем временный файл
            child->deserialize(is);    // Десериализуем файл
        }
        else {                       // Если это папка
            child = std::make_unique<Directory>("");    // Создаем временную папку
            child->deserialize(is);    // Десериализуем папку
        }
        children.push_back(std::move(child));    // Добавляем в вектор
    }
}