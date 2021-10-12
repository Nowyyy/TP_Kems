#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <ncurses.h>

#include <commun.h>
#include <paquet.h>
#include <ecran.h>


void arret( int sig )
{
  /* printf( "Arret utilisateur\n");*/
} 

int
main( int argc , char * argv[] ) 
{
  err_t cr = OK ; 

  paquet_t * paquet = PAQUET_NULL ; 
  tapis_t * tapis_central = TAPIS_NULL ; /* tapis central */
  tapis_t ** tapis = NULL ; /* tapis des joueurs */

  tapis_id_t t = 0 ; /* Compteur de tapis */
  carte_id_t c = 0 ; /* Compteur de cartes */

  ecran_t * ecran = NULL ; 
  
  signal( SIGINT, arret ) ; 

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 2 ) 
    {
      printf( " Programme de test sur l'affichage de l'ecran du jeu de kems pour <nb joueurs> avec ncurses\n" );
      printf( " usage : %s <nb joueurs>\n" , argv[0] );
      exit(0); 
    }

  int NbJoueurs = atoi(argv[1]) ;
 
  /* Creation du paquet */
  printf("Creation du paquet..." ) ; fflush(stdout) ; 
  paquet = paquet_creer() ; 
  printf( "OK\n"); 

  /* Creation tapis central */
  printf("Creation du tapis central...")  ; fflush(stdout) ; 
  if( ( tapis_central = tapis_creer() ) == TAPIS_NULL )
    {
      printf("Erreur sur creation du tapis central\n" ) ;
      exit(-1) ;
    }

  for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
    {
      if( ( cr = tapis_carte_distribuer( tapis_central  , c , paquet ) ) )
	{
	  erreur_afficher(cr) ; 
	  exit(-1) ; 
	}
    }
  printf("OK\n"); 

  /* Creation des tapis des joueurs */
  printf("Creations des %d tapis..." , NbJoueurs ) ; fflush(stdout) ; 

  if( ( tapis = malloc( sizeof(tapis_t *) * NbJoueurs ) ) == NULL )
    {
      printf(" Erreur allocation memoire tableau des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(tapis_t *) * NbJoueurs) ) ;
      exit(-1) ; 
    }

  for( t=0 ; t<NbJoueurs ; t++ ) 
    {

      if( ( tapis[t] = tapis_creer() ) == TAPIS_NULL )
	{
	  printf("Erreur sur creation du tapis %ld\n" , t ) ;
	  exit(-1) ;
	}

      for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
	{
	  if( ( cr = tapis_carte_distribuer( tapis[t]  , c , paquet ) ) )
	    {
	      if( cr == ERR_PAQUET_VIDE ) printf("Pas assez de cartes pour tous les joueurs\n"); 
	      erreur_afficher(cr) ; 
	      exit(-1) ; 
	    }
	}
    }
  printf( "OK\n") ; 

  /*
   * Creation de l'ecran  
   */

  if( ( ecran = ecran_creer( tapis_central , 
			     tapis , 
			     NbJoueurs ) ) == NULL ) 
    {
      printf("Erreur sur la creation de l'ecran\n"); 
      exit(-1) ;
    }

  
  /* Affichage de l'ecran */
  ecran_afficher( ecran , tapis_central , tapis ) ;
  pause() ; 

 
  /* Destruction de l'ecran */
  if( ( cr = ecran_detruire( &ecran ) ) )
    {
      printf( "Erreur lors de la destruction de l'ecran, cr = %d\n" , cr ) ;
      exit(-1) ; 
    }
   
 
  /* Destruction des tapis */
  printf( "Destruction des tapis...") ; fflush(stdout) ; 
  for( t=0 ; t<NbJoueurs ; t++ )  
    {

      if( ( cr = tapis_detruire( &tapis[t] ) ) )
       {
	 printf(" Erreur sur destruction du tapis%ld\n"  , t ) ;
	 erreur_afficher(cr) ; 
	 exit(-1) ; 
       }

    }
  free( tapis ) ;
  printf("OK\n"); 

  /*  Destruction du paquet */
  printf("\nDestruction du paquet..." ) ; fflush(stdout) ; 
  paquet_detruire( &paquet ) ;
  printf("OK\n") ; 
 
  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
}

