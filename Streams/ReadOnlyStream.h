#pragma once
#include <fstream>
#include <string>
#include <optional>

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
        if (file_.is_open()) file_.close();
    }

    bool is_end() {
        int c = file_.peek();
        while (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            file_.get();
            c = file_.peek();
        }
        return file_.eof() || c == EOF;
    }

    std::optional<T> try_read() {
        if (is_end()) return std::nullopt;
        return read();
    }

    T read() {
        T value;
        file_ >> value;
        if (file_.fail() && !file_.eof()) {
            throw StreamError("Can't read");
        }
        return value;
    }

    void close() {
        if (file_.is_open()) file_.close();
    }
};