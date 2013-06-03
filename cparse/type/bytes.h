#ifndef ARG3_CPARSE_TYPE_BYTES_H_
#define ARG3_CPARSE_TYPE_BYTES_H_

#include <string>
#include <arg3/string/string.h>
#include "parsetype.h"

namespace cparse
{
    namespace type
    {
        class Bytes : public ParseType
        {
        public:
            typedef arg3::binary Type;

            Bytes(const Type &data );
            Bytes(const JSON &value);
            Bytes();
            virtual ~Bytes();
            Bytes(const Bytes &other);
            Bytes(Bytes &&other);
            Bytes &operator=(const Bytes &other);
            Bytes &operator=(Bytes &&other);

            JSON toJSON() const;
            void fromJSON(const JSON &attributes);

            Type getData() const;

            void setData(const Type &value);

        private:
            Type value_;
        };
    }
}

#endif
