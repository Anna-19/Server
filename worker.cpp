#include <memory>
#include <random>
#include <iostream>

#include <netinet/in.h>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/simple.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

#include <boost/cast.hpp>

#include "server_errors.h"
#include "worker.h"

void Worker::authentificate() {
    namespace cpp = CryptoPP;
    string id = recv_message();

    cout << "ID клиента: " << id << endl;

    auto it = base.find(id);
    if( it == base.end() ) {
        send(work_socket, "ERR", 4, 0);
        throw AuthError("Неизвестный id");
    }
    string pass = it->second;

    std::random_device rnd;
    std::mt19937_64 rnd_generator(rnd());
    uint64_t rnd_number = rnd_generator();
    std::string salt;

    CryptoPP::StringSource((const CryptoPP::byte*)&rnd_number,
                           8,
                           true,
                           new CryptoPP::HexEncoder(new CryptoPP::StringSink(salt))
                          );



    if( send(work_socket, salt.c_str(), 16, 0) == -1) {
        throw AuthError("Ошибка отправки salt");
    }

    string client_hash = recv_message();

    CryptoPP::Weak::MD5 hash;
    std::string result_hash;
    CryptoPP::StringSource(salt + pass, true,
                           new CryptoPP::HashFilter(hash,
                                   new CryptoPP::HexEncoder(
                                       new CryptoPP::StringSink(result_hash)
                                   )
                                                   )
                          );

    if( client_hash != result_hash ) {
        send(work_socket, "ERR", 4, 0);
        throw AuthError("Ошибка аутентификации");
    }
    if( send(work_socket, "OK", 3, 0) == -1 ) {
        throw AuthError("Ошибка отправки OK");
    }
}

void Worker::math() {
    uint32_t vectors_count;

    if( recv(work_socket, (void*)&vectors_count, sizeof(uint32_t), 0) == -1) {
        throw MathError("Ошибка получения количества векторов");
    }

    cout << "Количество векторов: "+std::to_string(vectors_count) << endl;

    for( uint32_t i=0; i<vectors_count; i++) {
        cout << "Вектор " << i+1 << " из " << vectors_count << endl;
        uint32_t vector_size;
        if( recv(work_socket, (void*)&vector_size, sizeof(uint32_t), 0) == -1) {
            throw MathError("Ошибка получения размера вектора");
        }

        cout << "Длина вектора: "+std::to_string(vector_size) << endl;

        unique_ptr< int32_t[] > v(new int32_t[vector_size]);
        int v_size;
        if( (v_size = recv(work_socket, (void*)v.get(), sizeof(int32_t)*vector_size, 0)) == -1 ) {
            throw MathError("Ошибка получения вектора");
        }
        if( sizeof(int32_t)*vector_size != (uint32_t)v_size ) {
            throw MathError("Неверный размер полученного вектора");
        }

        cout << "Вектор успешно получен" << endl;

        std::vector<int32_t> vec(vector_size);
        for (uint32_t i=0; i<vector_size; ++i) {
            vec.at(i) = v[i];
        }

        int32_t result = calculate(vec);

        if( send(work_socket, (void*)&result, sizeof(int32_t), 0) == -1 ) {
            throw MathError("Ошибка отправки результата");
        }
        cout << "Ср. арифм. вектора No" << i+1 << " успешно отправлено" << endl;
    }
}

string Worker::recv_message() {
    int rc;
    int buflen = 32;
    unique_ptr <char[]> buf(new char[buflen]);
    while (true) {
        rc = recv(work_socket, buf.get(), buflen, MSG_PEEK);
        if (rc == -1)
            throw ServerError("Ошибка получения строки");
        if (rc < buflen)
            break;
        buflen *= 2;
        buf = std::unique_ptr<char[]>(new char[buflen]);
    }
    string res(buf.get(), rc);
    rc = recv(work_socket, nullptr, rc, MSG_TRUNC);
    if (rc == -1)
        throw ServerError("Clear bufer error");
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}

int32_t Worker::calculate(const vector<int32_t>& v) {
    int32_t sum = 0;
    bool flag = false;
    for(uint32_t i=0; i<v.size(); i++) {
        if( (v[i] > 0) && (sum > (std::numeric_limits< int32_t >::max() - v[i])) ) {
            sum = std::numeric_limits< int32_t >::max();
            flag = true;
            break;
        }
        if( (v[i] < 0) && (sum < (std::numeric_limits< int32_t >::min() - v[i])) ) {
            sum = std::numeric_limits< int32_t >::min();
            flag = true;
            break;
        }
        sum += v[i];
    }

    int32_t count = v.size();

    int32_t average;
    if (flag or v.size() == 0)
        average = sum;
    else
        average = sum/count;
    return average;
}