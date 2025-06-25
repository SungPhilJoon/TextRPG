#include <iostream>
#include <string>
#include <sstream>

static class StringUtil
{
private:
    static std::string newline;
    static std::string space;
    static std::ostringstream oss;

public:
    static void AppendStart()
    {
        oss.str("");
        oss.clear();
    }

    template<typename... Args>
    static void Append(Args&&... args)
    {
        ((oss << args), ...);
    }

    template<typename... Args>
    static void AppendLine(Args&&... args)
    {
        Append(args...);
        oss << newline;
    }

    static void Space()
    {
        oss << space;
    }

    static void NewLine()
    {
        oss << newline;
    }

    static void AppendEnd(int length = 40)
    {
        oss << std::string(length, '=');
        std::cout << oss.str();
        std::cout << std::endl;
    }
};

std::string StringUtil::newline = "\n"; 
std::string StringUtil::space = " ";
std::ostringstream StringUtil::oss = std::ostringstream();