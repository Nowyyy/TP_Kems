#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <paquet.h>


/*
 * Methodes d'acces 
 */

/* Acces a une carte du paquet */
extern
carte_t * paquet_carte_lire( const paquet_t * const paquet ,
			     const carte_id_t ind_carte ) 
{
  return( tab_lire( paquet , ind_carte ) ) ; 
}

/* Acces au nombre de cartes du paquet */
extern
carte_id_t paquet_nb_cartes( const paquet_t * const paquet ) 
{
  return( tab_nb_lire( paquet ) ) ; 
}

/*
 * Tests 
 */

extern
booleen_t paquet_existe( const paquet_t * const paquet ) 
{
  return( tab_existe(paquet) ) ; 
}

extern 
booleen_t paquet_vide( const paquet_t * const paquet )
{
  return( tab_vide(paquet) ) ;
}

/*
 * Methodes d'affectations
 */

/* 
 * Le paquet est proprietaire des cartes
 * -> il les detruits 
 * -> il manipule les cartes 
 */ 

/* 
 * Ajout d'une carte au fond du paquet 
 */
extern
err_t paquet_carte_ajouter( paquet_t * const paquet , 
			    const carte_t * const carte ) 
{
  return( tab_fin_inserer( paquet , carte ) ) ; 
}

/* 
 * Retrait d'une carte au sommet du paquet 
 */
extern
carte_t * paquet_carte_retirer( paquet_t * const paquet ) 
{
  return( tab_debut_extraire( paquet ) ) ; 
}

/* 
 * Copie  d'un paquet dans un autre paquet 
 *  - la structure du paquet est recreee
 *  - les references sur les cartes sont dupliquees
 */
extern 
err_t paquet_copier( paquet_t ** paquet_cible , const paquet_t * const paquet_source ) 
{
  return( tab_copier( paquet_cible , paquet_source ) ) ;
}

extern 
err_t cb_paquet_copier( void ** paquet_cible , const void * paquet_source ) 
{
  return( paquet_copier( (paquet_t **)paquet_cible , (paquet_t *)paquet_source ) ) ;
}

/*
 * Creation d'un paquet 
 */
extern
paquet_t * paquet_creer() 
{
  paquet_t * paquet = PAQUET_NULL ; 

  /* Creation du conteneur */
  if( ( paquet = tab_creer( 0,
			    cb_carte_copier , 
			    cb_carte_detruire ) ) == TAB_NULL )
    return(PAQUET_NULL) ; 

  /* Affectation du contenu (les cartes) */
  couleur_t c = CARTE_PREMIERE_COULEUR ; 
  numero_t  n = CARTE_PREMIER_NUMERO ; 
  carte_t * carte = CARTE_NULL ;
  carte_id_t i=0;
  for( c=CARTE_PREMIERE_COULEUR ; c<= CARTE_DERNIERE_COULEUR ; c++ ) 
    for( n=CARTE_PREMIER_NUMERO ; n<=CARTE_DERNIER_NUMERO ; n++ )
      {
	if( ( carte = carte_creer( c , n ) ) == CARTE_NULL )
	  return(PAQUET_NULL) ;

	if( ( paquet_carte_ajouter( paquet , carte ) ) )
	  return(PAQUET_NULL) ; 

	i++ ; 
      }

  /* Melange des cartes */
  srandom(getpid()) ; 
  int nb_cartes = paquet_nb_cartes(paquet) ;
  int nb_permuts=random()%nb_cartes ;
  int i1 ; 
  int i2 ; 
  for( i=0 ; i<nb_permuts ; i++ ) 
    {
      i1 = random()%nb_cartes ;
      i2 = random()%nb_cartes ; 
      if( i1 != i2 ) 
	{
	  paquet_carte_permuter( paquet , i1 , i2 ) ; 
	}
    }

  return(paquet) ; 
}


/* 
 * Destruction d'un paquet 
 * -> destruction des cartes 
 */

extern
err_t paquet_detruire( paquet_t ** paquet ) 
{
  return( tab_detruire( paquet ) ) ;
}

extern
err_t cb_paquet_detruire( void ** paquet ) 
{
  return( paquet_detruire( (paquet_t **) paquet ) ) ;
}

/*
 * Affichage
 */

/* Debuggage */
extern
void paquet_stdout_afficher( const paquet_t * const paquet  )
{
  carte_id_t i = 0 ; 
  carte_id_t nb_cartes = paquet_nb_cartes(paquet) ;
  printf( "*--------------------\n"); 
  for( i=0 ; i< nb_cartes ; i++ ) 
    {
      printf("* ") ; 
      carte_stdout_afficher( paquet_carte_lire( paquet , i ) ) ;
      printf("\n") ; 
    }
  printf( "*--------------------\n"); 
  return ; 
}

extern
void cb_paquet_stdout_afficher( const void * paquet )
{
  return(paquet_stdout_afficher( (paquet_t *) paquet )) ;
}


/*
 * Comparaison de 2 paquets
 */

extern 
int paquet_comparer( const paquet_t * const paquet_1 , const paquet_t * const paquet_2 )
{
  int cmp = 0 ; 
  carte_id_t i = 0 ;
  paquet_t * w1 = PAQUET_NULL ;
  paquet_t * w2 = PAQUET_NULL ; 

  /* Copie des 2 paquets */
  paquet_copier( &w1 , paquet_1 ) ;
  paquet_copier( &w2 , paquet_2 ) ; 

  /* Tri des copies */
  tab_trier( w1 , cb_carte_comparer ) ;
  tab_trier( w2 , cb_carte_comparer ) ;

  /* 1) Comparaison sur le nombre de cartes */
  carte_id_t nb1 = paquet_nb_cartes(w1) ; 
  carte_id_t nb2 = paquet_nb_cartes(w2) ; 
  cmp = nb1 - nb2 ;

  /* 2) Comparaison des copies des cartes */
  for( i=0 ; (!cmp) && (i<nb1) ; i++ )
    {
      cmp = carte_comparer( paquet_carte_lire(w1 , i) ,
			    paquet_carte_lire(w2 , i) ) ;
    }

  /* Destruction des copies */
  paquet_detruire(&w1) ; 
  paquet_detruire(&w2) ; 

  return(cmp) ;
}

extern 
int cb_paquet_comparer( const void * paquet_1 , const void * paquet_2 )
{
  const paquet_t * const * paquet_cb1 = paquet_1 ;
  const paquet_t * const * paquet_cb2 = paquet_2 ;

  return( paquet_comparer( (*paquet_cb1) , (*paquet_cb2) ) ) ;
}

/* Permutation de 2 cartes dans le paquet */

extern 
err_t paquet_carte_permuter( paquet_t * const paquet , const carte_id_t ind1 , const carte_id_t ind2 )
{
  return( tab_permuter( paquet , ind1 , ind2 ) ) ;
}
