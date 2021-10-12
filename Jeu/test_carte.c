#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <carte.h>

#define NB_CARTES 32


int
main( int argc , char * argv[] ) 
{
  carte_t * Carte[NB_CARTES] ;
  int i = 0 ;
  couleur_t c = CARTE_PREMIERE_COULEUR ; 
  numero_t  n = CARTE_PREMIER_NUMERO ; 
  err_t cr = OK ; 

  srandom(getpid());

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  printf("Creation du paquet de cartes\n") ;
  i=0 ;
  for( c=CARTE_PREMIERE_COULEUR ; c<= CARTE_DERNIERE_COULEUR ; c++ ) 
    for( n=CARTE_PREMIER_NUMERO ; n<=CARTE_DERNIER_NUMERO ; n++ )
      {
	if( ( Carte[i] = carte_creer( c , n ) ) == CARTE_NULL )
	  printf("Erreur sur creation de la carte %d\n" , i ) ;
	i++ ; 
      }
	  
  printf("\tAffichage du paquet de cartes\n") ;
  for( i=0 ; i<NB_CARTES ; i++ ) 
    {
      carte_stdout_afficher( Carte[i] ) ;
      printf("\n");
    }
 
  printf("Comparaison de 2 cartes (X pour arreter)\n") ;
  int c1= 0 ;
  int c2= 0 ;
  char rep = ' ' ; 
  int cmp = 0 ; 
  while( rep != 'X' )
    {
      c1=random()%NB_CARTES ;
      c2=random()%NB_CARTES ;
      cmp = carte_comparer( Carte[c1] , Carte[c2] ) ;   

      carte_stdout_afficher(Carte[c1]) ;
      if( cmp == 0 ) printf( " = " ) ;
      if( cmp < 0 ) printf( " < " ) ; 
      if( cmp > 0 ) printf( " > " ) ; 
      carte_stdout_afficher( Carte[c2] ) ;
      printf( "\n" ) ; 

      printf("Continu ? --> " ) ; 
      rep=getchar() ;
    }

  printf("\nDestruction des cartes du paquet..." ) ; fflush(stdout) ; 
  for( i=0 ; i<NB_CARTES ; i++ ) 
    {
      if( ( cr = carte_detruire( &Carte[i] ) ) )
	printf(" Erreur sur destruction de la carte numero %d\n" , i ) ;
    }
  printf("OK\n") ; 

  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
}

