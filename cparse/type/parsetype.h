#ifndef ARG3_CPARSE_TYPE_BASE_H_
#define ARG3_CPARSE_TYPE_BASE_H_

#include "../value.h"

namespace cparse
{
    namespace type
    {
        class ParseType
        {
        public:
            virtual Value toValue() const = 0;
            virtual void fromValue(const Value &attributes) = 0;
        };
    }
}

#endif
