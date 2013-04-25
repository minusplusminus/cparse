#include <cparse/PFObject.h>
#include <cparse/PFException.h>
#include <arg3/format/format.h>
#include "IO.h"
#include "Util.h"
#include <cparse/PFUser.h>
#include "Protocol.h"
#include "PFLog.h"

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

    PFObject *PFObject::create(const string &className)
    {
        PFObject *obj = new PFObject(className);

        return obj;
    }

    PFObject *PFObject::create(const string &className, const PFValue &attributes)
    {
        PFObject *obj = new PFObject(className);

        obj->merge(attributes);

        return obj;
    }

    PFObject *PFObject::createWithoutData(const string &className, const string &objectId)
    {
        PFObject *obj = new PFObject(className);

        obj->objectId_ = objectId;

        return obj;
    }

    PFObject::PFObject(const string &className) : className_(className),
        createdAt_(0),
        objectId_(),
        updatedAt_(0),
        attributes_(),
        dataAvailable_(false)
    {
        if (!validate_class_name(className_))
            throw PFException("invalid class name");
    }

    PFObject::PFObject(const PFObject &other) : className_(other.className_),
        createdAt_(other.createdAt_),
        objectId_(other.objectId_),
        updatedAt_(other.updatedAt_),
        attributes_(other.attributes_),
        dataAvailable_(other.dataAvailable_)
    {
        copy_fetched(other);
    }

    PFObject::~PFObject() {
        for(auto &e : fetched_)
        {
            if(e.second) {
                PFLog::trace("deleting fetched object " + e.first);
                delete e.second;
            }
        }
    }

    void PFObject::copy_fetched(const PFObject &obj)
    {
        for(auto &e : obj.fetched_) {
            if(fetched_[e.first] != NULL) {
                PFLog::trace("deleting fetched object " + e.first);
                delete fetched_[e.first];
            }
            PFLog::trace("copying fetched object " + e.first);
            fetched_[e.first] = new PFObject(*e.second);
        }
    }

    PFObject &PFObject::operator=(const PFObject &other) {
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

    bool PFObject::isNew() const {
        return objectId_.empty();
    }

    void PFObject::merge(PFValue attributes)
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

    bool PFObject::is_valid() const
    {
        return !objectId_.empty();
    }

    string PFObject::id() const
    {
        return objectId_;
    }

    time_t PFObject::createdAt() const
    {
        return createdAt_;
    }

    time_t PFObject::updatedAt() const
    {
        return updatedAt_;
    }

    PFValue PFObject::get(const string &key) const
    {
        if (!attributes_.contains(key))
            throw PFException(key + " not found.");

        return attributes_.get(key);
    }

    int32_t PFObject::getInt(const string &key) const
    {
        return get(key).toInt();
    }

    int64_t PFObject::getInt64(const string &key) const
    {
        return get(key).toInt64();
    }

    double PFObject::getDouble(const string &key) const
    {
        return get(key).toDouble();
    }

    string PFObject::getString(const string &key) const
    {
        return get(key).toString();
    }

    PFArray PFObject::getArray(const string &key) const
    {
        return get(key).toArray();
    }

    PFObject *PFObject::getObject(const string &key) {
        if(fetched_.find(key) != fetched_.end())
            return fetched_[key];

        PFValue val = get(key);

        if(!val.contains(protocol::KEY_TYPE) || val.getString(protocol::KEY_TYPE) != protocol::TYPE_POINTER)
            return NULL;

        PFLog::trace("creating fetched object " + key);
        fetched_[key] = PFObject::create(val.getString(protocol::KEY_CLASS_NAME), val);

        return fetched_[key];
    }

    bool PFObject::isPointer(const string &key) const {

        PFValue val = get(key);

        if(!val.contains(protocol::KEY_TYPE)) return false;

        return val.getString(protocol::KEY_TYPE) == protocol::TYPE_POINTER;
    }

    PFUser *PFObject::getUser(const string &key) {
        return static_cast<PFUser*>(fetched_[key]);
    }

    void PFObject::set(const string &key, const PFValue &value)
    {
        attributes_.set(key, value);
    }

    void PFObject::setInt(const string &key, int32_t value)
    {
        attributes_.setInt(key, value);
    }

    void PFObject::setInt64(const string &key, int64_t value)
    {
        attributes_.setInt64(key, value);
    }

    void PFObject::setDouble(const string &key, double value)
    {
        attributes_.setDouble(key, value);
    }

    void PFObject::setString(const string &key, const string &value)
    {
        attributes_.setString(key, value);
    }

    void PFObject::setArray(const string &key, const PFArray &value)
    {
        attributes_.setArray(key, value);
    }

    void PFObject::setObject(const string &key, const PFObject &obj)
    {
        PFValue value;
        value.setString(protocol::KEY_TYPE, protocol::TYPE_POINTER);
        value.setString(protocol::KEY_OBJECT_ID, obj.objectId_);
        value.setString(protocol::KEY_CLASS_NAME, obj.className_);
        attributes_.set(key, value);

        if(fetched_[key] != NULL)
        {
            PFLog::trace("deleting fetched object " + key);
            delete fetched_[key];
        }
        PFLog::trace("setting fetched object " + key);
        fetched_[key] = new PFObject(obj);
    }
    void PFObject::remove(const string &key)
    {
        attributes_.remove(key);
    }

    bool PFObject::contains(const string &key) const
    {
        return attributes_.contains(key);
    }

    void PFObject::add(const string &key, const PFValue &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.get(key) != value)
            attributes_.add(key, value);
    }

    void PFObject::addInt(const string &key, int32_t value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getInt(key) != value)
            attributes_.addInt(key, value);
    }

    void PFObject::addInt64(const string &key, int64_t value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getInt64(key) != value)
            attributes_.addInt64(key, value);
    }

    void PFObject::addDouble(const string &key, double value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getDouble(key) != value)
            attributes_.addDouble(key, value);
    }

    void PFObject::addString(const string &key, const string &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getString(key) != value)
            attributes_.addString(key, value);
    }

    void PFObject::addArray(const string &key, const PFArray &value, bool unique)
    {
        if (!unique || !attributes_.contains(key) || attributes_.getArray(key) != value)
            attributes_.addArray(key, value);
    }

    string PFObject::className() const
    {
        return className_;
    }

    bool PFObject::save()
    {
        PFResponse response;
        PFRequest request;

        /* build the request based on the id */
        if (objectId_.empty())
        {
            request.setPath(format("{0}/{1}", protocol::OBJECTS_PATH, className_));

            request.setMethod(kPFRequestPost);
        }
        else
        {
            request.setPath(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

            request.setMethod(kPFRequestPut);
        }

        request.setPayload(attributes_.toString());

        /* do the deed */
        try
        {
            PFLog::trace("saving: " + request.getPayload());

            response = request.getResponse();

            PFLog::trace("saved: " + response.getString());
        }
        catch (const PFException &e)
        {
            return false;
        }

        /* merge the result with the object */
        merge(response.getValue());

        dataAvailable_ = true;

        return true;
    }

    std::thread PFObject::saveInBackground(std::function<void(PFObject *)> callback)
    {
        return std::thread([&]()
        {
            if (save() && callback != nullptr)
                callback(this);
        });
    }

    bool PFObject::refresh()
    {
        if (objectId_.empty())
        {
            return false;
        }

        PFRequest request(kPFRequestGet, format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

        PFResponse response;

        try
        {
            response = request.getResponse();

            PFLog::trace("refresh: " + response.getString());
        }
        catch(const PFException &e) {
            return false;
        }

        merge(response.getValue());

        return (dataAvailable_ = true);
    }

    bool PFObject::fetch()
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

        PFRequest request(kPFRequestGet, format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

        PFResponse response;

        try
        {
            response = request.getResponse();

            PFLog::trace("fetch: " + response.getString());
        }
        catch(const PFException &e) {

            PFLog::debug(e.what());
            return false;
        }

        merge(response.getValue());

        return (dataAvailable_ = true);

    }

    std::thread PFObject::fetchInBackground(std::function<void(PFObject *)> callback)
    {
        return std::thread([&]()
        {
            if (fetch() && callback != nullptr)
                callback(this);
        });
    }

    bool PFObject::saveAll(std::vector<PFObject> objects)
    {
        bool success = true;

        for (auto & obj : objects)
        {
            success = success && obj.save();
        }

        return success;
    }

    std::thread PFObject::saveAllInBackground(std::vector<PFObject> objects, std::function<void()> callback)
    {
        return std::thread([&]()
        {
            if (saveAll(objects) && callback != nullptr)
                callback();
        });
    }

    bool PFObject::destroy()
    {
        PFRequest request;

        if (objectId_.empty())
        {
            return false;
        }

        request.setPath(format("{0}/{1}/{2}", protocol::OBJECTS_PATH, className_, objectId_));

        request.setMethod(kPFRequestDelete);

        /* do the deed */
        try
        {
            request.perform();
        }
        catch (const PFException &e)
        {
            return false;
        }

        return true;
    }

    std::thread PFObject::destroyInBackground(std::function<void(PFObject *)> callback)
    {
        return std::thread([&]()
        {
            if (destroy() && callback != nullptr)
                callback(this);
        });
    }

    bool PFObject::isDataAvailable() const
    {
        return dataAvailable_;
    }

}

