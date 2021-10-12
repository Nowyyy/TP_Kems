include ../Makefile.compilation 
include ../Makefile.dir         

#
# Objets
#

fich_obj_test_carte=test_carte.o carte.o \
	$(DirCommun)/erreur.o

fich_obj_test_tapis=test_tapis.o tapis.o  \
	carte.o \
	paquet.o \
	$(DirCommun)/erreur.o

fich_obj_test_paquet=test_paquet.o paquet.o  \
	carte.o \
	$(DirCommun)/erreur.o

#
# Tests (executables+objets)
#

BIN = test_carte test_tapis test_paquet 



test_carte : $(fich_obj_test_carte) 
	$(CCLNK) $(CFLAGS)  $(fich_obj_test_carte) -o test_carte 

test_carte.o : test_carte.c carte.h 
	$(CCOBJ) $(CFLAGS)  test_carte.c -o test_carte.o -I./ -I$(DirCommun) -I$(DirIncTDA)


test_tapis : $(fich_obj_test_tapis) 
	$(CCLNK) $(CFLAGS)  $(fich_obj_test_tapis) -o test_tapis -ltda -L$(DirLibTDA) $(LIB_M)

test_tapis.o : test_tapis.c tapis.h 
	$(CCOBJ) $(CFLAGS)  test_tapis.c -o test_tapis.o -I./ -I$(DirCommun) -I$(DirIncTDA)


test_paquet : $(fich_obj_test_paquet) 
	$(CCLNK) $(CFLAGS)  $(fich_obj_test_paquet) -o test_paquet -ltda -L$(DirLibTDA) 

test_paquet.o : test_paquet.c paquet.h 
	$(CCOBJ) $(CFLAGS)  test_paquet.c -o test_paquet.o -I./ -I$(DirCommun) -I$(DirIncTDA)


#
# Modules (objets)
#

OBJ =  carte.o tapis.o paquet.o

carte.o : carte.c carte.h 
	$(CCOBJ) $(CFLAGS)  carte.c -o carte.o -I./ -I$(DirCommun) -I$(DirIncTDA)

tapis.o : tapis.c tapis.h 
	$(CCOBJ) $(CFLAGS)  tapis.c -o tapis.o -I./ -I$(DirCommun) -I$(DirIncTDA)

paquet.o : paquet.c paquet.h 
	$(CCOBJ) $(CFLAGS)  paquet.c -o paquet.o -I./ -I$(DirCommun) -I$(DirIncTDA)



#
# Modules externes
#



$(DirCommun)/erreur.o : $(DirCommun)/erreur.c $(DirCommun)/erreur.h
	cd $(DirCommun) ; make -f Makefile erreur.o

#
# Headers 
#



carte.h : $(DirCommun)/erreur.h  $(DirCommun)/commun.h 
	touch carte.h

tapis.h : carte.h 
	touch tapis.h


paquet.h : carte.h 
	touch paquet.h

#
#
#

all : $(BIN) $(OBJ)

clean :
	- rm  $(BIN) *.o *~

tests :
	./test_carte < ../Fichiers_Tests/test_carte_stdin 
	./test_tapis

