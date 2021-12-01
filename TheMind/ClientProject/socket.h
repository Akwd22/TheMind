#pragma once

#include "messaging/enums.h"

/**
 * @brief Se connecter � un serveur distant.
 * @param ip Adresse IP du serveur.
 * @param port Port du serveur.
*/
void socket_connect(const char* ip, unsigned short port);

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