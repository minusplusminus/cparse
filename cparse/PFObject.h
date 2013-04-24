#ifndef ARG3_CPARSE_OBJECT_H
#define ARG3_CPARSE_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <cparse/PFValue.h>
#include <thread>

using namespace arg3;

namespace cparse
{
    class PFObject;

    class PFObject
    {
    public:
        static PFObject *objectWithClassName(const std::string &className);

        static PFObject *objectWithClassName(const std::string &className, const PFValue &attributes);

        static bool saveAll(std::vector<PFObject> objects);

        static std::thread saveAllInBackground(std::vector<PFObject> objects, std::function<void()> callback = nullptr);

        PFObject(const std::string &className);

        std::string keys() const;

        bool is_valid() const;

        PFValue get(const std::string &key) const;
        int32_t getInt(const std::string &key) const;
        int64_t getInt64(const std::string &key) const;
        double getDouble(const std::string &key) const;
        PFArray getArray(const std::string &key) const;
        string getString(const std::string &key) const;

        void set(const std::string &key, const PFValue &value);
        void setInt(const std::string &key, int32_t value);
        void setInt64(const std::string &key, int64_t value);
        void setDouble(const std::string &key, double value);
        void setString(const std::string &key, const std::string& value);
        void setArray(const std::string &key, const PFArray& value);

        void remove(const std::string &key);
        bool contains(const std::string &key) const;

        void add(const std::string &key, const PFValue &value);
        void addInt(const std::string &key, int32_t value);
        void addInt64(const std::string &key, int64_t value);
        void addDouble(const std::string &key, double value);
        void addString(const std::string &key, const std::string& value);
        void addArray(const std::string &key, const PFArray& value);

        bool save();
        bool fetch();
        bool destroy();

        std::thread saveInBackground(std::function<void(PFObject*)> callback = nullptr);
        std::thread fetchInBackground(std::function<void(PFObject*)> callback = nullptr);
        std::thread destroyInBackground(std::function<void(PFObject*)> callback = nullptr);

        std::string id() const;

        time_t createdAt() const;

        time_t updatedAt() const;

        bool isDataAvailable() const;

        bool isNew() const;

    protected:
        void merge(PFValue attributes);
    private:
        std::string className_;
        time_t createdAt_;
        std::string objectId_;
        time_t updatedAt_;
        PFValue attributes_;
        bool dataAvailable_;
    };


}

#endif
