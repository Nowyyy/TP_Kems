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
  
  char mess[256] ; 
  booleen_t fini = FAUX ; 
  booleen_t echange = FAUX ; 
  int i = 0 ; 

  carte_id_t ind_carte ; 
  carte_id_t ind_carte_central ; 

  signal( SIGINT, arret ) ; 

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 2 ) 
    {
      printf( " Programme de test sur l'affichage de <nb joueurs> tapis avec ncurses\n" );
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
   * Creation et affichage de l'ecran  
   */

  if( ( ecran = ecran_creer( tapis_central , 
			     tapis , 
			     NbJoueurs ) ) == NULL ) 
    {
      printf("Erreur sur la creation de l'ecran\n"); 
      exit(-1) ;
    }

  ecran_message_afficher( ecran , "Debut de partie: ^C pour commencer");
  pause() ; 

  /* Phase de jeu */
  fini = FAUX ; 
  while( ! fini ) 
    {
      echange=FAUX ; 

      for( i=0 ; i<NbJoueurs ; i++ ) /* boucle des joueurs */
	{

	  /* Test arret */
	  if( tapis_carre( tapis[i] ) )
	    {
	      
	      fini = VRAI ;
	      sprintf( mess ,  "Le joueur %2d a gagne" , i ) ; 
	      ecran_message_pause_afficher( ecran , mess ) ; 
	      goto fin ;
	    }

	  if( ( cr = tapis_cartes_choisir( &echange , tapis[i] , &ind_carte , tapis_central , &ind_carte_central) ) )
	    {
	      sprintf( mess , "Pb dans choix des cartes, code retour = %d\n", cr ) ;
	      ecran_message_pause_afficher( ecran , mess ) ; 
	      erreur_afficher(cr) ; 
	      goto fin ;
	    }

	  if( echange ) 
	    {
	      if( ( cr = tapis_cartes_echanger( tapis[i] , ind_carte , tapis_central , ind_carte_central ) ) )
		{
		  sprintf( mess, "Pb d'echange de cartes entre la carte %ld du tapis du joueur %d et la carte %ld du tapis central" , 
			   ind_carte , i , ind_carte_central ); 
		  ecran_message_pause_afficher( ecran , mess ) ; 
		  erreur_afficher(cr) ; 
		  goto fin ; 
		}	     	 
	      sprintf( mess , "Joueur %i: Echange carte %ld avec carte %ld du tapis central " , 
		       i , ind_carte , ind_carte_central ) ;
	      ecran_message_pause_afficher( ecran , mess ) ; 
	      ecran_cartes_echanger( ecran , 
				     f_tapis_f_carte_lire( ecran_tapis_central_lire( ecran ) , ind_carte_central ) ,
				     f_tapis_f_carte_lire( ecran_tapis_joueur_lire( ecran , i ) , ind_carte ) ) ;
	      ecran_afficher( ecran , tapis_central , tapis ) ; 
	      ecran_message_effacer( ecran ) ;
	    }
	}

      if( ! echange ) 
	{
	  /* 
	   * Pas un seul echange des joueur 
	   * --> redistribution du tapis central 
	   */
	  ecran_message_pause_afficher( ecran , "Pas d'echange --> Redistribution tapis central") ; 
	  for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
	    {
	      if( ( cr = tapis_carte_retirer( tapis_central , c , paquet ) ) )
		{
		  ecran_message_pause_afficher(ecran , "Pb dans retrait d'une carte du tapis central" ); 
		  erreur_afficher(cr) ; 
		  goto fin ; 
		}
	  
	      if( ( cr = tapis_carte_distribuer( tapis_central , c , paquet ) ) )
		{
		  ecran_message_pause_afficher( ecran , "Pb dans distribution d'une carte pour le tapis central" ); 
		  erreur_afficher(cr) ; 
		  goto fin ; 
		}
	    }
	  ecran_afficher( ecran  , tapis_central , tapis ) ;
	  ecran_message_effacer(ecran) ; 
	}

    }

  /* Fin du jeu */

 fin : 
  pause() ; 

  /* Destruction de l'ecran */
  if( ( cr = ecran_detruire( &ecran ) ) )
    {
      fprintf( stderr , "Erreur lors de la destruction de l'ecran, cr = %d\n" , cr ) ;
      exit(-1) ; 
    }
    

  /* Destructions du tapis central */
  printf( "Destructions du tapis central...") ; fflush(stdout) ; 
  if( ( cr = tapis_detruire( &tapis_central ) ) )
    {
      fprintf( stderr , " Erreur sur destruction du tapis central\n") ;
      erreur_afficher(cr) ; 
      exit(-1) ; 
    }
  printf("OK\n"); 
  
  /* Destructions des tapis des joueurs */
  printf( "Destructions des tapis des joueurs...") ; fflush(stdout) ; 
  for( t=0 ; t<NbJoueurs ; t++ )  
    {

      if( ( cr = tapis_detruire( &tapis[t] ) ) )
       {
	 fprintf( stderr , " Erreur sur destruction du tapis %ld\n"  , t ) ;
	 erreur_afficher(cr) ; 
	 exit(-1) ; 
       }

    }
  free( tapis ) ;
  printf("OK\n"); 

  /*  Destruction du paquet */
  printf("\nDestruction du paquet..." ) ; fflush(stdout) ; 
  if( ( cr = paquet_detruire( &paquet ) ) )
    {
	 fprintf( stderr , " Erreur sur destruction du paquet\n" ) ;
	 erreur_afficher(cr) ; 
	 exit(-1) ; 
    }
  printf("OK\n") ; 
 
  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
}

