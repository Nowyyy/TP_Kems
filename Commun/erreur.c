#include <stdio.h>
#include <erreur.h>
#include <standard_erreurs.h>
#include <tda_erreurs.h>

extern
void erreur_afficher( const err_t code )
{
  switch( code ) 
    {   
    case ERR_DEB_MEMOIRE		:	fprintf( stderr , " Debordement memoire \n"); break ; 
    case ERR_EFF_MEMOIRE		:	fprintf( stderr , " Erreur lors de la liberation de la memoire \n"); break ; 
    case ERR_NOM_FICHIER		:	fprintf( stderr , " Erreur sur ouverture nom d'un fichier liste \n"); break ; 
    case ERR_PARAM_VIDE			:	fprintf( stderr , " Une fonction a un parametre vide \n"); break ; 
    case ERR_SAISIE			:	fprintf( stderr , " Erreur dans la saisie au clavier d'un parametre \n"); break ; 
    case FIN_SAISIE			:	fprintf( stderr , " Indicateur de fin de saisie d'un parametre \n"); break ; 

    case ERR_CARTE_UNK			:	fprintf( stderr , " Erreur sur Carte inconnue \n"); break ; 
    case ERR_CARTE_NULL			:	fprintf( stderr , " Erreur sur Carte inexistante \n"); break ; 

    case ERR_TAPIS_UNK			:	fprintf( stderr , " Erreur sur Tapis inconnu\n"); break ; 
    case ERR_TAPIS_CARTE_ID		:	fprintf( stderr , " Erreur sur sur indice de carte tapis dans le tapis\n"); break ; 

    case ERR_PAQUET_UNK			:	fprintf( stderr , " Erreur sur paquet inconnu\n"); break ; 
    case ERR_PAQUET_VIDE		:	fprintf( stderr , " Operation impossible sur un paquet vide\n"); break ; 

    default : 
      fprintf( stderr , "Code message inconnu : %d\n" , code ) ;  break ;
    }

  return  ; 
}

