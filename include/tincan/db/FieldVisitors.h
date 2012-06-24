#ifndef FIELDBUILDERS_H__
#define FIELDBUILDERS_H__

#include "DbFieldType.h"
#include <dbccpp/PreparedStatement.h>
#include <dbccpp/ResultSet.h>

#include "../disable_copy.h"

#include <sstream>

namespace tincan
{

class DbFieldDeclarationBuilder
{
public:
    DbFieldDeclarationBuilder(std::ostringstream& out) :
        _out(out)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _out << field.label
            << " "
            << DbFieldType<typename TypedField::type>(field.options)
            << ",";
    }

private:
    DISABLE_COPY(DbFieldDeclarationBuilder)

    std::ostringstream& _out;
};

class DbInsertQueryFieldBuilder
{
public:
    DbInsertQueryFieldBuilder(std::ostringstream& columnLabels,
            std::ostringstream& fieldPlaceholders) :
        _labels(columnLabels),
        _placeholders(fieldPlaceholders)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _labels << field.label << ",";
        _placeholders << "?,";
    }

private:
    DISABLE_COPY(DbInsertQueryFieldBuilder)

    std::ostringstream& _labels;
    std::ostringstream& _placeholders;
};

class DbUpdateQueryFieldBuilder
{
public:
    DbUpdateQueryFieldBuilder(std::ostringstream& out) :
        _out(out)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _out << field.label << " = ?,";
    }

private:
    DISABLE_COPY(DbUpdateQueryFieldBuilder)

    std::ostringstream& _out;
};

class DbFieldBinder
{
public:
    DbFieldBinder(dbc::PreparedStatement::ptr& statement) :
        _statement(statement),
        _counter(1)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _statement->set<typename TypedField::type>(_counter, field.value());
        _counter++;
    }

private:
    DISABLE_COPY(DbFieldBinder)

    dbc::PreparedStatement::ptr& _statement;
    unsigned int _counter;
};

class ObjectFieldBinder
{
public:
    ObjectFieldBinder(const dbc::ResultSet::ptr& result) :
        _result(result),
        // index is 0-based, skip id, which is already set
        _counter(1)
    {}

    template <class TypedField>
    void operator>> (TypedField& field)
    {
        field = _result->get<typename TypedField::type>(_counter);
        _counter++;
    }

private:
    DISABLE_COPY(ObjectFieldBinder)

    const dbc::ResultSet::ptr& _result;
    unsigned int _counter;
};

}

#endif /* FIELDBUILDERS_H */
