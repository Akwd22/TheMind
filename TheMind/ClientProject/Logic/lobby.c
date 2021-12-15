#include "lobby.h"
#include "partie.h"
#include "joueur.h"
#include <stdlib.h>
#include <strings.h>
#include "../input.h"
#include <string.h>
#include "messaging/structs.h"
#include "messaging/enums.h"
#include "../socket.h"
#include "utils.h"

lobby l;

void initLobby()
{
	l.inLobby = true;
	l.nbBots = 0;
	l.nbJoueurs = 1;
	l.nbManches = 0;
	l.nbPrets = 0;
	bzero(l.joueurs, sizeof(l.joueurs));
}

void setInfoLobby(int roundCount, int bCount, int readyCount)
{
	l.nbManches = roundCount;
	l.nbBots = bCount;
	l.nbPrets = readyCount;
}

void printLobby()
{
	if (!l.inLobby)
	{
		return;
	}

	clear();
	printf("-- Lobby : en attente des autres joueurs --\n");

	printf("Nombre de joueurs prêts : %i / %i.\n", l.nbPrets, l.nbJoueurs);
	printf("Nombre de bots : %i.\n", l.nbBots);
	printf("\n");
	printf("Votre pseudo : %s.\n", j.nom);
	printf("Les autres joueurs :\n");
	for (int i = 0; i < l.nbJoueurs; i++) {
		if (i != j.id)
			printf("\t- %s\n", l.joueurs[i].nom);
	}
	printf("\n");
	if (j.ready)
	{
		printf("Vous êtes prêt.\n");
	}
	else
	{
		printfc(TERM_PURPLE, "Appuyer sur P pour devenir prêt.\n");
	}
	printfc(TERM_PURPLE, "Saisir un nombre pour changer le nombre de bots dans la partie.\n");
}

void gestionInputLobby()
{
	char* str = getUserInput();
	int nbBot = atoi(str);

	if (strcmp(str, "P") == 0 || strcmp(str, "p") == 0)
	{
		if ((l.nbBots > 0 || l.nbJoueurs > 1) && !j.ready)
		{
			j.ready = true;
			socket_send(CLI_MSG_SET_READY, NULL, 0);
		}
	}
	else {
		struct CliMsg_SetNumBot msgData = { .botCount = nbBot };
		socket_send(CLI_MSG_SET_NUM_BOT, &msgData, sizeof(msgData));
	}
}

void addPlayerToLobby(int id, char* name)
{
	l.joueurs[id].id = id;
	l.joueurs[id].ready = false;
	strcpy(l.joueurs[id].nom, name);
	l.nbJoueurs++;
}


