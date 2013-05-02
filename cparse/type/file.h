#ifndef ARG3_CPARSE_TYPE_FILE_H_
#define ARG3_CPARSE_TYPE_FILE_H_

#include <string>
#include <arg3/string/string.h>
#include "../value.h"

using namespace std;

namespace cparse
{
    namespace type
    {
        class File
        {
        public:
            typedef string ContentType;

            File();
            File(const Value& obj);
            File(const string &fileName, const ContentType &content, const string &contentType);

            Value toValue() const;
            void fromValue(const Value &attributes);

            string getLocaleFileName() const;
            string getParseFileName() const;
            string getContentType() const;
            ContentType getContents() const;
            string getUrl() const;

            void setLocaleFileName(const string &fileName);
            void setContentType(const string &contentType);
            void setContents(const ContentType &value);

            bool save();

        private:
            string localFileName_;
            string parseFileName_;
            string contentType_;
            ContentType body_;
            string url_;
        };
    }
}
#endif
