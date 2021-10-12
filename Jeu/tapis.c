#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <tapis.h>


/*
 * Methodes d'acces 
 */

/* Acces a une carte du tapis */
extern
carte_t * tapis_carte_lire( const tapis_t * tapis ,
			    const carte_id_t ind_carte ) 
{
  return( tab_lire( tapis , ind_carte ) ) ;  
}

/* Acces au nombre de cartes du tapis */
extern
carte_id_t tapis_nb_cartes( const tapis_t * const tapis ) 
{
  return( tab_nb_lire( tapis ) ) ; 
}

/*
 * Tests 
 */

extern
booleen_t tapis_existe( const tapis_t * const tapis ) 
{
  return( tab_existe(tapis) ) ; 
}

extern 
booleen_t tapis_vide( const tapis_t * const tapis )
{
  return( tab_vide(tapis) ) ;
}

/*! Teste si le tapis contient un carre (4 cartes de meme valeur) */
extern 
booleen_t tapis_carre( const tapis_t * const tapis ) 
{
  carte_id_t nb_cartes = tapis_nb_cartes(tapis) ; 
  numero_t numero = carte_numero_lire( tapis_carte_lire( tapis , 0 ) ) ;
  booleen_t carre  = VRAI ; 
  carte_id_t i = 1 ;
  while( (i<nb_cartes) && (carre) )
    {
      if( numero != carte_numero_lire( tapis_carte_lire( tapis , i ) ) )
	carre=FAUX ; 
      else
	i++ ; 
    }
  return(carre) ; 
}


/*
 * Methodes d'affectations
 */

/* 
 * Le tapis est proprietaire des cartes
 * -> il les detruits 
 * -> il manipule les cartes 
 */ 

/* 
 * Pose d'une carte a un emplacement 
 */
extern
err_t tapis_carte_poser( tapis_t * const tapis , 
			 const carte_id_t ind_carte , 
			 const carte_t * const carte  ) 
{
  err_t cr = OK ;

  if( ( cr = tab_affecter( tapis , carte , ind_carte ) ) )
    return(cr) ;
 
  return(cr) ; 
}

/* 
 * Pose de la carte sommet du paquet a un emplacement du tapis 
 */
extern
err_t tapis_carte_distribuer( tapis_t * const tapis , 
			      const carte_id_t ind_carte , 
			      paquet_t * const paquet  ) 
{
  err_t cr = OK ;
  carte_t * carte = CARTE_NULL ; 

  if( ( carte = paquet_carte_retirer( paquet ) ) == CARTE_NULL )
    return(ERR_PAQUET_VIDE) ;

  if( ( cr = tab_affecter( tapis , carte , ind_carte ) ) )
    return(cr) ;

  carte_detruire( &carte ) ; 
  
  return(cr) ; 
}

/*! 
 * Remise d'une carte a un emplacement du tapis dans le paquet
 */
extern
err_t tapis_carte_retirer( tapis_t * const tapis ,
			   const carte_id_t ind_carte , 
			   paquet_t * const paquet ) 
{
  err_t cr = OK ; 
  carte_t * carte = CARTE_NULL ; 

  if( ( carte = tab_lire( tapis , ind_carte ) ) == CARTE_NULL )
    return(ERR_CARTE_NULL) ;
  
  if( ( cr = paquet_carte_ajouter( paquet , carte ) ) )
    return(cr) ;

  /* Effacement de la reference + destruction de la carte dans le tapis */
  if( ( cr = tab_affecter( tapis , CARTE_NULL , ind_carte ) ) )
    return(cr) ; 

  return(cr) ; 
}

/* 
 * Copie  d'un tapis dans un autre tapis 
 *  - la structure du tapis est recreee
 *  - les references sur les cartes sont dupliquees
 */
extern 
err_t tapis_copier( tapis_t ** tapis_cible , const tapis_t * const tapis_source ) 
{
  return( tab_copier( tapis_cible , tapis_source ) ) ;
}

extern 
err_t cb_tapis_copier( void ** tapis_cible , const void * tapis_source ) 
{
  return( tapis_copier( (tapis_t **)tapis_cible , (tapis_t *)tapis_source ) ) ;
}

/*
 * Creation d'un tapis 
 */
extern
tapis_t * tapis_creer() 
{
  tapis_t * tapis = TAPIS_NULL ; 

  /* Creation du conteneur */
  if( ( tapis = tab_creer( TAPIS_NB_CARTES,
			   cb_carte_copier , 
			   cb_carte_detruire ) ) == TAB_NULL )
    return(TAPIS_NULL) ; 

  return(tapis) ; 
}


/* 
 * Destruction d'un tapis 
 * -> destruction des cartes 
 */

extern
err_t tapis_detruire( tapis_t ** tapis ) 
{
  return( tab_detruire( tapis ) ) ;
}

extern
err_t cb_tapis_detruire( void ** tapis ) 
{
  return( tapis_detruire( (tapis_t **) tapis ) ) ;
}

/*
 * Affichage
 */

/* Debuggage */
extern
void tapis_stdout_afficher( const tapis_t * const tapis  )
{
  carte_id_t i = 0 ; 
  printf( "*--------------------\n"); 
  for( i=0 ; i< TAPIS_NB_CARTES ; i++ ) 
    {
      printf("* ") ; 
      carte_stdout_afficher( tapis_carte_lire( tapis , i ) ) ;
      printf("\n") ; 
    }
  printf( "*--------------------\n"); 
  return ; 
}

extern
void cb_tapis_stdout_afficher( const void * tapis )
{
  return(tapis_stdout_afficher( (tapis_t *) tapis )) ;
}


/*
 * Comparaison de 2 tapis
 */

extern 
int tapis_comparer( const tapis_t * const tapis_1 , const tapis_t * const tapis_2 )
{
  int cmp = 0 ; 
  carte_id_t i = 0 ;
  tapis_t * w1 = TAPIS_NULL ;
  tapis_t * w2 = TAPIS_NULL ; 

  /* Copie des 2 tapis */
  tapis_copier( &w1 , tapis_1 ) ;
  tapis_copier( &w2 , tapis_2 ) ; 

  /* Tri des copies */
  tab_trier( w1 , cb_carte_comparer ) ;
  tab_trier( w2 , cb_carte_comparer ) ;

  /* 1) Comparaison sur le nombre de cartes */
  carte_id_t nb1 = tapis_nb_cartes(w1) ; 
  carte_id_t nb2 = tapis_nb_cartes(w2) ; 
  cmp = nb1 - nb2 ;

  /* 2) Comparaison des copies des cartes */
  for( i=0 ; (!cmp) && (i<nb1) ; i++ )
    {
      cmp = carte_comparer( tapis_carte_lire(w1 , i) ,
			    tapis_carte_lire(w2 , i) ) ;
    }

  /* Destruction des copies */
  tapis_detruire(&w1) ; 
  tapis_detruire(&w2) ; 

  return(cmp) ;
}

extern 
int cb_tapis_comparer( const void * tapis_1 , const void * tapis_2 )
{
  const tapis_t * const * tapis_cb1 = tapis_1 ;
  const tapis_t * const * tapis_cb2 = tapis_2 ;

  return( tapis_comparer( (*tapis_cb1) , (*tapis_cb2) ) ) ;
}

/*
 * Choix d'une carte a enlever du tapis 
 */
typedef struct occ_s
{
  numero_t num ;
  int nb_occs ; 
} occ_t ;

static occ_t * occ_creer( const numero_t num )
{
  occ_t * occ = malloc( sizeof(occ_t) ) ;
  occ->num = num ; 
  occ->nb_occs = 1 ;  
  return(occ) ; 
}

static err_t occ_detruire( occ_t ** occ ) 
{
  free( (*occ) ) ;
  (*occ) = NULL ; 
  return(OK) ;
}

static err_t cb_occ_detruire( void ** occ )
{
  return( occ_detruire( (occ_t**)occ ) );
}

static err_t occ_affecter( occ_t ** occ_cible , const occ_t * occ_source )
{ 
  if( (*occ_cible) != NULL )
    occ_detruire( occ_cible ) ; 

  (*occ_cible) = occ_creer( occ_source->num ) ;
  (*occ_cible)->nb_occs = occ_source->nb_occs ; 

  return(OK); 
}

static err_t cb_occ_affecter( void ** occ_cible , const void * occ_source )
{
  return( occ_affecter( (occ_t**)occ_cible , occ_source ) ) ;
}
		       
static int occ_comparer_occ( const occ_t * occ1 , 
			     const occ_t * occ2 )
{
  return( occ2->nb_occs - occ1->nb_occs ) ;
}

static int cb_occ_comparer_occ( const void * occ1 , 
				const void * occ2 )
{
  const occ_t * const * occ_cb1 = occ1 ;
  const occ_t * const * occ_cb2 = occ2 ;
  return( occ_comparer_occ( *occ_cb1 , *occ_cb2 ) ) ;
} 
  
static int occ_comparer_num( const occ_t * occ1 , 
			     const occ_t * occ2 )
{
  return( occ1->num - occ2->num ) ;
} 

static int cb_occ_comparer_num( const void * occ1 , 
				const void * occ2 )
{
  const occ_t * const * occ_cb1 = occ1 ;
  const occ_t * const * occ_cb2 = occ2 ;
  return( occ_comparer_num( *occ_cb1 , *occ_cb2 ) ) ;
}

/*
 * Comptage du nombre d'occurences dans un tapis 
 */

static 
tab_t * tapis_occ_creer( const tapis_t * const tapis )
{
  int i = 0 ; 
  tab_id_t ind_occ = 0 ; 
  occ_t * occ = NULL ; 
  
  tab_t * tab_occ = tab_creer( 0 ,
			       cb_occ_affecter,
			       cb_occ_detruire ) ;

  /* Comptage du nombre d'occurences des numeros des cartes */
  for( i=0 ; i<TAPIS_NB_CARTES ; i++ )
    {
      if( ! carte_existe( tapis_carte_lire( tapis , i ) ) )
	continue ;
	
      occ = occ_creer(  carte_numero_lire( tapis_carte_lire( tapis , i )  ) ) ;

      /* Recherche du numero dans le tableau des occurences */
      if( tab_rechercher( tab_occ , occ , cb_occ_comparer_num , &ind_occ ) )
	{
	  occ_t * exist_occ = tab_lire( tab_occ , ind_occ ) ;
	  exist_occ->nb_occs++ ; 
	}
      else
	{
	  tab_fin_inserer( tab_occ , occ ) ;
	}
      
      /* Suppression occ de W */
      occ_detruire( &occ ) ; 
    }

  /* tri par ordre decroissant */
  tab_trier( tab_occ , cb_occ_comparer_occ ) ;

  /* Retour */
  return(tab_occ) ; 
} 

/*
 * Estimation d'un tapis 
*/

static 
double tapis_occ_estimer( const tab_t * const tab_occ ) 
{
  double p = TAPIS_NB_CARTES-1 ;
  double h = 0 ; 
  tab_id_t nb_occs=tab_nb_lire(tab_occ) ; 
  int i = 0 ; 
  occ_t * occ = NULL ; 

  for( i=0 ; i<nb_occs ; i++ )
    {
      occ = tab_lire( tab_occ , i ) ;
      h = h + ( occ->nb_occs * pow( 10 , p ) ) ;
      p-- ; 
    }
  return(h) ;
}

/* 
 * Choix d'une carte du tapis_1 a echanger avec une du tapis_2 
 * On maximise le tapis_1
 * \return VRAI Si un choix a ete fait FAUX sinon 
 */
extern 
err_t tapis_cartes_choisir( booleen_t * choix ,		/* Choix effectue */
			   const tapis_t * const tapis_1 , 
			    carte_id_t * ind_carte_1 ,  /* indice de la carte a echanger dans le tapis 1 */
			    const tapis_t * const tapis_2 , 
			    carte_id_t * ind_carte_2 )  /* indice de la carte a echanger dans le tapis 2 */
{
  err_t cr = OK ; 
  
  tab_t * tab_occ = TAB_NULL ;
  tapis_t * w_tapis_1 = TAPIS_NULL ; /* Copie tapis 1 */
  tapis_t * w_tapis_2 = TAPIS_NULL ; /* Copie tapis 2 */

  int i1 = 0 ; 
  int i2 = 0 ; 
  int i1_max = 0 ; 
  int i2_max = 0 ; 

  double h = 0 ; 
  double h_prec = 0 ; 
  double h_max = -1 ;

  /* Initialisation des resultats */
  (*choix) = FAUX ;
  (*ind_carte_1) = -1 ;
  (*ind_carte_2) = -1 ;

  /* Estimation du tapis avant choix */
  tab_occ = tapis_occ_creer( tapis_1 ) ; 
  h_prec = tapis_occ_estimer( tab_occ ) ;
  if( ( cr = tab_detruire( &tab_occ ) ) )
    return(cr) ; 

  /* Copies des tapis */
  if( ( cr = tapis_copier( &w_tapis_1 , tapis_1 ) ) )
    return(cr) ; 
  if( ( cr = tapis_copier( &w_tapis_2 , tapis_2 ) ) )
    return(cr) ; 

  /* Recherche meilleure combinaison */
  for( i1=0 ; i1<TAPIS_NB_CARTES ; i1++ ) 
    {
      for( i2=0 ; i2<TAPIS_NB_CARTES ; i2++ ) 
	{
	  /* Echange */
	  if( ( cr = tapis_cartes_echanger( w_tapis_1 , i1 , w_tapis_2 , i2 ) ) )
	    return(cr) ;
 
	  /* Estimation du tapis courant */
	  tab_occ = tapis_occ_creer( w_tapis_1 ) ; 
	  h = tapis_occ_estimer( tab_occ ) ;
	  if( ( cr = tab_detruire( &tab_occ ) ) ) 
	    return(cr) ; 

	  /* Recherche meilleure estimation */
	  if( h > h_max ) 
	    {
	      h_max = h ; 
	      i1_max = i1 ;
	      i2_max = i2 ; 
	    }

	  /* Undo */
	  if( ( cr = tapis_cartes_echanger( w_tapis_1 , i1 , w_tapis_2 , i2 ) ) )
	    return(cr) ; 
	}
    }

  if( h_max > h_prec ) 
    {
      (*ind_carte_1) = i1_max ; 
      (*ind_carte_2) = i2_max ; 
      (*choix) = VRAI ; 
    }

  return(cr) ; 
}

/*
 * Echange de 2 cartes entre 2 tapis 
 */
extern
err_t tapis_cartes_echanger( tapis_t * const tapis_1 , const carte_id_t ind_carte_1 ,
			     tapis_t * const tapis_2 , const carte_id_t ind_carte_2 )
{
  err_t cr = OK ; 
  carte_t * carte_1 = CARTE_NULL ; 
  carte_t * carte_2 = CARTE_NULL ; 

  if( ( cr = carte_copier( &carte_1 , tapis_carte_lire( tapis_1 , ind_carte_1 ) ) ) )
    return(cr) ; 
  if( ( cr = carte_copier( &carte_2 , tapis_carte_lire( tapis_2 , ind_carte_2 ) ) ) )
    return(cr) ; 

  if( ( cr = tapis_carte_poser( tapis_1 , ind_carte_1 , carte_2 ) ) )
    return(cr) ; 
  if( ( cr = tapis_carte_poser( tapis_2 , ind_carte_2 , carte_1 ) ) )
    return(cr) ; 

  if( ( cr = carte_detruire( &carte_1 ) ) )
    return(cr) ; 
  if( ( cr = carte_detruire( &carte_2 ) ) )
    return(cr) ; 

  return(cr) ; 
}
