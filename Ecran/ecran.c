#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include <ecran.h>


/*
 * Methodes d'acces 
 */
extern 
WINDOW * ecran_ecran_lire( const ecran_t * const ecran )
{
  return( ecran->ecran) ; 
} 

extern 
f_tapis_t * ecran_tapis_central_lire( const ecran_t * const ecran )
{
  return( ecran->tapis_central ) ;
}

extern 
f_tapis_t * ecran_tapis_joueur_lire( const ecran_t * const ecran , 
				     const int ind_joueur )
{ 
  return( ecran->tapis_joueurs[ind_joueur])  ;
}

extern 
int ecran_nb_joueurs_lire( const ecran_t * const ecran )
{
  return( ecran->nb_joueurs) ; 
}

extern 
ligne_id_t  ecran_y_messages_lire( const ecran_t * ecran ) 
{
  return( ecran->y_messages ) ; 
}

/*
 * Creation fenetre du tapis 
 */

extern 
ecran_t * ecran_creer( tapis_t * const tapis_central ,
		       tapis_t ** const tapis_joueurs ,
		       const tapis_id_t nb_joueurs ) 
{
  ecran_t * ecran ;
  static booleen_t first = VRAI ; 

  if( first ) 
    {
      /* Initialisation compteur de references sur les objets ecran */
      Ecran_Nb_Ref = 0 ; 

      /* Initialisation de la bibliothèque curses */
      initscr() ;	/* Init de l'ecran */
      curs_set(0) ;	/* Curseur invisible (si supporte) */

      first = FAUX ; 
    }

  /* Incrementation des references sur un objet ecran  */
  Ecran_Nb_Ref++ ; 

  if( (ecran = (ecran_t *)malloc(sizeof(ecran_t))) == NULL )
    {
      fprintf( stderr , "ecran_creer: pd d'llaocation memoire (%lu octets demandes)\n",
	       (long unsigned int)sizeof(ecran_t) ) ;
      return(NULL) ; 	       
    }

  /* Creation de l'ecran */
  int NbRangees = (nb_joueurs/ECRAN_NB_TAPIS_LIGNE) + ( nb_joueurs%ECRAN_NB_TAPIS_LIGNE ? 1 : 0 ) ;
  ecran->ecran = newwin( F_TAPIS_NB_LIGNES+((F_TAPIS_NB_LIGNES+1)*NbRangees)+5 , 
			 F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE+2 , 
			 0 , 0 ) ;
  box( ecran->ecran , 0 , 0 ) ;
  wrefresh(ecran->ecran ) ; 

  /* Creation fenetre du tapis central */
  if( ( ecran->tapis_central = f_tapis_creer( ecran->ecran , 
					       1  ,  /* Tapis central sur la 1ere ligne */ 
					       1+((F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE)-F_TAPIS_NB_COLONNES)/2 ) ) == NULL ) /* Milieu de la ligne */
       {
	 printf( "ecran_creer: Pb creation fenetre du tapis\n") ;
	 pause() ; 
	 return(NULL) ; 
       } 
  
  /* Affichage fenetre du tapis central */
  f_tapis_afficher( ecran->tapis_central , tapis_central ) ; 

  /* Fioritures */
  mvwprintw( ecran->ecran , 1 , 1 , "Tapis central" );
  mvwhline( ecran->ecran , F_TAPIS_NB_LIGNES+1 , 1 , 0 , F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE );
  mvwprintw( ecran->ecran , F_TAPIS_NB_LIGNES+2 , 1 , "Tapis des joueurs" );
  wrefresh(ecran->ecran ) ;

  /* Creations fenetres des tapis des joueurs*/
 if( ( ecran->tapis_joueurs = malloc( sizeof(f_tapis_t *) * nb_joueurs ) ) == NULL )
    {
      printf(" Erreur allocation memoire tableau des fenetres des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(f_tapis_t *) * nb_joueurs) ) ;
      exit(-1) ; 
    }
 
 int t = 0 ; 
 int t_l = 1 ;
 int t_c = 0 ;
 int l = 0 ; 
 int c = 0 ; 
 for( t=0 ; t<nb_joueurs ; t++ ) 
   {
     l = 3+F_TAPIS_NB_LIGNES*t_l+t_l ; 
     c = 1+F_TAPIS_NB_COLONNES*t_c ; 
     if( ( ecran->tapis_joueurs[t] = f_tapis_creer( ecran->ecran , l , c ) ) == NULL )
       {
	 printf( "Pb creation fenetre du tapis\n") ;
	 pause() ; 
	 return(NULL) ; 
       } 
     mvwprintw( ecran->ecran , l+F_TAPIS_NB_LIGNES , c+F_TAPIS_NB_COLONNES/2 , "Joueur %d" , t ) ;

     t_c++ ;
     if( t_c >= ECRAN_NB_TAPIS_LIGNE )
       {
	 t_c = 0 ; 
	 t_l++ ; 
       }
   }

  /* Affichages fenetres des tapis des joueurs */
  for( t=0 ; t<nb_joueurs ; t++ ) 
    {
      f_tapis_afficher( ecran->tapis_joueurs[t] , tapis_joueurs[t] ) ; 
    }

  /* Affectation nb joueurs */
  ecran->nb_joueurs = nb_joueurs ; 

  /* Creation fenetre des messages */
  ecran->y_messages = F_TAPIS_NB_LIGNES+((F_TAPIS_NB_LIGNES+1)*NbRangees)+4  ;
  ecran->messages = newwin( 3 , F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE+2 , ecran->y_messages , 0 );
  box( ecran->messages , 0 , 0 ) ;
  wrefresh( ecran->messages );
  
  /* 
   * Redirection de la sortie erreur standard pour 
   * afficher les erreurs dans la fenetre des messages 
   */
  /* - creation fichier */
  char nom_err[1024] ; 
  int fd_err ; 
  sprintf( nom_err , "%d_stderr" , getpid() ) ;
  ecran->nom_err = (char*)malloc( strlen(nom_err)+1 * sizeof(char) ) ;
  strcpy( ecran->nom_err , nom_err );
  if( ( fd_err = open( nom_err , O_RDWR | O_CREAT , 0666 ) ) == -1 )
    {
      char mess[1024] ; 
      sprintf( mess , "ecran_creer: impossible d'ouvrir %s\n" , nom_err );
      perror( mess ) ; 
      return(ECRAN_NULL) ;
    }
  
  /* - redirection */
  close(2) ;
  dup(fd_err) ;
  close(fd_err) ; 

  wrefresh( ecran->ecran );

  return(ecran) ; 
}

/*
 * Destruction fenetre de l'tapis 
 */

extern 
err_t ecran_detruire( ecran_t ** ecran ) 
{
  int j = 0 ; 
  err_t cr = OK ; 

  if( ecran_existe((*ecran)) )
    {
      /* Destruction de la fenetre du tapis central */
      if( ( cr = f_tapis_detruire(&((*ecran)->tapis_central)) ) )
	return(cr); 
      
      /* Destructions des fenetres des tapis des joueurs */
      for( j=0 ; j<(*ecran)->nb_joueurs ; j++ ) 
	if( ( cr = f_tapis_detruire(&((*ecran)->tapis_joueurs[j])) ) )
	  return(cr) ; 

      /* Destruction de la fenetre des messages */
      delwin((*ecran)->messages) ;

      /* Destruction de la fenetre de l'ecran */
      delwin((*ecran)->ecran) ;

      /* Retablissement de la sortie erreur standard a l'ecran */
      close(2) ; 
      dup(1) ; 
      unlink( (*ecran)->nom_err) ;
      free( (*ecran)->nom_err ) ;

      /* Liberation structure ecran */
      free((*ecran)) ; 

      (*ecran) = NULL ;
    }

  /* Decrementation compteur de references */
  Ecran_Nb_Ref-- ; 

  /* Fermeture bibliotheque curses */
  if( Ecran_Nb_Ref == 0 )
    {
      endwin() ; 
    }
 
  return(OK) ; 
}

/* 
 * Affichage de l'ecran 
 */
extern 
err_t ecran_afficher( const ecran_t * ecran  , 
		      tapis_t * const tapis_central ,
		      tapis_t ** tapis_joueurs ) 
{
  err_t cr = OK; 
  int t = 0 ; 

  /* Rafraichissement de toutes les fenetres */

  /* Affichage fenetre du tapis central */
  f_tapis_afficher( ecran->tapis_central , tapis_central ) ; 

  /* Affichages fenetres des tapis des joueurs */
  for( t=0 ; t<ecran->nb_joueurs ; t++ ) 
    {
      f_tapis_afficher( ecran->tapis_joueurs[t] , tapis_joueurs[t] ) ; 
    }

  /* Affichage ecran */
  wrefresh(ecran->ecran) ; 

  return(cr); 
}



/*
 * Affichage d'un message en bas de la fenetre
 */
extern
void ecran_message_afficher( const ecran_t * ecran , char * const mess )
{
  size_t lg_mess = strlen(mess) ; 

  if( lg_mess > ECRAN_LG_MESS ) 
    {
      /* Affichage du message directement */
      mvwprintw( ecran->messages , 1 , 2 , mess ) ; 
    }
  else
    {
      /* Effacement d'un eventuel message precedent */
      ecran_message_effacer( ecran ) ; 

      /* Affichage sur la fenetre */
      mvwprintw( ecran->messages , 1 , 2 , mess ) ; 
    }
  wrefresh( ecran->messages ) ; 
}

extern
void ecran_message_pause_afficher( const ecran_t * ecran , char * const mess )
{
  ecran_message_afficher( ecran , mess ) ;
  sleep(ECRAN_PAUSE_MESS) ;
}

/*
 * Effacement d'un message
 */
extern
void ecran_message_effacer( const ecran_t * ecran )
{  
  /* Affichage du message directement */
  werase( ecran->messages ) ; 
  box( ecran->messages ,  0 , 0 ) ;

  wrefresh( ecran->messages ) ; 
}

/*
 * Tests
 */

extern 
booleen_t  ecran_existe( const ecran_t * ecran ) 
{
  if( ecran == NULL )
    return(FAUX) ; 
  else
    return(VRAI) ; 
}


/* 
 * Affichage de la sortie standard d'erreurs sur l'ecran 
 * dans la fenetre des messages 
 */

extern 
void ecran_stderr_afficher( ecran_t * ecran ) 
{
  int fd_err ; 
  char buffer[ECRAN_LG_MESS+1] ;
  int nb_lus ;

  /* Ouverture fichier erreurs */
  if(  ( fd_err = open( ecran->nom_err , O_RDONLY , 0666 ) ) == -1 ) 
    {
      char mess[1024] ; 
      sprintf( mess , "Impossible d'ouvrir le fichier %s" , ecran->nom_err ) ;
      ecran_message_pause_afficher( ecran , mess ) ;
      return ; 
    }

  /* Affichage du contenu */
  while( ( nb_lus = read( fd_err , buffer , ECRAN_LG_MESS ) ) )
    {
      buffer[nb_lus-1] = '\0' ;
      ecran_message_pause_afficher( ecran , buffer ) ;
    }

  /* Fermeture fichier erreurs */
  close(fd_err) ; 
}

/*
 * Animation pour l'echange de 2 fenetre de cartes 
 */

/* 
 * Retrace les dessins de toutes es fenetres de l'ecran  
 * sauf ceux des 2 fenetres en deplacement 
 */


/* - Redessine la fenetre d'une carte  */
static
void ecran_carte_redessiner( f_carte_t * f_carte  ,
			     WINDOW * const w_carte_1 ,
			     WINDOW * const w_carte_2) 
{
 
  if( ( f_carte->fenetre != w_carte_1 ) && 
      ( f_carte->fenetre != w_carte_2 ) ) 
    {
      redrawwin(f_carte->fenetre) ; 
      wnoutrefresh(f_carte->fenetre ) ;
    }

  return ; 
}


/* - Redessine les fenetres d'un tapis  */
static 
void ecran_tapis_redessiner( f_tapis_t * f_tapis  ,
			     WINDOW * const w_carte_1 ,
			     WINDOW * const w_carte_2 ) 
{
  carte_id_t i = 0 ; 


  for( i=0 ; i<TAPIS_NB_CARTES ; i++ ) 
    {
      ecran_carte_redessiner( f_tapis_f_carte_lire( f_tapis , i )  , 
			      w_carte_1 ,
			      w_carte_2) ;
    }

  redrawwin(f_tapis->fenetre) ; 
  wnoutrefresh(f_tapis->fenetre ) ; 

  return ; 
}

/* - Redessine les fenetres d'un ecran  */
static 
void ecran_redessiner( const ecran_t * const ecran , 
		       WINDOW * const w_carte_1 ,
		       WINDOW * const w_carte_2 )
{
  int i = 0 ; 

  int t_l = 1 ;
  int t_c = 0 ;
  int l = 0 ; 
  int c = 0 ; 

  /* Fenetres tapis central */
  ecran_tapis_redessiner( ecran_tapis_central_lire(ecran) , 
			  w_carte_1 ,
			  w_carte_2 ) ; 
 
  /* Fenetres des tapis des joueurs */
  int nb_tapis = ecran_nb_joueurs_lire( ecran ) ;
  for( i=0 ; i<nb_tapis ; i++ )
    {
      ecran_tapis_redessiner( ecran_tapis_joueur_lire( ecran , i ) , 
			      w_carte_1 , 
			      w_carte_2 ) ;
    }

  /* Retracer les fioritures */
  mvwprintw( ecran->ecran , 1 , 1 , "Tapis central" );
  mvwhline( ecran->ecran , F_TAPIS_NB_LIGNES+1 , 1 , 0 , F_TAPIS_NB_COLONNES*ECRAN_NB_TAPIS_LIGNE );
  mvwprintw( ecran->ecran , F_TAPIS_NB_LIGNES+2 , 1 , "Tapis des joueurs" );
 
  for( i=0 ; i<nb_tapis ; i++ ) 
   {
     l = 3+F_TAPIS_NB_LIGNES*t_l+t_l ; 
     c = 1+F_TAPIS_NB_COLONNES*t_c ; 
     mvwprintw( ecran->ecran , l+F_TAPIS_NB_LIGNES , c+F_TAPIS_NB_COLONNES/2 , "Joueur %d" , i ) ;
     t_c++ ;
     if( t_c >= ECRAN_NB_TAPIS_LIGNE )
       {
	 t_c = 0 ; 
	 t_l++ ; 
       }
   }

  wnoutrefresh(ecran->ecran) ; 
}

extern 
void ecran_cartes_echanger( ecran_t * const ecran , 
			    f_carte_t * const f_carte_1 ,
			    f_carte_t * const f_carte_2 ) 
{
  int d = 0 ; 

  double f1_x = f_carte_x_lire( f_carte_1 ) ; 
  double f1_y = f_carte_y_lire( f_carte_1 ) ; 
  double f2_x = f_carte_x_lire( f_carte_2 ) ; 
  double f2_y = f_carte_y_lire( f_carte_2 ) ; 


  WINDOW * f1 = f_carte_fenetre_lire(f_carte_1) ; 
  WINDOW * f2 = f_carte_fenetre_lire(f_carte_2) ; 
  WINDOW * f1_old  = NULL ; 
  WINDOW * f2_old  = NULL ;
  

  /* Calcul du nb de pas */
  double N = round( ( abs(f2_x - f1_x) + abs(f2_y - f1_y) ) / 2 ) ; 
  /* Calcul des deltas */
  double delta_x = (f2_x - f1_x) / N ;
  double delta_y = (f2_y - f1_y) / N ;
  
  for( d=0 ; d<N ; d++ ) 
    {
      
      /* Effacements des anciennes positions des 2 fenetres */ 
     f1_old = dupwin(f1) ;
     wclear(f1_old) ;
     wnoutrefresh(f1_old);
     
     f2_old = dupwin(f2) ;
     wclear(f2_old) ;
     wnoutrefresh(f2_old);
      
     doupdate() ;

      /* Reaffichages des autres fenetres */
     ecran_redessiner( ecran , f1 , f2 ) ; 

     /* Calcul deplacement */
     f1_x = f1_x + delta_x ;
     f1_y = f1_y + delta_y ;
     f2_x = f2_x - delta_x ;
     f2_y = f2_y - delta_y ;

     /* Deplacement et affichage des 2 fenetres */
     mvwin(f1, round(f1_y) , round(f1_x));
     wnoutrefresh(f1);

     mvwin(f2, round(f2_y) , round(f2_x));
     wnoutrefresh(f2);

     /* Mise a jour de l'affichage de toutes les  fenetres */
     doupdate() ; 

     /* Destruction fenetres de travail */
     delwin(f1_old) ;
     delwin(f2_old) ;

     /* Pause pour voir le deplacement */
     usleep(ECRAN_PAUSE_DEPLACEMENT) ; 
    }

  /* Reposionnement des fenetres a leurs positions d'origine */
  mvwin(f1, f_carte_y_lire( f_carte_1 ) , f_carte_x_lire( f_carte_1 ) );
  mvwin(f2, f_carte_y_lire( f_carte_2 ) , f_carte_x_lire( f_carte_2 ) );

  return ;
}
				      
