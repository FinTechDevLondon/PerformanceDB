#include "dbtable.h"


#include <optional>
#include <memory>
#include <stdexcept>
#include <map>
#include <utility>  // std::pair
#include <algorithm>
#include <string>


#include "myformat.h"


// use of factory class is not mandatory

//
// DBTable
//

DBTable::DBTable()
{
}

DBTable::~DBTable()
{
}

void DBTable::AddColumnInt(const std::string name, const bool nullable, const std::optional<int> default_value)
{
    if(nullable)
    {
        std::unique_ptr<DBTableColumnInt> column(new DBTableColumnInt(name));
        column->SetNullable(nullable);
        column->SetDefaultValue(default_value);
    }
    else
    {
        if(default_value.has_value())
        {
            std::unique_ptr<DBTableColumnInt> column(new DBTableColumnInt(name));
            column->SetNullable(nullable);
            column->SetDefaultValue(default_value);
        }
        else
        {
            throw std::runtime_error("Cannot add non nullable column with no default value specified");
        }
    }
}

void DBTable::AddColumnDouble(const std::string name, const bool nullable, const std::optional<double> default_value)
{
    if(nullable)
    {
        std::unique_ptr<DBTableColumnDouble> column(new DBTableColumnDouble(name));
        column->SetNullable(nullable);
        column->SetDefaultValue(default_value);
    }
    else
    {
        if(default_value.has_value())
        {
            std::unique_ptr<DBTableColumnDouble> column(new DBTableColumnDouble(name));
            column->SetNullable(nullable);
            column->SetDefaultValue(default_value);
        }
        else
        {
            throw std::runtime_error("Cannot add non nullable column with no default value specified");
        }
    }
}

void DBTable::AddColumnString(const std::string name, const bool nullable, const std::optional<std::string> default_value)
{
    if(nullable)
    {
        std::unique_ptr<DBTableColumnString> column(new DBTableColumnString(name));
        column->SetNullable(nullable);
        column->SetDefaultValue(default_value);
    }
    else
    {
        if(default_value.has_value())
        {
            std::unique_ptr<DBTableColumnString> column(new DBTableColumnString(name));
            column->SetNullable(nullable);
            column->SetDefaultValue(default_value);
        }
        else
        {
            throw std::runtime_error("Cannot add non nullable column with no default value specified");
        }
    }
}

// TODO: add interface using a DBRow object
void DBTable::AddData(std::vector<std::pair<const std::string, std::unique_ptr<DBData>>> data)
{
    //DBRow ... construct a database row here

    // use the external type to build
    DBTableRow row;

    //for(const auto& [key, value] : data)
    for(const auto& element : data)
    {
        // this will thow if the same name is used twice
        row.Insert(element);
    }
}

// interface to add data
// converts an external type row to an internal type row
void DBTable::AddData(const DBTableRow& row)
{
    DBTableRowInternal row_internal;

    // row is external type
    for(const auto& element : row)
    {
        // get name of incoming table row
        const auto &column_name_external(element.first);
        const auto &column_data_external(element.second); // unique_ptr<DBData>

        // check if there is a column with the correct name
        bool contains_column_with_name(ContainsColumnWithName(column_name_external));
        
        if(contains_column_with_name)
        {
            // try to insert data, this will fail if there is already an element with the same name
            try
            {
                // need to create a copy of the data using a clone operation and move it into the new row ?

                DBData *p(column_data_external->Clone());
                if(dynamic_cast<DBDataInt>(p))
                {
                    row_internal.Insert(column_name_external, std::unique_ptr<DBDataInt>(p));
                }
                else if(dynamic_cast<DBDataDouble>(p))
                {
                    row_internal.Insert(column_name_external, std::unique_ptr<DBDataInt>(p));
                }
                else if(dynamic_cast<DBDataString>(p))
                {
                    row_internal.Insert(column_name_external, std::unique_ptr<DBDataInt>(p));
                }
                else
                {
                    throw std::runtime_error("dynamic_cast failure");
                }
            }
            catch(const std::exception& e)
            {
                std::string error_string(std::format("Column with name {} has already contains data for this row", column_name));
                throw std::runtime_error(error_string);
            }
        }
        else
        {
            const std::string error_string(std::format("Column with name {} does not exist in database table", column_name));
            throw std::runtime_error(error_string);
        }
    }
}

// check the table columns for a column with the name `name`
// returns true if a column exists with name `name`, false otherwise
bool DBTable::ContainsColumnWithName(const std::string& name)
{
    auto predicate = [&column_name_external](const auto &column)
    {
        return column.GetName() == column_name_external;
    }

    const auto it(std::find_if(table_columns.cbegin(), table_columns.cend(), predicate));
    
    // found column with name column_name
    if(it != table_columns.cend())
    {
        return true;
    }
    // column with name column_name was not found
    else
    {
        return false;
    }
}



void DBTable::DBTableRowInternal::Insert(const std::string &column_name, std::unique_ptr data)
{
    DBTable::DBTableRowInernal::data.insert(std::make_pair(column_name, data));
}

void DBTable::DBTableRow::Insert(const std::string column_name, std::unique_ptr<DBData> data)
{
    DBTable::DBTableRow::data.insert(std::make_pair(column_name, data));
}

void DBTable::DBTableRow::Insert(const std::pair<const std::string, std::unique_ptr<DBData>> &data)
{
    DBTable::DBTableRow::data.insert(data);
}

DBTable::DBTableRow::data_t::const_iterator DBTable::DBTableRow::begin() const
{
    return data.begin();
}

DBTable::DBTableRow::data_t::const_iterator DBTable::DBTableRow::end() const
{
    return data.end();
}


// remove this function replace with another function below
/*
void DBTable::DBTableRowInternal::Insert(const std::pair<const std::string, std::unique_ptr<DBData>> &data)
{
    // check if there is a column with the correct name
    const std::string &column_name(data.first);

    //std::list<std::unique_ptr<DBTableColumnBase>> table_columns;
    
    auto predicate = [&column_name](const auto column)
    {
        return column.GetName() == column_name;
    };

    const auto it(std::find_if(table_columns.cbegin(), table_columns.cend(), predicate));

    // column with name column_name was not found
    if(it != table_columns.cend())
    {
        // try to insert data, this will fail if there is already an element with the same name
        try
        {
            DBTable::DBTableRowInternal::data.insert(data);
        }
        catch(const std::exception& e)
        {
            std::string error_string(std::format("Column with name {} has already contains data for this row", column_name));
            throw std::runtime_error(error_string);
        }
    }
    // found column with name column_name
    else
    {
        const std::string error_string(std::format("Column with name {} does not exist in database table", column_name));
        throw std::runtime_error(error_string);
    }
}
*/

// this doesn't currently work becuase it requires access to the columns of the dbdatable
// which there is no access for currently - would require adding a function to get these
// columns or permit access via public or friend
void DBTable::DBTableRowInternal::BuildFromExternal(const DBTableRow& external_row, const DBTable& table)
{
    for(const auto& element : external_row)
    {
        // get name of incoming table row
        const auto &column_name_external(element.first);
        const auto &column_data_external(element.second);

        // check if there is a column with the correct name

        auto predicate = [&column_name_external](const auto &column)
        {
            return colum.GetName() == column_name_external;
        }

        // TODO: make a function in table called ContainsColumnWithName() ?
        const auto it(std::find_if(table.columns.cbegin(), table.columns.cend(), predicate));
        
        // column with name column_name was not found
        if(it != table_columns.cend())
        {
            // try to insert data, this will fail if there is already an element with the same name
            try
            {
                DBTable::DBTableRowInternal::data.insert(data);
            }
            catch(const std::exception& e)
            {
                std::string error_string(std::format("Column with name {} has already contains data for this row", column_name));
                throw std::runtime_error(error_string);
            }
        }
        // found column with name column_name
        else
        {
            const std::string error_string(std::format("Column with name {} does not exist in database table", column_name));
            throw std::runtime_error(error_string);
        }
    }
}

    //std::map<std::string, std::unique_ptr<DBData>> data;

/*
template<typename T1, typename T2>
DBTable::AddColumnInt(T1 arg1, T2 arg2)
{

}
*/

DBTable::DBTableColumnBase::DBTableColumnBase(const std::string name)
    : name(name)
    , nullable{true}
{
    DBTable::DBTableColumnBase::ValidateName(name);
}

DBTable::DBTableColumnBase::~DBTableColumnBase()
{
}

void DBTable::DBTableColumnBase::SetNullable(const bool nullable)
{
    void DBTable::DBTableColumnBase::nullable = nullable;
}

bool DBTable::DBTableColumnBase::GetNullable() const
{
    return nullable;
}

void DBTable::DBTableColumnBase::ValidateName(const std::string name)
{
    if(name.size() > 0)
    {
        // ok
    }
    else
    {
        throw std::runtime_error("Table column name cannot be empty string");
    }
}

void DBTable::DBTableColumnBase::SetName(const std::string name)
{
    ValidateName(name);
    this->name = name;
}

std::string DBTable::DBTableColumnBase::GetName() const
{
    return name;
}

DBTable::DBTableColumnInt::DBTableColumnInt(const std::string name)
    : DBTableColumnBase(name)
    , default_value(0)
{
}

void DBTable::DBTableColumnInt::SetDefaultValue(const std::optional<int> default_value)
{
    if(GetNullable())
    {
        // can be null value as default
        this->default_value = default_value.value();
    }
    else
    {
        if(default_value.has_value())
        {
            this->default_value = default_value.value();
        }
        else
        {
            throw std::runtime_error("Non null default value must be provided for non nullable column");
        }
    }
}

DBTable::DBTableColumnDouble::DBTableColumnDouble(const std::string name)
    : DBTableColumnBase(name)
    , default_value(0.0)
{
}

void DBTable::DBTableColumnDouble::SetDefaultValue(const std::optional<double> default_value)
{
    if(GetNullable())
    {
        // can be null value as default
        this->default_value = default_value.value();
    }
    else
    {
        if(default_value.has_value())
        {
            this->default_value = default_value.value();
        }
        else
        {
            throw std::runtime_error("Non null default value must be provided for non nullable column");
        }
    }
}
    
DBTable::DBTableColumnString::DBTableColumnString(const std::string name)
    : DBTableColumnBase(name)
{
}

void DBTable::DBTableColumnString::SetDefaultValue(const std::optional<std::string> default_value)
{
    if(GetNullable())
    {
        // can be null value as default
        this->default_value = default_value.value();
    }
    else
    {
        if(default_value.has_value())
        {
            this->default_value = default_value.value();
        }
        else
        {
            throw std::runtime_error("Non null default value must be provided for non nullable column");
        }
    }
}



//
// DBTableFactory
//

DBTableFactory& DBTableFactory::getFactory()
{
    static DBTableFactory factory;
    return factory;
}

std::unique_ptr<DBTable> DBTableFactory::create(const std::string& instruction)
{
    return nullptr;
}

DBTableFactory::DBTableFactory()
{
}


