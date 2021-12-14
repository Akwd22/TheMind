#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "socket/consts.h"
#include "socket/utils_io.h"
#include "socket/utils_io.h"
#include "messaging/cli_handlers.h"
#include "socket.h"

#define FATAL_ERR(msg) perror(msg); socket_close(); exit(errno);

/// Structure stockant les informations de la connexion d'un client.
struct ClientCon
{
	/// Descripteur du socket.
	SOCKET fd;
	/// Socket ouvert ?
	bool isOpened;
	/// Adresse IP et port.
	struct sockaddr_in address;
	/// Thread associ� � la connexion et g�rant la communication de ce client.
	pthread_t threadId;
};

/// Liste des connexions ouvertes (identifiant d'un client = index dans la liste).
struct ClientCon cliSockets[MAX_CONNECTIONS];
/// Nombre de clients connect�s.
unsigned int nbConnections = 0;

/// Descripteur de socket du serveur.
SOCKET srvSocket;
/// Socket du serveur est-il ouvert ?
bool isSocketOpened = false;

/**
 * @brief [Thread] �couter les messages d'un client et appeler le bon gestionnaire selon le message re�u.
 * @param ptrClientId Identifiant client.
*/
void* listenMessages(void* ptrClientId)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	unsigned int clientId = *(unsigned int*)ptrClientId;

	while (cliSockets[clientId].isOpened)
	{
		struct MsgHeader header;
		char data[MAX_MSG_SIZE];

		// Lecture en-t�te message : taille contenu du message & type du message.
		if (recvn(cliSockets[clientId].fd, &header, sizeof(header)) == -1)
		{
			FATAL_ERR("recvn()");
		};

		// Lecture contenu message.
		if (recvn(cliSockets[clientId].fd, data, header.dataLen) == -1)
		{
			FATAL_ERR("recvn()");
		};

		printfc(TERM_BLUE, "Message received (type %i) from client id %i (%lu bytes).\n", header.msgType, clientId, header.dataLen + sizeof(header));

		// [SECTION CRITIQUE] TODO : mieux documenter.
		pthread_mutex_lock(&mutex);
		{
			// Appel du gestionnaire correspondant au type du message.
			cliMsgHandler[header.msgType](clientId, data);
		}
		pthread_mutex_unlock(&mutex);
	}

	printf("Stopped communication with client id %i.\n", clientId);
}

/**
 * @brief �couter les demandes de connexion et les accepter si possible.
*/
void listenConnections()
{
	printf("Listening for client connections (max. %i)...\n", MAX_CONNECTIONS);

	while (nbConnections < MAX_CONNECTIONS)
	{
		SOCKET cliSocket;
		struct sockaddr_in cliAddress;
		unsigned int cliAddressSize = sizeof cliAddress;

		/// �couter et accepter une demande de connexion.
		cliSocket = accept(srvSocket, (struct sockaddr*)&cliAddress, &cliAddressSize);

		if (cliSocket == -1)
		{
			FATAL_ERR("Error while listening client connections");
		}

		nbConnections++;

		// Sauvegarder les informations du client.
		unsigned int clientId = nbConnections - 1;
		cliSockets[clientId].fd = cliSocket;
		cliSockets[clientId].address = cliAddress;
		cliSockets[clientId].isOpened = true;

		// Cr�ation du thread g�rant la communication de ce client.
		if (pthread_create(&cliSockets[clientId].threadId, NULL, &listenMessages, &clientId) == -1)
		{
			FATAL_ERR("listenMessages - thread_create()");
		}

		printf("Connection from %s accepted.\n", inet_ntoa(cliAddress.sin_addr));
	}

	printf("Stopped listening client connections. Total of clients connected: %i.\n", nbConnections);
}

void socket_broadcast(enum SrvMsg type, const void* msg, size_t size)
{
	if (size > MAX_MSG_SIZE)
	{
		fprintf(stderr, "Could not broadcast message (type %i) as the size %lu exceed the max. size %i.\n", type, size, MAX_MSG_SIZE);
		return;
	}

	struct MsgHeader header = { size, type };

	for (unsigned int i = 0; i < nbConnections; i++)
	{
		// Envoyer l'en-t�te du message.
		if (sendn(cliSockets[i].fd, &header, sizeof(header)) == -1)
		{
			FATAL_ERR("socket_broadcast - sendn()");
		}

		// Envoyer le contenu du message.
		if (sendn(cliSockets[i].fd, msg, size) == -1)
		{
			FATAL_ERR("socket_broadcast - sendn()");
		}
	}

	printfc(TERM_GREEN, "Broadcasted message (type %i) (%lu bytes).\n", header.msgType, header.dataLen + sizeof(header));
}

void socket_send(unsigned int clientId, enum SrvMsg type, const void* msg, size_t size)
{
	if (size > MAX_MSG_SIZE)
	{
		fprintf(stderr, "Could not send message (type %i) to client id %i as the size %lu exceed the max. size %i.\n", type, clientId, size, MAX_MSG_SIZE);
		return;
	}

	if (clientId >= nbConnections)
	{
		fprintf(stderr, "Could not send message (type %i) as client id %i does not exist.\n", type, clientId);
		return;
	}

	struct MsgHeader header = { size, type };

	// Envoyer l'en-t�te du message.
	if (sendn(cliSockets[clientId].fd, &header, sizeof(header)) == -1)
	{
		FATAL_ERR("socket_send - sendn()");
	}

	// Envoyer le contenu du message.
	if (sendn(cliSockets[clientId].fd, msg, size) == -1)
	{
		FATAL_ERR("socket_send - sendn()");
	}

	printfc(TERM_GREEN, "Sent message to client id %i (type %i) (%lu bytes).\n", clientId, header.msgType, header.dataLen + sizeof(header));
}

void socket_bots(unsigned int nb)
{
	for (unsigned int i = 0; i < nb; i++)
	{
		usleep(50000);

		int pidBot = fork();

		if (pidBot == -1)
		{
			FATAL_ERR("socket_bot - fork()");
		}

		// Fils : processus robot.
		if (pidBot == 0)
		{
			// Fermer tous les descripteurs de fichier non-standard ouverts (sockets).
			for (long i = 3; i < sysconf(_SC_OPEN_MAX); i++)
			{
				close(i);
			}

			// D�marrer le programme robot.
			if (execl("../BotProject/BotProject", "BotProject", "--ip", "127.0.0.1", "--port", TO_STR(SERVER_PORT), NULL) == -1)
			{
				FATAL_ERR("socket_bots - execl()");
			};

			break;
		}
	}
}

void socket_close()
{
	printf("Closing server...\n");

	socket_broadcast(SRV_MSG_DISCONNECT_ALL, NULL, 0);

	for (unsigned int i = 0; i < nbConnections; i++)
	{
		cliSockets[i].isOpened = false;

		pthread_cancel(cliSockets[i].threadId); // TODO : attention, il faut que cette fonction soit appel�e dans le main thread.
		pthread_join(cliSockets[i].threadId, NULL);

		close(cliSockets[i].fd);
	}

	isSocketOpened = false;
	if (close(srvSocket) == -1)
	{
		perror("close");
	}

	printf("Server closed.\n");
}

void socket_open()
{
	printf("Starting server...\n");

	// Cr�ation du socket.
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (srvSocket == -1)
	{
		FATAL_ERR("socket()");
	}

	// Autoriser la r�utilisation du port pour pouvoir imm�diatement r�ouvrir un serveur,
	// quand un vient d'�tre ferm�.
	int reuseAddr = 1;
	setsockopt(srvSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(int));

	// Cr�ation de l'interface
	// et attribution de celle-ci au socket cr��.
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(SERVER_PORT);

	if (bind(srvSocket, (struct sockaddr*)&address, sizeof address) == -1)
	{
		FATAL_ERR("bind()");
	}

	isSocketOpened = true;

	// Attendre et accepter les demandes de connexion.
	if (listen(srvSocket, MAX_CONNECTIONS) == -1)
	{
		FATAL_ERR("listen()");
	}

	listenConnections();

	while (isSocketOpened) usleep(5000);
}