#include <stdbool.h>
#include <string.h>
#include "partie.h"
#include "joueur.h"
#include "messaging/structs.h"
#include "messaging/enums.h"
#include "../socket.h"

joueur j = {0};

void setName(char * nom)
{
	strcpy(j.nom, nom);
}

void jouerCarte(int carte)
{
	int carteIndex = 0;

	// R�cup�rer l'index de la carte � partir de son num�ro.
	for (int i = 0; i < j.nbCartes; i++) {
		if (j.cartes[i] == carte) {
			carteIndex = i;
		}
	}

	j.cartes[carteIndex] = 0;

	struct CliMsg_PlayCard msgData = { .cardIndex = carteIndex };
	socket_send(CLI_MSG_PLAY_CARD, &msgData, sizeof(msgData));
}

void setId(int id)
{
	j.id = id;
}

int changeNbBot(int nbBot) {
	return nbBot;
}
