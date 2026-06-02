#include <stdexcept>

class StreamError : public std::runtime_error {
public:
    explicit StreamError(const std::string& msg) : std::runtime_error(msg) {}
};