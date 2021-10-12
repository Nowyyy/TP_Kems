include ../Makefile.compilation 
include ../Makefile.dir         


#
# Objets
#

fich_obj_test_ecran_carte=test_ecran_carte.o \
	ecran.o \
	f_carte.o \
	f_tapis.o \
	$(DirJeu)/carte.o $(DirJeu)/paquet.o $(DirJeu)/tapis.o \
	$(DirCommun)/erreur.o

fich_obj_test_ecran_tapis=test_ecran_tapis.o \
	ecran.o \
	f_carte.o \
	f_tapis.o \
	$(DirJeu)/carte.o $(DirJeu)/paquet.o $(DirJeu)/tapis.o \
	$(DirCommun)/erreur.o

fich_obj_test_ecran_kems=test_ecran_kems.o \
	ecran.o \
	f_carte.o \
	f_tapis.o \
	$(DirJeu)/carte.o $(DirJeu)/paquet.o $(DirJeu)/tapis.o \
	$(DirCommun)/erreur.o

#
# Tests (executables+objets)
#

BIN = test_ecran_carte test_ecran_tapis  test_ecran_kems

test_ecran_carte : $(fich_obj_test_ecran_carte) 
	$(CCLNK) $(CFLAGS) $(fich_obj_test_ecran_carte) -o test_ecran_carte $(LIB_CURSES) -ltda -L$(DirLibTDA) $(LIB_M)

test_ecran_carte.o : test_ecran_carte.c ecran.h 
	$(CCOBJ) $(CFLAGS)  test_ecran_carte.c -o test_ecran_carte.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES)

test_ecran_tapis : $(fich_obj_test_ecran_tapis) 
	$(CCLNK) $(CFLAGS) $(fich_obj_test_ecran_tapis) -o test_ecran_tapis $(LIB_CURSES) -ltda -L$(DirLibTDA) $(LIB_M)

test_ecran_tapis.o : test_ecran_tapis.c ecran.h 
	$(CCOBJ) $(CFLAGS)  test_ecran_tapis.c -o test_ecran_tapis.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES)


test_ecran_kems : $(fich_obj_test_ecran_kems) 
	$(CCLNK) $(CFLAGS) $(fich_obj_test_ecran_kems) -o test_ecran_kems $(LIB_CURSES) -ltda -L$(DirLibTDA) $(LIB_M)

test_ecran_kems.o : test_ecran_kems.c ecran.h 
	$(CCOBJ) $(CFLAGS)  test_ecran_kems.c -o test_ecran_kems.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES)


#
# Modules (objets)
#

OBJ =  ecran.o f_carte.o f_tapis.o

ecran.o : ecran.c ecran.h 
	$(CCOBJ) $(CFLAGS)  ecran.c -o ecran.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES) 

f_carte.o : f_carte.c f_carte.h 
	$(CCOBJ) $(CFLAGS)  f_carte.c -o f_carte.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES) 

f_tapis.o : f_tapis.c f_tapis.h 
	$(CCOBJ) $(CFLAGS)  f_tapis.c -o f_tapis.o -I./ -I$(DirCommun) -I$(DirJeu) -I$(DirIncTDA) $(INC_CURSES) 


#
# Modules externes
#

$(DirCommun)/erreur.o : $(DirCommun)/erreur.c $(DirCommun)/erreur.h
	cd $(DirCommun) ; make -f Makefile erreur.o 

$(DirJeu)/carte.o : $(DirJeu)/carte.c $(DirJeu)/carte.h
	cd $(DirJeu) ; make -f Makefile carte.o 

$(DirJeu)/paquet.o : $(DirJeu)/paquet.c $(DirJeu)/paquet.h
	cd $(DirJeu) ; make -f Makefile paquet.o 

$(DirJeu)/tapis.o : $(DirJeu)/tapis.c $(DirJeu)/tapis.h
	cd $(DirJeu) ; make -f Makefile tapis.o 

#
# Headers 
#

ecran.h : $(DirJeu)/tapis.h f_carte.h 
	touch ecran.h

#
#
#

all : $(BIN) $(OBJ)

clean :
	- rm  $(BIN) *.o *~

tests :
	./test_ecran_carte \
	./test_ecran_tapis \
	./test_ecran_kems 

