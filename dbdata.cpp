#include "dbdata.h"

// it is permitted to initialize these objects directly
// use of the factory class is not mandatory

DBData::DBData()
{
}

DBData::~DBData()
{
}


// could changes these for templates
DBDataInt::DBDataInt(const int data)
    : data{data}
{
}

DBDataInt* DBDataInt::Clone() const
{
    return new DBDataInt(*this);
}

/*std::unique_ptr<DBData> DBDataInt::CloneUniquePtr() const
{
    return std::unique_ptr<new DBDataInt(*this)>;
}*/

DBDataDouble::DBDataDouble(const double data)
    : data{data}
{
}

DBDataDouble* DBDataDouble::Clone() const
{
    return new DBDataDouble(*this);
}

/*std::unique_ptr<DBData> DBDataDouble::CloneUniquePtr() const
{
    return std::unique_ptr<new DBDataDouble(*this)>;
}*/

DBDataString::DBDataString(const std::string data)
    : data{std::move(data)}
{
}

DBDataString* DBDataString::Clone() const
{
    return new DBDataString(*this);
}

/*std::unique_ptr<DBData> DBDataString::CloneUniquePtr() const
{
    return std::unique_ptr<new DBDataString(*this)>;
}*/



DBDataFactory& DBDataFactory::getFactory()
{
    static DBDataFactory factory;
    return factory;
}

std::unique_ptr<DBData> DBDataFactory::create(const std::string& sType, const std::string& sValue)
{
    if(sType == getIntToken())
    {
        regexValidateInt(sValue);

        int value{convertToInt(sValue)};

        std::unique_ptr<DBData> data{new DBDataInt(value)};
        return data;
    }
    else if(sType == getDoubleToken() || sType == getFloatToken())
    {
        regexValidateDouble(sValue);

        double value{convertToDouble(sValue)};

        std::unique_ptr<DBData> data{new DBDataDouble(value)};
        return data;
    }
    else if(sType == getStringToken())
    {
        regexValidateString(sValue);

        std::string value{convertToString(sValue)};
        
        std::unique_ptr<DBData> data{new DBDataString(value)};
        return data;
    }
    else
    {
        throw std::runtime_error("Invalid type token");
    }
}

DBDataFactory::DBDataFactory()
{

}

void DBDataFactory::regexValidateInt(const std::string& value)
{
    std::regex r("^[+-]?[0-9]+$");
    if(!std::regex_match(value, r))
    {
        throw std::runtime_error("Invalid string format for integer type");
    }
}

void DBDataFactory::regexValidateDouble(const std::string& value)
{
    std::regex r("^[+-]?[0-9]+[.][0-9]*([e][+-]?[0-9]+)?$");
    if(!std::regex_match(value, r))
    {
        throw std::runtime_error("Invalid string format for floating point type");
    }
}

void DBDataFactory::regexValidateString(const std::string& value)
{
    std::regex r("^[[:print:]]*$");
    if(!std::regex_match(value, r))
    {
        throw std::runtime_error("Invalid string format for string type");
    }
}

std::string DBDataFactory::convertToString(const std::string& value)
{
    auto lambda = [](auto elem)
    {
        return std::isprint(elem);
    };

    if(!std::all_of(value.begin(), value.end(), lambda))
    {
        throw std::runtime_error("Invalid non-printable character");
    }

    return value;
}

int DBDataFactory::convertToInt(const std::string& value)
{
    std::size_t index;
    int converted_value{std::stoi(value, &index)};

    if(index != value.size())
    {
        throw std::runtime_error("Bad format input");
    }

    return converted_value;
}

double DBDataFactory::convertToDouble(const std::string& value)
{
    return std::stod(value);
}
    
std::string DBDataFactory::getStringToken() { return mStringToken; }
std::string DBDataFactory::getIntToken() { return mIntToken; }
std::string DBDataFactory::getFloatToken() { return mFloatToken; }
std::string DBDataFactory::getDoubleToken() { return mDoubleToken; }

const std::string DBDataFactory::mStringToken{"string"};
const std::string DBDataFactory::mIntToken{"int"};
const std::string DBDataFactory::mFloatToken{"float"};
const std::string DBDataFactory::mDoubleToken{"double"};
