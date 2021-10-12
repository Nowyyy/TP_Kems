include ../Makefile.compilation 
include ../Makefile.dir         

#
# Objets
#

fich_obj_test_erreur=test_erreur.o erreur.o  

#
# Tests (executables+objets)
#

BIN = test_erreur 

test_erreur : $(fich_obj_test_erreur) 
	$(CCLNK) $(CFLAGS)  $(fich_obj_test_erreur) -o test_erreur 

test_erreur.o : test_erreur.c
	$(CCOBJ) $(CFLAGS)  test_erreur.c -o test_erreur.o -I./ -I$(DirIncTDA)

#
# Modules (objets)
#

OBJ = erreur.o

erreur.o : erreur.c erreur.h 
	$(CCOBJ) $(CFLAGS)  erreur.c -o erreur.o -I./ -I$(DirIncTDA)

#
# Modules externes
#

#
# Headers 
#

HDR = 

#
#
#

all : $(BIN) $(OBJ) $(HDR) 

clean :
	- rm  $(BIN) *.o *~

tests :
	./test_erreur
