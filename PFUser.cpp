#include <cparse/PFUser.h>

namespace cparse
{
	PFUser *PFUser::currentUser_ = NULL;

	bool PFUser::automaticUser_ = false;

	PFUser *PFUser::currentUser() {
		if(currentUser_ == NULL && automaticUser_)
		{
			currentUser_ = new PFUser();
		}
		return currentUser_;
	}

	void PFUser::logout() {
		if(currentUser_) 
		{
			delete currentUser_;
			currentUser_ = NULL;
		}
	}

	void PFUser::automaticUser(bool value) {
		automaticUser_ = value;
	}

	string PFUser::username() const {
		return username_;
	}
	void PFUser::setUsername( const string &value) {
		username_ = value;
	}
	string PFUser::email() const {
		return email_;
	}
	void PFUser::setEmail(const string &value) {
		email_ = value;
	}
	string PFUser::sessionToken() const {
		return sessionToken_;
	}
	void PFUser::setPassword(const string &value) {
		password_ = value;
	}
	bool PFUser::isNew() const {
		return isNew_;
	}
}