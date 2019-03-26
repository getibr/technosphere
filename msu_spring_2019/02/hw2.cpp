#include <sstream>
#include <iostream>


class Calculator
{
public:
    int64_t Result(const char* str) 
    {
        std::istringstream expr(str);
        return add_sub(expr);
    }
private:
    struct Operations
    {
        static const char Add = '+';
        static const char Sub = '-'; 
        static const char Mul = '*';
        static const char Div = '/';
        static const char Minus = '-';
    };

    int64_t add_sub(std::istringstream &expr)
    {
        int64_t right, left = div_mul(expr);
        char oper;
        while (expr >> oper)
        {
            right = div_mul(expr);
            if (oper == Operations::Add)
                left += right;
            if (oper == Operations::Sub)
                left -= right;
        }
        return left;
    }

    int64_t div_mul(std::istringstream &expr)
    {
        int64_t right, left = number_s_minusom(expr);
        char oper;
        while (expr >> oper && (oper == Operations::Mul || oper == Operations::Div))
        {
            right = div_mul(expr);
            if (oper == Operations::Mul) 
                left *= right;
            if (oper == Operations::Div)
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

    int64_t number_s_minusom(std::istringstream &expr)
    {
        char znak;
        int64_t znak_opredel = 1;
        if (!(expr >> znak))
            throw std::invalid_argument("");
        if (znak == Operations::Minus)
            znak_opredel = -1;
        else 
            expr.putback(znak);
        int64_t number = 0;
        if (!(expr >> number))
            throw std::invalid_argument("");
        return (number * znak_opredel);
    }
};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "error"<< std::endl;
        return (1);
    }
    try 
    {
        std::cout << Calculator().Result(argv[1]) << std::endl;
    }
    catch (std::invalid_argument&)
    {
        std::cout << "error" << std::endl;
        return (1);
    }
    return (0);
}