#ifndef ARG3_CPARSE_IO_H
#define ARG3_CPARSE_IO_H

#include <string>
#include <curl/curl.h>
#include <cparse/PFValue.h>

using namespace std;

namespace cparse
{
	typedef enum
	{
		kPFRequestGet,
		kPFRequestPost,
		kPFRequestPut,
		kPFRequestDelete
	} 
	PFRequestMethod;

	class PFResponse
	{
		friend class PFRequest;
	public:

		PFResponse();
		PFResponse(const PFResponse &other);
		~PFResponse();
		PFResponse& operator=(const PFResponse& other);
		bool is_valid() const;

		PFValue getValue() const;

		friend size_t curl_append_response_callback(void *ptr, size_t size, size_t nmemb, PFResponse *s);

	private:
		int code_;
		string text_;
	};

	class PFRequest
	{
	public:
		PFRequest();
		PFRequest(PFRequestMethod method, const string &path);
		PFResponse getResponse() const;
		bool is_valid() const;
		void setMethod(PFRequestMethod method);
		void setPath(const string &path);
		string getPath() const;
		PFRequestMethod getMethod() const;
		void setPayload(const string &value);
		string getPayload() const;
	protected:
		void set_curl(CURL *curl) const;
	private:
		string path_;
		string payload_;
		PFRequestMethod method_;
	};

}

#endif

