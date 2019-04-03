#include <sstream>
#include <iostream>


int64_t number(std::istringstream &expr)
{
    char sign;
    int64_t number_sign = 1;
    if (!(expr >> sign))
        throw std::invalid_argument("");
    if (sign == '-')
        number_sign = -1;
    else 
        expr.putback(sign);
    int64_t number = 0;
    if (!(expr >> number))
        throw std::invalid_argument("");
    return (number * number_sign);
}

int64_t div_mul(std::istringstream &expr)
{
    int64_t right, left = number(expr);
    char oper;
    while (expr >> oper && (oper == '*' || oper == '/'))
    {
        right = div_mul(expr);
        if (oper == '*') 
            left *= right;
        if (oper == '/')
        {
            if (right == 0) 
                throw std::invalid_argument("");
            left /= right;
        }
    }
    if (expr)
        expr.putback(oper);
    return left;
}

int64_t add_sub(std::istringstream &expr)
{
    int64_t right, left = div_mul(expr);
    char oper;
    while (expr >> oper)
    {
        right = div_mul(expr);
        if (oper == '+')
            left += right;
        if (oper == '-')
            left -= right;
    }
    return left;
}

int64_t Result(const char* str) 
{
    std::istringstream expr(str);
    return add_sub(expr);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "error"<< std::endl;
        return (1);
    }
    try 
    {
        std::cout << Result(argv[1]) << std::endl;
    }
    catch (std::invalid_argument&)
    {
        std::cout << "error" << std::endl;
        return (1);
    }
    return (0);
}