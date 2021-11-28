#pragma once

/**
 * @brief D�finir la fonction de rappel lorsqu'une saisie a �t� faite par l'utilisateur dans l'entr�e standard.
 * @param newPtrCallback Fonction de rappel (ou @c NULL si aucune fonction de rappel).
*/
void setInputCallback(void (*newPtrCallback)());

/**
 * @brief Retourner la derni�re saisie de l'utilisateur dans l'entr�e standard.
 * @return Cha�ne de caract�re saisie.
*/
char* getUserInput();

/**
 * @brief D�marrer la boucle capturant les saisies utilisateur dans l'entr�e standard.
 * � chaque saisie, une fonction de rappel est appel�e.
*/
void beginInputLoop();