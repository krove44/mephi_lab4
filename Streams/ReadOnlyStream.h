#pragma once
#include <fstream>
#include <string>
#include "..//Optional.h"

template<typename T>
class ReadOnlyStream {
private:
    std::ifstream file_;
public:
    ReadOnlyStream(const std::string& filename) {
        file_.open(filename);
        if (!file_.is_open()) {
            throw StreamError("Can't open " + filename);
        }
    }

    ~ReadOnlyStream() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    Optional<T> try_read() {
        T value;
        if (file_ >> value) {
            return Optional<T>(value);
        }
        return Optional<T>();
    }

    void close() {
        if (file_.is_open()) file_.close();
    }
};