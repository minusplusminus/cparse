#ifndef ARG3_CPARSE_TYPE_BYTES_H_
#define ARG3_CPARSE_TYPE_BYTES_H_

#include <string>
#include <arg3/strings/strings.h>
#include "../value.h"

namespace cparse
{
    namespace type
    {
        class Bytes
        {
        public:
            Bytes(const Value &value);

            Value toValue() const;

            arg3::ustring getBytes() const;

        private:
            arg3::ustring value_;
        };
    }
}

#endif
