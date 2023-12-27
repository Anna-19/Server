/**
 * @file worker.h
 * @author Kalgudaeva A.E.
 * @brief Заголовочный файл определения класса Worker
 * @version 1.0
 * @date 2023-12-27
 */
#pragma once

#include <map>
#include <vector>

using namespace std;

/**
 * @brief Класс проводит вычисления с векторами
 */
class Worker {
private:
    int work_socket;          ///< Сокет

    map<string,string> base;  ///< Массив с данными клиентов <id:pass>
    /**
     * @brief Получить сообщение от клиента.
     * @throw ServerError Ошибки при получении данных.
     * @code {.cpp}
     * what = "Ошибка получения строки"
     *        "Clear bufer error"
     * @endcode
     *
     * @return string Полученное сообщение.
     */
    string recv_message();
#ifdef UNIT_TEST_TEST
public:
#endif
    /**
     * @brief Вычислить среднее арифм. по значениям из вектора.
     *
     * @param [in] v Вектор чисел.
     * @return int32_t Среднее арифм. значение.
     */
    int32_t calculate(const vector<int32_t>& v);
public:
    /**
     * @brief Конструктор.
     *
     * @param [in] sock Сокет.
     * @param [in] client_base Массив с данными клиентов <id:pass>.
     */
    Worker(int sock, const map<string,string>& client_base):
        work_socket(sock), base(client_base) {  };
    /**
     * @brief Аутентифицировать клиента.
     * @throw AuthError Ошибка при аутентификации клиента.
     * @code {.cpp}
     * what = "Неизвестный id"
     *        "Ошибка отправки salt"
     *        "Ошибка аутентификации"
     *        "Ошибка отправки OK"
     * @endcode
     */
    void authentificate();
    /**
     * @brief Обработать числовые данные клиента.
     * @throw MathError Ошибка при работе с данными клиента.
     * @code {.cpp}
     * what = "Ошибка получения количества векторов"
     *        "Ошибка получения размера вектора"
     *        "Ошибка получения вектора"
     *        "Неверный размер полученного вектора"
     *        "Ошибка отправки результата"
     * @endcode
     */
    void math();
};
