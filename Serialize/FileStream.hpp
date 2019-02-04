#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include <fstream>
#include <string>
#include <type_traits>
#include "Serialize.hpp"

namespace os {

class FileReadWrite {
   public:
    FileReadWrite& write(const std::string& filename) { file.open(filename, std::ios::out); }
    FileReadWrite& read(const std::string& filename) { file.open(filename, std::ios::in); }

    template <class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    FileReadWrite& operator<<(T t) {
        file << t << '\n';
    }

    FileReadWrite& operator<<(std::string str) { file << str << '\n'; }

    template <class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    FileReadWrite& operator>>(T& t) {
        file >> t;
    }

    FileReadWrite& operator>>(std::string& str) {
        file.ignore();
        std::getline(file, str);
    }

    operator bool() { return !file.fail(); }

   private:
    std::fstream file;
};

}  // namespace os

using FileSerializer = os::FileReadWrite;

#endif