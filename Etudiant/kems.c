#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <paquet.h>
#include <tapis.h>
#include <pthread.h>


// Liste de toutes les variables communes entre les threads
paquet_t * paquet = PAQUET_NULL ; 

tapis_t * tapis_central = TAPIS_NULL ; 
tapis_t ** tapis = NULL ; /* tableau des tapis */
carte_id_t c = -1 ; 
  
err_t cr = OK ; 
	
booleen_t fini = FAUX;
int NbJoueurs = 0;
int tourSansAction = 0;


// Liste de tous les Mutex

pthread_mutex_t mutex_Tapis = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Fin = PTHREAD_MUTEX_INITIALIZER;

	
pthread_mutex_t mutex_Carte1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Carte4 = PTHREAD_MUTEX_INITIALIZER;



//Fonction permettant de faire fonctionner les joueurs
void Joueur(void* arg){

	carte_id_t ind_carte = -1 ;
    carte_id_t ind_carte_central = -1 ;
	booleen_t echange = FAUX;
    fini = FAUX;

	int num_joueur =*((int*)arg);


//On créé un décallage dans le lancement des threads
	pthread_mutex_lock(&mutex_Fin);
	while(!fini)
	{
		pthread_mutex_unlock(&mutex_Fin);

		/* Affichage Joueur */
		printf( "Tapis joueur %d\n" , num_joueur+1 ) ;
		tapis_stdout_afficher( tapis[num_joueur] ) ;
		printf( "\n" ); 

		/* Test arret */
		if( tapis_carre( tapis[num_joueur] ) )
		{
				
			pthread_mutex_lock(&mutex_Fin);
			fini = VRAI ;
			pthread_mutex_unlock(&mutex_Fin);

			printf( "*----------------------*\n") ; 
			printf( "* Le joueur %2d a gagne *\n" , num_joueur +1 ) ;
			printf( "*----------------------*\n") ; 
			exit(0);  /* Sort de la boucle des joueurs */
		}
		
		
		pthread_mutex_lock(&mutex_Tapis);

		

		//On choisit la carte a remplacer

		if( ( cr = tapis_cartes_choisir( &echange , tapis[num_joueur] , &ind_carte , tapis_central , &ind_carte_central) ) )
		{
			printf( "Pb dans choix des cartes, code retour = %d\n", cr ) ;
			erreur_afficher(cr) ; 
			pthread_exit(0) ; 
		}


		//Echange de carte si mouvement valable
		if( echange ) 
		{

			int indice_carte_centrale = ind_carte_central;

			//On lock l'emplacement de la carte a echanger sur le tapis central
			switch (indice_carte_centrale){
			case 0: pthread_mutex_lock(&mutex_Carte1);
				break;
			case 1: pthread_mutex_lock(&mutex_Carte2);
				break;
			case 2: pthread_mutex_lock(&mutex_Carte3);
				break;		
			case 3: pthread_mutex_lock(&mutex_Carte4);
				break;
			}

			if( ( cr = tapis_cartes_echanger( tapis[num_joueur] , ind_carte , tapis_central , ind_carte_central ) ) )
			{
				printf( "Pb d'echange de cartes entre la carte %ld du tapis du joueur %d\n" , ind_carte , num_joueur ); 
				carte_stdout_afficher( tapis_carte_lire( tapis[num_joueur] , ind_carte ) ) ; 
				printf( "\n et la carte %ld du tapis central\n" , ind_carte_central ); 
				carte_stdout_afficher( tapis_carte_lire( tapis_central , ind_carte_central ) ) ; 
				erreur_afficher(cr) ; 
				pthread_exit(0) ; 
			}

			indice_carte_centrale = ind_carte_central;


			//On délocke l'emplacement de la carte du tapis central
			switch (indice_carte_centrale){
			case 0: pthread_mutex_unlock(&mutex_Carte1);
				break;
			case 1: pthread_mutex_unlock(&mutex_Carte2);
				break;
			case 2: pthread_mutex_unlock(&mutex_Carte3);
				break;		
			case 3: pthread_mutex_unlock(&mutex_Carte4);
				break;
			}

		}
		
		
		//On met a jour le nombre de tours ou des joueurs n'ont pas jouer

		if(echange){
			tourSansAction = 0;
		}
		else{
			tourSansAction++;
		}
		
		pthread_mutex_unlock(&mutex_Tapis);

	}
}

//Fonction permettant de faire fonctionner le tapis central
void Tapis()
{
	pthread_mutex_lock(&mutex_Fin);
	while(!fini)
	{
		pthread_mutex_unlock(&mutex_Fin);

		sleep(1);
		/* Affichage Central */
     	printf( "Tapis central \n" ) ;
      	tapis_stdout_afficher( tapis_central ) ;
      	printf( "\n" ); 


		
	//Si un certain nombre de tours de joueurs n'ont pas eu d'action, alors on redistribue le plateau

		if(tourSansAction>6){
			pthread_mutex_lock(&mutex_Tapis);

			printf( "Redistribution tapis central\n") ; 

			for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
			{
				if( ( cr = tapis_carte_retirer( tapis_central , c , paquet ) ) )
				{
					printf( "Pb dans retrait d'une carte du tapis central\n" ); 
					erreur_afficher(cr) ; 
					exit(-1) ; 
				}
		
				if( ( cr = tapis_carte_distribuer( tapis_central , c , paquet ) ) )
				{
					printf( "Pb dans distribution d'une carte pour le tapis central\n" ); 
					erreur_afficher(cr) ; 
					exit(-1) ; 
				}
			}

			pthread_mutex_unlock(&mutex_Tapis);
		}
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


	//On créé les threads du tapis général et des joueurs
	pthread_t joueurs[NbJoueurs+1];

	int id[]={0,1,2,3,4,5,6};


	pthread_create(&joueurs[NbJoueurs], NULL, (void *)Tapis, (void *)NULL);

	for(i = 0; i<NbJoueurs; i++){ 
		pthread_create(&joueurs[i], NULL, (void *)Joueur, (void*)&id[i]);
	}

	//On attend la fin de l'exécution de tous les threads
	for(i = 0; i<NbJoueurs+1; i++){ 
		pthread_join(joueurs[i], NULL);
	}
	




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

	printf("OK\n") ;

 
 	printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );
 
 	return(0) ;
}

