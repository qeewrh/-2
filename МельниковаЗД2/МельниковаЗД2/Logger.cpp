#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS    // Отключаем предупреждения безопасности в Visual Studio для функций localtime
#include "Logger.h"                // Подключаем заголовочный файл класса Logger
#include <chrono>                  // Подключаем библиотеку для работы со временем (std::chrono)
#include <iomanip>                 // Подключаем библиотеку для форматирования вывода (std::put_time)
#include <sstream>                 // Подключаем библиотеку для строковых потоков (std::stringstream)
#include <iostream>                // Подключаем библиотеку для ввода-вывода (std::cerr)

Logger* Logger::instance = nullptr;    // Инициализируем статический указатель на единственный экземпляр (Singleton) значением nullptr
std::mutex Logger::mutex;              // Инициализируем статический мьютекс для потокобезопасности

Logger::Logger() {                     // Приватный конструктор класса Logger (вызывается только из getInstance)
    logFile.open("history.log", std::ios::app);    // Открываем файл лога в режиме добавления (append) - записи добавляются в конец
    if (!logFile.is_open()) {          // Проверяем, успешно ли открылся файл
        std::cerr << "Warning: Cannot open history.log" << std::endl;    // Если не открылся, выводим предупреждение в консоль ошибок
    }
}

Logger::~Logger() {                    // Деструктор класса Logger
    if (logFile.is_open()) {           // Проверяем, открыт ли файл лога
        logFile.close();               // Закрываем файл лога
    }
}

Logger* Logger::getInstance() {        // Статический метод получения единственного экземпляра (Singleton)
    std::lock_guard<std::mutex> lock(mutex);    // Блокируем мьютекс для обеспечения потокобезопасности (автоматически разблокируется при выходе из области видимости)
    if (instance == nullptr) {         // Если экземпляр еще не создан
        instance = new Logger();       // Создаем новый экземпляр класса Logger
    }
    return instance;                   // Возвращаем указатель на единственный экземпляр
}

std::string Logger::getCurrentTimestamp() {    // Метод получения текущей временной метки в виде строки
    auto now = std::chrono::system_clock::now();    // Получаем текущий момент времени с использованием системных часов
    auto time = std::chrono::system_clock::to_time_t(now);    // Преобразуем время в тип time_t (секунды с 1970 года)
    struct tm tm_buf;                  // Создаем структуру для хранения разобранного времени (год, месяц, день, час, минута, секунда)
#ifdef _WIN32                          // Директива препроцессора: если компилируем под Windows
    localtime_s(&tm_buf, &time);       // Используем безопасную версию localtime для Windows (требует адрес буфера)
#else                                  // Иначе (Linux, macOS и другие Unix-подобные системы)
    localtime_r(&time, &tm_buf);       // Используем безопасную версию localtime для Unix (локальная версия)
#endif
    std::stringstream ss;              // Создаем строковый поток для построения строки с временем
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");    // Форматируем время: ГГГГ-ММ-ДД ЧЧ:ММ:СС
    return ss.str();                   // Преобразуем поток в строку и возвращаем
}

void Logger::log(const std::string& operation, bool success, const std::string& details) {    // Метод записи операции в лог
    std::lock_guard<std::mutex> lock(mutex);    // Блокируем мьютекс для потокобезопасности (автоматически разблокируется при выходе)
    if (logFile.is_open()) {           // Проверяем, открыт ли файл лога
        logFile << "[" << getCurrentTimestamp() << "] "    // Записываем временную метку в квадратных скобках
            << (success ? "[SUCCESS] " : "[ERROR] ")      // Записываем статус: SUCCESS или ERROR
            << operation << " " << details << std::endl;  // Записываем название операции и детали, затем переводим строку
        logFile.flush();               // Принудительно сбрасываем буфер и записываем данные в файл
    }
}