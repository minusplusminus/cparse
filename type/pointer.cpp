#include <cparse/type/pointer.h>
#include <cparse/object.h>
#include "../protocol.h"

namespace cparse
{

    namespace type
    {

        Pointer::Pointer(const Object &obj)
        {
            value_.setString(protocol::KEY_TYPE, protocol::TYPE_POINTER);

            value_.setString(protocol::KEY_OBJECT_ID, obj.id());

            value_.setString(protocol::KEY_CLASS_NAME, obj.className());
        }

        Value Pointer::toValue() const {
            return value_;
        }

        string Pointer::id() const {
            return value_.getString(protocol::KEY_OBJECT_ID);
        }

        string Pointer::className() const
        {
            return value_.getString(protocol::KEY_CLASS_NAME);
        }

        bool Pointer::operator==(const Pointer &other) const
        {
            return id() == other.id();
        }

        bool Pointer::operator!=(const Pointer &other) const
        {
            return !Pointer::operator==(other);
        }

        bool Pointer::operator==(const Object &other) const
        {
            return !other.isNew() && id() == other.id();
        }

        bool Pointer::operator!=(const Object &other) const
        {
            return !Pointer::operator==(other);
        }

    }
}