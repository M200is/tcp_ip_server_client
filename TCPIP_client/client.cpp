#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

#define PORT 2003
#define PACKET_SIZE 1024

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, buf, len, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main() {
	WSADATA wsaData;		//소켓 초기화 저장 구조체
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//시작

	SOCKET hSocket;		//핸들
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//IPv6, 연결지향형, TCP

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;		//소켓 구성요소
	tAddr.sin_port = htons(PORT);
	string temp;
	cout << "ip > ";
	cin >> temp;
	const char* server_ip = temp.c_str();
	tAddr.sin_addr.s_addr = inet_addr(server_ip);

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));


	while (1) {
		char buffer[PACKET_SIZE + 1] = {};
		char msg[PACKET_SIZE + 1] = {};
		cout << "send data > ";
		scanf("%s", buffer);
		int bufsize = strlen(buffer);
		if (buffer[bufsize] == '\n')
			buffer[bufsize] = '\0';
		send(hSocket, buffer, strlen(buffer), 0);

		recv(hSocket, msg, PACKET_SIZE, 0);
		cout << "recv data > " << msg << endl;
	}
	closesocket(hSocket);

	WSACleanup();
	return 0;
}