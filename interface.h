/**
 * @file interface.h
 * @author Kalgudaeva A.E.
 * @brief Заголовочный файл определения класса Interface
 * @version 1.0
 * @date 2023-12-27
 */
#pragma once

#include <string>

#include <boost/program_options.hpp>

#include "listener.h"
#include "logger.h"

using namespace std;
namespace po = boost::program_options;

/**
 * @brief Класс для обработки командной строки.
 */
class Interface {
private:
    po::options_description desc; ///< Описание параметров
    po::variables_map vm;         ///< Массив для значений параметров

    string base_path;             ///< Имя файла с базой клиентов
    string log_path;              ///< Имя файла с журналом
    int port;                     ///< Порт сервера
public:
    /**
     * @brief Конструктор класса
     * Создаёт описания параметров.
     */
    Interface();
    /**
     * @brief Запустить сервер.
     *
     * @param [in] argc Количество параметров ком.строки.
     * @param [in] argv Параметры ком.строки.
     * @return true Нет ошибок в параметрах.
     * @return false В параметрах обнаружены ошибки.
     */
    bool start_server(int argc, char* argv[]);
};
