#include <stdio.h>
#include <stdlib.h>

#include <commun.h>
#include <erreur.h>
#include <carte.h>


/*
 * Methodes d'acces 
 */

/* Acces a la couleur de la carte */
extern
couleur_t carte_couleur_lire( const carte_t * carte ) 
{
  return(carte->couleur) ; 
}

extern
char * carte_couleur_string( const carte_t * carte  )
{
  switch( carte->couleur )
    {
    case TREFLE  : return("TREFLE") ; 
    case PIQUE   : return("PIQUE") ;  
    case CARREAU : return("CARREAU") ;
    case COEUR   : return("COEUR") ; 
    default :      return("???") ; 
    }
}


/* Acces au numero de la carte */
extern
numero_t carte_numero_lire( const carte_t * carte ) 
{
  return(carte->numero) ; 
}

extern
char * carte_numero_string( const carte_t * carte )
{
  switch( carte->numero )
    {
    case SEPT  : return( "  7  " ) ;   
    case HUIT  : return( "  8  " ) ;  
    case NEUF  : return( "  9  " ) ;  
    case DIX   : return( "  10 " ) ;  
    case VALET : return( "Valet" ) ;  
    case DAME  : return( "Dame " ) ;  
    case ROI   : return( " Roi " ) ;  
    case AS    : return( "  As " ) ;  
    default    : return( "?????" ) ;  
    }
}

/*
 * Tests 
 */

extern
booleen_t carte_existe( const carte_t * carte ) 
{
  if( carte == NULL )
    return(FAUX) ; 
  else
    return(VRAI);
}

/*
 * Methodes d'affectations
 */

/* Affectation de la couleur de la carte */
extern
err_t carte_couleur_ecrire( carte_t * carte , const couleur_t couleur ) 
{
  if( ! carte_existe(carte) )
    return(ERR_CARTE_UNK) ;
  carte->couleur = couleur ;
  return(OK) ; 
}


/* Affectation du numero  de la carte */
extern
err_t carte_numero_ecrire( carte_t * carte , const numero_t numero ) 
{
   if( ! carte_existe(carte) )
    return(ERR_CARTE_UNK) ;
  carte->numero = numero ;
  return(OK) ; 
}

/* Affectation d'une carte par une autre carte */
extern 
err_t carte_copier( carte_t ** c1 , const carte_t * c2 ) 
{
  err_t cr = OK ; 

  if( carte_existe((*c1)) ) 
    {
      if((cr=carte_detruire(c1) ) )
	return(cr); 
    }

  if( !carte_existe( c2 ) )
    return(OK) ; 

  if( ( (*c1) = carte_creer( carte_couleur_lire(c2) ,
			     carte_numero_lire(c2) ) ) == CARTE_NULL )
    {
      return(ERR_DEB_MEMOIRE);
    }
  
  return(OK) ;
}

extern 
err_t cb_carte_copier( void ** c1 , const void * c2 ) 
{
  return( carte_copier( (carte_t **)c1 , (carte_t *)c2 ) ) ;
}


extern 
err_t carte_referencer( carte_t ** c1 , carte_t * c2 ) 
{
  err_t cr = OK ; 

  if( carte_existe((*c1)) ) 
    {
      if((cr=carte_detruire(c1) ) )
	return(cr); 
    }

  (*c1) = c2 ;

  return(OK) ;
}

extern 
err_t cb_carte_referencer( void * * c1 , const void * c2 ) 
{
  return( carte_referencer( (carte_t **)c1 , (carte_t *)c2 ) ) ;
}

/*
 * Creation d'une carte 
 */
extern
carte_t * carte_creer( const couleur_t couleur , 
		       const numero_t numero ) 
{
  carte_t * carte ;
 
  if( (carte=(carte_t *)malloc(sizeof(carte_t)) ) == NULL )
    {
      fprintf( stderr , "carte_creer: debordement memoire %lu octets demandes\n" , 
	       (long unsigned int)sizeof(carte_t) );
      return(NULL);
    }

  carte->couleur = couleur ;
  carte->numero = numero ; 

  return(carte) ; 
}


/* 
 * Destruction d'une carte 
 */

extern
err_t carte_detruire( carte_t ** carte ) 
{
  err_t cr = OK ; 

  free( (*carte) ) ;
  (*carte) = CARTE_NULL ; 

  return(cr) ; 
}

extern
err_t cb_carte_detruire( void * * carte ) 
{
  return( carte_detruire( (carte_t **) carte ) ) ;
}

/*
 * Effacement des references 
 */

extern
err_t carte_reference_effacer( carte_t ** carte ) 
{
  err_t cr = OK ; 

  (*carte) = NULL ; 

  return(cr) ; 
}

extern
err_t cb_carte_reference_effacer( void * * carte ) 
{
  return( carte_reference_effacer( (carte_t **) carte ) ) ;
} 

/*
 * Affichage
 */

/* Debuggage */
static
void carte_couleur_afficher( const carte_t * carte  )
{
  switch( carte->couleur )
    {
    case TREFLE  : printf("TREFLE") ; break ; 
    case PIQUE   : printf("PIQUE") ; break ; 
    case CARREAU : printf("CARREAU") ; break ; 
    case COEUR   : printf("COEUR") ; break ; 
    default :      printf("???") ; break ; 
    }

  return ; 
}

static 
void carte_numero_afficher( const carte_t * carte )
{
  switch( carte->numero )
    {
    case SEPT  : printf( "7" ) ; break ;  
    case HUIT  : printf( "8" ) ; break ; 
    case NEUF  : printf( "9" ) ; break ; 
    case DIX   : printf( "10" ) ; break ; 
    case VALET : printf( "Valet" ) ; break ; 
    case DAME  : printf( "Dame" ) ; break ; 
    case ROI   : printf( "Roi" ) ; break ; 
    case AS    : printf( "As" ) ; break ; 
    default    : printf( "??" ) ; break ; 
    }
  return ; 
}

extern
void carte_stdout_afficher( const carte_t * carte )
{
  if( ! carte_existe( carte ) )
    {
      printf(" --- " ) ;
      return ; 
    }

  carte_numero_afficher(carte) ; 
  printf(" de ") ; 
  carte_couleur_afficher(carte) ; 

  return ;
}

extern
void cb_carte_stdout_afficher( const void * carte )
{
  return(carte_stdout_afficher( (carte_t *) carte )) ;
}


/*
 * Comparaison de 2 cartes
 */

extern 
int carte_comparer( const carte_t * c1 , const carte_t * c2 )
{
  int cmp = 0 ; 

  /* 1) Comparaison sur le numero */
  if( (cmp = carte_numero_lire(c1) - carte_numero_lire(c2) ) )
    return(cmp) ;

  return(cmp) ;
}

extern 
int cb_carte_comparer( const void * c1 , const void * c2 )
{
  const carte_t * const * carte_cb1 = c1 ;
  const carte_t * const * carte_cb2 = c2 ;
  
  return(carte_comparer( (*carte_cb1) , (*carte_cb2) )) ;
}


