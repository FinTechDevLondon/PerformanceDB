
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>

#include <stdexcept>





#include "dbdata.h"
#include "dbtable.h"


#include "myformat.h"


int main()
{

    myformat::do_test();
    std::cout << "all tests completed successfully" << std::endl;

    std::vector<std::string> tokens
    {
        "int", "42",
        "float", "3.14",
        "double", "3.14",
        "int", "2.8", // this is a failure
        "string", "hello world",
        "invalid", "anything"
    };

    DBDataFactory &f1{DBDataFactory::getFactory()};

    std::vector<std::unique_ptr<DBData>> vector_data;

    for(auto it{tokens.begin()}; ; )
    {
        std::string s1{*it};
        ++ it;

        std::cout << "s1=" << s1 << std::endl;

        if(it == tokens.end())
        {
            break;
        }

        std::string s2{*it};
        ++ it;

        std::unique_ptr<DBData> t(nullptr);
        try
        {
            t = f1.create(s1, s2);
            vector_data.push_back(std::move(t));
        }
        catch(const std::runtime_error& e)
        {
            const std::string acceptable_error("Invalid non-printable character");

            if(e.what() == acceptable_error)
            {
                std::cout << e.what() << std::endl;
            }
            else if(s2 == std::string("2.8"))
            {
                std::cout << e.what() << std::endl;
            }
            else if(e.what() == std::string("Invalid type token"))
            {
                std::cout << e.what() << std::endl;
            }
            else
            {
                throw;
            }
        }

        if(it == tokens.end()) break;
    }



    std::vector<std::string> table_creation_commands
    {
        "create table table1"
    };

    std::list<std::unique_ptr<DBTable>> dbtables;

    for(auto it: table_creation_commands)
    {
        std::unique_ptr<DBTable> table(DBTableFactory::getFactory().create(it));

        dbtables.push_back(std::move(table));
    }

    // add a column
    const std::unique_ptr<DBTable> &table(*dbtables.begin());
    table->AddColumnInt("intColumn", false);
    table->AddColumnInt("floatColumn", true);
    table->AddColumnInt("stringColumn", true);

    std::vector<std::pair<std::string, std::unique_ptr<DBData>>> data_values
    {
        {"intColumn", new DBDataInt(42)},
        {"floatColumn", new DBDataDouble(3.14)},
        {"stringColumn", new DBDataString("hello world")}
    };

    try
    {
        table->AddData(data_values);
    }
    catch(const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }



    return 0;
}