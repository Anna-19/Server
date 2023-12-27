/**
 * @file logger.h
 * @author Kalgudaeva A.E.
 * @brief Заголовочный файл определения класса Logger
 * @version 1.0
 * @date 2023-12-27
 */
#pragma once

#include <string>

using namespace std;

/**
 * @brief Класс записывает сообщения и время их получения в файл журнала.
 */
class Logger {
private:
    string path;  ///< Имя файла журнала
public:
    /**
     * @brief Конструктор с именем файла.
     *
     * @param [in] log_path Имя файла журнала.
     */
    Logger(const string& log_path): path(log_path) {  };
    /**
     * @brief Конструктор копирования.
     *
     * @param [in] other Другой объект класса Logger.
     */
    Logger(Logger& other): path(other.path) {  };
    /**
     * @brief Записать в журнал.
     *
     * @param [in] message Сообщение для записи.
     * @return true Сообщение успешно записано в файл журнала.
     * @return false Сообщение не удалось записать в файл журнала.
     */
    bool log(const string& message);
};
