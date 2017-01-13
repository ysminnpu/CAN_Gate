﻿//this is used to test CAN_UDP_Sevrer by acting as the CAN to Ethernet Gateway

/*
Function:
Generate simulated CAN packet
Transmit CAN packet to CAN_UDP_Server (192.168.0.10:4284)
*/

/*
TODO:
- (done) socket init outside of loop 
- send data as relevant data type
*/

#include<iostream>
#include<arpa/inet.h> //source: http://unix.superglobalmegacorp.com/xnu/newsrc/bsd/include/arpa/inet.h.html
#include<unistd.h> //source http://unix.superglobalmegacorp.com/Net2/newsrc/sys/unistd.h.html
#include<sys/socket.h> //source http://unix.superglobalmegacorp.com/Net2/newsrc/sys/socket.h.html
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h> //source http://unix.superglobalmegacorp.com/Net2/newsrc/netinet/in.h.html
#include<sys/time.h> //source http://unix.superglobalmegacorp.com/Net2/newsrc/sys/time.h.html
#include<fstream>
#include<sstream>
#include<bitset>

using namespace std;

int CLIENT_PRT = 4284; //transmit to
const char* IP_ADR = "192.168.0.10"; //192.168.0.74 is the actual CAN<>Eth gateway IP

void error(const char *msg)
{
	perror(msg);
	//write to syslog
	exit(1);
}

void transmit(int pCount)
{
	int _pCount = pCount;
	int sockfd;
	struct sockaddr_in client;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(IP_ADR);
	client.sin_port = htons(CLIENT_PRT);

	//Assemble CAN packet vv (this is probably not correct...)
	string datapack;
	stringstream ss_dp;
	/*
	string ts_hi = "22220 ";
	string ts_lo = "11110 ";
	string ide = "1 ";
	string type = "0 ";
	string data[] = { "10001 ","10002 ","10003 ","10004 ","10005 ","10006 " };


	ss_dp << ts_hi << ts_lo << ide << type << data[0] << data[1] << data[2] << data[3] << data[4] << data[5];
	


	datapack = ss_dp.str();
	int temp = datapack.size();
	char buffer[temp + 1];
	
	for (int a = 0; a <= temp; a++)
	{
		buffer[a] = datapack[a];
	}
	*/


	//Assemble CAN packet ^^

	//char message[20] = { 0x5f, 0x52, 0x69, 0x63, 0x6b, 0x5f, 0x53, 0x69, 0x6d, 0x6f, 0x6e, 0x5f, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x6f, 0x6e, 0x5f };
	char message[20] = { 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05 };


	socklen_t c_size = sizeof(client);

	for (int i = 1; i <= _pCount; i++)
	{
		cout << "Packet #" << i << "; Data: ";
		if (sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *) &client, c_size) < 0)
		{
			error("send fail");
		}
		else
		{
			cout << message << endl;
		}
		sleep(1);
	}

	close(sockfd);
}

int main()
{
	int pCount = 3; //number of packets to be transmitted
	transmit(pCount);

	return 0;
}