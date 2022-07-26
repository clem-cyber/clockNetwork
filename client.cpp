#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TAILLE_BUFFER 30

using namespace std;

void exitError (const char * msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main (int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "Invalid use" << endl;
		cout << "Use clock {ip_address} {port_number}" << endl;
		exit(EXIT_FAILURE);
	}
	int sock_client = socket(AF_INET, SOCK_STREAM, 0)
	struct sockaddr_in addr_serveur;
	
	addr_serveur.sin_family = AF_INET;
	inet_aton(argv[1], &addr_serveur.sin_addr);
	addr_serveur.sin_port = htons(atoi(argv[2]));
	
	if (connect(sock_client, (struct sockaddr *) &addr_serveur, sizeof (addr_serveur)) == -1)
		exitError("connect");
	cout << "Connected with success to the server" << endl;
	
	while (true)
	{
		char msg_write[1024] = "";
		cout << ">>";
		cin.getline(msg_write, 256);
		
		if (write(sock_client, msg_write, strlen(msg_write)) == -1)
			exitError("write");
			
		int n;
		
		char msg[TAILLE_BUFFER];
		
		n = recv(sock_client, msg, sizeof(msg), 0);
		
		if (n == -1)
			exitError("read");
			
		string retour = string(msg, n);
		if (retour == "Bad Parameter")
		{
			cout << "Error : Possible parameter : 12h or 24h or blank" << endl;
			continue;
		}
		else if (retour == "Bad Command")
		{
			cout << "Error : Unknown Command" << endl;
			continue;
		}
		else if (retour == "Date Changed")
		{
			cout << "Date changed with success" << endl;
			continue;
		}
		else if (retour == "Arguments Error")
		{
			cout << "Invalid Use" << endl;
			continue;
		}
		else if (retour == "Exit")
		{
			close(sock_client);
			break;
		}
		cout << retour;
	}
	
	return 0;
}
