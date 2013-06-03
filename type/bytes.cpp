#include <cparse/type/bytes.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        Bytes::Bytes(const JSON &value)
        {
            fromJSON(value);
        }

        Bytes::Bytes(const Type &data) : value_(data)
        {

        }

        Bytes::Bytes()
        {
        }

        Bytes::~Bytes()
        {}

        Bytes::Bytes(const Bytes &value) : value_(value.value_)
        {}

        Bytes::Bytes(Bytes &&value) : value_(std::move(value.value_))
        {}

        Bytes &Bytes::operator=(const Bytes &a)
        {
            if(this != &a)
            {
                value_ = a.value_;
            }
            return *this;
        }

        Bytes &Bytes::operator=(Bytes &&a)
        {
            if(this != &a)
            {
                value_ = std::move(a.value_);
            }
            return *this;
        }

        void Bytes::fromJSON(const JSON &attributes)
        {
            if(attributes.contains("base64"))
                value_ = arg3::base64::decode(attributes.getString("base64"));
        }

        JSON Bytes::toJSON() const
        {
            JSON value;

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