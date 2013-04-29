#include <cparse/op/increment.h>
#include "../protocol.h"

namespace cparse
{
    namespace op
    {
        Increment::Increment()
        {}

        Increment::Increment(int value) : amount_(value)
        {}

        int Increment::getAmount() const
        {
            return amount_;
        }

        void Increment::setAmount(int value)
        {
            amount_ = value;
        }

        Value Increment::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_OP, protocol::KEY_INCREMENT);

            value.setInt(protocol::KEY_AMOUNT, amount_);

            return value;
        }
    }
}