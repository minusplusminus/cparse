#include <cparse/op/array.h>

namespace cparse
{
    namespace op
    {
        Array::Array(const string &op, const cparse::Array &objs)
            : operation_(op), objects_(objs)
        {}

        Array::Array()
        {}

        void Array::setOperation(const string &value)
        {
            operation_ = value;
        }

        void Array::setObjects(const cparse::Array &objs)
        {
            objects_ = objs;
        }
    }
}