#ifndef STRING_HPP
#define STRING_HPP
#include <string>
#include <sstream>
namespace tf
{
template <class T>
std::string to_string(const T& t)
{
    std::stringstream stream;
    stream << t;
    return stream.str();
}

}

#endif // STRING_HPP

