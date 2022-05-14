#ifndef DBTABLE_H
#define DBTABLE_H


#include <memory>
#include <string>
#include <list>
#include <optional>
#include <map>


#include "dbdata.h"


// use of factory class is not mandatory

class DBTable
{

    public:

    // constraints on the external row type:
    // names of data elements have to be unique (enforced by map)

    // constraints on the internal row type:
    // (1): all names of data elements must match a name of a column
    // (2): no columns can be missing unless the columns is nullable
    // (3): there cannot be any named data elements which do not have
    //      a corresponding column name

    class DBTableRow
    {

        typedef std::map<std::string, std::unique_ptr<DBData>> data_t;

        public:

        void Insert(const std::string &column_name, std::unique_ptr<DBData> data);
        void Insert(const std::pair<const std::string, std::unique_ptr<DBData>> &data);

        data_t::const_iterator begin() const;
        data_t::const_iterator end() const;

        private:

        data_t data;
    };
    private:

    
    // this has to be correct
    class DBTableRowInternal
    {

        typedef std::map<std::string, std::unique_ptr<DBData>> data_t;

        public:

        void Insert(const std::string &column_name, std::unique_ptr<DBData> data);
        
        // maybe don't want this function
        //void Insert(const std::pair<const std::string, std::unique_ptr<DBData>> &data);

        // convert to class DBTableRow
        //void GetExternal() const;

        // validate a class DBTableRow
        //void ValidateExternal() const;

        // convert a class DBTableRow
        //void BuildFromExternal(const DBTableRow& external_row, const DBTable& table);

        private:

        std::map<std::string, std::unique_ptr<DBData>> data;

    };



    public:

    DBTable();

    virtual ~DBTable();

    void AddColumnInt(const std::string name, const bool nullable, const std::optional<int> default_value = std::optional<int>());
    void AddColumnDouble(const std::string name, const bool nullable, const std::optional<double> default_value = std::optional<double>());
    void AddColumnString(const std::string name, const bool nullable, const std::optional<std::string> default_value = std::optional<std::string>());

    void AddData(std::vector<std::pair<const std::string, std::unique_ptr<DBData>>> data);

    // interface to add data to table using an external type table row
    // DBTableRow
    void AddData(const DBTableRow& row);

    private:

    bool ContainsColumnWithName(const std::string& name);

    public:




    // columns are private because it makes no sense for a user to instantiate a column
    // without a table - in other words a "free" table column does not make sense
    // because it would be table by itself, just with only one column, and no data
    // because the data is stored elsewhere

    class DBTableColumnBase
    {

        public:

        DBTableColumnBase(const std::string name);

        virtual ~DBTableColumnBase();

        void SetNullable(const bool nullable);
        bool GetNullable() const;

        void ValidateName(const std::string name);

        void SetName(const std::string name);
        std::string GetName() const;

        private:

        std::string name;
        bool nullable;

    };

    class DBTableColumnInt : public DBTableColumnBase
    {

        public:

        DBTableColumnInt(const std::string name);

        void SetDefaultValue(const std::optional<int> default_value);

        private:

        int default_value;

    };

    class DBTableColumnDouble : public DBTableColumnBase
    {

        public:

        DBTableColumnDouble(const std::string name);

        void SetDefaultValue(const std::optional<double> default_value);
        
        private:

        double default_value;
    };

    class DBTableColumnString : public DBTableColumnBase
    {

        public:

        DBTableColumnString(const std::string name);

        void SetDefaultValue(const std::optional<std::string> default_value);

        private:

        std::string default_value;
    };



    std::list<std::unique_ptr<DBTableColumnBase>> table_columns;
    //std::list<std::unique_ptr<DBData>> table_data;
    // currently not used ^

};


class DBTableFactory
{

    public:

    static DBTableFactory& getFactory();

    std::unique_ptr<DBTable> create(const std::string& instruction);

    private:

    DBTableFactory();

};


#endif // DBTABLE_H