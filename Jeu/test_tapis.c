#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <paquet.h>
#include <tapis.h>

int
main( int argc , char * argv[] ) 
{
  tapis_t * tapis1 = TAPIS_NULL ;
  tapis_t * tapis2 = TAPIS_NULL ;
  int i = 0 ;
  err_t cr = OK ; 

  paquet_t * paquet = PAQUET_NULL ; 
  booleen_t echange = FAUX ;

  srandom(getpid());

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  printf("Creation du paquet de cartes\n") ;
  if( ( paquet = paquet_creer() ) == PAQUET_NULL )
    {
      printf("Erreur sur creation du paquet\n" ) ;
      exit(-1) ; 
    }

  printf("Creation d'un tapis\n") ;
  if( ( tapis1 = tapis_creer() ) == TAPIS_NULL )
    printf("Erreur sur creation du tapis\n" ) ;
	  
  printf("\tAffichage du tapis vide\n") ;
  tapis_stdout_afficher( tapis1 ) ;
  printf("\n");


  printf("\tPose de %d cartes sur le tapis\n" , TAPIS_NB_CARTES ) ;
  for( i=0 ; i<TAPIS_NB_CARTES ; i++ ) 
    {
      /* Pose d'une carte prise du paquet sur le tapis */
      tapis_carte_distribuer( tapis1  , i , paquet ) ;
    }
  printf("\n");

  printf("\tAffichage du tapis rempli\n") ;
  tapis_stdout_afficher( tapis1 ) ;
  printf("\n");
  
  
  printf("Comparaison de 2 tapis\n") ;
  int cmp = 0 ; 
  if( ( cr = tapis_copier( &tapis2 , tapis1 ) ) )
    {
      printf( "Pb dans la copîe d'un tapis, code erreur = %d\n" , cr ) ;
      exit(-1) ; 
    }
 
  cmp = tapis_comparer( tapis1 , tapis2 ) ;   

  tapis_stdout_afficher(tapis1) ;
  if( cmp == 0 ) printf( " = \n" ) ;
  if( cmp < 0 ) printf( " < \n" ) ; 
  if( cmp > 0 ) printf( " > \n" ) ; 
  tapis_stdout_afficher( tapis2 ) ;
  printf( "\n" ) ; 

  if( cmp == 0 ) 
    printf( "---> OK \n" ) ; 
  else
    printf( "---> KO !!!!\n") ; 

  for( i=0 ; i<TAPIS_NB_CARTES ; i++ )   
    tapis_carte_distribuer( tapis2  , i , paquet ) ;

  cmp = tapis_comparer( tapis1 , tapis2 ) ;   

  tapis_stdout_afficher(tapis1) ;
  if( cmp == 0 ) printf( " = \n" ) ;
  if( cmp < 0 ) printf( " < \n" ) ; 
  if( cmp > 0 ) printf( " > \n" ) ; 
  tapis_stdout_afficher( tapis2 ) ;
  printf( "\n" ) ; 


  printf( "Retrait de cartes du tapis1 \n");
  for( i=0 ; i<TAPIS_NB_CARTES ; i++ ) 
    {
      printf( "Retrait carte %d\n" , i ) ;
      tapis_carte_retirer( tapis1  , i , paquet ) ;
      printf( "Affichage du tapis\n");
      tapis_stdout_afficher(tapis1) ; 
      printf( "Affichage du paquet\n");
      paquet_stdout_afficher(paquet); 
      
    }

  carte_t * carte = carte_creer( PIQUE , HUIT ) ;
  tapis_carte_poser( tapis1 , 0 , carte ) ; 
  carte_couleur_ecrire( carte , TREFLE ) ; 
  tapis_carte_poser( tapis1 , 1 , carte ) ;
  carte_numero_ecrire( carte , DIX ) ;  
  carte_couleur_ecrire( carte , PIQUE ) ; 
  tapis_carte_poser( tapis1 , 2 , carte ) ; 
  carte_numero_ecrire( carte , HUIT ) ;  
  carte_couleur_ecrire( carte , CARREAU ) ; 
  tapis_carte_poser( tapis1 , 3 , carte ) ;

  tapis_stdout_afficher( tapis1 ) ;
  if( tapis_carre( tapis1 ) )
    printf( "\tLe tapis contient un carre --> KO !!!\n") ; 
  else
    printf("\tLe tapis ne contient pas un carre ---> OK \n" ) ;

  carte_numero_ecrire( carte , ROI ) ;
  carte_couleur_ecrire( carte , TREFLE ) ;
  tapis_carte_poser( tapis2 , 0 , carte ) ; 
  carte_couleur_ecrire( carte , PIQUE ) ; 
  tapis_carte_poser( tapis2 , 1 , carte ) ;
  carte_numero_ecrire( carte , HUIT ) ;  
  carte_couleur_ecrire( carte , COEUR ) ; 
  tapis_carte_poser( tapis2 , 2 , carte ) ; 
  carte_numero_ecrire( carte , ROI ) ;  
  carte_couleur_ecrire( carte , CARREAU ) ; 
  tapis_carte_poser( tapis2 , 3 , carte ) ;

  tapis_stdout_afficher( tapis2 ) ;

  printf( "Choix de 2 cartes a echanger\n") ; 
  carte_id_t ind_carte_1 = -1 ; 
  carte_id_t ind_carte_2 = -1 ; 
  if( ( cr = tapis_cartes_choisir( &echange , tapis1 , &ind_carte_1, tapis2 , &ind_carte_2 ) ) )
    {
      printf( "Pb dans choix des cartes, code erreur = %d\n" , cr ) ;
      exit(-1) ; 
    }

  if( echange ) 
    {
      printf("Un choix a ete fait\n") ; 
 
      printf( "Carte tapis 1 choisie : ") ; 
      carte_stdout_afficher( tapis_carte_lire( tapis1 , ind_carte_1 ) ) ;
      if( ind_carte_1 == 2 ) 
	printf( " ---> OK \n" ) ;
      else
	printf( " ---> KO !!!!\n" ) ; 

      printf( "Carte tapis 2 choisie : ") ; 
      carte_stdout_afficher( tapis_carte_lire( tapis2 , ind_carte_2 ) ) ;
      if( ind_carte_2 == 2 ) 
	printf( " ---> OK \n" ) ;
      else
	printf( " ---> KO !!!!\n" ) ; 

      if( ( cr = tapis_cartes_echanger( tapis1 , ind_carte_1 , 
					tapis2 , ind_carte_2 ) ) )
	{
	  printf( "Pb dans l'echange des cartes, code erreur = %d\n" , cr ) ;
	  exit(-1) ; 
	}

      printf( "Tapis apres echange\nTapis 1:\n" ) ; 
      tapis_stdout_afficher( tapis1 ) ;
      printf( "\nTapis2 :\n" ) ; 
      tapis_stdout_afficher( tapis2 ) ;
      printf( "\n" ) ; 

    }
  else
    {
      printf("Pas de choix fait\n") ; 
    }

  printf("\nDestruction des tapis ..." ) ; fflush(stdout) ; 
  
  if( ( cr = tapis_detruire( &tapis1) ) )
    printf(" Erreur sur destruction du tapis numero 1\n" ) ;
  if( ( cr = tapis_detruire( &tapis2) ) )
    printf(" Erreur sur destruction du tapis numero 2\n" ) ;
  printf("OK\n") ; 
 

  printf("\nDestruction du paquet..." ) ; fflush(stdout) ; 
  paquet_detruire( &paquet ) ;
  printf("OK\n") ; 
 
  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );
 
  return(0) ;
}

