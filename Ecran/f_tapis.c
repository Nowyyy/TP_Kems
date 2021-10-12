#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <f_tapis.h>


/*
 * Methodes d'acces 
 */


extern 
f_carte_t * f_tapis_f_carte_lire( const f_tapis_t * f_tapis , 
				  const carte_id_t ind_carte ) 
{
  return( f_tapis->cartes[ind_carte]) ; 
}

/*
 * Creation fenetre du tapis 
 */

extern 
f_tapis_t * f_tapis_creer( WINDOW * fenetre_mere , 
			   const int x , const int y ) 
{
  f_tapis_t * f_tapis ; 
  carte_id_t i = 0 ; 

  if( (f_tapis = malloc(sizeof(f_tapis_t))) == NULL )
    {
      fprintf( stderr , "f_tapis_creer: pd d'allocation memoire (%lu octets demandes)\n",
	       (long unsigned int)sizeof(f_tapis_t) ) ;
      return(NULL) ; 	       
    }

  /* Creation fenetre du tapis */
  if( ( f_tapis->fenetre = subwin( fenetre_mere , 
				   F_TAPIS_NB_LIGNES , F_TAPIS_NB_COLONNES , 
				   x , y ) ) == NULL )
    {
      char mess[512] ;
      sprintf( mess , "f_tapis_creer: Pb creation fenetre du tapis;\nDim tapis demande: %d lignes X %d colonnes\n" ,
	       F_TAPIS_NB_LIGNES , F_TAPIS_NB_COLONNES );
      perror(mess);
      return(NULL) ;
    }

  box( f_tapis->fenetre , '.' , '.' ) ;
  wrefresh(f_tapis->fenetre) ; 

  /* Creations fenetres des cartes */ 
  for( i=0 ; i<TAPIS_NB_CARTES ; i++ )
    {
      f_tapis->cartes[i] = f_carte_creer( f_tapis->fenetre , x+1 , y+1+(F_CARTE_NB_COLONNES+1)*i ) ; 
    }


  
  return(f_tapis) ; 
}

/*
 * Destruction fenetre de tapis
 */

extern 
err_t f_tapis_detruire( f_tapis_t ** f_tapis ) 
{
  carte_id_t i = 0 ; 

  if( f_tapis_existe((*f_tapis)) )
    {
      /* Destruction des fenetres des cartes */
      for( i=0 ; i<TAPIS_NB_CARTES ; i++ ) 
	f_carte_detruire( &((*f_tapis)->cartes[i]) ) ;

      /* Destruction de la fenetre du tapis */
      delwin((*f_tapis)->fenetre) ;

      /* Liberation structure f_tapis */
      free((*f_tapis)) ; 

      (*f_tapis) = NULL ;
    }
 
  return(OK) ; 
}

/*
 * Tests
 */

extern 
booleen_t  f_tapis_existe( const f_tapis_t * f_tapis ) 
{
  if( f_tapis == NULL )
    return(FAUX) ; 
  else
    return(VRAI) ; 
}



/*
 * Affichage d'un tapis dans une fenetre de tapis 
 */

extern 
void f_tapis_afficher( f_tapis_t * f_tapis  ,
		       const tapis_t * const tapis ) 
{
  carte_id_t i = 0 ; 


  for( i=0 ; i<TAPIS_NB_CARTES ; i++ ) 
    {
      f_carte_afficher( f_tapis_f_carte_lire( f_tapis , i )  , 
			tapis_carte_lire(tapis , i ) ) ;
    }

  wrefresh(f_tapis->fenetre) ; 

  return ; 
}
