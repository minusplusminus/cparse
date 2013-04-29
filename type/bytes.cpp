#include <cparse/type/bytes.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        Bytes::Bytes(const Value &value) : value_(arg3::base64::decode(value.getString("base64")))
        {
        }

        Value Bytes::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_BYTES);

            value.setString("base64",  arg3::base64::encode(value_.c_str(), value_.size()));

            return value;
        }

        arg3::ustring Bytes::getBytes() const {
            return value_;
        }
    }
}