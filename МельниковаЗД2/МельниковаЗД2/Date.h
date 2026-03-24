#pragma once                      // Защита от повторного включения заголовочного файла
#include <string>                 // Подключаем библиотеку для работы со строками (std::string)
#include <ctime>                  // Подключаем библиотеку для работы со временем (time_t, struct tm)

class Date {                      // Объявление класса для работы с датой и временем
private:                          // Начало секции private (доступно только внутри класса)
    int year;                     // Переменная для хранения года (например, 2026)
    int month;                    // Переменная для хранения месяца (от 1 до 12)
    int day;                      // Переменная для хранения дня (от 1 до 31)
    int hour;                     // Переменная для хранения часа (от 0 до 23)
    int minute;                   // Переменная для хранения минут (от 0 до 59)
    int second;                   // Переменная для хранения секунд (от 0 до 59)

public:                           // Начало секции public (доступно извне класса)
    Date();                       // Конструктор по умолчанию - создает дату с текущим системным временем
    Date(int y, int m, int d, int h = 0, int min = 0, int s = 0);    // Конструктор с параметрами (год, месяц, день, час, минуты, секунды) с значениями по умолчанию
    explicit Date(time_t time);   // Конструктор из time_t (explicit запрещает неявное преобразование типов)

    // Геттеры (методы для получения значений полей) - все объявлены inline
    int getYear() const { return year; }        // Геттер для года, const - метод не изменяет объект
    int getMonth() const { return month; }      // Геттер для месяца
    int getDay() const { return day; }          // Геттер для дня
    int getHour() const { return hour; }        // Геттер для часа
    int getMinute() const { return minute; }    // Геттер для минут
    int getSecond() const { return second; }    // Геттер для секунд

    // Операторы сравнения для работы с датами
    bool operator<(const Date& other) const;    // Оператор "меньше" - сравнивает две даты
    bool operator>(const Date& other) const;    // Оператор "больше" - сравнивает две даты
    bool operator==(const Date& other) const;   // Оператор "равно" - проверяет равенство дат
    bool operator<=(const Date& other) const;   // Оператор "меньше или равно"
    bool operator>=(const Date& other) const;   // Оператор "больше или равно"

    std::string toString() const;               // Метод преобразования даты в строку в формате "ГГГГ-ММ-ДД ЧЧ:ММ:СС"
    time_t toTimeT() const;                     // Метод преобразования даты в time_t (секунды с 1 января 1970 года)

    static bool isInRange(const Date& date, const Date& start, const Date& end);    // Статический метод проверки, находится ли дата в указанном диапазоне
};