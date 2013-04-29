#include <cparse/object.h>
#include <cparse/exception.h>
#include <arg3/format/format.h>
#include "util.h"
#include <cparse/user.h>
#include "protocol.h"
#include "log.h"
#include "client.h"

using namespace std;

namespace cparse
{
    bool validate_class_name(const string &value)
    {

        /* check first char is an alpha char */
        auto c = value.begin();

        if (c == value.end() || !isalpha(*c)) return false;

        /* rest of the string has to be alpha numeric */
        while (++c != value.end())
        {
            if (!isalnum(*c))
                return false;
        }
        return true;
    }

    Object *Object::create(const string &className)
    {
        Object *obj = new Object(className);

        return obj;
    }

    Object *Object::create(const string &className, const Value &attributes)
    {
        Object *obj = new Object(className);

        obj->merge(attributes);

        return obj;
    }

    Object *Object::createWithoutData(const string &className, const string &objectId)
    {
        Object *obj = new Object(className);

        obj->objectId_ = objectId;

        return obj;
    }

    Object::Object(const string &className) : className_(className),
        createdAt_(0),
        objectId_(),
        updatedAt_(0),
        attributes_(),
        dataAvailable_(false)
    {
        if (!validate_class_name(className_))
            throw Exception("invalid class name");
    }

    Object::Object(const Object &other) : className_(other.className_),
        createdAt_(other.createdAt_),
        objectId_(other.objectId_),
        updatedAt_(other.updatedAt_),
        attributes_(other.attributes_),
        dataAvailable_(other.dataAvailable_)
    {
        copy_fetched(other);
    }

    Object::~Object() {
        for(auto &e : fetched_)
        {
            if(e.second) {
                Log::trace("deleting fetched object " + e.first);
                delete e.second;
            }
        }
    }

    void Object::copy_fetched(const Object &obj)
    {
        for(auto &e : obj.fetched_) {
            if(fetched_[e.first] != NULL) {
                Log::trace("deleting fetched object " + e.first);
                delete fetched_[e.first];
            }
            Log::trace("copying fetched object " + e.first);
            fetched_[e.first] = new Object(*e.second);
        }
    }

    Object &Object::operator=(const Object &other) {
        if(this != &other) {
            className_ = other.className_;
            createdAt_ = other.createdAt_;
            objectId_ = other.objectId_;
            updatedAt_ = other.updatedAt_;
            attributes_ = other.attributes_;
            dataAvailable_ = other.dataAvailable_;

            copy_fetched(other);
        }

        return *this;
    }

    bool Object::isNew() const {
        return objectId_.empty();
    }

    void Object::merge(Value attributes)
    {
        attributes.remove(protocol::KEY_CLASS_NAME);

        if (attributes.contains(protocol::KEY_OBJECT_ID))
        {
            objectId_ = attributes.getString(protocol::KEY_OBJECT_ID);

            // remove for the loop below
            attributes.remove(protocol::KEY_OBJECT_ID);
        }

        if (attributes.contains(protocol::KEY_CREATED_AT))
        {
            createdAt_ = datetime(attributes.getString(protocol::KEY_CREATED_AT));

            attributes.remove(protocol::KEY_CREATED_AT);
        }

        if (attributes.contains(protocol::KEY_UPDATED_AT))
        {
            updatedAt_ = datetime(attributes.getString(protocol::KEY_UPDATED_AT));

            attributes.remove(protocol::KEY_UPDATED_AT);
        }

        for (auto & a : attributes)
        {
            set(a.first, a.second);
        }
    }

    bool Object::is_valid() const
    {
        return !objectId_.empty();
    }

    string Object::id() const
    {
        return objectId_;
    }

    time_t Object::createdAt() const
    {
        return createdAt_;
    }

    time_t Object::updatedAt() const
    {
        return updatedAt_;
    }

    Value Object::get(const string &key) const
    {
        if (!attributes_.contains(key))
            throw Exception(key + " not found.");

        return attributes_.get(key);
    }

    int32_t Object::getInt(const string &key) const
    {
        return get(key).toInt();
    }

    int64_t Object::getInt64(const string &key) const
    {
        return get(key).toInt64();
    }

    double Object::getDouble(const string &key) const
    {
        return get(key).toDouble();
    }

    string Object::getString(const string &key) const
    {
        return get(key).toString();
    }

    Array Object::getArray(const string &key) const
    {
        return get(key).toArray();
    }

    Object *Object::getObject(const string &key) {
        if(fetched_.find(key) != fetched_.end())
            return fetched_[key];

        Value val = get(key);

        if(!val.contains(protocol::KEY_TYPE) || val.getString(protocol::KEY_TYPE) != protocol::TYPE_POINTER)
            return NULL;

        Log::trace("creating fetched object " + key);
        fetched_[key] = Object::create(val.getString(protocol::KEY_CLASS_NAME), val);

        return fetched_[key];
    }

    bool Object::isPointer(const string &key) const {

        Value val = get(key);

        if(!val.contains(protocol::KEY_TYPE)) return false;

        return val.getString(protocol::KEY_TYPE) == protocol::TYPE_POINTER;
    }

    User *Object::getUser(const string &key) {
        return static_cast<User*>(fetched_[key]);
    }

    void Object::set(const string &key, const Value &value)
    {
        attributes_.set(key, value);
    }

    void Object::setInt(const string &key, int32_t value)
    {
        attributes_.setInt(key, value);
    }

    void Object::setInt64(const string &key, int64_t value)
    {
        attributes_.setInt64(key, value);
    }

    void Object::setDouble(const string &key, double value)
    {
        attributes_.setDouble(key, value);
    }

    void Object::setString(const string &key, const string &value)
    {
        attributes_.setString(key, value);
    }

    void Object::setArray(const string &key, const Array &value)
    {
        attributes_.setArray(key, value);
    }

    void Object::setObject(const string &key, const Object &obj)
    {
        Value value;
        value.setString(protocol::KEY_TYPE, protocol::TYPE_POINTER);
        value.setString(protocol::KEY_OBJECT_ID, obj.objectId_);
        value.setString(protocol::KEY_CLASS_NAME, obj.className_);
        attributes_.set(key, value);

        if(fetched_[key] != NULL)
        {
            Log::trace("deleting fetched object " + key);
            delete fetched_[key];
        }
        Log::trace("setting fetched object " + key);
        fetched_[key] = new Object(obj);
    }
    void Object::remove(const string &key)
    {
        attributes_.remove(key);
    }

    bool Object::contains(const string &key) const
    {
        return attributes_.contains(key);
    }

    void Object::add(const string &key, const Value &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.get(key) != value)
            attributes_.add(key, value);
    }

    void Object::addInt(const string &key, int32_t value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getInt(key) != value)
            attributes_.addInt(key, value);
    }

    void Object::addInt64(const string &key, int64_t value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getInt64(key) != value)
            attributes_.addInt64(key, value);
    }

    void Object::addDouble(const string &key, double value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getDouble(key) != value)
            attributes_.addDouble(key, value);
    }

    void Object::addString(const string &key, const string &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getString(key) != value)
            attributes_.addString(key, value);
    }

    void Object::addArray(const string &key, const Array &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getArray(key) != value)
            attributes_.addArray(key, value);
    }

    string Object::className() const
    {
        return className_;
    }

    bool Object::save()
    {
        Value response;

        Client client;

        client.setPayload(attributes_.toString());

        try
        {
            /* build the request based on the id */
            if (objectId_.empty())
            {
                client.post(format("{0}/{1}", protocol::OBJECTS_PATH, className_));
            }
            else
            {
                client.put(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));
            }

            Log::trace("saving: " + client.getPayload());

            response = client.getResponseValue();

            Log::trace("saved: " + response.toString());
        }
        catch (const exception &e)
        {
            Log::trace(e.what());
            return false;
        }

        /* merge the result with the object */
        merge(response);

        dataAvailable_ = true;

        return true;
    }

    std::thread Object::saveInBackground(std::function<void(Object *)> callback)
    {
        return std::thread([&]()
        {
            if (save() && callback != nullptr)
                callback(this);
        });
    }

    bool Object::refresh()
    {
        if (objectId_.empty())
        {
            return false;
        }

        Client client;
        Value response;

        try
        {

            client.get(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

            response = client.getResponseValue();

            Log::trace("refresh: " + response.toString());
        }
        catch(const exception &e) {
            Log::trace(e.what());
            return false;
        }

        merge(response);

        return (dataAvailable_ = true);
    }

    bool Object::fetch()
    {
        if (objectId_.empty())
        {
            return false;
        }

        if(!contains(protocol::KEY_TYPE))
        {
            return false;
        }

        string type = getString(protocol::KEY_TYPE);

        if(type != protocol::TYPE_POINTER)
        {
            return false;
        }

        Client client;

        Value response;

        try
        {

            client.get(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

            response = client.getResponseValue();

            Log::trace("fetch: " + response.toString());
        }
        catch(const exception &e) {

            Log::debug(e.what());
            return false;
        }

        merge(response);

        return (dataAvailable_ = true);

    }

    std::thread Object::fetchInBackground(std::function<void(Object *)> callback)
    {
        return std::thread([&]()
        {
            if (fetch() && callback != nullptr)
                callback(this);
        });
    }

    bool Object::saveAll(std::vector<Object> objects)
    {
        bool success = true;

        for (auto & obj : objects)
        {
            success = success && obj.save();
        }

        return success;
    }

    std::thread Object::saveAllInBackground(std::vector<Object> objects, std::function<void()> callback)
    {
        return std::thread([&]()
        {
            if (saveAll(objects) && callback != nullptr)
                callback();
        });
    }

    bool Object::de1ete()
    {
        Client client;

        if (objectId_.empty())
        {
            return false;
        }

        /* do the deed */
        try
        {
            client.de1ete(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));
        }
        catch (const exception &e)
        {
            Log::trace(e.what());
            return false;
        }

        return true;
    }

    std::thread Object::destroyInBackground(std::function<void(Object *)> callback)
    {
        return std::thread([&]()
        {
            if (de1ete() && callback != nullptr)
                callback(this);
        });
    }

    bool Object::isDataAvailable() const
    {
        return dataAvailable_;
    }

    Pointer Object::toPointer() const
    {
        if(isNew())
            throw Exception("object uninitialized");

        return Pointer(*this);
    }

    bool Object::operator==(const Object &other) const
    {
        if(isNew())
        {
            return other.isNew() && this == &other;
        }

        return objectId_ == other.objectId_;
    }

    bool Object::operator!=(const Object &other) const
    {
        return !Object::operator==(other);
    }

    bool Object::operator==(const Pointer &other) const
    {
        if(isNew()) return false;

        return objectId_ == other.id();
    }

    bool Object::operator!=(const Pointer &other) const
    {
        return !Object::operator==(other);
    }

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

