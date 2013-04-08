#include <bitset>
#include <iostream>
#include <string>
#include <list>
#include <stdint.h>

// g++ -std=c++11 vari.cpp -o vari

namespace hacks
{
    class Base
    {
    public:
        static std::string GetName() { return "Nothing"; }
    };

    class Foo : Base
    {
    public:
        static std::string GetName() { return "Foo"; }
    };

    class Bar : Base
    {
    public:
        static std::string GetName() { return "Bar"; }
    };

    class Blech : Base
    {
    public:
        static std::string GetName() { return "Blech"; }
    };

    class Blarg : Base
    {
    public:
        static std::string GetName() { return "Blarg"; }
    };

    typedef std::list<std::string> TypeList;

    template <typename T>
    TypeList type_list() 
    {
        TypeList list;
        list.push_back(T::GetName());
        return list;
    }   

    template <typename T1, typename T2, typename ... Types>
    TypeList type_list() 
    {
        TypeList list;
        list.push_back(T1::GetName());
        list.splice(list.begin(), type_list<T2, Types ...>());
        return list;
    }
}


int main (int argc, char * argv[])
{
    hacks::TypeList list1 = hacks::type_list<hacks::Foo>();

    std::cout << "List 1 contains:\n";
    for (auto str : list1)
    {
         std::cout << "\t" << str << "\n";
    }


    hacks::TypeList list2 = hacks::type_list<hacks::Foo, hacks::Blech, hacks::Blarg>();

    std::cout << "List 2 contains:\n";
    for (auto str : list2)
    {
         std::cout << "\t" << str << "\n";
    }

    return 0;
}