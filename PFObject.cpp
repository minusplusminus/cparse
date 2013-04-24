#include <cparse/PFObject.h>
#include <cparse/PFException.h>
#include <arg3/format/format.h>
#include "IO.h"
#include "Util.h"
#include "Protocol.h"

using namespace std;

namespace cparse
{

    static const char *BASE_PATH = "classes";

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

    PFObject *PFObject::objectWithClassName(const string &className)
    {
        PFObject *obj = new PFObject(className);

        return obj;
    }

    PFObject *PFObject::objectWithClassName(const string &className, const PFValue &attributes)
    {
        PFObject *obj = new PFObject(className);

        obj->merge(attributes);

        return obj;
    }

    PFObject::PFObject(const string &className) : className_(className)
    {
        if (!validate_class_name(className_))
            throw PFException("invalid class name");
    }

    bool PFObject::isNew() const {
        return objectId_.empty();
    }

    void PFObject::merge(PFValue attributes)
    {
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

    void PFObject::remove(const string &key)
    {
        attributes_.remove(key);
    }

    bool PFObject::contains(const string &key) const
    {
        return attributes_.contains(key);
    }

    void PFObject::add(const string &key, const PFValue &value)
    {
        attributes_.add(key, value);
    }

    void PFObject::addInt(const string &key, int32_t value)
    {
        attributes_.addInt(key, value);
    }

    void PFObject::addInt64(const string &key, int64_t value)
    {
        attributes_.addInt64(key, value);
    }

    void PFObject::addDouble(const string &key, double value)
    {
        attributes_.addDouble(key, value);
    }

    void PFObject::addString(const string &key, const string &value)
    {
        attributes_.addString(key, value);
    }

    void PFObject::addArray(const string &key, const PFArray &value)
    {
        attributes_.addArray(key, value);
    }

    bool PFObject::save()
    {
        PFResponse response;
        PFRequest request;

        /* build the request based on the id */
        if (objectId_.empty())
        {
            request.setPath(format("{0}/{1}", BASE_PATH, className_));

            request.setMethod(kPFRequestPost);
        }
        else
        {
            request.setPath(format("{0}/{1}/{2}", BASE_PATH, className_, objectId_));

            request.setMethod(kPFRequestPut);
        }

        request.setPayload(attributes_.toString());

        /* do the deed */
        try
        {
            response = request.getResponse();
        }
        catch (const PFException &e)
        {
            return false;
        }

        /* merge the result with the object */
        merge(response.getValue());

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

    bool PFObject::fetch()
    {
        PFRequest request;
        PFResponse response;

        if (objectId_.empty())
        {
            return false;
        }

        request.setPath(format("{0}/{1}/{2}", BASE_PATH, className_, objectId_));

        request.setMethod(kPFRequestGet);

        /* do the deed */
        try
        {
            response = request.getResponse();
        }
        catch (const PFException &e)
        {
            return false;
        }

        /* merge the response with the object */
        merge(response.getValue());

        dataAvailable_ = true;

        return true;
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

        request.setPath(format("{0}/{1}/{2}", className_, objectId_));

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

