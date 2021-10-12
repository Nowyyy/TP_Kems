#ifndef _F_TAPIS_H_
#define _F_TAPIS_H_

#include <ncurses.h>
#include <commun.h>
#include <tapis.h>

#include <f_carte.h>


/* 
 * Dans certain UBUNTU 
 * #include <ncurses/ncurses.h>
 */

/*! 
 * \file  f_tapis.h
 * \brief Structures et Primitives d'une fenetre tapis 
 */

/*
 * CONSTANTES
 */
/*!
 * \def F_TAPIS_NULL
 * \brief Valeur d'un f_tapis inexistant
 */
#define F_TAPIS_NULL NULL

#define F_TAPIS_NB_LIGNES (F_CARTE_NB_LIGNES+2) 
#define F_TAPIS_NB_COLONNES ((F_CARTE_NB_COLONNES+1)*TAPIS_NB_CARTES+1)

/*
 * VARIABLES GLOBALES 
 */


/*
 * TYPES
 */


/*!
 * \struct f_tapis_t 
 * \brief Type de la structure d'une fenetre de tapis 
 */
typedef struct f_tapis_s
{
  WINDOW * fenetre ;				/*!< fenetre du tapis */
  f_carte_t * cartes[TAPIS_NB_CARTES] ;		/*!< fenetres des cartes du tapis */
} f_tapis_t ; 

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */
extern 
f_carte_t * f_tapis_f_carte_lire( const f_tapis_t * f_tapis , 
				  const carte_id_t ind_carte ) ;


/*! @} */

/*!
 * \name Tests 
 * @{
 */

/*!
 * \fn booleen_t  f_tapis_existe( const f_tapis_t * f_tapis ) 
 * \brief Test d'exsiatnce d'un f_tapis 
 * \param[in] f_tapis : un f_tapis
 */
extern booleen_t  f_tapis_existe( const f_tapis_t * f_tapis ) ; 
/*! @} */

  ;
/*!
 * \fn f_tapis_t * f_tapis_creer( tapis_t * const tapis )
 * \brief Creation d'une fenetre de tapis  
 */
extern f_tapis_t * f_tapis_creer( WINDOW * fenetre_mere , 
				  const int x , const int y ) ;

/*!
 * \fn err_t f_tapis_detruire( f_tapis_t ** f_tapis )
 * \brief Destruction f_tapis de l'tapis 
 * \param[in,out] f_tapis:  f_tapis &agrave; d&eacute;truire
 */
extern err_t f_tapis_detruire( f_tapis_t ** f_tapis )  ; 

/*
 * Affichage d'une tapis dans une fenetre de tapis 
 * Dim fenetre de tapis: 3 lignes X 9 colonnes 
 */

extern 
void f_tapis_afficher( f_tapis_t * f_tapis  ,
		       const tapis_t * const tapis ) ;

#endif
