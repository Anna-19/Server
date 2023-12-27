#include <memory>
#include <iostream>

#include <netinet/in.h>
#include <unistd.h>

#include "worker.h"
#include "server_errors.h"
#include "listener.h"

Listener::Listener(int port,
                   int queue_length,
                   const map<string,string>& client_base,
                   Logger log): server_socket(socket(AF_INET, SOCK_STREAM, 0)),
    base(client_base),
    logger(log) {
    if( server_socket == -1 ) {
        throw ServerError("Ошибка создания сокета");
    }

    const int enable = 1;

    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof enable) == -1 ) {
        throw ServerError("Ошибка настройки сокета");
    }

    unique_ptr<sockaddr_in> server_addr(new sockaddr_in);
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(server_socket, reinterpret_cast< const sockaddr* >(server_addr.get()), sizeof(sockaddr_in)) == -1 ) {
        throw ServerError("Ошибка в 'bind' сокета");
    }

    if( listen(server_socket, queue_length) == -1 ) {
        throw ServerError("Не удалось запустить 'listen'");
    }
}

void Listener::start_listen() {
    socklen_t len = sizeof(sockaddr_in);
    unique_ptr<sockaddr_in> client_addr(new sockaddr_in);
    int client_socket;

    while (true) {
        client_socket = accept(server_socket, reinterpret_cast< sockaddr* >(client_addr.get()), &len);
        if (client_socket == -1 ) {
            logger.log("[Не критическая]\t Не удалось установить соединение с клиентом");
            continue;
        }

        cout << "Соединение с клиентом установлено" << endl;

        Worker worker(client_socket, base);

        try {
            cout << "Этап аутентификации" << endl;
            worker.authentificate();
            cout << "Этап вычислений" << endl;
            worker.math();
        } catch( const ServerError& e ) {
            logger.log("[Не критическая]\t"+string(e.what()));
        }

        close(client_socket);

        cout << "Соединение с клиентом разорвано" << endl;
    }
}