#pragma once                      // Защита от повторного включения заголовочного файла
#include <exception>              // Подключаем базовый класс std::exception для создания иерархии исключений
#include <string>                 // Подключаем библиотеку для работы со строками (std::string)

class FileSystemException : public std::exception {    // Базовый класс для всех исключений файловой системы, наследуется от std::exception
private:                          // Начало секции private (доступно только внутри класса)
    std::string message;          // Поле для хранения сообщения об ошибке

public:                           // Начало секции public (доступно извне класса)
    explicit FileSystemException(const std::string& msg);    // Конструктор с сообщением об ошибке (explicit запрещает неявное преобразование)
    const char* what() const noexcept override;    // Переопределенный виртуальный метод получения сообщения об ошибке (noexcept - не выбрасывает исключения)
};

class SerializationException : public FileSystemException {    // Класс исключения для ошибок сериализации/десериализации, наследуется от FileSystemException
public:                           // Начало секции public
    explicit SerializationException(const std::string& msg);    // Конструктор с сообщением об ошибке
};

class ValidationException : public FileSystemException {    // Класс исключения для ошибок валидации (проверки данных), наследуется от FileSystemException
public:                           // Начало секции public
    explicit ValidationException(const std::string& msg);    // Конструктор с сообщением об ошибке
};

class AccessDeniedException : public FileSystemException {    // Класс исключения для ошибок доступа (недостаточно прав), наследуется от FileSystemException
public:                           // Начало секции public
    explicit AccessDeniedException(const std::string& msg);    // Конструктор с сообщением об ошибке
};