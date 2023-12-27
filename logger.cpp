#include <fstream>
#include <iostream>
#include <ctime>

#include "logger.h"

bool Logger::log(const string& message) {
    time_t cur_time = time(nullptr);
    size_t buf_size = sizeof("dd/mmm/yyyy:HH:MM:SS");
    char buf[buf_size];
    strftime(buf, buf_size, "%d/%b/%Y:%H:%M:%S", std::localtime(&cur_time));

    ofstream out_stream(path, ios_base::app);
    if (out_stream.fail()) {
        cout << string(buf) << '\t' << message << endl;
        return false;
    }

    out_stream << string(buf) << '\t' << message << endl << flush;
    out_stream.close();

    return true;
}