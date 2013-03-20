#ifndef ARG3_CPARSE_USER_H
#define ARG3_CPARSE_USER_H

#include <string>

using namespace std;

namespace cparse
{
	class PFUser
	{
	public:
		static PFUser *currentUser();
		static void logout();
		static void automaticUser(bool value);
		string username() const;
		void setUsername( const string &value);
		string email() const;
		void setEmail(const string &value);
		string sessionToken() const;
		void setPassword(const string &value);
		bool isNew() const;
	private:
		static PFUser *currentUser_;

		string username_;
		string password_;
		string email_;
		string sessionToken_;
		bool isNew_;
		static bool automaticUser_;

	};
}

#endif
