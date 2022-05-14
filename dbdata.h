#ifndef DBDATA_H
#define DBDATA_H

#include <memory>
#include <string>
#include <regex>
#include <cctype>
#include <stdexcept>

// it is permitted to initialize these objects directly
// use of the factory class is not mandatory

class DBData
{

    public:

    DBData();

    virtual
    ~DBData() = 0;

    virtual
    DBData* Clone() const = 0;

    //virtual
    //std::unique_ptr<DBData> CloneUniquePtr() const = 0;

    public:

};

// could changes these for templates
class DBDataInt final : public DBData
{

    public:

    DBDataInt(const int data);

    DBDataInt(const DBDataInt& other) = default;
    
    virtual
    DBDataInt* Clone() const override;

    //virtual
    //std::unique_ptr<DBData> CloneUniquePtr() const override;

    private:

    int data;
};

class DBDataDouble final : public DBData
{

    public:

    DBDataDouble(const double data);

    DBDataDouble(const DBDataDouble& other) = default;

    virtual
    DBDataDouble* Clone() const override;

    //virtual
    //std::unique_ptr<DBData> CloneUniquePtr() const override;

    private:

    double data;
};

class DBDataString final : public DBData
{

    public:

    DBDataString(const std::string data);

    DBDataString(const DBDataString& other) = default;

    virtual
    DBDataString* Clone() const override;

    //virtual
    //std::unique_ptr<DBData> CloneUniquePtr() const override;

    private:

    std::string data;
};


class DBDataFactory
{

    public:

    static DBDataFactory& getFactory();

    std::unique_ptr<DBData> create(const std::string& sType, const std::string& sValue);

    private:

    DBDataFactory();

    static void regexValidateInt(const std::string& value);
    static void regexValidateDouble(const std::string& value);
    static void regexValidateString(const std::string& value);

    static std::string convertToString(const std::string& value);
    static int convertToInt(const std::string& value);
    static double convertToDouble(const std::string& value);

    static std::string getStringToken();
    static std::string getIntToken();
    static std::string getFloatToken();
    static std::string getDoubleToken();
    
    static const std::string mStringToken;
    static const std::string mIntToken;
    static const std::string mFloatToken;
    static const std::string mDoubleToken;

};

#endif // DBDATA_H