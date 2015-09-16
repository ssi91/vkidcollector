//
// Created by ssi on 28.04.15.
//

#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

#include <curl/curl.h>
#include <string>
#include "../Stack.h"
#include <sstream>
#include <string.h>
#include "../renderexception.h"
#include <iostream>
#include <unistd.h>

namespace vkmes
{
	struct sp
	{
		char *key;
		char *value;

		sp(const char *_key = NULL, const char *_value = NULL);
		sp(const char *_key, const int _value);
		sp(const sp &_sp);
		~sp();
		sp &operator=(const sp &_sp);
	};

	class Request
	{
	private:
		char *secretKey;
		char *token;
		char *code;
		char *clientId;
		std::string *buffer;
		size_t req_counter;

		char *addGetParam(char *link, const char *pKey, const char *val) const;
		char *getToken() const;

		static const char REQUEST_GET_PARAM = 0;
		static const char REQUEST_POST_PARAM = 1;
		static int writeResp(char *data, size_t size, size_t nmemb, std::string *buffer);
		Request(const char *_clientId, const char *_secretKey, const char *_token);
	public:
		Request(const char *_token);
		~Request();
		bool auth() const;
		std::string * req(const char *uri, Stack< sp > &_paramStack, const bool paramType = 0);
	};

}
#endif //CLIENT_REQUEST_H
