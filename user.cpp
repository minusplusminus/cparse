#include <cparse/user.h>
#include "client.h"
#include "log.h"
#include "protocol.h"

namespace cparse
{
    User *User::currentUser_ = NULL;

    bool User::automaticUser_ = false;

    User *User::currentUser()
    {
        if (currentUser_ == NULL && automaticUser_)
        {
            currentUser_ = new User();
        }
        return currentUser_;
    }

    void User::logout()
    {
        if (currentUser_)
        {
            delete currentUser_;
            currentUser_ = NULL;
        }
    }

    User *User::authenticate(const string &username, const string &password)
    {

        Client client;

        Value body;

        body.setString("username", username);

        body.setString("password", password);

        client.setPayload(body.toString());

        client.get("login");

        Value response;

        try {
            response = client.getResponseValue();
        }
        catch(const exception &e) {
            Log::trace(e.what());
            return NULL;
        }

        if(currentUser_ != NULL)
            delete currentUser_;

        currentUser_ = new User();

        currentUser_->setUsername(username);

        currentUser_->setPassword(password);

        currentUser_->sessionToken_ = response.getString(protocol::KEY_USER_SESSION_TOKEN);

        return currentUser_;
    }

    User::User() : Object(protocol::CLASS_USER)
    {

    }

    void User::enableAutomaticUser()
    {
        automaticUser_ = true;
    }

    string User::getUsername() const
    {
        return username_;
    }
    void User::setUsername( const string &value)
    {
        username_ = value;
    }
    string User::getEmail() const
    {
        return email_;
    }
    void User::setEmail(const string &value)
    {
        email_ = value;
    }
    string User::sessionToken() const
    {
        return sessionToken_;
    }
    void User::setPassword(const string &value)
    {
        password_ = value;
    }
    bool User::isNew() const
    {
        return isNew_;
    }
}