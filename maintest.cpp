//
// Created by ssi on 17.03.15.
//

#include "request/Request.h"
#include <iostream>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/property_tree/ptree.hpp>
#include <mongo/client/dbclient.h>

int main()
{
	mongo::client::GlobalInstance instance;
	std::string fullUri = "mongodb://172.17.0.1:27017";
	std::string errmsg;
	mongo::ConnectionString cs = mongo::ConnectionString::parse(fullUri, errmsg);
	boost::scoped_ptr< mongo::DBClientBase > conn(cs.connect(errmsg));

	//тут уже есть токен
	std::string token = "f49d945923af5f55e04fa7b7c18336cea840c6dcf1cd62f06585bd13ccc57010f5ebb4eaa19a490e515bd";
	vkmes::Request request(token.c_str());

	Stack< vkmes::sp > stack;
	vkmes::sp sp1("group_id", "cxem_net_payalnik");
	vkmes::sp sp2("offset", "0");
	stack.push(sp1);
	stack.push(sp2);
	request.req("https://api.vk.com/method/execute.collect", stack);


	mongo::BSONObj obj = mongo::fromjson(*buffer);
	std::vector< mongo::BSONElement > vector = obj["response"]["users"].Array();
	int s = vector[0].numberInt();

	return 0;
}
//https://oauth.vk.com/access_token?code=348217fc6cc67a100b&client_id=4562232&client_secret=WxjRCrntDupvyqcz321a&redirect_uri=https://oauth.vk.com/blank.html