#pragma execution_character_set("utf-8")
#include "Exceptions.h"    // Подключаем заголовочный файл с объявлениями классов исключений

FileSystemException::FileSystemException(const std::string& msg) : message(msg) {}    // Конструктор базового класса исключений файловой системы, инициализируем поле message переданным сообщением

const char* FileSystemException::what() const noexcept {    // Переопределенный виртуальный метод из std::exception, возвращает сообщение об ошибке
    return message.c_str();    // Преобразуем std::string в C-строку (const char*) и возвращаем указатель на неё
}

SerializationException::SerializationException(const std::string& msg)    // Конструктор исключения для ошибок сериализации/десериализации
    : FileSystemException(msg) {    // Вызываем конструктор базового класса FileSystemException, передавая ему сообщение об ошибке
}

ValidationException::ValidationException(const std::string& msg)    // Конструктор исключения для ошибок валидации (проверки корректности данных)
    : FileSystemException(msg) {    // Вызываем конструктор базового класса FileSystemException, передавая ему сообщение об ошибке
}

AccessDeniedException::AccessDeniedException(const std::string& msg)    // Конструктор исключения для ошибок доступа (недостаточно прав)
    : FileSystemException(msg) {    // Вызываем конструктор базового класса FileSystemException, передавая ему сообщение об ошибке
}