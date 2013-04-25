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
    class PFUser;

    class PFObject
    {
    public:

        static PFObject *create(const std::string &className);

        static PFObject *create(const std::string &className, const PFValue &attributes);

        static PFObject *createWithoutData(const std::string &className, const std::string &objectId);

        static bool saveAll(std::vector<PFObject> objects);

        static std::thread saveAllInBackground(std::vector<PFObject> objects, std::function<void()> callback = nullptr);

        PFObject(const std::string &className);
        virtual ~PFObject();
        PFObject(const PFObject &other);
        PFObject &operator=(const PFObject &other);

        std::string keys() const;

        bool is_valid() const;

        PFValue get(const std::string &key) const;
        int32_t getInt(const std::string &key) const;
        int64_t getInt64(const std::string &key) const;
        double getDouble(const std::string &key) const;
        PFArray getArray(const std::string &key) const;
        string getString(const std::string &key) const;
        PFObject *getObject(const std::string &key);
        PFUser *getUser(const std::string &key);

        void set(const std::string &key, const PFValue &value);
        void setInt(const std::string &key, int32_t value);
        void setInt64(const std::string &key, int64_t value);
        void setDouble(const std::string &key, double value);
        void setString(const std::string &key, const std::string &value);
        void setArray(const std::string &key, const PFArray &value);
        void setObject(const std::string &key, const PFObject &obj);

        void remove(const std::string &key);
        bool contains(const std::string &key) const;

        void add(const std::string &key, const PFValue &value, bool unique = false);
        void addInt(const std::string &key, int32_t value, bool unique = false);
        void addInt64(const std::string &key, int64_t value, bool unique = false);
        void addDouble(const std::string &key, double value, bool unique = false);
        void addString(const std::string &key, const std::string &value, bool unique = false);
        void addArray(const std::string &key, const PFArray &value, bool unique = false);

        bool save();
        bool fetch();
        bool destroy();
        bool refresh();

        std::thread saveInBackground(std::function<void(PFObject *)> callback = nullptr);
        std::thread fetchInBackground(std::function<void(PFObject *)> callback = nullptr);
        std::thread destroyInBackground(std::function<void(PFObject *)> callback = nullptr);

        std::string id() const;

        time_t createdAt() const;

        time_t updatedAt() const;

        std::string className() const;

        bool isDataAvailable() const;

        bool isPointer(const string &key) const;

        bool isNew() const;

    protected:
        void merge(PFValue attributes);
        void copy_fetched(const PFObject &obj);
    private:
        std::string className_;
        time_t createdAt_;
        std::string objectId_;
        time_t updatedAt_;
        PFValue attributes_;
        bool dataAvailable_;
        map<std::string,PFObject*> fetched_;
    };


}

#endif
