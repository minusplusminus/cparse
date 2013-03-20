#include <cparse/PFObject.h>
#include <cparse/PFException.h>
#include <arg3/format/format.h>
#include "IO.h"
#include "Util.h"

using namespace std;

namespace cparse
{
	static bool validate_class_name(const string &value) {

		/* check first char is an alpha char */
		auto c = value.begin();

		if(c == value.end() || !isalpha(*c)) return false;

		/* rest of the string has to be alpha numeric */
		while(++c != value.end()) {
			if(!isalnum(*c))
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
		if(!validate_class_name(className_))
			throw PFException("invalid class name");
	}

	void PFObject::merge(PFValue attributes)
	{	
		if(attributes.contains("objectId"))
		{
			objectId_ = attributes.get_str("objectId");

			attributes.remove("objectId");
		}

		if(attributes.contains("createdAt"))
		{
			createdAt_ = datetime(attributes.get_str("createdAt"));

			attributes.remove("createdAt");
		}

		if(attributes.contains("updatedAt"))
		{
			updatedAt_ = datetime(attributes.get_str("updatedAt"));

			attributes.remove("updatedAt");
		}

		for(auto &a : attributes)
		{
			set(a.first, a.second);
		}
	}

	bool PFObject::is_valid() const {
		return !objectId_.empty();
	}

	string PFObject::id() const {
		return objectId_;
	}

	time_t PFObject::createdAt() const {
		return createdAt_;
	}

	time_t PFObject::updatedAt() const {
		return updatedAt_;
	}

	PFValue PFObject::get(const string &key) const 
	{
		if(!attributes_.contains(key))
			throw PFException(key + " not found.");

		return attributes_.get(key);
	}

	int32_t PFObject::get_int(const string &key) const 
	{
		return get(key).to_int();
	}

	int64_t PFObject::get_int64(const string &key) const 
	{
		return get(key).to_int64();
	}

	double PFObject::get_double(const string &key) const 
	{
		return get(key).to_double();
	}

	string PFObject::get_str(const string &key) const 
	{
		return get(key).to_str();
	}

	PFArray PFObject::get_array(const string &key) const 
	{
		return get(key).to_array();
	}

	void PFObject::set(const string &key, const PFValue &value)
	{
		attributes_.set(key, value);
	}

	void PFObject::set_int(const string &key, int32_t value)
	{
		attributes_.set_int(key, value);
	}

	void PFObject::set_int64(const string &key, int64_t value)
	{
		attributes_.set_int64(key, value);
	}

	void PFObject::set_double(const string &key, double value)
	{
		attributes_.set_double(key, value);
	}

	void PFObject::set_str(const string &key, const string & value)
	{
		attributes_.set_str(key, value);
	}

	void PFObject::set_array(const string &key, const PFArray& value)
	{
		attributes_.set_array(key, value);
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

	void PFObject::add_int(const string &key, int32_t value)
	{
		attributes_.add_int(key, value);
	}

	void PFObject::add_int64(const string &key, int64_t value)
	{
		attributes_.add_int64(key, value);
	}

	void PFObject::add_double(const string &key, double value)
	{
		attributes_.add_double(key, value);
	}

	void PFObject::add_str(const string &key, const string& value)
	{
		attributes_.add_str(key, value);
	}

	void PFObject::add_array(const string &key, const PFArray& value)
	{
		attributes_.add_array(key, value);
	}

	bool PFObject::save() 
	{
		PFResponse response;
		PFRequest request;

	    /* build the request based on the id */
		if(objectId_.empty()) {
			request.setPath(format("classes/{0}", className_));

			request.setMethod(kPFRequestPost);
		}
		else {
			request.setPath(format("classes/{0}/{1}", className_, objectId_));

			request.setMethod(kPFRequestPut);
		}

		request.setPayload(attributes_.to_str());

	    /* do the deed */
	    try {
			response = request.getResponse();
		}
		catch(const PFException &e) {
			return false;
		}

	    /* merge the result with the object */
	    merge(response.getValue());

		return true;
	}

	std::thread PFObject::saveInBackground(std::function<void(PFObject*)> callback)
	{
		return std::thread([&](){ 
			if(save() && callback != nullptr)
				callback(this);
		});
	}

	bool PFObject::refresh()
	{
		PFRequest request;
	    PFResponse response;

	    if(objectId_.empty()) {
	        return false;
	    }

	    request.setPath(format("classes/{0}/{1}", className_, objectId_));

	    request.setMethod(kPFRequestGet);

	    /* do the deed */
	    try 
	    {
	    	response = request.getResponse();
		}
	    catch(const PFException &e)
	    {
	        return false;
	    }

	    /* merge the response with the object */
	    merge(response.getValue());

	    return true;
	}

	std::thread PFObject::refreshInBackground(std::function<void(PFObject*)> callback)
	{
		return std::thread([&]() {
			if(refresh() && callback != nullptr)
				callback(this);
		});
	}

	bool PFObject::saveAll(std::vector<PFObject> objects)
	{
		bool success = true;

		for(auto &obj : objects) {
			success = success && obj.save();
		}

		return success;
	}

	std::thread PFObject::saveAllInBackground(std::vector<PFObject> objects, std::function<void()> callback)
	{
		return std::thread([&]() {
			if(saveAll(objects) && callback != nullptr)
				callback();
		});
	}

	bool PFObject::destroy()
	{
		PFRequest request;

	    if(objectId_.empty()) {
	        return false;
	    }

	    request.setPath(format("classes/{0}/{1}", className_, objectId_));

	    request.setMethod(kPFRequestDelete);

	    /* do the deed */
	    try 
	    {
	    	request.perform();
		}
	    catch(const PFException &e)
	    {
	        return false;
	    }

	    return true;
	}

	std::thread PFObject::destroyInBackground(std::function<void(PFObject*)> callback)
	{
		return std::thread([&]() {
			if(destroy() && callback != nullptr)
				callback(this);
		});
	}

}

