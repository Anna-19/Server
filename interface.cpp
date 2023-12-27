#include <iostream>
#include <fstream>
#include <map>

#include "interface.h"
#include "server_errors.h"

Interface::Interface() {
    desc.add_options()
    (
        "help,h",
        "Справка"
    )
    (
        "base,b",
        po::value<string>(&base_path)->required(),
        "Имя файла с базой клиентов"
    )
    (
        "errors,e",
        po::value<string>(&log_path)->required(),
        "Имя файла журнала ошибок"
    )
    (
        "port,p",
        po::value<int>(&port)->required(),
        "Порт сервера"
    );
}

bool Interface::start_server(int argc, char* argv[]) {
    if (argc < 2) {
        desc.print(cout);
        return false;
    }

    po::store(
        po::parse_command_line(argc, argv, desc),
        vm
    );
    po::notify(vm);

    if (vm.count("help") == 1) {
        desc.print(cout);
        return false;
    }
    // Проверка порта
    if (port < 1024 or port > 65535) {
        throw ServerError("Неверно задан порт");
    }

    // Проверка файлов
    fstream stream;
    //Проверка журнала
    stream.open(log_path, ios_base::out | ios_base::app);
    if (stream.fail()) {
        throw ServerError("Ошибка открытия журнала");
    }
    stream.close();
    // Проверка базы клиентов
    stream.open(base_path, ios_base::in);
    if (stream.fail()) {
        throw ServerError("Ошибка открытия базы клиентов");
    }

    if (stream.peek() == stream.eof()) {
        throw ServerError("Пустой файл базы клиентов");
    }
    // Чтение базы клиентов
    string line;
    map<string,string> base;
    while( getline(stream, line) ) {

        size_t i = line.find_first_of(':');

        if (i == string::npos) {
            throw ServerError("Ошибка в базе клиентов");
        }

        std::string login = line.substr(0, i);
        std::string password = line.substr(i+1, line.size());

        base.insert({login, password});
    }

    stream.close();

    // Запуск
    Logger logger(log_path);
    try {
        Listener server(port, 3, base, logger);
        server.start_listen();
    } catch(const ServerError& e) {
        logger.log("[Критическая]\t"+string(e.what()));
        return false;
    }
    return true;
}