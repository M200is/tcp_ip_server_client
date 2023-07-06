#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

#define PORT 2003
#define PACKET_SIZE 1024


int main() {
	WSADATA wsaData;		//소켓 초기화 저장 구조체
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//시작

	SOCKET hListen;		//핸들
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//IPv4, 연결지향형, TCP

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;		//소켓 구성요소
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	while (1) {
		char buffer[PACKET_SIZE + 1] = {};
		char ret[PACKET_SIZE + 1]= {};
		recv(hClient, buffer, PACKET_SIZE, 0);
		cout << "recv data > " << buffer << endl;
		cout << "send data > ";
		scanf("%s", ret);
		int retsize = strlen(ret);
		if (ret[retsize] == '\n')
			ret[retsize] = '\0';
		send(hClient, ret, strlen(ret), 0);
		buffer[0] = { 0, };
		ret[0] = { 0, };
	}

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;
}
