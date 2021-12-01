#pragma once

#define SOCKET int

/**
 * @brief Lire des donn�es � partir d'un flux d'octets.
 * Cette fonction assure la lecture de la totalit� des donn�es jusqu'au @p n i�me octets.
 * @param fd Descripteur de fichier o� r�cup�rer les donn�es du flux.
 * @param ptrBuffer Pointeur du buffer o� stocker les donn�es.
 * @param n Nombre d'octets � lire.
 * @return -1 si erreur, sinon 0.
*/
int recvn(int fd, void* ptrBuffer, size_t n);

/**
 * @brief �crire des donn�es dans flux d'octets.
 * Cette fonction assure l'�criture de la totalit� des donn�es jusqu'au @p n i�me octets.
 * @param fd Descripteur de fichier o� �crire les donn�es.
 * @param ptrBuffer Pointeur du buffer o� lire les donn�es � �crire.
 * @param n Nombre d'octets � �crire.
 * @return -1 si erreur, sinon 0.
*/
int sendn(int fd, const void* ptrBuffer, size_t n);