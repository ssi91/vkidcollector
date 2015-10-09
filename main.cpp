#include <stdio.h>
#include <iostream>
//#include <string>
//#include "compil.h"
#include "servsock.h"

//#include "varrender.h"
//extern string path;

int socketId;


using namespace std;

int main()
{
	try
	{
//		VarRender varRender("index.templ");
//		varRender.initVar("var0", "русский");
//		varRender.initVar("var1", "val1");
//
//		Compil com(varRender);
//		com.createHTML();

		int i;
		pthread_t id[THREAD_COUNT];

		//инициализация библилиотеки
		FCGX_Init();
		printf("Lib is inited\n");

		//открываем новый сокет
		socketId = FCGX_OpenSocket(SOCKET_PATH, 20);
		if (socketId < 0)
		{
			//ошибка при открытии сокета
			return 1;
		}
		printf("Socket is opened\n");

		//создаём рабочие потоки
		for (i = 0; i < THREAD_COUNT; i++)
		{
			pthread_create(&id[i], NULL, doit, NULL);
		}

		//ждем завершения рабочих потоков
		for (i = 0; i < THREAD_COUNT; i++)
		{
			pthread_join(id[i], NULL);
		}
	}
	catch (RenderException *r)
	{
		cout << r->what() << endl;
	}
	return 0;
}
