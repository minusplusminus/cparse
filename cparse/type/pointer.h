#ifndef ARG3_CPARSE_TYPE_POINTER_H_
#define ARG3_CPARSE_TYPE_POINTER_H_

#include <string>
#include "../value.h"

namespace cparse
{
    class Object;

    namespace type
    {
        class Pointer
        {
        public:
            Pointer(const Object &obj);
            Value toValue() const;
            std::string id() const;
            std::string className() const;
            bool operator==(const Pointer &other) const;
            bool operator!=(const Pointer &other) const;
            bool operator==(const Object &other) const;
            bool operator!=(const Object &other) const;
        private:
            Value value_;
        };
    }
}

#endif
