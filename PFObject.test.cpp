#include <cparse/Parse.h>
#include <cparse/PFObject.h>
#include <cparse/PFException.h>
#include <igloo/igloo.h>
#include <typeinfo>

using namespace cparse;
using namespace igloo;
using namespace std;

namespace cparse
{
    bool validate_class_name(const string &value);
}

Context(PFObjectTest)
{
    PFObject *obj_;

    bool backgroundSuccess_;

    void SetUp()
    {
        obj_ = PFObject::create("TestCase");
    }

    void TearDown()
    {
        obj_->destroy();
        delete obj_;
    }

    Spec(ValidClassName)
    {
        bool value = validate_class_name("_@#$@#$^*");

        Assert::That(false, Equals(value));

        value = validate_class_name("1234ABC");

        Assert::That(false, Equals(value));

        value = validate_class_name("ACBD123412");

        Assert::That(true, Equals(value));
    }

    Spec(get)
    {
        AssertThrows(cparse::PFException, obj_->get("testVal1"));

        PFValue sValue("test string 1");

        obj_->set("testVal1", sValue);

        Assert::That(sValue, Equals(obj_->get("testVal1")));

        PFValue iValue(1234567LL);

        obj_->set("testVal2", iValue);

        Assert::That(iValue, Equals(obj_->get("testVal2")));

        PFValue bValue(true);

        obj_->set("testVal3", bValue);

        Assert::That(bValue, Equals(obj_->get("testVal3")));
    }

    Spec(save)
    {

        obj_->setInt("score", 1234);

        obj_->setString("name", "spok");

        Assert::That(obj_->save(), Equals(true));

        Assert::That(obj_->is_valid(), Equals(true));
    }

    Spec(saveInBackground)
    {
        obj_->setInt("score", 4567);

        obj_->setString("status", "saved in background");

        backgroundSuccess_ = false;

        std::thread thread = obj_->saveInBackground([&](PFObject * obj)
        {
            backgroundSuccess_ = true;
        });

        thread.join();

        Assert::That(backgroundSuccess_, Equals(true));
    }


    Spec(setInt)
    {
        obj_->setInt("id", 1234);

        Assert::That(obj_->getInt("id"), Equals(1234));
    }

    Spec(setString)
    {
        obj_->setString("id", "replaced");

        Assert::That(obj_->getString("id"), Equals("replaced"));
    }

    Spec(remove)
    {
        PFValue value(1234);

        obj_->set("main", value);

        Assert::That(obj_->getString("main"), Equals("1234"));

        obj_->remove("main");

        Assert::That(obj_->contains("main"), Equals(false));

    }

};

class MySubclass : public PFObject
{
public:
    MySubclass() : PFObject("MySubclass")
    {

    }
};

Context(SubclassTest)
{
    Spec(className)
    {
        MySubclass subclass;

        Assert::That(subclass.className(), Equals("MySubclass"));
    }

    Spec(setInt)
    {
        MySubclass subclass;

        int value = rand() % 1000 + 1;

        subclass.setInt("val1", value);

        Assert::That(subclass.getInt("val1"), Equals(value));

        Assert::That(subclass.save(), Equals(true));
    }
};
