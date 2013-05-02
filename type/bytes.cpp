#include <cparse/type/bytes.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        Bytes::Bytes(const Value &value)
        {
            fromValue(value);
        }

        Bytes::Bytes(const Type &data) : value_(data)
        {

        }

        Bytes::Bytes()
        {
        }

        void Bytes::fromValue(const Value &attributes)
        {
            if(attributes.contains("base64"))
                value_ = arg3::base64::decode(attributes.getString("base64"));
        }

        Value Bytes::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_BYTES);

            value.setString("base64",  arg3::base64::encode(value_));

            return value;
        }

        Bytes::Type Bytes::getData() const {
            return value_;
        }

        void Bytes::setData(const Type &value) {
            value_ = value;
        }
    }
}