#include "Util.h"
using namespace std;

bool Util::OutOfScreen(const Vector2& pos)
{
    uint32_t minX = 0;
    uint32_t minY = 0;
    return pos.x < minX || WIN_WIDTH < pos.x || pos.y < minY || WIN_HEIGHT < pos.y;
}

uint32_t Util::CenterX()
{
    return WIN_WIDTH / 2;
}

uint32_t Util::CenterY()
{
    return WIN_HEIGHT / 2;
}

vector<string> Util::SplitString(const string& str, const char* delimiter)
{
    vector<string> vec;
    size_t len = str.length();
    
    size_t n = 0;

    for (size_t i = 0; i < len; i = n + 1)
    {
        n = str.find_first_of(delimiter, i);
        if (n == string::npos)
        {
            n = len;
        }
        vec.push_back(str.substr(i, n - i));
    }

    return vec;
}

bool Util::IsNumber(const std::string& str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}

bool Util::CheckString(const std::string& checkStr, const std::string& searchStr)
{
    return checkStr.find(searchStr) != std::string::npos;
}

std::string Util::GetString(const std::string& checkStr, const std::string& searchStr)
{
    if (checkStr.find(searchStr) != std::string::npos)
    {
        return searchStr;
    }
    return "notFound";
}

int32_t Util::GetNumber(const std::string& str, const char* delimiter)
{
    std::vector<std::string> split = Util::SplitString(str, delimiter);

    for (auto number : split)
    {
        //数字だけ抜き出す
        if (Util::IsNumber(number))
        {
            return int32_t(atoi(number.c_str()));
        }
    }

    return -1;
}
