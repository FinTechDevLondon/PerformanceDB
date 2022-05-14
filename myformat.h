#ifndef MYFORMAT_H
#define MYFORMAT_H

#include <iterator>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>

class myformat
{

    public:

    template<typename T>
    static std::string to_string(T arg)
    {
        return std::to_string(arg);
    }

    template<typename... Ts>
    static std::string format(std::string format_string, Ts... args)
    {
        format_internat(format_string, args...);

        return format_string;

        /*
        const std::string delimiter("{}");
        auto it{std::find(format_string.cbegin(), format_string.cend(), delimiter)};
        for(; it != format_string.cend(); )
        {
            const auto it_end(std:advance(it, 1));
            format_string.replace(it, it_end, )

            // an infinite loop?
            const auto it2(it);
            it = std::find(it, format_string.cend(), delimiter)};

            if(it2 == it) throw std::runtime_error("Infinite loop!");
        }
        */
    }

    template<typename T, typename... Ts>
    static void format_internal(std::string& format_string, T arg, Ts... args)
    {
        format1(format_string, arg);

        format_internal(format_string, args...);
    }

    static void format1(std::string& format_string, const std::string& arg)
    {
        const std::string delimiter("{}");
        auto it{std::find(format_string.cbegin(), format_string.cend(), delimiter)};
        if(it != format_string.cend())
        {
            auto it_end{std::next(it, 2)};
            format_string.replace(it, it_end, arg);
        }
        else
        {
            throw std::runtime_error("Format string too many substitute arguments");
        }
    }

    static void do_test()
    {
        do_test_1();
    }

    static void do_test_1()
    {
        std::string s1("hello {}");
        std::string s2("world");
        std::string s3 = format(s1, s2);
        std::cout << s3 << std::endl;
        std::string s4("hello world");

        if(s3 == s4)
        {
            // ok
        }
        else
        {
            throw std::runtime_error("test 1 failed");
        }
    }

};

#endif // MYFORMAT_H