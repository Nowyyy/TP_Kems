#ifndef _TAPIS_H_
#define _TAPIS_H_

#include <carte.h>
#include <paquet.h>

/*! 
 * \file  tapis.h
 * \brief Type et primitives d'un tapis
 */


/*
 * CONSTANTES 
 */
/*!
 * \def TAPIS_NULL 
 * \brief Tapis inexistante
 */
#define TAPIS_NULL NULL

/*
 * TYPES 
 */

/*!
 * \struct tapis_t 
 * \brief Type d'une pi&egrave;ce d'un damier
 */

typedef tab_t tapis_t ; 

/*!
 * \def TAPIS_NB_CARTES
 * \brief Nombre de cartes sur le tapis
 */
#define TAPIS_NB_CARTES 4



/*!
 * \typedef tapis_id_t 
 * \brief Type d'index pour les  pi&egrave;ces (indices, compteurs...)
 */
typedef tab_id_t tapis_id_t  ;

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */

/*! Acces a une carte du tapis */
extern
carte_t * tapis_carte_lire( const tapis_t * tapis ,
			    const carte_id_t ind_carte ) ;

/*! Acces au nombre de cartes du tapis */
extern
carte_id_t tapis_nb_cartes( const tapis_t * const tapis ) ;

/*!
 * @}
 * \name Tests
 * @{
 */

/*! Teste si le tapis a ete cree */
extern
booleen_t tapis_existe( const tapis_t * const tapis ) ; 

/*! Teste si le tapis contient des cartes */
extern 
booleen_t tapis_vide( const tapis_t * const tapis ) ;

/*! Teste si le tapis contient un carre (4 cartes de meme valeur) */
extern 
booleen_t tapis_carre( const tapis_t * const tapis ) ;

/*!
 * @}
 * \name Methodes d'affectations
 * @{
 */

/*! 
 * Pose d'une carte a un emplacement du tapis
 */
extern
err_t tapis_carte_poser( tapis_t * const tapis , 
			 const carte_id_t ind_carte , 
			 const carte_t * const carte  ) ;

/*! 
 * Pose de la carte sommet du paquet a un emplacement du tapis 
 */
extern
err_t tapis_carte_distribuer( tapis_t * const tapis , 
			      const carte_id_t ind_carte , 
			      paquet_t * const paquet  ) ; 


/*! 
 * Remise d'une carte a un emplacement du tapis dans le paquet
 */
extern
err_t tapis_carte_retirer( tapis_t * const tapis ,
			   const carte_id_t ind_carte , 
			   paquet_t * const paquet ) ;

/*!
 * @}
 * \name Primitives
 * @{
 */

/*!
 * Copie  d'un tapis dans un autre tapis 
 *  - la structure du tapis est recreee
 *  - les references sur les cartes sont dupliquees
 */

extern 
err_t tapis_copier( tapis_t ** tapis_cible , const tapis_t * const tapis_source ) ;

extern 
err_t cb_tapis_copier( void ** tapis_cible , const void * tapis_source ) ;

/*!
 * Creation d'un tapis 
 */

extern 
tapis_t * tapis_creer()  ;


/*! 
 * Destruction d'un tapis 
 */

extern
err_t tapis_detruire( tapis_t ** tapis )  ; 

/*! 
 * Destruction d'un tapis (callback)
 */
extern
err_t cb_tapis_detruire( void ** tapis )  ;

/*!
 * Affichage
 */

/*! Pour  Debuggage */
extern
void tapis_stdout_afficher( const tapis_t * tapis  );


/*! Affichage (callback) */
extern
void cb_tapis_stdout_afficher( const void * tapis ) ;


/*! 
 * Choix de 2 cartes a echanger dans les tapis 1 et 2 
 * On maximise le tapis_1 
 * \param choix: indique si un choix a ete fait 
 * \li VRAI si un choix a ete fait
 * \li FAUX sinon
 * \param ind_carte1 : si choix VRAI alors indice de la carte 1 a echanger, -1 sinon 
 * \param ind_carte2 : si choix VRAI alors indice de la carte 2 a echanger, -1 sinon 
 */
extern 
err_t tapis_cartes_choisir( booleen_t * choix ,		/* Choix effectue */
			    const tapis_t * const tapis_1 , 
			    carte_id_t * ind_carte_1 ,  /* indice de la carte a echanger dans le tapis 1 */
			    const tapis_t * const tapis_2 , 
			    carte_id_t * ind_carte_2 ) ;  /* indice de la carte a echanger dans le tapis 2 */

/*!
 * Comparaison de 2 tapis
 */

extern 
int tapis_comparer( const tapis_t * const tapis_1 , const tapis_t * const tapis_2 ) ;

/*! Comparaison de 2 tapis (callback) */
extern 
int cb_tapis_comparer( const void * t1 , const void * t2 ) ; 

/*!
 * Echange de 2 cartes entre 2 tapis 
 */

extern
err_t tapis_cartes_echanger( tapis_t * const tapis_1 ,		/*!< tapis 1 */
			     const carte_id_t ind_carte_1 ,	/*!< indice carte dans tapis 1 */
			     tapis_t * const tapis_2 , 		/*!< tapis 2 */
			     const carte_id_t ind_carte_2 ) ;	/*!< indice carte dans tapis 2 */

/*! @} */

#endif
