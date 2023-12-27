/**
 * @file listener.h
 * @author Kalgudaeva A.E.
 * @brief Заголовочный файл определения класса Listener
 * @version 1.0
 * @date 2023-12-27
 */
#pragma once

#include <map>

#include "logger.h"

using namespace std;

/**
 * @brief Класс для соединения с клиентом.
 */
class Listener {
private:
    int server_socket;        ///< Сокет

    map<string,string> base;  ///< Массив с данными клиентов <id:pass>
    Logger logger;            ///< Копия объекта записи в журнал.
public:
    /**
     * @brief Конструктор.
     *
     * @param [in] port Порт.
     * @param [in] queue_length Длина очереди клиентов.
     * @param [in] client_base Массив с данными клиентов <id:pass>.
     * @param [in] log Объект записи в журнал.
     *
     * @throw ServerError Ошибка создания сервера.
     * @code {.cpp}
     * what = "Ошибка создания сокета"
     *        "Ошибка настройки сокета"
     *        "Ошибка в 'bind' сокета"
     *        "Не удалось запустить 'listen'"
     * @endcode
     *
     */
    Listener(int port, int queue_length, const map<string,string>& client_base, Logger log);
    /**
     * @brief Установить сервер и запустить прослушивание.
     */
    void start_listen();
};
