#pragma once

/**
 * @brief D�marrer le serveur.
 * D�marrer le socket, �couter les demandes de connexion, et g�rer la communication avec les clients.
*/
void socket_open();

/**
 * @brief Fermer le serveur.
 * Terminer la connexion des clients et du serveur.
*/
void socket_close();

/**
 * @brief Diffuser un message � tous les clients connect�s au serveur.
 * @param msg Message � envoyer. TODO : � revoir.
 * @param size Taille du message.
*/
void socket_broadcast(const char* msg, size_t size);

/**
 * @brief Envoyer un message � un client.
 * @param clientId Identifiant du client.
 * @param msg Message � envoyer. TODO : � revoir.
 * @param size Taille du message.
*/
void socket_send(unsigned int clientId, const char* msg, size_t size);