#ifndef ARG3_CPARSE_OBJECT_H
#define ARG3_CPARSE_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include "value.h"
#include <thread>

using namespace arg3;

namespace cparse
{
    class Object;
    class User;
    class Pointer;

    class Object
    {
    public:

        static Object *create(const std::string &className);

        static Object *create(const std::string &className, const Value &attributes);

        static Object *createWithoutData(const std::string &className, const std::string &objectId);

        static bool saveAll(std::vector<Object> objects);

        static std::thread saveAllInBackground(std::vector<Object> objects, std::function<void()> callback = nullptr);

        Object(const std::string &className);
        virtual ~Object();
        Object(const Object &other);
        Object &operator=(const Object &other);

        std::string keys() const;

        bool is_valid() const;

        Value get(const std::string &key) const;
        int32_t getInt(const std::string &key) const;
        int64_t getInt64(const std::string &key) const;
        double getDouble(const std::string &key) const;
        Array getArray(const std::string &key) const;
        string getString(const std::string &key) const;
        Object *getObject(const std::string &key);
        User *getUser(const std::string &key);

        void set(const std::string &key, const Value &value);
        void setInt(const std::string &key, int32_t value);
        void setInt64(const std::string &key, int64_t value);
        void setDouble(const std::string &key, double value);
        void setString(const std::string &key, const std::string &value);
        void setArray(const std::string &key, const Array &value);
        void setObject(const std::string &key, const Object &obj);

        void remove(const std::string &key);
        bool contains(const std::string &key) const;

        void add(const std::string &key, const Value &value, bool unique = false);
        void addInt(const std::string &key, int32_t value, bool unique = false);
        void addInt64(const std::string &key, int64_t value, bool unique = false);
        void addDouble(const std::string &key, double value, bool unique = false);
        void addString(const std::string &key, const std::string &value, bool unique = false);
        void addArray(const std::string &key, const Array &value, bool unique = false);

        bool save();
        bool fetch();
        bool de1ete();
        bool refresh();

        std::thread saveInBackground(std::function<void(Object *)> callback = nullptr);
        std::thread fetchInBackground(std::function<void(Object *)> callback = nullptr);
        std::thread destroyInBackground(std::function<void(Object *)> callback = nullptr);

        std::string id() const;

        time_t createdAt() const;

        time_t updatedAt() const;

        std::string className() const;

        bool isDataAvailable() const;

        bool isPointer(const string &key) const;

        bool isNew() const;

        Pointer toPointer() const;

        bool operator==(const Object &other) const;
        bool operator!=(const Object &other) const;

        bool operator==(const Pointer &other) const;
        bool operator!=(const Pointer &other) const;

    protected:
        void merge(Value attributes);
        void copy_fetched(const Object &obj);
    private:
        std::string className_;
        time_t createdAt_;
        std::string objectId_;
        time_t updatedAt_;
        Value attributes_;
        bool dataAvailable_;
        map<std::string,Object*> fetched_;
    };

    class Pointer
    {
    public:
        Pointer(const Object &obj);
        Value toValue() const;
        string id() const;
        string className() const;
        bool operator==(const Pointer &other) const;
        bool operator!=(const Pointer &other) const;
        bool operator==(const Object &other) const;
        bool operator!=(const Object &other) const;
    private:
        Value value_;
    };
}

#endif