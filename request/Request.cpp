//
// Created by ssi on 28.04.15.
//

#include "Request.h"
#include <string.h>
#include "../renderexception.h"
#include <iostream>

namespace vkmes
{
	Request::Request(const char *_clientId, const char *_secretKey, const char *_token)
	{
		if (_secretKey && _token && _clientId)
		{
			secretKey = new char[strlen(_secretKey)];
			strcpy(secretKey, _secretKey);
			token = new char[strlen(_token)];
			stpcpy(token, _token);
			clientId = new char[strlen(_clientId)];
			strcpy(clientId, _clientId);
		}
		else
		{
			LOG_TRACE("Secret Key or Token or ClientId is NULL")
		}
	}

//	Request::Request(const char *_clientId, const char *_secretKey, const char *_code, const char *_token) : Request(_clientId, _secretKey, _code)
//	{
//		if (_token)
//		{
//			token = new char[strlen(_token)];
//			stpcpy(token, _token);
//		}
//		else
//		{
//			LOG_TRACE("Token is NULL")
//		}
//	}

	Request::~Request()
	{
		if (clientId)
			delete[] clientId;
		if (secretKey)
			delete[] secretKey;
		if (code)
			delete[] code;
		if (token)
			delete[] token;
		if (buffer)
			delete buffer;
	}

//	char *Request::getToken() const
//	{
//		CURL *curl;
//		CURLcode result;
//		curl = curl_easy_init();
//		char errorBuffer[CURL_ERROR_SIZE];
//		std::string buffer;
//
//		if (curl)
//		{
//			char *url = new char[strlen("https://oauth.vk.com/access_token/")];
//			strcpy(url, "https://oauth.vk.com/access_token/");
//			url = addGetParam(url, "client_id", clientId);
//			url = addGetParam(url, "secret_key", secretKey);
//			url = addGetParam(url, "code", code);
//			url = addGetParam(url, "redirect_uri", "http://api.vk.com/blank.html");
//			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
//			curl_easy_setopt(curl, CURLOPT_URL, url);
//			curl_easy_setopt(curl, CURLOPT_HEADER, 0);
//			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResp);
//			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
//			result = curl_easy_perform(curl);
//			if (result == CURLE_OK)
//			{
//				std::cout << buffer << "\n";
////				Json json(buffer);
////				std::string tokenString = json.getValueByKey("token");
////				if (tokenString != "")
////				{
//				char *tokenCharptr = new char[1];
////					strcpy(tokenCharptr, tokenString.c_str());
////					curl_easy_cleanup(curl);
////					delete[] url;
//				return tokenCharptr;
////				return "12";
////				}
//			}
//			else
//			{
//				std::cout << "Ошибка! " << errorBuffer << std::endl;
//				delete[] url;
//			}
//		}
//		curl_easy_cleanup(curl);
//		return NULL;
//	}

	char *Request::addGetParam(char *link, const char *pKey, const char *val) const
	{
		char *keyValString = new char[strlen(pKey) + 2 + strlen(val)];
		strcpy(keyValString, pKey);
		strcat(keyValString, "=");
		strcat(keyValString, val);
		size_t l = strlen(link) + 2 + strlen(keyValString);
		char *temp = new char[l];
		strcpy(temp, link);
		if (strchr(link, '?'))
		{
			strcat(temp, "&");
		}
		else
		{
			strcat(temp, "?");
		}
		strcat(temp, keyValString);
		delete[] keyValString;
		delete[] link;
		link = new char[l];
		strcpy(link, temp);
		delete[] temp;
		return link;
	}

	int Request::writeResp(char *data, size_t size, size_t nmemb, std::string *buffer)
	{
		//переменная - результат, по умолчанию нулевая
		int result = 0;
		//проверяем буфер
		if (buffer != NULL)
		{
			//добавляем к буферу строки из data, в количестве nmemb
			buffer->append(data, size * nmemb);
			//вычисляем объем принятых данных
			result = size * nmemb;
		}
		//вовзращаем результат
		return result;
	}

//	bool Request::auth() const
//	{
//		char *token = getToken();
//		return false;
//	}

	void *Request::req(const char *uri, Stack< sp > &_paramStack, const bool paramType)
	{
		CURL *curl;
		CURLcode result;
		curl = curl_easy_init();
		char errorBuffer[CURL_ERROR_SIZE];
		if (buffer)
			delete buffer;
		buffer = new std::string;
		if (curl)
		{
			char *url = new char[strlen(uri) + 1];
			strcpy(url, uri);
			if (!paramType) //судя по всему, это параметры get
			{
				while (_paramStack.getCount())
				{
					sp sp1;
					sp1 = _paramStack.pop();
					url = addGetParam(url, sp1.key, sp1.value);
				}
				url = addGetParam(url, "access_token", token);

			}
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_HEADER, 0); //TODO уметь редактировать через параметр
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResp);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
			result = curl_easy_perform(curl);
			if (result != CURLE_OK)
			{
				std::stringstream ss;
				ss << (int) result;
				*buffer = "{\"curl_error\":" + ss.str() + "}";
			}
			curl_easy_cleanup(curl);
			return buffer;
		}

		*buffer = "{\"curl_error\":\"curl not init\"}";
		return buffer;
	}

	Request::Request(const char *_token)
	{
		token = new char[strlen(_token)];
		stpcpy(token, _token);
		clientId = NULL;
		secretKey = NULL;
		code = NULL;
		buffer = NULL;
	}
}
