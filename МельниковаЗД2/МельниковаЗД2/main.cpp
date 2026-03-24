#pragma execution_character_set("utf-8")
#include "Archive.h"               // Подключаем заголовочный файл класса Archive (основной класс системы)
#include "Logger.h"                // Подключаем заголовочный файл класса Logger (для логирования операций)
#include <iostream>                // Подключаем библиотеку для ввода-вывода (std::cout, std::cin)
#include <string>                  // Подключаем библиотеку для работы со строками (std::string)
#include <limits>                  // Подключаем библиотеку для работы с пределами типов (std::numeric_limits)

void printMenu() {                 // Функция вывода главного меню программы
    std::cout << "\n=== Virtual Archive System ===\n";    // Выводим заголовок меню
    std::cout << "1. Create Directory\n";                 // Пункт 1: создание директории
    std::cout << "2. Create File\n";                      // Пункт 2: создание файла
    std::cout << "3. Remove Resource\n";                  // Пункт 3: удаление ресурса
    std::cout << "4. Move Resource\n";                    // Пункт 4: перемещение ресурса
    std::cout << "5. Print Tree Structure\n";             // Пункт 5: вывод структуры дерева
    std::cout << "6. Print Statistics\n";                 // Пункт 6: вывод статистики
    std::cout << "7. Search by Mask\n";                   // Пункт 7: поиск по маске
    std::cout << "8. Filter by Date Range\n";             // Пункт 8: фильтрация по диапазону дат
    std::cout << "9. Save Archive\n";                     // Пункт 9: сохранение архива в файл
    std::cout << "10. Load Archive\n";                    // Пункт 10: загрузка архива из файла
    std::cout << "11. Export to CSV\n";                   // Пункт 11: экспорт в CSV
    std::cout << "12. Set User Level (0=GUEST,1=USER,2=ADMIN)\n";    // Пункт 12: установка уровня доступа
    std::cout << "0. Exit\n";                             // Пункт 0: выход из программы
    std::cout << "Choose option: ";                       // Приглашение к вводу выбора
}

void createDirectory(Archive& archive) {    // Функция создания директории
    std::string path, name;                // Переменные для пути родителя и имени новой директории
    std::cout << "Enter parent path (empty for root): ";    // Запрашиваем путь родительской директории
    std::cin.ignore();                     // Игнорируем символ новой строки, оставшийся после предыдущего ввода
    std::getline(std::cin, path);          // Считываем путь родителя (может быть пустым)
    std::cout << "Enter directory name: "; // Запрашиваем имя новой директории
    std::getline(std::cin, name);          // Считываем имя директории

    try {                                  // Начало блока try для перехвата исключений
        archive.createDirectory(path, name);    // Вызываем метод создания директории
        std::cout << "Directory created successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата любых исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void createFile(Archive& archive) {        // Функция создания файла
    std::string path, name, extension;     // Переменные для пути, имени и расширения
    size_t size;                           // Переменная для размера файла

    std::cout << "Enter parent path (empty for root): ";    // Запрашиваем путь родительской директории
    std::cin.ignore();                     // Игнорируем символ новой строки
    std::getline(std::cin, path);          // Считываем путь родителя
    std::cout << "Enter file name: ";       // Запрашиваем имя файла
    std::getline(std::cin, name);          // Считываем имя файла
    std::cout << "Enter extension: ";       // Запрашиваем расширение файла
    std::getline(std::cin, extension);     // Считываем расширение
    std::cout << "Enter size (bytes): ";    // Запрашиваем размер файла
    std::cin >> size;                      // Считываем размер

    try {                                  // Начало блока try для перехвата исключений
        archive.createFile(path, name, extension, size);    // Вызываем метод создания файла
        std::cout << "File created successfully!\n";        // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void removeResource(Archive& archive) {    // Функция удаления ресурса
    std::string path;                      // Переменная для пути к удаляемому ресурсу
    std::cout << "Enter path to resource: ";    // Запрашиваем путь к ресурсу
    std::cin.ignore();                     // Игнорируем символ новой строки
    std::getline(std::cin, path);          // Считываем путь

    try {                                  // Начало блока try для перехвата исключений
        archive.removeResource(path);      // Вызываем метод удаления ресурса
        std::cout << "Resource removed successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void moveResource(Archive& archive) {      // Функция перемещения ресурса
    std::string src, dst;                  // Переменные для исходного и целевого пути
    std::cout << "Enter source path: ";     // Запрашиваем исходный путь
    std::cin.ignore();                     // Игнорируем символ новой строки
    std::getline(std::cin, src);           // Считываем исходный путь
    std::cout << "Enter destination path: ";    // Запрашиваем целевой путь
    std::getline(std::cin, dst);           // Считываем целевой путь

    try {                                  // Начало блока try для перехвата исключений
        archive.moveResource(src, dst);    // Вызываем метод перемещения ресурса
        std::cout << "Resource moved successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void searchByMask(Archive& archive) {      // Функция поиска по маске
    std::string mask, extension;           // Переменные для маски имени и расширения
    std::cout << "Enter name mask (regex pattern): ";    // Запрашиваем маску имени (регулярное выражение)
    std::cin.ignore();                     // Игнорируем символ новой строки
    std::getline(std::cin, mask);          // Считываем маску
    std::cout << "Enter extension (empty for any): ";    // Запрашиваем расширение (можно оставить пустым)
    std::getline(std::cin, extension);     // Считываем расширение

    try {                                  // Начало блока try для перехвата исключений
        auto results = archive.searchByMask(mask, extension);    // Вызываем метод поиска, получаем вектор результатов
        std::cout << "\nFound " << results.size() << " resources:\n";    // Выводим количество найденных ресурсов
        for (const auto* res : results) {  // Проходим по всем найденным ресурсам
            std::cout << "  - " << res->getName() << " [Created: "    // Выводим имя ресурса
                << res->getCreationDate().toString() << "]\n";        // Выводим дату создания
        }
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void filterByDate(Archive& archive) {      // Функция фильтрации по диапазону дат
    int y1, m1, d1, y2, m2, d2;           // Переменные для года, месяца, дня начала и конца диапазона
    std::cout << "Enter start date (YYYY MM DD): ";    // Запрашиваем начальную дату
    std::cin >> y1 >> m1 >> d1;            // Считываем год, месяц, день начала
    std::cout << "Enter end date (YYYY MM DD): ";      // Запрашиваем конечную дату
    std::cin >> y2 >> m2 >> d2;            // Считываем год, месяц, день конца

    try {                                  // Начало блока try для перехвата исключений
        Date start(y1, m1, d1);            // Создаем объект Date для начальной даты
        Date end(y2, m2, d2);              // Создаем объект Date для конечной даты
        auto results = archive.filterByDateRange(start, end);    // Вызываем метод фильтрации

        std::cout << "\nFound " << results.size() << " resources in date range:\n";    // Выводим количество найденных
        for (const auto* res : results) {  // Проходим по всем найденным ресурсам
            std::cout << "  - " << res->getName() << " [Created: "    // Выводим имя ресурса
                << res->getCreationDate().toString() << "]\n";        // Выводим дату создания
        }
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void saveArchive(Archive& archive) {       // Функция сохранения архива
    std::string filename;                  // Переменная для имени файла
    std::cout << "Enter filename to save: ";    // Запрашиваем имя файла для сохранения
    std::cin >> filename;                  // Считываем имя файла

    try {                                  // Начало блока try для перехвата исключений
        archive.saveToFile(filename);      // Вызываем метод сохранения архива
        std::cout << "Archive saved successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void loadArchive(Archive& archive) {       // Функция загрузки архива
    std::string filename;                  // Переменная для имени файла
    std::cout << "Enter filename to load: ";    // Запрашиваем имя файла для загрузки
    std::cin >> filename;                  // Считываем имя файла

    try {                                  // Начало блока try для перехвата исключений
        archive.loadFromFile(filename);    // Вызываем метод загрузки архива
        std::cout << "Archive loaded successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void exportToCSV(Archive& archive) {       // Функция экспорта в CSV
    std::string filename;                  // Переменная для имени файла
    std::cout << "Enter CSV filename: ";    // Запрашиваем имя CSV файла
    std::cin >> filename;                  // Считываем имя файла

    try {                                  // Начало блока try для перехвата исключений
        archive.exportToCSV(filename);     // Вызываем метод экспорта в CSV
        std::cout << "Data exported to " << filename << " successfully!\n";    // Сообщение об успехе
    }
    catch (const std::exception& e) {      // Блок catch для перехвата исключений
        std::cout << "Error: " << e.what() << "\n";    // Выводим сообщение об ошибке
    }
}

void setUserLevel(Archive& archive) {      // Функция установки уровня доступа пользователя
    int level;                             // Переменная для уровня доступа
    std::cout << "Select user level (0=GUEST,1=USER,2=ADMIN): ";    // Запрашиваем уровень доступа
    std::cin >> level;                     // Считываем уровень

    if (level >= 0 && level <= 2) {        // Проверяем, что уровень в допустимом диапазоне
        archive.setUserLevel(static_cast<AccessLevel>(level));    // Устанавливаем уровень доступа
        std::cout << "User level set to " << level << "\n";    // Сообщение об успехе
    }
    else {                                 // Если уровень вне диапазона
        std::cout << "Invalid level!\n";   // Выводим сообщение об ошибке
    }
}

int main() {                               // Главная функция - точка входа в программу
    Archive archive;                       // Создаем объект архива
    int choice;                            // Переменная для хранения выбора пользователя

    std::cout << "Virtual Archive System\n";    // Выводим название программы
    std::cout << "=======================\n";   // Выводим разделитель
    std::cout << "Note: All operations are logged to history.log\n\n";    // Сообщаем о логировании

    while (true) {                         // Бесконечный цикл для работы программы
        printMenu();                       // Выводим меню
        std::cin >> choice;                // Считываем выбор пользователя

        if (std::cin.fail()) {             // Если произошла ошибка ввода (введен не число)
            std::cin.clear();              // Сбрасываем флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // Очищаем буфер ввода
            std::cout << "Invalid input. Please enter a number.\n";    // Выводим сообщение об ошибке
            continue;                      // Переходим к следующей итерации цикла
        }

        switch (choice) {                  // Обрабатываем выбор пользователя
        case 1:                            // Если выбрано 1 - создание директории
            createDirectory(archive);      // Вызываем функцию создания директории
            break;                         // Выходим из switch
        case 2:                            // Если выбрано 2 - создание файла
            createFile(archive);           // Вызываем функцию создания файла
            break;
        case 3:                            // Если выбрано 3 - удаление ресурса
            removeResource(archive);       // Вызываем функцию удаления ресурса
            break;
        case 4:                            // Если выбрано 4 - перемещение ресурса
            moveResource(archive);         // Вызываем функцию перемещения ресурса
            break;
        case 5:                            // Если выбрано 5 - вывод структуры дерева
            archive.printTree();           // Вызываем метод вывода дерева
            break;
        case 6:                            // Если выбрано 6 - вывод статистики
            archive.printStatistics();     // Вызываем метод вывода статистики
            break;
        case 7:                            // Если выбрано 7 - поиск по маске
            searchByMask(archive);         // Вызываем функцию поиска по маске
            break;
        case 8:                            // Если выбрано 8 - фильтрация по дате
            filterByDate(archive);         // Вызываем функцию фильтрации по дате
            break;
        case 9:                            // Если выбрано 9 - сохранение архива
            saveArchive(archive);          // Вызываем функцию сохранения
            break;
        case 10:                           // Если выбрано 10 - загрузка архива
            loadArchive(archive);          // Вызываем функцию загрузки
            break;
        case 11:                           // Если выбрано 11 - экспорт в CSV
            exportToCSV(archive);          // Вызываем функцию экспорта в CSV
            break;
        case 12:                           // Если выбрано 12 - установка уровня доступа
            setUserLevel(archive);         // Вызываем функцию установки уровня
            break;
        case 0:                            // Если выбрано 0 - выход
            std::cout << "Goodbye!\n";     // Выводим прощальное сообщение
            return 0;                      // Завершаем программу с кодом 0 (успех)
        default:                           // Если выбран несуществующий пункт
            std::cout << "Invalid option. Please try again.\n";    // Сообщаем об ошибке
        }
    }

    return 0;                              // Завершаем программу (никогда не выполнится из-за бесконечного цикла)
}