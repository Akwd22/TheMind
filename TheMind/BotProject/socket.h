#pragma once

#include "messaging/enums.h"

/**
 * @brief D�finir l'adresse IP du serveur
 * @param ip Adresse IP.
*/
void socket_setIp(char* ip);

/**
 * @brief D�finir le port du serveur.
 * @param port Port.
*/
void socket_setPort(unsigned short port);

/**
 * @brief Se connecter � un serveur distant.
*/
void socket_connect();

/**
 * @brief Se d�connecter du serveur actuel.
*/
void socket_disconnect();

/**
 * @brief Envoyer un message au serveur.
 * @param type Type du message.
 * @param msg Message � envoyer.
 * @param size Taille du message.
*/
void socket_send(enum CliMsg type, const void* msg, size_t size);