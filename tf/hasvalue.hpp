#ifndef HASVALUE_HPP
#define HASVALUE_HPP
namespace tf {
/**************************************************************************/
// has_value template
template <class Value>
struct has_value
{
    typedef Value value_type;

    Value v;

    has_value(const Value& value = Value()):v(value) {}
    virtual std::string valueToString()
    {
        return to_string(v);
    }
};

template <class HasValue>
typename HasValue::value_type value(HasValue* pnode)
{
    return pnode->v;
}
/**************************************************************************/
}
#endif // HASVALUE_HPP
