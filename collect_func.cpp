//
// Created by ssi on 16.09.15.
//

#include "collect_func.h"


std::vector< mongo::BSONElement > &collectData(vkmes::Request &request, boost::scoped_ptr< mongo::DBClientBase > &conn, const char *group_id)
{

	size_t execute_limit = 25000;
	Stack< vkmes::sp > stack;
	stack.push({"group_id", group_id});
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
		mongo::BSONObj **secondObj = new mongo::BSONObj *[countOfReq];
		for (int i = 0; i < countOfReq; ++i)
		{
			offset += execute_limit;
			stack.push({"group_id", group_id});
			stack.push({"offset", offset});

			rest = request.req("https://api.vk.com/method/execute.collect", stack);
			secondObj[i] = new mongo::BSONObj;
			*secondObj[i] = mongo::fromjson(*rest);
			std::vector< mongo::BSONElement > secondVector = (*secondObj[i])["response"]["users"].Array();
			(*vector).insert((*vector).end(), secondVector.begin(), secondVector.end());
		}
	}
	return *vector;
}

std::vector< unsigned > *newcomers(const std::vector< mongo::BSONElement > &newData, const char *group_id, boost::scoped_ptr< mongo::DBClientBase > &conn)
{
	//TODO принимать id группы в разном виде
	//TODO вытащить числовой id группы, базы групп будут именоваться строкой из этого id

	std::vector< unsigned > *ncvec = NULL;

	std::string base(group_id);
	std::string meta(base + ".meta");
	std::string data(base + ".data");

	mongo::BSONArrayBuilder arrayBuilder;
	mongo::BSONObjBuilder objBuilder;
	mongo::Query query;
	if (conn->exists(meta))
	{
		//TODO обновение коллекции meta
		ncvec = new std::vector< unsigned >;
		std::auto_ptr< mongo::DBClientCursor > cursor = conn->query(data, query, 0);
		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			std::vector< mongo::BSONElement > elemVector = obj["users"].Array();
			unsigned i = 0, j = 0;
			while (i < newData.size() && j < elemVector.size())
			{
				if (newData[i].numberInt() == elemVector[j].numberInt())
				{
					arrayBuilder << newData[i];
					++i;
					++j;
				}
				else
				{
					while (true)
					{
						if (newData[i].numberInt() < elemVector[j].numberInt())
						{
							ncvec->push_back(newData[i].numberInt());
							arrayBuilder << newData[i];
							++i;
						}
						else if (newData[i].numberInt() > elemVector[j].numberInt())
						{
							++j;
						}
						else
						{
							arrayBuilder << newData[i];
							++j;
							++i;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		int i = 0;
		for (std::vector< mongo::BSONElement >::const_iterator it = newData.begin(); it != newData.end(); ++it)
		{
			std::cout << i << std::endl;
			std::cout.flush();
			arrayBuilder << *it;
			++i;
		}
		mongo::BSONObj obj = BSON("lastUpdate" << (int) time(NULL) << "monitoring" << false);
		conn->insert(meta, obj);
	}
	objBuilder.append("count", (int) newData.size()).appendArray("users", arrayBuilder.arr());
	conn->remove(data, query);
	conn->insert(data, objBuilder.obj());
	return ncvec;
}
