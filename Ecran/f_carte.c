#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <f_carte.h>


/*
 * Methodes d'acces 
 */


extern 
WINDOW * f_carte_fenetre_lire( const f_carte_t * f_carte ) 
{
  return( f_carte->fenetre ) ; 
}

extern 
int f_carte_x_lire( const f_carte_t * f_carte )
{
  return( f_carte->x ) ;
} 

extern 
int f_carte_y_lire( const f_carte_t * f_carte )
{
  return( f_carte->y ) ;
} 

/*
 * Creation fenetre du tapis 
 */

extern 
f_carte_t * f_carte_creer( WINDOW * fenetre_mere , 
			   int lig , int col ) 
{
  f_carte_t * f_carte ; 

  if( (f_carte = (f_carte_t *)malloc(sizeof(f_carte_t))) == NULL )
    {
      fprintf( stderr , "f_carte_creer: pd d'allocation memoire (%lu octets demandes)\n",
	       (long unsigned int)sizeof(f_carte_t) ) ;
      return(NULL) ; 	       
    }

  f_carte->fenetre = subwin( fenetre_mere , 
			     F_CARTE_NB_LIGNES , F_CARTE_NB_COLONNES , 
			     lig , col );
  box( f_carte->fenetre , 0 , 0 ) ;
  wrefresh( f_carte->fenetre ) ; 

  f_carte->x = col ;
  f_carte->y = lig ;

  return(f_carte) ; 
}

/*
 * Destruction fenetre de carte
 */

extern 
err_t f_carte_detruire( f_carte_t ** f_carte ) 
{
  if( f_carte_existe((*f_carte)) )
    {
      /* Destrcution des fenetres */
      delwin((*f_carte)->fenetre) ;

      /* Liberation structure f_carte */
      free((*f_carte)) ; 

      (*f_carte) = NULL ;
    }
 
  return(OK) ; 
}

/*
 * Tests
 */

extern 
booleen_t  f_carte_existe( const f_carte_t * f_carte ) 
{
  if( f_carte == NULL )
    return(FAUX) ; 
  else
    return(VRAI) ; 
}



/*
 * Affichage d'une carte dans une fenetre de carte 
 * Dim fenetre de carte: 3 lignes X 9 colonnes 
 */

extern 
void f_carte_afficher( f_carte_t * f_carte  ,
		       const carte_t * const carte ) 
{
  char * coul = carte_couleur_string(carte) ;
  WINDOW * fenetre = f_carte_fenetre_lire(f_carte) ;

  /* Coin Haut gauche */
  mvwprintw( fenetre , 1 , 1 , "%c%c" , coul[0] , coul[1] );
  /* Milieu */
  mvwprintw( fenetre , 2 , 3 , "%s" , carte_numero_string(carte) ) ;
  /* Coin Bas droit */
  mvwprintw( fenetre , 3 , 8 , "%c%c" , coul[0] , coul[1] );

  wrefresh(fenetre) ; 

  return ; 
}
