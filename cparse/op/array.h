#ifndef ARG3_CPARSE_OP_ARRAY_H_
#define ARG3_CPARSE_OP_ARRAY_H_

#include <string>
#include "../value.h"

namespace cparse
{
    namespace op
    {
        class Array
        {
        public:
            Array(const string &operation, const cparse::Array &objects);

            Array();

            Value toValue() const;

            void setOperation(const string &value);
            void setObjects(const cparse::Array &value);

        private:
            string operation_;
            cparse::Array objects_;
        };
    }
}

#endif
