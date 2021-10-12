#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <ncurses.h>

#include <commun.h>
#include <paquet.h>
#include <ecran.h>


booleen_t fini = FAUX ; 

int
main( int argc , char * argv[] ) 
{
  WINDOW * ecran ;
  f_carte_t * fenetre_carte ;

  paquet_t * paquet = PAQUET_NULL ; 

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 1 ) 
    {
      printf( " Programme de test sur l'affichage des cartes avec ncurses\n" );
      printf( " usage : %s \n" , argv[0] );
      exit(0); 
    }

  initscr() ; 

  ecran = newwin( 7 , 15 , 0, 0 );
  noecho() ; 

  box( ecran , '.' , '.' ) ;
  wrefresh(ecran)  ; 

  sleep(1)  ;

  /* Creation fenetre de carte */
  fenetre_carte = f_carte_creer( ecran , 1 , 2 ) ; 
  
  /* Creation paquet de cartes */
  paquet = paquet_creer() ; 
  int i ; 
  int nb_cartes = paquet_nb_cartes(paquet) ; 
  for( i=0 ; i<nb_cartes ; i++ ) 
    {
      f_carte_afficher( fenetre_carte , 
			paquet_carte_lire( paquet , i) ) ; 
      sleep(1) ; 
    }

  f_carte_detruire( &fenetre_carte ) ; 

  endwin() ; 

  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
}

