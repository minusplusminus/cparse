#include <cparse/type/file.h>
#include "../log.h"
#include "../client.h"
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        File::File(const Value &obj)
        {
            fromValue(obj);
        }

        File::File(const string &fileName, const ContentType &content, const string &contentType) :
            localFileName_(fileName), contentType_(contentType), body_(content)
        {

        }

        File::File()
        {}

        void File::fromValue(const Value &obj)
        {
            if(obj.contains("local_filename"))
                localFileName_ = obj.getString("local_filename");

            if(obj.contains("name"))
                parseFileName_ = obj.getString("name");
            else if(obj.contains("parse_filename"))
                parseFileName_ = obj.getString("parse_filename");

            if(obj.contains("content_type"))
                contentType_ = obj.getString("content_type");

            if(obj.contains("url"))
                url_ = obj.getString("url");

            if(obj.contains("body"))
                body_ = obj.getString("body");
        }

        Value File::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_FILE);

            value.setString("name", parseFileName_);

            value.setString("url", url_);

            return value;
        }

        string File::getLocaleFileName() const
        {
            return localFileName_;
        }
        string File::getParseFileName() const
        {
            return parseFileName_;
        }
        string File::getContentType() const {
            return contentType_;
        }
        File::ContentType File::getContents() const {
            return body_;
        }
        string File::getUrl() const {
            return url_;
        }

        void File::setLocaleFileName(const string &fileName) {
            localFileName_ = fileName;
        }
        void File::setContentType(const string &contentType) {
            contentType_ = contentType;
        }
        void File::setContents(const ContentType &value) {
            body_ = value;
        }

        bool File::save()
        {
            Client client;

            client.addHeader("Content-Type", contentType_);

            client.setPayload(body_);

            Value response;

            try {
                client.post("files/" + localFileName_);

                response = client.getResponseValue();
            }
            catch(const exception &e) {
                Log::trace(e.what());
                return false;
            }
            fromValue(response);
            return true;
        }

    }
}