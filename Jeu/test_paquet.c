#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <paquet.h>
#define NB_CARTES 32

int
main( int argc , char * argv[] ) 
{
  paquet_t * paquet1 = PAQUET_NULL ;
  paquet_t * paquet2 = PAQUET_NULL ;
  carte_t * carte = CARTE_NULL ;
  err_t cr = OK ; 
 
  srandom(getpid());

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  printf("Creation du paquet de cartes\n") ;
  if( ( paquet1 = paquet_creer() ) == PAQUET_NULL ) 
    {
      printf("Erreur sur la creation du paquet\n");
      exit(-1) ; 
    }

  printf("\tAffichage du paquet\n") ;
  paquet_stdout_afficher( paquet1 ) ;
  printf("\n");

  printf( "Copie d'un paquet\n") ; 
  if( ( cr = paquet_copier( &paquet2 , paquet1 ) ) )
    {
      printf("Erreur dans la copie d'un paquet, cr=%d\n" ,cr ) ; 
      exit(-1) ; 
    }


 printf("Comparaison de 2 paquets identiques\n") ;
 int cmp = 0 ; 
 cmp = paquet_comparer( paquet1 , paquet2 ) ;    
 if( cmp == 0 ) 
   printf( "---> OK \n" ) ; 
 else
   printf( "---> KO !!!!\n") ; 

 printf("Retrait de la carte du sommet\n");
 if( ( carte = paquet_carte_retirer( paquet2 ) ) == CARTE_NULL )
   {
     printf("Erreur dans le retrait d'une carte\n" ) ; 
     exit(-1) ; 
   }

 printf("\t-->" ) ;
 carte_stdout_afficher(carte) ; 
 printf("\n");

 printf("Comparaison de 2 paquets differents\n") ; 
 cmp = paquet_comparer( paquet1 , paquet2 ) ;   
 if( cmp != 0 ) 
   printf( "---> OK \n" ) ; 
 else
   printf( "---> KO !!!!\n") ; 

 printf("\nDestruction des paquets ..." ) ; fflush(stdout) ; 
  
 if( ( cr = paquet_detruire( &paquet1) ) )
   printf(" Erreur sur destruction du paquet numero 1\n" ) ;
 if( ( cr = paquet_detruire( &paquet2) ) )
   printf(" Erreur sur destruction du paquet numero 2\n" ) ;
 printf("OK\n") ; 
 
 printf("\nDestruction de la carte retiree du paquet..." ) ; fflush(stdout) ; 
 if( ( cr = carte_detruire( &carte ) ) )
   {
     printf(" Erreur sur destruction de la carte\n") ;
     exit(-1); 
   }
 printf("OK\n") ; 
 
 printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );
 
 return(0) ;
}

