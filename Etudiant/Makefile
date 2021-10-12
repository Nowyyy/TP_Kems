include ../Makefile.compilation 
include ../Makefile.dir         

#
# Objets
#

fich_obj_kems=kems.o \
	$(DirJeu)/carte.o \
	$(DirJeu)/paquet.o \
	$(DirJeu)/tapis.o \
	$(DirCommun)/erreur.o

fich_obj_kems_ncurses=kems_ncurses.o \
	$(DirEcran)/ecran.o $(DirEcran)/f_carte.o $(DirEcran)/f_tapis.o \
	$(DirJeu)/carte.o $(DirJeu)/paquet.o $(DirJeu)/tapis.o \
	$(DirCommun)/erreur.o

#
# Tests (executables+objets)
#

BIN = kems kems_ncurses

kems : $(fich_obj_kems) 
	$(CCLNK) $(CFLAGS)  $(fich_obj_kems) -o kems -ltda -L$(DirLibTDA) $(LIB_M) $(LIB_PTHREAD) $(MACRO_PTHREAD)

kems.o : kems.c $(DirJeu)/paquet.h $(DirJeu)/tapis.h 
	$(CCOBJ) $(CFLAGS)  kems.c -o kems.o -I./ -I$(DirCommun) -I$(DirIncTDA) -I$(DirJeu)

kems_ncurses : $(fich_obj_kems_ncurses) 
	$(CCLNK) $(CFLAGS) $(fich_obj_kems_ncurses) -o kems_ncurses $(LIB_CURSES) -ltda -L$(DirLibTDA) $(LIB_M) $(LIB_PTHREAD) $(MACRO_PTHREAD)

kems_ncurses.o : kems_ncurses.c $(DirEcran)/ecran.h 
	$(CCOBJ) $(CFLAGS)  kems_ncurses.c -o kems_ncurses.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirEcran) -I$(DirIncTDA) $(INC_CURSES)

#
# Modules externes
#

$(DirJeu)/carte.o : $(DirJeu)/carte.c $(DirJeu)/carte.h
	cd $(DirJeu) ; make -f Makefile carte.o

$(DirJeu)/paquet.o : $(DirJeu)/paquet.c $(DirJeu)/paquet.h
	cd $(DirJeu) ; make -f Makefile paquet.o

$(DirJeu)/tapis.o : $(DirJeu)/tapis.c $(DirJeu)/tapis.h
	cd $(DirJeu) ; make -f Makefile tapis.o


$(DirCommun)/erreur.o : $(DirCommun)/erreur.c $(DirCommun)/erreur.h
	cd $(DirCommun) ; make -f Makefile erreur.o


$(DirEcran)/ecran.o : $(DirEcran)/ecran.c $(DirEcran)/ecran.h
	cd $(DirEcran) ; make -f Makefile ecran.o

$(DirEcran)/f_carte.o : $(DirEcran)/f_carte.c $(DirEcran)/f_carte.h
	cd $(DirEcran) ; make -f Makefile f_carte.o

$(DirEcran)/f_tapis.o : $(DirEcran)/f_tapis.c $(DirEcran)/f_tapis.h
	cd $(DirEcran) ; make -f Makefile f_tapis.o

#
#
#

all : $(BIN) 

clean :
	- rm  $(BIN) *.o *~

tests :

