#ifndef _PAQUET_H_
#define _PAQUET_H_

#include <carte.h>

/*! 
 * \file  paquet.h
 * \brief Type et primitives d'un paquet
 */


/*
 * CONSTANTES 
 */
/*!
 * \def PAQUET_NULL 
 * \brief Paquet inexistante
 */
#define PAQUET_NULL NULL

/*
 * TYPES 
 */

/*!
 * \struct paquet_t 
 * \brief Type d'un paquet
 */

typedef tab_t paquet_t ; 

/*!
 * \typedef paquet_id_t 
 * \brief Type d'index pour les  pi&egrave;ces (indices, compteurs...)
 */
typedef tab_id_t paquet_id_t  ;

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */

/*! Acces a une carte du paquet */
extern
carte_t * paquet_carte_lire( const paquet_t * const paquet ,
			     const carte_id_t ind_carte ) ;

/*! Acces au nombre de cartes du paquet */
extern
carte_id_t paquet_nb_cartes( const paquet_t * const paquet ) ;

/*!
 * @}
 * \name Tests
 * @{
 */

/*! Existance d'un paquet */
extern
booleen_t paquet_existe( const paquet_t * const paquet ) ; 

extern 
booleen_t paquet_vide( const paquet_t * const paquet ) ;

/*!
 * @}
 * \name Methodes d'affectations
 * @{
 */

/*! 
 * Ajout d'une carte dans le  paquet 
 */
extern
err_t paquet_carte_ajouter( paquet_t * const paquet , 
			    const carte_t * const carte )  ;

/*! 
 * Retrait d'une carte au sommet du paquet 
 */
extern
carte_t * paquet_carte_retirer( paquet_t * const paquet ) ;

/*!
 * @}
 * \name Primitives
 * @{
 */

/*!
 * Copie  d'un paquet dans un autre paquet 
 *  - la structure du paquet est recreee
 *  - les references sur les cartes sont dupliquees
 */
extern 
err_t paquet_copier( paquet_t ** paquet_cible , const paquet_t * const paquet_source ) ;


extern 
err_t cb_paquet_copier( void ** paquet_cible , const void * paquet_source )  ; 

/*!
 * Creation d'un paquet 
 */

extern 
paquet_t * paquet_creer()  ;


/*! 
 * Destruction d'un paquet 
 */

extern
err_t paquet_detruire( paquet_t ** paquet )  ; 

/*! 
 * Destruction d'un paquet (callback)
 */
extern
err_t cb_paquet_detruire( void ** paquet )  ;

/*!
 * Affichage
 */

/*! Pour  Debuggage */
extern
void paquet_stdout_afficher( const paquet_t * const paquet  );


/*! Affichage (callback) */
extern
void cb_paquet_stdout_afficher( const void * paquet ) ;


/*!
 * Comparaison de 2 paquet
 */
extern 
int paquet_comparer( const paquet_t * const paquet_1 , const paquet_t * const paquet_2 ) ;

/*! Comparaison de 2 paquet (callback) */
extern 
int cb_paquet_comparer( const void * paquet_1 , const void * paquet_2 ) ; 

/*! Permutation de 2 cartes dans le paquet */
extern 
err_t paquet_carte_permuter( paquet_t * const paquet , const carte_id_t ind1 , const carte_id_t ind2 ) ; 

/*! @} */

#endif
