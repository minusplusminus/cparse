#ifndef ARG3_CPARSE_TYPE_POINTER_H_
#define ARG3_CPARSE_TYPE_POINTER_H_

#include <string>
#include "parsetype.h"

namespace cparse
{
    class Object;

    namespace type
    {
        class Pointer : public ParseType
        {
        public:
            Pointer();
            Pointer(const Object &obj);
            Pointer(const Value &attributes);
            virtual ~Pointer();
            Pointer(const Pointer &value);
            Pointer(Pointer &&value);
            Pointer &operator=(const Pointer &value);
            Pointer &operator=(Pointer &&value);
            Value toValue() const;
            void fromValue(const Value &attributes);
            string id() const;
            string className() const;
            bool operator==(const Pointer &other) const;
            bool operator!=(const Pointer &other) const;
            bool operator==(const Object &other) const;
            bool operator!=(const Object &other) const;
        private:
            string className_;
            string objectId_;
        };
    }
}

#endif
