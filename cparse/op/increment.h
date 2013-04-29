#ifndef ARG3_CPARSE_OP_INCREMENT_H_
#define ARG3_CPARSE_OP_INCREMENT_H_

#include "../value.h"

namespace cparse
{
    namespace op
    {
        class Increment
        {
        public:
            Increment();
            Increment(int value);

            Value toValue() const;

            int getAmount() const;
            void setAmount(int value);

        private:
            int amount_;
        };
    }
}

#endif
