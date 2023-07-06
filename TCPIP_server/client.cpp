#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32")

#define PORT 2003
#define PACKET_SIZE 1024
#define SERVER_IP "127.0.0.1"

//오류 출력용, 가져옴
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//수신 함수,  혼자서 생각하려다 머리 터질것 같아 가져옴
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
	cout << "start\n";
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	SOCKET hSocket;
	hSocket = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
		err_quit("socket()");

	char server_ip[16];
	cin >> server_ip;
	SOCKADDR_IN6 tAddr = {};
	ZeroMemory(&tAddr, sizeof(tAddr));
	tAddr.sin6_family = AF_INET6;
	int addrlen = sizeof(tAddr);
	WSAStringToAddress((LPWSTR)server_ip, AF_INET6, NULL, (SOCKADDR*)&tAddr, &addrlen);
	tAddr.sin6_port = htons(PORT);
	int retval = connect(hSocket, (SOCKADDR*)&tAddr, sizeof(addrlen));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");
	char buf[PACKET_SIZE + 1];
	int len;
	while (1) {
		cout << "\nsend > ";
		if (fgets(buf, PACKET_SIZE + 1, stdin) == NULL)
			break;
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		else if (len == 0)
			break;
		retval = send(hSocket, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		retval = recvn(hSocket, buf, retval, 0);
		if (retval == 0)
			break;
		buf[retval] = '\0';
		cout << "recv data : " << buf << "\n";
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}