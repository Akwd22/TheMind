#pragma once

#include <stdio.h>
#include "joueur.h"
#include "../socket.h"

typedef struct {
	int nbManches;
	int nbBots;
	int nbPrets;
	int nbJoueurs;
	joueur joueurs[MAX_CONNECTIONS];
}lobby;

extern lobby l;

void startGame();
void initLobby();
void addPlayerToLobby(int id);

void setPlayerName(int id, char* name);
void setReady(int id);
void setNumBot(int nb);