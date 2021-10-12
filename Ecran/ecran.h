#ifndef _ECRAN_H_
#define _ECRAN_H_

#include <ncurses.h>
#include <commun.h>
#include <tapis.h>

#include <f_tapis.h>

/* 
 * Dans certain UBUNTU 
 * #include <ncurses/ncurses.h>
 */

/*! 
 * \file  ecran.h
 * \brief Structures et Primitives de l'&eacute;cran 
 * 
 * L'ecran est g&eacute;r&eacute; par les primitives de la biblioth&egrave;que "curses"  
 * L'ecran est divise en 2 zones : 
 * \li la zone d'affichage du tapis
 * \li la zone d'affichage des messages
 */

/*
 * CONSTANTES
 */
/*!
 * \def ECRAN_NULL
 * \brief Valeur d'un ecran inexistant
 */
#define ECRAN_NULL NULL

/*!
 * \def ECRAN_LG_MESS
 * \brief Longueur max d'un message 
 */
#define ECRAN_LG_MESS 126

/*!
 * \def ECRAN_PAUSE_MESS
 * \brief Nombre de secondes minimum pendant lesquelles un message reste affiche 
 */
#define ECRAN_PAUSE_MESS 1
/*!
 * \def ECRAN_NB_TAPIS_LIGNE 
 * \brief Nombre de lignes composant la fenetre d'un tapis
 */
#define ECRAN_NB_TAPIS_LIGNE 3

/*!
 * \def  ECRAN_PAUSE_DEPLACEMENT 
 * \brief Nombre de millisecondes entre deux deplacements d'une carte 
 */
#define ECRAN_PAUSE_DEPLACEMENT 50000

/*
 * VARIABLES GLOBALES 
 */

/*!
 * \var Ecran_Nb_Ref
 * \brief Compteur de r&eacute;f&eacute;rences sur un objet fenetre de damier 
 */
int Ecran_Nb_Ref ; 

/*
 * TYPES
 */

/*!
 * \typedef ligne_id_t 
 * \brief type d'un index de ligne d'ecran 
 */
typedef unsigned int ligne_id_t ; 

/*!
 * \struct ecran_t 
 * \brief Type de la structure d'&eacute;cran d'un damier 
 */
typedef struct ecran_s
{
  WINDOW * ecran ; 		/*!< Ecran  */
  f_tapis_t * tapis_central ;		/*!< Fenetre du tapis central */
  f_tapis_t ** tapis_joueurs ;		/*!< Fenetres des tapis des joueurs */
  int nb_joueurs ; 
  WINDOW * messages ;		/*!< Fenetre des messages */
  ligne_id_t y_messages ;	/*!< Ordonnee de la fenetre des messages */
  char * nom_err ;		/*!< Nom du fichier d'erreurs */
} ecran_t ; 

/*
 * FONCTIONS 
 */

/*!
 * \name Methodes d'acces 
 * @{
 */

/*! 
 * \fn WINDOW * ecran_ecran_lire( const ecran_t * const ecran ) ; 
 * \brief acces a la fenetre de l'ecran 
 * \param[in] ecran : ecran d'affichage
 */
extern WINDOW * ecran_ecran_lire( const ecran_t * const ecran ) ; 

/*! 
 * \fn f_tapis_t * ecran_tapis_central_lire( const ecran_t * const ecran )
 * \brief acces a la fenetre du tapis central 
 * \param[in] ecran : ecran d'affichage
 */
extern f_tapis_t * ecran_tapis_central_lire( const ecran_t * const ecran ) ; 
/*! 
 * \fn f_tapis_t * ecran_tapis_joueur_lire( const ecran_t * const ecran , 
 *					    const int ind_joueur ) ; 
 * \brief acces au tapis du joueur d'indice ind_joueur
 * \param[in] ecran : ecran d'affichage
 * \param[in] ind_joueur : indice du joueur 
 */
extern f_tapis_t * ecran_tapis_joueur_lire( const ecran_t * const ecran , 
					    const int ind_joueur ) ; 
/*! 
 * \fn int ecran_nb_joueurs_lire( const ecran_t * const ecran )
 * \brief acces au nombre de joueurs
 * \param[in] ecran : ecran d'affichage
 */
extern int ecran_nb_joueurs_lire( const ecran_t * const ecran ) ; 

/*!
 * \fn ligne_id_t  ecran_y_messages_lire( const ecran_t * ecran ) 
 * \brief Acces au numero de la ligne (l'ordonnee y) ou commence la fenetre des messages. 
 * \param[in] ecran : ecran d'affichage 
 */
extern ligne_id_t  ecran_y_messages_lire( const ecran_t * ecran ) ; 

/*! @} */

/*!
 * \name Tests 
 * @{
 */

/*!
 * \fn booleen_t  ecran_existe( const ecran_t * ecran ) 
 * \brief Test d'exsiatnce d'un ecran 
 * \param[in] ecran : un ecran
 */
extern booleen_t  ecran_existe( const ecran_t * ecran ) ; 
/*! @} */

  ;
/*!
 * \fn ecran_t * ecran_creer( tapis_t * const tapis_central ,
 *		       tapis_t ** const tapis_joueurs ,
 *		       const tapis_id_t nb_joueurs)
 * \brief Creation fenetre de l'tapis 
 *
 * La sortie erreur standard est redirig&eacute;e dans un fichier pour 
 * afficher les erreurs dans la fenetre des messages et ne pas polluer l'affichage de l'ecran
 * \param[in] tapis : tapis a afficher
 * \return une fenetre dans laquelle on peut afficher le tapis
 */
extern ecran_t * ecran_creer(tapis_t * const tapis_central ,
			     tapis_t ** const tapis_joueurs ,
			     const tapis_id_t nb_joueurs ) ;

/*!
 * \fn err_t ecran_detruire( ecran_t ** ecran )
 * \brief Destruction fenetre de l'tapis 
 * \param[in,out] ecran:  fenetre &agrave; d&eacute;truire
 */
extern err_t ecran_detruire( ecran_t ** ecran )  ; 

/*!
 * \fn err_t ecran_afficher( const ecran_t * ecran  , tapis_t * const tapis)
 * \brief Affichage du jeu 
 *
 */
extern err_t ecran_afficher( const ecran_t * ecran  , 
			     tapis_t * const tapis_central ,
			     tapis_t ** tapis_joueurs )  ; 

/*!
 * \fn void ecran_message_afficher( const ecran_t * ecran , char * const mess ) 
 * \brief Affichage d'un message dans la fenetre des messages
 * \param[in] ecran : ecran dans lequel on affiche
 * \param[in] mess : message a afficher
 */
extern void ecran_message_afficher( const ecran_t * ecran , char * const mess ) ; 

/*!
 * \fn void ecran_message_pause_afficher( const ecran_t * ecran , char * const mess ) 
 * \brief Affichage d'un message avec une pause de ECRAN_PAUSE_MESS seconde(s) avant qu'il ne soit &eacute;ventuellment effac&eacute; 
 * dans la fenetre des messages
 * \param[in] ecran : ecran dans lequel on affiche
 * \param[in] mess : message a afficher
 */
extern void ecran_message_pause_afficher( const ecran_t * ecran , char * const mess ) ; 

/*!
 * \fn void ecran_message_effacer( const ecran_t * ecran ) 
 * \brief Efface le contenu de la fenetre des messages
 * \param[in] ecran : ecran dans lequel on affiche
 */
extern void ecran_message_effacer( const ecran_t * ecran ) ; 



  
/*!
 * \fn void ecran_stderr_afficher( ecran_t * ecran ) 
 * \brief Affichage de la sortie standard d'erreurs sur l'ecran 
 * dans la fenetre des messages 
 * \param[in] ecran : ecran dans lequel on affiche
 */

extern void ecran_stderr_afficher( ecran_t * ecran )  ; 


/*!
 * \fn void ecran_cartes_echanger( ecran_t * const ecran , 
 *		  	           f_carte_t * const f_carte_1 ,
 *		  	           f_carte_t * const f_carte_2 ) 
 * \brief Animation pour l'echange de 2 fenetre de cartes
 * dans la fenetre des messages 
 */
extern 
void ecran_cartes_echanger( ecran_t * const ecran , 
			    f_carte_t * const f_carte_1 ,
			    f_carte_t * const f_carte_2 ) ;
#endif
