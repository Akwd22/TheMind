#pragma once

#include <stdbool.h>
#include <errno.h>

#define FATAL_ERR(msg) { perror(msg); exit(errno); }

#define MIN_PLAYING_INTERVAL 1
#define MAX_PLAYING_INTERVAL 3

/// Structure stockant les informations du joueur.
typedef struct {
	/// Identifiant du joueur attribu� par le serveur.
	unsigned int playerId;
	/// Nom du robot.
	char playerName[8];
	/// Nombre de cartes dans la main du joueur.
	unsigned int nbCards;
	/// Nombre de cartes jou�es par le joueur dans la manche.
	unsigned int nbPlayedCards;
	/// Liste des cartes de la main du joueur. La valeur d'une carte est 0 si d�j� jou�e.
	unsigned int* cards;
	/// Le joueur peut jouer ? Il peut s'il lui reste des cartes � jouer.
	bool canPlay;
} Player;

/**
 * @brief Initialiser la structure du joueur.
 * @param playerId Identifiant du joueur attribu� par le serveur.
*/
void logic_initPlayer(unsigned int playerId);

/**
 * @brief Fin de partie.
*/
void logic_endGame();

/**
 * @brief Passer � la manche suivante. Le joueur peut rejouer s'il ne pouvait plus.
*/
void logic_nextRound();

/**
 * @brief Supprimer une carte de la main du joueur.
 * La carte reste pr�sente dans la liste mais sa valeur est affect�e � 0.
 * @param cardIndex Index de la carte.
*/
void logic_removePlayerCard(unsigned int cardIndex);

/**
 * @brief Attribuer les cartes dans la main du joueur.
 * @param nbCards Nombre de cartes dans la main.
 * @param cards Liste des cartes.
*/
void logic_setPlayerCards(unsigned int nbCards, const unsigned int* cards);

/**
 * @brief Jouer la plus petite carte de la main.
*/
void logic_playCard();

/**
 * @brief Planifier le jouage de la carte.
 * La fonction d�clenche @link logic_playCard() @endlink au bout d'un d�lai entre @link MIN_PLAYING_INTERVAL @endlink et @link MAX_PLAYING_INTERVAL @endlink secondes.
*/
void logic_schedulePlayCard();