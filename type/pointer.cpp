#include <cparse/type/pointer.h>
#include <cparse/object.h>
#include "../protocol.h"

namespace cparse
{

    namespace type
    {

        Pointer::Pointer(const Object &obj) : className_(obj.className()), objectId_(obj.id())
        {
        }

        Pointer::Pointer()
        {
        }

        Pointer::Pointer(const Value &obj)
        {
            fromValue(obj);
        }

        void Pointer::fromValue(const Value &obj)
        {
            if(obj.contains(protocol::KEY_CLASS_NAME))
                className_ = obj.getString(protocol::KEY_CLASS_NAME);

            if(obj.contains(protocol::KEY_OBJECT_ID))
                objectId_ = obj.getString(protocol::KEY_OBJECT_ID);
        }

        Value Pointer::toValue() const {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_POINTER);

            value.setString(protocol::KEY_CLASS_NAME, className_);

            value.setString(protocol::KEY_OBJECT_ID, objectId_);

            return value;
        }

        string Pointer::id() const {
            return objectId_;
        }

        string Pointer::className() const
        {
            return className_;
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