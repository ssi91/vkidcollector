//
// Created by ssi on 16.09.15.
//

#ifndef VKIDCOLLECTOR_COLLECT_FUNC_H
#define VKIDCOLLECTOR_COLLECT_FUNC_H

#include "request/Request.h"
#include <iostream>
#include <mongo/client/dbclient.h>

std::vector< mongo::BSONElement > & collectData(vkmes::Request &request, boost::scoped_ptr< mongo::DBClientBase > &conn);
#endif //VKIDCOLLECTOR_COLLECT_FUNC_H
