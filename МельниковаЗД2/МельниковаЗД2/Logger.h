#pragma once                      // Защита от повторного включения заголовочного файла
#include <string>                 // Подключаем библиотеку для работы со строками (std::string)
#include <fstream>                // Подключаем библиотеку для работы с файловыми потоками (std::ofstream)
#include <mutex>                  // Подключаем библиотеку для синхронизации потоков (std::mutex)

class Logger {                    // Класс для журналирования операций (реализован как Singleton - одиночка)
private:                          // Начало секции private (доступно только внутри класса)
    static Logger* instance;      // Статический указатель на единственный экземпляр класса (Singleton)
    static std::mutex mutex;      // Статический мьютекс для обеспечения потокобезопасности
    std::ofstream logFile;        // Поток для записи в файл лога
    Logger();                     // Приватный конструктор (запрещает создание объектов вне класса)
    ~Logger();                    // Приватный деструктор

public:                           // Начало секции public (доступно извне класса)
    static Logger* getInstance(); // Статический метод получения единственного экземпляра класса (Singleton)
    void log(const std::string& operation, bool success, const std::string& details = "");    // Метод записи операции в лог
    std::string getCurrentTimestamp();    // Метод получения текущей временной метки в виде строки

    Logger(const Logger&) = delete;       // Запрещаем копирование (удаляем конструктор копирования)
    Logger& operator=(const Logger&) = delete;    // Запрещаем оператор присваивания (удаляем)
};