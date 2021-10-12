#ifndef _CARTE_H_
#define _CARTE_H_

#include <commun.h>
#include <erreur.h>

/*! 
 * \file  carte.h
 * \brief Type et primitives d'une carte
 */


/*
 * CONSTANTES 
 */
/*!
 * \def CARTE_NULL 
 * \brief Carte inexistante
 */
#define CARTE_NULL NULL

/*
 * TYPES 
 */


/*!
 * \enum couleur_t 
 * \brief Couleur d'une carte
 */
typedef enum couleur_s { TREFLE , PIQUE , CARREAU , COEUR } couleur_t ;
#define CARTE_PREMIERE_COULEUR TREFLE
#define CARTE_DERNIERE_COULEUR COEUR

/*!
 * \enum numero_t 
 * \brief numero d'une carte
 */
typedef enum numero_s { SEPT=7 , HUIT=8 , NEUF=9 , DIX=10 , VALET=11 , DAME=12 , ROI=13 , AS=14 } numero_t ;
#define CARTE_PREMIER_NUMERO SEPT
#define CARTE_DERNIER_NUMERO AS

/*!
 * \struct carte_t 
 * \brief Type d'une pi&egrave;ce d'un damier
 */
typedef struct carte_s 
{
  couleur_t couleur ;		/*!< Couleur de la carte */
  numero_t numero ;		/*!< Numero de la carte */
} carte_t ; 

/*!
 * \typedef carte_id_t 
 * \brief Type d'index pour les  pi&egrave;ces (indices, compteurs...)
 */
typedef tab_id_t carte_id_t  ;

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */

/*! Acces a la couleur de la carte */
extern couleur_t carte_couleur_lire( const carte_t * carte ) ;
extern char * carte_couleur_string( const carte_t * carte  ) ; 

/*! Acces au numero de la carte */
extern numero_t carte_numero_lire( const carte_t * carte ) ;
extern char * carte_numero_string( const carte_t * carte );

/*!
 * @}
 * \name Tests
 * @{
 */

/*! Existance d'une carte */
extern 
booleen_t carte_existe( const carte_t * carte ) ;

/*!
 * @}
 * \name Methodes d'affectations
 * @{
 */

/*! Affectation de la couleur de la carte */
extern 
err_t carte_couleur_ecrire( carte_t * carte , const couleur_t couleur ) ;

/*! Affectation du numero  de la carte */
extern
err_t carte_numero_ecrire( carte_t * carte , const numero_t numero ) ;

/*!
 * @}
 * \name Primitives
 * @{
 */

/*! Copie profonde d'une carte par une autre carte */
extern 
err_t carte_copier( carte_t ** c1 , const carte_t * c2 )  ; 

/*! Copie profonde d'une carte par une autre carte (callback) */
extern 
err_t cb_carte_copier( void ** c1 , const void * c2 ) ;

/*! Referencement d'une carte dans une autre carte */
extern 
err_t carte_referencer( carte_t ** c1 , carte_t * c2 )  ; 

/*! Referencement d'une carte dans une autre carte (callback) */
extern 
err_t cb_carte_referencer( void * * c1 , const void * c2 ) ;

/*!
 * Creation d'une carte 
 */
extern
carte_t * carte_creer( const couleur_t couleur , 
		       const numero_t numero )  ; 

/*! 
 * Destruction d'une carte 
 */

extern
err_t carte_detruire( carte_t ** carte )  ;

/*! 
 * Destruction d'une carte (callback)
 */
extern
err_t cb_carte_detruire( void * * carte )  ;

/*!
 * Effacement de la reference d'une carte 
 */

extern
err_t carte_reference_effacer( carte_t ** carte )  ;

/*!
 * Effacement de la reference d'une carte (callback)
 */

extern
err_t cb_carte_reference_effacer( void * * carte )  ;

/*!
 * Affichage
 */

/*! Pour  Debuggage */

extern
void carte_stdout_afficher( const carte_t * carte ) ; 

/*! Affichage (callback) */
extern
void cb_carte_stdout_afficher( const void * carte ) ;

/*!
 * Comparaison de 2 cartes
 */

extern 
int carte_comparer( const carte_t * c1 , const carte_t * c2 ) ;

/*! Comparaison de 2 cartes (callback) */
extern 
int cb_carte_comparer( const void * c1 , const void * c2 ) ;

/*! @} */

#endif
