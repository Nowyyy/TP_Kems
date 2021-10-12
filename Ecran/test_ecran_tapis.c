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
  tapis_t ** tapis = NULL ; 
  tapis_id_t t = 0 ; /* Compteur de tapis */
  int t_l = 0 ; /* Compteur tapis ligne */
  int t_c = 0 ; /* Compteur tapis colonne */
  carte_id_t c = 0 ; /* Compteur de cartes */

  WINDOW * ecran = NULL ; 
  f_tapis_t ** fenetre_tapis = NULL ;  


  signal( SIGINT, arret ) ; 

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 2 ) 
    {
      printf( " Programme de test sur l'affichage de <nb tapis> tapis avec ncurses\n" );
      printf( " usage : %s <nb tapis>\n" , argv[0] );
      exit(0); 
    }

  int NbTapis = atoi(argv[1]) ;

  /* Creation du paquet */
  printf("Creation du paquet..." ) ; fflush(stdout) ; 
  paquet = paquet_creer() ; 
  printf( "OK\n"); 

  /* Creation des tapis */
  printf("Creations des %d tapis..." , NbTapis ) ; fflush(stdout) ; 

  if( ( tapis = malloc( sizeof(tapis_t *) * NbTapis ) ) == NULL )
    {
      printf(" Erreur allocation memoire tableau des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(tapis_t *) * NbTapis) ) ;
      exit(-1) ; 
    }

  for( t=0 ; t<NbTapis ; t++ ) 
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
   * Affichage des tapis avec ncurses 
   */

  initscr() ; 
  noecho() ; 
  
  /* Creation ecran */
  int NbLignes = (NbTapis/ECRAN_NB_TAPIS_LIGNE) + ( NbTapis%ECRAN_NB_TAPIS_LIGNE ? 1 : 0 ) ;
  ecran = newwin( F_TAPIS_NB_LIGNES*NbLignes+2 , 
		  F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE+2 , 
		  0 , 0 ) ;
  box( ecran , '.' , '.' ) ;
  wrefresh(ecran ) ; 

  pause() ; 

  /* Creation fenetres des tapis */

 if( ( fenetre_tapis = malloc( sizeof(f_tapis_t *) * NbTapis ) ) == NULL )
    {
      printf(" Erreur allocation memoire tableau des fenetres des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(f_tapis_t *) * NbTapis) ) ;
      exit(-1) ; 
    }
 
 t_l = 0 ;
 t_c = 0 ;
 for( t=0 ; t<NbTapis ; t++ ) 
   {
     if( ( fenetre_tapis[t] = f_tapis_creer( ecran , 1+F_TAPIS_NB_LIGNES*t_l , 1+F_TAPIS_NB_COLONNES*t_c ) ) == NULL )
       {
	 printf( "Pb creation fenetre du tapis\n") ;
	 pause() ; 
	 goto fin ; 
       } 
     t_c++ ;
     if( t_c >= ECRAN_NB_TAPIS_LIGNE )
       {
	 t_c = 0 ; 
	 t_l++ ; 
       }
   }

  /* Affichages fenetres des tapis */
  for( t=0 ; t<NbTapis ; t++ ) 
    {
      f_tapis_afficher( fenetre_tapis[t] , tapis[t] ) ; 
    }

  pause() ; 

 fin : 
  endwin() ; 

 /* Destruction ecran */
  delwin(ecran) ;
  
  /* Destruction des fenetres des tapis */
  printf( "Destruction des fenetres des tapis...") ; fflush(stdout) ; 
  for( t=0 ; t<NbTapis ; t++ ) 
    {
      if( ( cr = f_tapis_detruire( &(fenetre_tapis[t]) ) ) )
	{
	  printf(" Erreur sur destruction de fenetre du tapis%ld\n"  , t ) ;
	  erreur_afficher(cr) ; 
	  exit(-1) ; 
	}
    }
  free( fenetre_tapis ) ;
  printf("OK\n") ; 
 
  /* Destruction des tapis */
  printf( "Destruction des tapis...") ; fflush(stdout) ; 
  for( t=0 ; t<NbTapis ; t++ )  
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

