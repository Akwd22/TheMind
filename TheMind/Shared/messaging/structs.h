#pragma once

/// Structure de l'en-t�te d'un message.
struct MsgHeader
{
	/// Longueur du contenu du message.
	unsigned int dataLen;
	/// Type du message (cf. les �num�rations TODO).
	unsigned int msgType;
};

/// Message d'affectation du nom du client.
struct CliMsg_SetName
{
	char name[20];
};

/// Message d'affectation du nombre de robots dans la partie.
struct CliMsg_SetNumBot
{
	unsigned int botCount;
};

/// Message lorsqu'un client veut jouer une carte.
struct CliMsg_PlayCard
{
	unsigned int cardIndex;
};

/// Message d'information du lobby.
struct SrvMsg_InfoLobby
{
	/// Nombre de manches au total.
	unsigned int roundCount;
	/// Nombre de robots dans la partie.
	unsigned int botCount;
	/// Nombre de clients pr�ts � jouer.
	unsigned int readyCount;
};

/// Message d'information d'un client qui vient de se connecter.
struct SrvMsg_PlayerConnected
{
	unsigned int playerId;
	unsigned int isBot;
	char name[20];
};

/// Message d'information qu'une carte a �t� jou�e par un autre client.
struct SrvMsg_CardPlayed
{
	unsigned int playerId;
	unsigned int cardNumber;
};

/// Message d'information du commencement d'une nouvelle manche.
struct SrvMsg_NextRound
{
	/// Num�ro de la nouvelle manche.
	unsigned int roundNumber;
	/// Nombre restant de vie.
	unsigned int lifeRemaining;
	/// La derni�re manche a �t� gagn�e ?
	unsigned int isLastRoundWon;
	/// Ensemble des cartes du client destinataire du message.
	unsigned int playerCards[50]; // TODO : v�rifier si la taille est suffisante.
};

/// Message d'information de fin de partie.
struct SrvMsg_GameEnd
{
	unsigned int isGameWon;
};

/// Message d'information � destination du joueur qui vient de se connecter.
struct SrvMsg_PlayerInfo
{
	unsigned int playerId;
};