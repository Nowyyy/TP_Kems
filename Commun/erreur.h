#ifndef _ERREUR_H_
#define _ERREUR_H_

#include <standard_erreurs.h>

/*! 
 * \file  erreur.h
 * \brief Liste des codes d'erreurs
 */
/*
 * TYPES 
 */

/*
 * CODES ERREURS 
 */

/*!
 * \name Erreurs sur les cartes
 * @{
 */

#define ERR_CARTE_UNK		-100	/*!< carte  inconnue */
#define ERR_CARTE_NULL		-101	/*!< erreur sur carte inexistante */

/*!
 * @}
 * \name Erreurs sur les tapis
 * @{
 */

#define ERR_TAPIS_UNK		-200	/*!< tapis  inconnu */
#define ERR_TAPIS_CARTE_ID	-201	/*!< erreur sur indice de carte tapis dans le tapis */

/*!
 * @}
 * \name Erreurs sur les paquets
 * @{
 */

#define ERR_PAQUET_UNK		-300	/*!< paquet  inconnu */
#define ERR_PAQUET_VIDE		-301	/*!< operation impossible sur un paquet vide */

/*! @} */

#define ERR_ERREUR_FIN		-999	/*!< Fin liste des codes erreurs */

/*!
 * \fn void erreur_afficher( const err_t code ) ;
 * \brief Affiche sur la sortie erreur le message d'erreur correspondant &agrave; un code erreur 
 * \param[in] code : code de l'erreur 
 */
extern void erreur_afficher( const err_t code ) ; 

#endif

