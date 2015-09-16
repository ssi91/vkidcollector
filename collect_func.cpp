//
// Created by ssi on 16.09.15.
//

#include "collect_func.h"


std::vector< mongo::BSONElement > &collectData(vkmes::Request &request, boost::scoped_ptr< mongo::DBClientBase > &conn)
{

	size_t execute_limit = 25000;
	Stack< vkmes::sp > stack;
	stack.push({"group_id", "tproger"});
	int offset = 0;
	stack.push({"offset", offset});
	std::string *rest = request.req("https://api.vk.com/method/execute.collect", stack);

	mongo::BSONObj *obj = new mongo::BSONObj;
	*obj = mongo::fromjson(*rest);
	std::vector< mongo::BSONElement > *vector = new std::vector< mongo::BSONElement >;
	*vector = (*obj)["response"]["users"].Array();


	if ((*vector).size() < (*obj)["response"]["count"].numberInt())
	{
		size_t usersCount = (*obj)["response"]["count"].numberInt();
		(*vector).reserve(usersCount);
		size_t countOfReq = usersCount / execute_limit;
		if (usersCount % execute_limit == 0)
			--countOfReq;
//		while ((usersCount / execute_limit) - 1)
		for (int i = 0; i < countOfReq; ++i)
		{
			offset += execute_limit;
			stack.push({"group_id", "tproger"});
			stack.push({"offset", offset});

			rest = request.req("https://api.vk.com/method/execute.collect", stack);
			mongo::BSONObj secondObj = mongo::fromjson(*rest);
			std::vector< mongo::BSONElement > secondVector = secondObj["response"]["users"].Array();
			(*vector).insert((*vector).end(), secondVector.begin(), secondVector.end());
		}
	}
	return *vector;

//	mongo::BSONArray bsonArray(obj["response"]["users"].embeddedObject());
//	mongo::BSONArrayBuilder builder;
//	builder << vector[0] << vector[1];
//
//	bsonArray = builder.arr();

//	mongo::BSONObjBuilder objBuilder;
//	objBuilder.appendElements(obj);
//	mongo::BSONObjBuilder sub(objBuilder.subobjStart("response"));
//	sub.append("users", vector).done();


//	obj["response"]["users"] = builder.arr().firstElement();
//	bsonArray.elems(vector);
//	int s = vector[0].numberInt();
//	std::cout << objBuilder.obj()["response"];

}
