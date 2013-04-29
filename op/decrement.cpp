#include <cparse/op/decrement.h>
#include "../protocol.h"

namespace cparse
{
    namespace op
    {
        Decrement::Decrement()
        {}

        Decrement::Decrement(int value) : amount_(value)
        {}

        int Decrement::getAmount() const
        {
            return amount_;
        }

        void Decrement::setAmount(int value)
        {
            amount_ = value;
        }

        Value Decrement::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_OP, protocol::KEY_INCREMENT);

            value.setInt(protocol::KEY_AMOUNT, amount_);

            return value;
        }
    }
}