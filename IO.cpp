#include "IO.h"
#include <cparse/Parse.h>
#include <cassert>
#include <cparse/PFException.h>
#include "Protocol.h"

namespace cparse
{
    extern string cparse_app_id_;
    extern string cparse_api_key_;

    PFRequest::PFRequest()
    {

    }

    PFResponse::PFResponse() : code_(-1)
    {

    }

    PFResponse::PFResponse(const PFResponse &other) : code_(other.code_), text_(other.text_)
    {

    }

    PFResponse::~PFResponse() {
    }

    PFResponse& PFResponse::operator=(const PFResponse& other) {
        if(this != &other)
        {
            code_ = other.code_;
            text_ = other.text_;
        }
        return *this;
    }

    bool PFRequest::is_valid() const
    {
        return !path_.empty();
    }

    void PFRequest::set_curl(CURL *curl) const
    {
        char buf[BUFSIZ + 1];
        struct curl_slist *headers = NULL;

        snprintf(buf, BUFSIZ, "https://%s/%s/%s", protocol::HOST, protocol::VERSION, path_.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, buf);

        assert(!cparse_app_id_.empty() && !cparse_api_key_.empty());

        snprintf(buf, BUFSIZ, "%s: %s", protocol::HEADER_APP_ID, cparse_app_id_.c_str());

        headers = curl_slist_append(headers, buf);

        snprintf(buf, BUFSIZ, "X-Parse-REST-API-Key: %s", cparse_api_key_.c_str());

        headers = curl_slist_append(headers, buf);

        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    }


    size_t curl_append_response_callback(void *ptr, size_t size, size_t nmemb, PFResponse *s)
    {
        assert(s != NULL);

        const size_t new_len = size * nmemb;

        char buf[new_len+1];

        memcpy(buf, ptr, size * nmemb);

        buf[new_len] = '\0';

        s->text_.append(buf);

        return new_len;
    }

    void PFRequest::perform() const {
        getResponse();
    }

    PFResponse PFRequest::getResponse() const
    {
        CURL *curl;
        CURLcode res;
        PFResponse response;

        curl = curl_easy_init();
        if (curl == NULL)
        {
            throw PFException("unable to initialize request");
        }

        switch (method_)
        {
        case kPFRequestPost:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload_.size());
            break;
        case kPFRequestPut:
            curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload_.size());
            break;
        case kPFRequestDelete:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        default:
            break;
        }

        set_curl(curl);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_append_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            throw PFException("problem requesting with curl");

        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &response.code_);

        /* always cleanup */
        curl_easy_cleanup(curl);

        return response;
    }

    PFValue PFResponse::getValue() const
    {
        PFValue obj;

        if(!obj.parse(text_))
            throw PFException("reponse is not a valid response");

        if(obj.contains("error")) {
            throw PFException(obj.getString("error"));
        }

        return obj;
    }

    void PFRequest::setMethod(PFRequestMethod method) {
        method_ = method;
    }

    void PFRequest::setPath(const string &path) {
        path_ = path;
    }

    string PFRequest::getPath() const {
        return path_;
    }

    PFRequestMethod PFRequest::getMethod() const {
        return method_;
    }

    void PFRequest::setPayload(const string &value) {
        payload_ = value;
    }

    string PFRequest::getPayload() const {
        return payload_;
    }
}
