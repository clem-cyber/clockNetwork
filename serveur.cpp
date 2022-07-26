#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fstream>
#include <thread>

#define BACKLOG 128

using namespace std;

void exitErreur(const char * msg)
{
	perror(msg);
	exit( EXIT_FAILURE);
}

bool setConfig (int &numPort, int &nbClients)
{
	ifstream input_file("/etc/clockNetwork/port.config");
	string client;
	string port;
	if (!input_file.is_open()) 
	{
		cerr << "Error : the configuration file cannot be opened or doesn't exist" << endl;
		return false;
	}
	getline(input_file, port);
	numPort = stoi(port);
	return true;
}

void settingTime ()
{
	sleep (1);
	while (true)
	{
		char msg_write[1024] = "";
		cout << "#>";
		cin.getline(msg_write, 256);
		string total(msg_write);
		if (total.substr(0, 7) == "setTime")
		{
			if (total.size() < 24)
			{
				cout << "Invalid use" << endl;
				cout << "Use : setTime {year} {month 01 - 12} {day 01 - 31} {hour 00 - 23} {min 00 - 59}" << endl;
				continue;
			}
			string command = "sudo /usr/local/bin/setTime ";
			if (total.size() == 24)
			{
				int year = stoi(total.substr(8, 12));
				int month = stoi(total.substr(13, 15));
				int day = stoi(total.substr(16, 18));
				int hour = stoi(total.substr(19, 21));
				int min = stoi(total.substr(22, 24));
				if (year > 1900 && month >= 01 && month <= 12 && day >= 01 && day <= 31 && hour >= 00 && hour <= 23 && min >= 00 && min <= 59)
					command += total.substr(8, 24);
				else
					cout << "Error entering arguments (out of range)" << endl;
				if (system(command.c_str()) == 0)
					cout << "Date changed with success" << endl;
				else
					cout << "Error changing the date" << endl;
			}
			else
			{
				cout << "Invalid use" << endl;
				cout << "Use : setTime {year} {month 01 - 12} {day 01 - 31} {hour 00 - 23} {min 00 - 59}" << endl;
			}
		}
		else
		{
			cout << "Error : Unknown command" << endl;
			continue;
		}
		
	}
}

void request ()
{
	int num_port;
	int nb_clients;
	if (setConfig (num_port, nb_clients)) 
		cout << "Server configuration : success" << endl;
	else
		cout << "Server configuration : error" << endl;

	int sock_serveur = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sockaddr_serveur;
	sockaddr_serveur.sin_family = AF_INET;
	sockaddr_serveur.sin_port = htons(num_port);
	sockaddr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);

	cout << "Server launched on port " << num_port << endl;

	int yes = 1;

	if (setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		exitErreur("setsockopt");

	if (bind(sock_serveur, (struct sockaddr *) &sockaddr_serveur, sizeof(sockaddr_in)) == -1)
		exitErreur("bind");

	if (listen(sock_serveur, BACKLOG) == -1)
		exitErreur("listen");
		
	char msg [1024];
	int n;
	char msg_read [1024];
	string command;
	
	while (true)
	{
	
		struct sockaddr_in descrip_client;
		socklen_t taille = sizeof(struct sockaddr_in);
		int sock_client = accept(sock_serveur, (struct sockaddr* ) &descrip_client, &taille);
	
		while (true)
		{
			n = read(sock_client, msg_read, sizeof(msg));
			if (n == -1)
			        exitErreur("read");

		        if (!n) break;

			command = string (msg_read, n);
			
			if(command.substr(0, 4) == "time")
			{
				time_t my_time;
				struct tm * timeinfo; 
				time (&my_time);
				timeinfo = localtime (&my_time);
				string pattern;
				if (command == "time" || command == "time ")
				{
					pattern = "%H : %M\n";
				}
				else
				{
					pattern = command.substr(5, command.size());
					pattern += '\n';
				}
				strftime (msg, sizeof(msg), pattern.c_str(), timeinfo);
				send(sock_client, msg, strlen(msg), 0);
				continue;
			}
			else if (command == "exit")
			{
				char * msgErr = "Exit";
				send(sock_client, msgErr, strlen(msgErr), 0);
				break;
			}
			else
			{
				char * msgErr = "Bad Command";
				send(sock_client, msgErr, strlen(msgErr), 0);
				continue;
			}
		}
		
		close(sock_client);
	}
	
	close(sock_serveur);
}

int main(int argc, char * argv[])
{
	
	thread answer (request);
	thread setTime (settingTime);
	
	answer.join();
	setTime.join();

	return EXIT_SUCCESS;
}
