#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS    // Отключаем предупреждения безопасности в Visual Studio
#include "Date.h"                  // Подключаем заголовочный файл класса Date
#include <sstream>                 // Подключаем библиотеку для строковых потоков
#include <iomanip>                 // Подключаем библиотеку для форматирования вывода

Date::Date() {                     // Конструктор по умолчанию - создает дату с текущим временем
    auto now = std::time(nullptr); // Получаем текущее время в секундах с 1970 года
    struct tm tm_buf;              // Создаем структуру для хранения разобранного времени
#ifdef _WIN32                      // Если компилируем под Windows
    localtime_s(&tm_buf, &now);    // Безопасная версия localtime для Windows
#else                              // Если компилируем под Linux/Mac
    localtime_r(&now, &tm_buf);    // Безопасная версия localtime для Unix
#endif
    year = tm_buf.tm_year + 1900;  // Устанавливаем год (tm_year - годы с 1900)
    month = tm_buf.tm_mon + 1;     // Устанавливаем месяц (tm_mon от 0 до 11)
    day = tm_buf.tm_mday;          // Устанавливаем день
    hour = tm_buf.tm_hour;         // Устанавливаем час
    minute = tm_buf.tm_min;        // Устанавливаем минуты
    second = tm_buf.tm_sec;        // Устанавливаем секунды
}

Date::Date(int y, int m, int d, int h, int min, int s)    // Конструктор с параметрами
    : year(y), month(m), day(d), hour(h), minute(min), second(s) {    // Инициализируем все поля
}

Date::Date(time_t time) {          // Конструктор из time_t (секунды с 1970 года)
    struct tm tm_buf;              // Создаем структуру для разобранного времени
#ifdef _WIN32                      // Если компилируем под Windows
    localtime_s(&tm_buf, &time);   // Безопасная версия localtime для Windows
#else                              // Если компилируем под Linux/Mac
    localtime_r(&time, &tm_buf);   // Безопасная версия localtime для Unix
#endif
    year = tm_buf.tm_year + 1900;  // Извлекаем год
    month = tm_buf.tm_mon + 1;     // Извлекаем месяц
    day = tm_buf.tm_mday;          // Извлекаем день
    hour = tm_buf.tm_hour;         // Извлекаем час
    minute = tm_buf.tm_min;        // Извлекаем минуты
    second = tm_buf.tm_sec;        // Извлекаем секунды
}

bool Date::operator<(const Date& other) const {    // Оператор меньше для сравнения двух дат
    if (year != other.year) return year < other.year;      // Сначала сравниваем года
    if (month != other.month) return month < other.month;  // Если года равны, сравниваем месяцы
    if (day != other.day) return day < other.day;          // Если месяцы равны, сравниваем дни
    if (hour != other.hour) return hour < other.hour;      // Если дни равны, сравниваем часы
    if (minute != other.minute) return minute < other.minute;  // Если часы равны, сравниваем минуты
    return second < other.second;      // Если минуты равны, сравниваем секунды
}

bool Date::operator>(const Date& other) const {    // Оператор больше
    return other < *this;              // Используем оператор меньше для определения больше
}

bool Date::operator==(const Date& other) const {    // Оператор равно
    return year == other.year && month == other.month && day == other.day &&    // Сравниваем год
        hour == other.hour && minute == other.minute && second == other.second;    // Сравниваем час, минуты, секунды
}

bool Date::operator<=(const Date& other) const {    // Оператор меньше или равно
    return *this < other || *this == other;        // Возвращаем true если меньше ИЛИ равно
}

bool Date::operator>=(const Date& other) const {    // Оператор больше или равно
    return *this > other || *this == other;        // Возвращаем true если больше ИЛИ равно
}

std::string Date::toString() const {               // Метод преобразования даты в строку
    std::stringstream ss;                          // Создаем строковый поток
    ss << std::setfill('0')                        // Устанавливаем заполнитель '0'
        << year << "-" << std::setw(2) << month << "-" << std::setw(2) << day    // Выводим год-месяц-день
        << " " << std::setw(2) << hour << ":" << std::setw(2) << minute << ":" << std::setw(2) << second;    // Выводим часы:минуты:секунды
    return ss.str();                               // Возвращаем полученную строку
}

time_t Date::toTimeT() const {                     // Метод преобразования даты в time_t
    struct tm tm = {};                             // Создаем и обнуляем структуру tm
    tm.tm_year = year - 1900;                      // Устанавливаем год (от 1900)
    tm.tm_mon = month - 1;                         // Устанавливаем месяц (от 0 до 11)
    tm.tm_mday = day;                              // Устанавливаем день
    tm.tm_hour = hour;                             // Устанавливаем час
    tm.tm_min = minute;                            // Устанавливаем минуты
    tm.tm_sec = second;                            // Устанавливаем секунды
    return std::mktime(&tm);                       // Преобразуем и возвращаем
}

bool Date::isInRange(const Date& date, const Date& start, const Date& end) {    // Проверка вхождения в диапазон
    return date >= start && date <= end;           // Возвращаем true если дата между start и end
}