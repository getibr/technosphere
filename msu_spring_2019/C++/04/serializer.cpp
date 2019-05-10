#include <iostream>

#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    std::ostream& out_;
    template <class T, class... Args>
    Error process(T&& value, Args&&... args)
    {
        if(process(value) == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }

    Error process(bool value)
    {
        if(value == true)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }

    Error process(uint64_t value)
    {
        out_ << value << Separator;
        return Error::NoError;
    }
};

class Deserializer
{
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(args...);
    }
	
private:
    std::istream& in_;
    template <class T, class... Args>
    Error process(T&& value, Args&&... args)
    {
        if(process(value) == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }

    Error process(bool& value)
    {
        std::string text;
        in_ >> text;
        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process(uint64_t& value)
    {
        std::string text;
        in_ >> text;

        if (text[0] == '-')
            return Error::CorruptedArchive;
        try
        {
            value = std::stoull(text.c_str());
        }
        catch(const std::invalid_argument&)
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};
