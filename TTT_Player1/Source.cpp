#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <string>
#include <windows.h>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib") // This part will disable some warnings 


// 158.39.218.239


using namespace std;
void Recieve();
void Send(std::string);
char Board[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
void printGame();
bool flag = 1;
int wincon = -1;
int Boardvalue = 0;
int WinCondition();
void Message();
void Message() {
	string message;
	std::cout << " it is your turn. Go ahead and play..." <<std::endl;
	do {
		std::cout << "whats your pick >>> ";
		std::cin >> message;

		if (flag == 1) {
			Boardvalue = static_cast<int>(message[0] - 49);
			Board[Boardvalue] = 'X';
			printGame();
			wincon = WinCondition();
			if (wincon == 0 || wincon == 1) {
				std::cout << "Player  >>>" << wincon + 1 << " <<< win ";

			}
			std::cout << "-------------" << Boardvalue << std::endl;
			Send(message);
			std::cout << " it's not your turn, Please wait for player 2 to play..." << std::endl;
			flag = !flag;
		}
		else {
			std::cout << "Opps!!! It's not your turn ... wait for player 1 to play ... !";
		}
	} while (message != "q");

}



void printGame() {
	system("CLS");
	for (int a = { 0 }; a < 3; a++)
	{
		std::cout << "  | ";
		for (int b = { 0 }; b < 3; b++)
		{
			std::cout << Board[b + (a * 3)] << " | ";
		}
		std::cout << "\n";
	}
}

/// Game win condition 

int playing = -1;
int WinCondition()
{
	if (((Board[0] == 'X') && (Board[1] == 'X') && (Board[2] == 'X')) || ((Board[0] == 'O') && (Board[1] == 'O') && (Board[2] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on top row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 2
	if (((Board[3] == 'X') && (Board[4] == 'X') && (Board[5] == 'X')) || ((Board[3] == 'O') && (Board[4] == 'O') && (Board[5] == 'O')))
	{
		std::cout << "Player " << Board[3] << " is the winner on second row!!\n";
		playing = 0;
		return 0;
	}
	// cheking vertical line 3
	if (((Board[6] == 'X') && (Board[7] == 'X') && (Board[8] == 'X')) || ((Board[6] == 'O') && (Board[7] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[6] << " is the winner on third row!!\n";
		playing = 0;
		return 0;
	}


	// cheking horizontal line 1
	if (((Board[0] == 'X') && (Board[3] == 'X') && (Board[6] == 'X')) || ((Board[0] == 'O') && (Board[3] == 'O') && (Board[6] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on first column!!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 2
	if (((Board[1] == 'X') && (Board[4] == 'X') && (Board[7] == 'X')) || ((Board[1] == 'O') && (Board[4] == 'O') && (Board[7] == 'O')))
	{
		std::cout << "Player " << Board[1] << " is the winner on second column!!\n";
		playing = 0;
		return 0;
	}
	// cheking horizontal line 3
	if (((Board[2] == 'X') && (Board[5] == 'X') && (Board[8] == 'X')) || ((Board[2] == 'O') && (Board[5] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[2] << " is the winner on second column!!\n";
		playing = 0;
		return 0;
	}
	// cheking diagonal line 1 from top left to bottom right
	if (((Board[0] == 'X') && (Board[4] == 'X') && (Board[8] == 'X')) || ((Board[0] == 'O') && (Board[4] == 'O') && (Board[8] == 'O')))
	{
		std::cout << "Player " << Board[0] << " is the winner on the diagonal row1 !!\n";
		playing = 0;
		return 0;

	}
	// cheking diagonal line 2 from top right to bottom left
	if (((Board[2] == 'X') && (Board[4] == 'X') && (Board[6] == 'X')) || ((Board[2] == 'O') && (Board[4] == 'O') && (Board[6] == 2)))
	{
		std::cout << "Player " << Board[2] << " is the winner on the diagonal row2!!\n";
		playing = 0;
		return 0;

	}
	return -1;
}

////////////////////////////////////////////// Server Codes 

void Recieve() {

	WSADATA data;
	WORD version = MAKEWORD(3, 2); // we just make a version here, can be any number 

	int wsret = WSAStartup(version, &data); // starting the socket 
	if (wsret != 0) // if socket is not started , then quit 
	{
		cout << "Can't start Winsock! " << wsret;
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);  //Sock_DGRAM is UDP socket

	sockaddr_in server;
	server.sin_addr.S_un.S_addr = ADDR_ANY; // any IP would be okay 
	server.sin_family = AF_INET; 
	server.sin_port = htons(54001); 



	if (bind(in, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) // biding to the socket 
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}


	sockaddr_in client; //client object
	int clientLength = sizeof(client); 

	char buf[1024]; // buffer size 

	// Enter a loop
	while (true)
	{
		ZeroMemory(&client, clientLength); 
		ZeroMemory(buf, 1024); 


		int Msg = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (Msg == SOCKET_ERROR)
		{
			perror("Error receiving from client ");
			WSAGetLastError();
			continue;
		}
		char clientIp[256]; 
		ZeroMemory(clientIp, 256); 

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		// Display the message / who sent it
		//cout << "Message recv from " << clientIp << " : " << buf << endl;
		std::cout << buf;
		Boardvalue = static_cast<int>(buf[0] - 49);
		std::cout << "-------------" << Boardvalue << std::endl;
		Board[Boardvalue] = 'O';
		printGame();
		WinCondition();
		flag = !flag;

	}
	// Close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
}

void Send(std::string s) {

	// initializing winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wret = WSAStartup(version, &data);
	if (wret!= 0)
	{
		cout << " Winsock! error " << wret;
		return;
	}

	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(54002); // Little to big endian conversion
	inet_pton(AF_INET, "10.164.1.117", &server.sin_addr);  // player 2 ip address here 
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	//do {

	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		cout << "That didn't work! " << WSAGetLastError() << endl;
	}
	//	} while (s != "q");
	closesocket(out);
	WSACleanup();

}
void Print(string name) {

	std::cout << "        \t \t\t            Player 1  === > " << name << "\n";

}
int main() // We can pass in a command line option!! 
{
	string name;
	//	std::cout << "Enter  your Name :";
		//cin >> name;
		//Print(name);
	printGame();
	thread T1(Message);
	thread T2(Recieve);
	T1.join();
	T2.join();
	return 0;
}