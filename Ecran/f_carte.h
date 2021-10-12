#ifndef _F_CARTE_H_
#define _F_CARTE_H_

#include <ncurses.h>
#include <commun.h>
#include <carte.h>


/* 
 * Dans certain UBUNTU 
 * #include <ncurses/ncurses.h>
 */

/*! 
 * \file  f_carte.h
 * \brief Structures et Primitives d'une fenetre carte 
 */

/*
 * CONSTANTES
 */
/*!
 * \def F_CARTE_NULL
 * \brief Valeur d'un f_carte inexistant
 */
#define F_CARTE_NULL NULL

#define F_CARTE_NB_LIGNES 5
#define F_CARTE_NB_COLONNES 11

/*
 * VARIABLES GLOBALES 
 */


/*
 * TYPES
 */


/*!
 * \struct f_carte_t 
 * \brief Type de la structure d'une fenetre de carte 
 */
typedef struct f_carte_s
{
  WINDOW * fenetre ;		/*!< fenetre Carte du tapis */
  int y ;			/*!< coord : numero de ligne sur l'ecran */
  int x ; 			/*!< coord : numero de colonne sur l'ecran */
} f_carte_t ; 

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */
extern 
WINDOW * f_carte_fenetre_lire( const f_carte_t * f_carte ) ;

extern 
int f_carte_x_lire( const f_carte_t * f_carte ) ;

extern 
int f_carte_y_lire( const f_carte_t * f_carte ) ;

/*! @} */

/*!
 * \name Tests 
 * @{
 */

/*!
 * \fn booleen_t  f_carte_existe( const f_carte_t * f_carte ) 
 * \brief Test d'exsiatnce d'un f_carte 
 * \param[in] f_carte : un f_carte
 */
extern booleen_t  f_carte_existe( const f_carte_t * f_carte ) ; 
/*! @} */

  ;
/*!
 * \fn f_carte_t * f_carte_creer( WINDOW * fenetre_mere , int x , int y )
 * \brief Creation d'une fenetre de carte  
 */
extern f_carte_t * f_carte_creer(WINDOW * fenetre_mere , 
				 int x , int y ) ;

/*!
 * \fn err_t f_carte_detruire( f_carte_t ** f_carte )
 * \brief Destruction f_carte de l'tapis 
 * \param[in,out] f_carte:  f_carte &agrave; d&eacute;truire
 */
extern err_t f_carte_detruire( f_carte_t ** f_carte )  ; 

/*
 * Affichage d'une carte dans une fenetre de carte 
 * Dim fenetre de carte: 3 lignes X 9 colonnes 
 */

extern 
void f_carte_afficher( f_carte_t * f_carte  ,
		       const carte_t * const carte ) ;

#endif
