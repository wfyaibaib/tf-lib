#ifndef STRING_HPP
#define STRING_HPP
#include <string>
#include <sstream>
#include <cstdlib>
namespace tf
{
template <class T>
std::string to_string(const T& t)
{
    std::stringstream stream;
    stream << t;
    return stream.str();
}
template <class Re>
Re stringTo(std::string str)
{

    std::istringstream in(str);
    Re re;
    in >> re;
    return re;
}
inline std::string upperCase(const std::string& s)
{

    std::string upper(s);
    for (size_t i = 0; i < s.length(); i++)
        upper[i] = toupper(upper[i]);
    return upper;
}

inline std::string lowerCase(const std::string& s)
{

    std::string lower(s);
    for (size_t i = 0; i < s.length(); i++)
        lower[i] = tolower(lower[i]);

    return lower;
}

}

#endif // STRING_HPP

