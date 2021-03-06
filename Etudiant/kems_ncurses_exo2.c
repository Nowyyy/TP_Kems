#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <ncurses.h>

#include <commun.h>
#include <paquet.h>
#include <ecran.h>
#include <pthread.h>



void arret( int sig )
{
  /* printf( "Arret utilisateur\n");*/
} 

// Liste de toutes les variables communes entre les threads
paquet_t * paquet = PAQUET_NULL ; 

tapis_t * tapis_central = TAPIS_NULL ; 
tapis_t ** tapis = NULL ; /* tableau des tapis */
carte_id_t c = -1 ; 
  
err_t cr = OK ; 
	
booleen_t fini = FAUX;
int NbJoueurs = 0;
int joueurJouant = 0;
int tourSansAction = 0;


char message[256];
ecran_t * ecran = ECRAN_NULL;


// Liste de tous les Mutex

pthread_mutex_t mutex_Tapis = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Fin = PTHREAD_MUTEX_INITIALIZER;

	
pthread_mutex_t mutex_Carte1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte4 = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_TapisGlobal = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_CompteurJoueur = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_Ecran = PTHREAD_MUTEX_INITIALIZER;



//Fonction permettant de faire fonctionner les joueurs
void Joueur(void* arg){

	carte_id_t ind_carte = -1 ;
  carte_id_t ind_carte_central = -1 ;
	booleen_t echange = FAUX;

//On récupère les données passées en paramètres en les convertissant.
  int num_joueur =*((int*)arg);


//On créé un décallage dans le lancement des threads
	while(!fini)
	{

		/* Test arret */
		if( tapis_carre( tapis[num_joueur] ) )
		{
				
			pthread_mutex_lock(&mutex_Fin);
			fini = VRAI ;
			pthread_mutex_unlock(&mutex_Fin);

			sprintf(message, "Le joueur %2d à remporté la partie", num_joueur);
			pthread_mutex_lock(&mutex_Ecran);
			ecran_message_pause_afficher(ecran, message);
			pause();
			pthread_mutex_unlock(&mutex_Ecran);
			pthread_exit(0) ; 


		}
		
		//On compte le nombre de joueurs en train de jouer
		//Si un seul joueur joue, on bloque le tapis pour lui
		pthread_mutex_lock(&mutex_CompteurJoueur);
		joueurJouant++;
		if(joueurJouant == 1)
		{
			pthread_mutex_lock(&mutex_Tapis);
		}
		
		pthread_mutex_unlock(&mutex_CompteurJoueur);

		//On choisit la carte a remplacer
		if( ( cr = tapis_cartes_choisir( &echange , tapis[num_joueur] , &ind_carte , tapis_central , &ind_carte_central) ) )
		{
			sprintf(message, "Pb dans choix des cartes, code retour = %d\n", cr ) ;
			pthread_mutex_lock(&mutex_Ecran);
			ecran_message_pause_afficher(ecran, message);
			pthread_mutex_unlock(&mutex_Ecran);
			erreur_afficher(cr) ; 
			pthread_exit(0) ; 
		}


		//Echange de carte si mouvement valable
		if( echange ) 
		{

			pthread_mutex_lock(&mutex_Tapis);
			if( ( cr = tapis_cartes_echanger( tapis[num_joueur] , ind_carte , tapis_central , ind_carte_central ) ) )
			{
				sprintf( message, "Pb d'echange de cartes pour le joueur %d\n" , num_joueur ); 
				pthread_mutex_lock(&mutex_Ecran);
      			ecran_message_pause_afficher(ecran, message);
				pthread_mutex_unlock(&mutex_Ecran);
				erreur_afficher(cr) ; 
				pthread_exit(0) ; 
			}
			
			//On fait jouer le joueur et on l'affiche a l'écran
			pthread_mutex_lock(&mutex_Ecran);
			sprintf(message, "Joueur %i : Echange carte %ld avec carte %ld du tapis central", num_joueur, ind_carte+1, ind_carte_central+1);
      		ecran_message_pause_afficher(ecran, message);
	        ecran_cartes_echanger( ecran , f_tapis_f_carte_lire( ecran_tapis_central_lire( ecran ) , ind_carte_central ) , f_tapis_f_carte_lire( ecran_tapis_joueur_lire( ecran , num_joueur ) , ind_carte ) ) ;
			ecran_afficher(ecran, tapis_central, tapis);
			ecran_message_effacer(ecran);
			pthread_mutex_unlock(&mutex_Ecran);

		}
		pthread_mutex_unlock(&mutex_Tapis);

		
		//On compte le nombre de joueurs en train de jouer
		//Si il n'y a plus de joueur qui joue alors on delocke le tapis
		pthread_mutex_lock(&mutex_CompteurJoueur);
		joueurJouant--;
		if(joueurJouant == 0)
		{
			pthread_mutex_unlock(&mutex_Tapis);
		}

	//Si un certain nombre de tours de joueurs n'ont pas eu d'action, alors on redistribue le plateau
		if(echange){
			tourSansAction = 0;
		}
		else{
			tourSansAction++;
		}
		
		pthread_mutex_unlock(&mutex_CompteurJoueur);
	}
}

//Fonction permettant de faire fonctionner le tapis central
void Tapis()
{

	while(!fini)
	{		
	  	/* 
	   	 * Pas un seul echange des joueur 
	   	 * --> redistribution du tapis central 
	   	 */
	sleep(1);
	//Si un certain nombre de tours de joueurs n'ont pas eu d'action, alors on redistribue le plateau
	if(tourSansAction>6)
		pthread_mutex_lock(&mutex_Ecran);
		pthread_mutex_lock(&mutex_Tapis);

		ecran_message_pause_afficher(ecran, "Pas d'échange = Redistribution du tapis central");

			for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
			{
				if( ( cr = tapis_carte_retirer( tapis_central , c , paquet ) ) )
					{
					printf("Problème de retrait de carte sur le tapis central");
					erreur_afficher(cr) ; 
					exit(-1) ; 
					}
		
				if( ( cr = tapis_carte_distribuer( tapis_central , c , paquet ) ) )
				{
					printf("Problème de distribution de carte sur le tapis central");
					erreur_afficher(cr) ; 
					exit(-1) ; 
				}
			}
	}

    ecran_afficher(ecran, tapis_central, tapis);
	ecran_message_effacer(ecran);

		pthread_mutex_unlock(&mutex_Tapis);
    pthread_mutex_unlock(&mutex_Ecran);
      
	}
}


int
main( int argc , char * argv[] ) 
{

	int i;


	printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

	if( argc != 2 ) 
   	{
    	printf( " Programme de test des joueurs de Kems\n" );
     	printf( " usage : %s <Nb joueurs>\n" , argv[0] );
     	exit(0); 
   	}

  	NbJoueurs  = atoi( argv[1] ) ;

  	srandom(getpid());

  	printf("Creation du paquet de cartes\n") ;
  	if( ( paquet = paquet_creer() ) == PAQUET_NULL )
    {
    	printf("Erreur sur creation du paquet\n" ) ;
      	exit(-1) ; 
    }

  	printf("Creation du tapis central\n")  ;
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

  	printf( "Tapis Central\n" ) ;
  	tapis_stdout_afficher( tapis_central ) ;
  	printf("\n");

  	printf("Creation des %d tapis des joueurs\n" , NbJoueurs ) ;
  	if( ( tapis = malloc( sizeof(tapis_t *) * NbJoueurs ) ) == NULL )
    {
      	printf(" Erreur allocation memoire tableau des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(tapis_t *) * NbJoueurs) ) ;
      	exit(-1) ; 
    }
	
  	for( i=0 ; i<NbJoueurs ; i++ ) 
    {
      	if( ( tapis[i] = tapis_creer() ) == TAPIS_NULL )
		{
	  		printf("Erreur sur creation du tapis %d\n" , i ) ;
	  		exit(-1) ;
		}

      	for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
		{
	  		if( ( cr = tapis_carte_distribuer( tapis[i]  , c , paquet ) ) )
	    	{
	      		if( cr == ERR_PAQUET_VIDE ) printf("Pas assez de cartes pour tous les joueurs\n"); 
	      			erreur_afficher(cr) ; 
	      			exit(-1) ; 
	    	}
		}

      	printf( "Tapis joueur %d\n" , i+1 ) ;
      	tapis_stdout_afficher( tapis[i] ) ;
      	printf("\n");
    }	

  	//Creation de l'écran
	ecran = ecran_creer(tapis_central, tapis, NbJoueurs);
	ecran_message_afficher(ecran, "Début de partie");
  


	//On créé les threads du tapis général et des joueurs
	int id[]={0,1,2,3,4,5,6};
	pthread_t joueurs[NbJoueurs+1];

	pthread_create(&joueurs[NbJoueurs], NULL, (void *)Tapis, (void *)NULL);

	for(i = 0; i<NbJoueurs; i++){ 
		pthread_create(&joueurs[i], NULL, (void *)Joueur, (void*)&id[i]);
	}

	//On attend la fin de l'exécution de tous les threads
	for(i = 0; i<NbJoueurs+1; i++){ 
		pthread_join(joueurs[i], NULL);
	}
	



	//Destruction de l'écran
	ecran_detruire(&ecran);

 	printf("\nDestruction des tapis..." ) ; fflush(stdout) ; 
 	for (i=0 ; i<NbJoueurs ; i++ ) 
   	{
     	if( ( cr = tapis_detruire( &tapis[i] ) ) )
       	{
	 		printf(" Erreur sur destruction du tapis du joueur %d\n"  , i ) ;
	 		erreur_afficher(cr) ; 
	 		exit(-1) ; 
       	}
   	}
	free(tapis);
	printf("OK\n") ; 
 

 	printf("\nDestruction du paquet..." ) ; fflush(stdout) ; 
 	paquet_detruire( &paquet ) ;
 	printf("OK\n") ; 


 	printf("\nDestruction des mutex..." ) ; fflush(stdout) ; 
	pthread_mutex_destroy(&mutex_Carte1);
	pthread_mutex_destroy(&mutex_Carte2);
	pthread_mutex_destroy(&mutex_Carte3);
	pthread_mutex_destroy(&mutex_Carte4);
	pthread_mutex_destroy(&mutex_Tapis);
	pthread_mutex_destroy(&mutex_Fin);
	pthread_mutex_destroy(&mutex_TapisGlobal);
	pthread_mutex_destroy(&mutex_CompteurJoueur);
	pthread_mutex_destroy(&mutex_Ecran);

	printf("OK\n") ;

 
 	printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );
 
 	return(0) ;
}

