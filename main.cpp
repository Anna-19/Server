#include <iostream>

#include "interface.h"
#include "server_errors.h"

int main(int argc, char* argv[]) {
    Interface interface;
    try {
        interface.start_server(argc, argv);
    } catch(const po::error& e) {
        cerr << e.what() << endl;
        return 1;
    } catch(const ServerError& er) {
        cerr << er.what() << endl;
        return 1;
    } catch(...) {
        cerr << "Неизвестная ошибка" << endl;
    }
    return 0;
}