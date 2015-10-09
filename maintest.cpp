//
// Created by ssi on 17.03.15.
//

#include "request/Request.h"
#include "collect_func.h"
//#include <iostream>
#include <ctime>

int main()
{
	time_t t = time(NULL);
	//инитциализация mongo client
	mongo::client::GlobalInstance instance;
	if (!instance.initialized())
	{
		std::ostringstream statusStream;
		statusStream << instance.status();
		std::string status = statusStream.str();
		LOG_TRACE("failed to initialize the client driver: " + status + "\n");
	}
	std::string fullUri = "mongodb://172.17.0.1:27017";
	std::string errmsg;
	mongo::ConnectionString cs = mongo::ConnectionString::parse(fullUri, errmsg);
	boost::scoped_ptr< mongo::DBClientBase > conn(cs.connect(errmsg));

//	std::cout << conn->exists("elsedb.info") << std::endl;

//	//тут уже есть токен
	std::string token = "3662b061f6c0f327406db8874f59334186ea37adac679f8554f78c829abb341e321c99a197cc2afdc8ad2";

//	//Объект для запросов
	vkmes::Request request(token.c_str());

	char groupid[] = "33105862\0";

	std::vector< mongo::BSONElement > &vectorNewData = collectData(request, conn, groupid);
//	std::cout << vectorNewData[0].numberInt() << std::endl;
	std::vector< unsigned > *v = newcomers(vectorNewData, groupid, conn);
	if (v)
		for (std::vector< unsigned >::const_iterator it = v->begin(); it != v->end(); ++it)
		{
			std::cout << *it << std::endl;
		}
	std::cout << std::endl << (double) clock() / CLOCKS_PER_SEC << std::endl;
	std::cout << time(NULL) - t << std::endl;
	return 0;

}
//https://oauth.vk.com/access_token?code=348217fc6cc67a100b&client_id=4562232&client_secret=WxjRCrntDupvyqcz321a&redirect_uri=https://oauth.vk.com/blank.html