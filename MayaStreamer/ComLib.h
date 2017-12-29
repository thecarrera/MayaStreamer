#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

class ComLib
{
public:
	enum TYPE { PRODUCER, CONSUMER };
	enum MSG_TYPE { NORMAL, DUMMY };
	struct Header
	{
		MSG_TYPE msgId;
		size_t msgSeq;
		size_t msgLength;
	};

	ComLib(const size_t& buffSize);
	~ComLib();

	// init and check status
	bool connect();
	bool isConnected();

	bool send(const char* msg, const size_t length);
	bool recv(char* msg, size_t& length);

	size_t nextSize();
	void calcFreeMem();
	void gen_random(char *s, const int len);

	HANDLE hFileMap;
	char* mData;
	bool exits = false;
	unsigned int mSize = 0;
	unsigned int freeMemSize = 0;

	Header h;
	size_t* head;
	size_t* tail;
};