VERSION = 1.3.2

CC = gcc

SOURCE_DIR = ./

EXEC_DIR = ../build/

SOM_LIB_SOURCES = $(SOURCE_DIR)som_routine.c $(SOURCE_DIR)som_io.c $(SOURCE_DIR)unit.c $(SOURCE_DIR)math_function.c $(SOURCE_DIR)flag_management.c

REDVISUAL_LIB_SOURCES = $(SOURCE_DIR)drawing.c $(SOURCE_DIR)red_routine.c $(SOURCE_DIR)visual_io.c $(SOURCE_DIR)math_function.c

NORMALIZER_LIB_SOURCES = $(SOURCE_DIR)normalizer_routine.c $(SOURCE_DIR)som_io.c $(SOURCE_DIR)math_function.c

UMATRIXVISUAL_LIB_SOURCES = $(SOURCE_DIR)umatrix_routine.c $(SOURCE_DIR)drawing.c $(SOURCE_DIR)visual_io.c $(SOURCE_DIR)math_function.c

DATAMAP_LIB_SOURCES= $(SOURCE_DIR)som_io.c $(SOURCE_DIR)datamap_routine.c $(SOURCE_DIR)math_function.c

SOM_SOURCE = $(SOURCE_DIR)som.c

REDVISUAL_SOURCE = $(SOURCE_DIR)redvisual.c

NORMALIZER_SOURCE = $(SOURCE_DIR)normalizer.c

UMATRIXVISUAL_SOURCE = $(SOURCE_DIR)umatrixvisual.c

DATAMAP_SOURCE = $(SOURCE_DIR)datamap.c

SOM_EXEC = som

NORMALIZER_EXEC = normalizer

REDVISUAL_EXEC = redvisual

UMATRIXVISUAL_EXEC = umatrixvisual

DATAMAP_EXEC = datamap

SOM_LIB_OBJ = $(SOURCE_DIR)som_routine.o $(SOURCE_DIR)som_io.o $(SOURCE_DIR)unit.o $(SOURCE_DIR)math_function.o $(SOURCE_DIR)flag_management.o

REDVISUAL_LIB_OBJ = $(SOURCE_DIR)drawing.o $(SOURCE_DIR)red_routine.o $(SOURCE_DIR)visual_io.o $(SOURCE_DIR)math_function.o

NORMALIZER_LIB_OBJ = $(SOURCE_DIR)som_io.o $(SOURCE_DIR)math_function.o $(SOURCE_DIR)normalizer_routine.o

UMATRIXVISUAL_LIB_OBJ = $(SOURCE_DIR)umatrix_routine.o $(SOURCE_DIR)drawing.o $(SOURCE_DIR)visual_io.o $(SOURCE_DIR)math_function.o

DATAMAP_LIB_OBJ= $(SOURCE_DIR)som_io.o $(SOURCE_DIR)datamap_routine.o $(SOURCE_DIR)math_function.o

SOM_CONF_FILES= som.conf fine_som.conf

SOM_LDFLAGS = -lpthread -lm

NORMALIZER_LDFLAGS = -lm

DATAMAP_LDFLAGS = -lm

REDVISUAL_LDFLAGS = -lm -lcv -lhighgui -lcvaux -lcxcore

UMATRIXVISUAL_LDFLAGS = -lm -lcv -lhighgui -lcvaux -lcxcore

OPTION = -Wall -march=native -O2 -pedantic

all:		som normalizer redvisual umatrixvisual datamap
		
normalizer:	put_info normalizer_lib
		mkdir -p $(EXEC_DIR)
		@echo making normalizer...
		$(CC) $(OPTION) $(NORMALIZER_LDFLAGS) -o $(NORMALIZER_EXEC) $(NORMALIZER_SOURCE) $(NORMALIZER_LIB_OBJ)
		cp --force --update --target-directory=$(EXEC_DIR) $(NORMALIZER_EXEC)
		@echo OK!!!!!!!!!

som: 		put_info som_lib
		mkdir -p $(EXEC_DIR)
		@echo making som...
		$(CC) $(OPTION) $(SOM_LDFLAGS) -o $(SOM_EXEC) $(SOM_SOURCE) $(SOM_LIB_OBJ)
		cp --force --update --target-directory=$(EXEC_DIR) $(SOM_EXEC)
		cp --no-clobber --target-directory=$(EXEC_DIR) $(SOM_CONF_FILES)
		@echo OK!!!!!!!!!

redvisual:	put_info redvisual_lib
		mkdir -p $(EXEC_DIR)
		@echo making redvisual...
		$(CC) $(OPTION) $(REDVISUAL_LDFLAGS) -o $(REDVISUAL_EXEC) $(REDVISUAL_SOURCE) $(REDVISUAL_LIB_OBJ)
		cp --force --update --target-directory=$(EXEC_DIR) $(REDVISUAL_EXEC)
		@echo OK!!!!!!!!!

umatrixvisual:	put_info umatrixvisual_lib
		mkdir -p $(EXEC_DIR)
		@echo making umatrixvisual
		$(CC) $(OPTION) $(UMATRIXVISUAL_LDFLAGS) -o $(UMATRIXVISUAL_EXEC) $(UMATRIXVISUAL_SOURCE) $(UMATRIXVISUAL_LIB_OBJ)
		cp --force --update --target-directory=$(EXEC_DIR) $(UMATRIXVISUAL_EXEC)
		@echo OK!!!!!!!!!

datamap:	put_info datamap_lib
		mkdir -p $(EXEC_DIR)
		@echo making datamap
		$(CC) $(OPTION) $(DATAMAP_LDFLAGS) -o $(DATAMAP_EXEC) $(DATAMAP_SOURCE) $(DATAMAP_LIB_OBJ)
		cp --force --update --target-directory=$(EXEC_DIR) $(DATAMAP_EXEC)
		@echo OK!!!!!!!!!

umatrixvisual_lib: put_info
		@echo making umatrixvisual libraries...
		$(CC) $(OPTION) -c $(UMATRIXVISUAL_LIB_SOURCES)
		@echo OK!!!!!!!!!

normalizer_lib: put_info
		@echo making normalizer libraries...
		$(CC) $(OPTION) -c $(NORMALIZER_LIB_SOURCES)
		@echo OK!!!!!!!!!

som_lib: 	put_info
		@echo making som libraries...
		$(CC) $(OPTION) -c $(SOM_LIB_SOURCES)
		@echo OK!!!!!!!!!

redvisual_lib: 	put_info
		@echo making revisual libraries...
		$(CC) $(OPTION) -c $(REDVISUAL_LIB_SOURCES)
		@echo OK!!!!!!!!!

datamap_lib: put_info
		@echo making datamap libraries...
		$(CC) $(OPTION) -c $(DATAMAP_LIB_SOURCES)
		@echo OK!!!!!!!!!

clean: 		put_info
		@echo making clean...
		rm --force $(REDVISUAL_LIB_OBJ) $(SOM_LIB_OBJ) $(NORMALIZER_LIB_OBJ) $(UMATRIXVISUAL_LIB_OBJ) $(DATAMAP_LIB_OBJ)
		rm --force $(EXEC_DIR)$(SOM_EXEC)
		rm --force $(EXEC_DIR)$(REDVISUAL_EXEC)
		rm --force $(EXEC_DIR)$(NORMALIZER_EXEC)
		rm --force $(EXEC_DIR)$(UMATRIXVISUAL_EXEC)
		rm --force $(EXEC_DIR)$(DATAMAP_EXEC)
		rm --force $(SOM_EXEC)
		rm --force $(REDVISUAL_EXEC)
		rm --force $(NORMALIZER_EXEC)
		rm --force $(UMATRIXVISUAL_EXEC)
		rm --force $(DATAMAP_EXEC)
		rmdir --ignore-fail-on-non-empty $(EXEC_DIR)

lib_clean: 	put_info
		@echo making lib_clean...
		rm --force $(REDVISUAL_LIB_OBJ) $(SOM_LIB_OBJ) $(NORMALIZER_LIB_OBJ) $(UMATRIXVISUAL_LIB_OBJ) $(DATAMAP_LIB_OBJ)

put_info:	
		@echo +—————————————————————+
		@echo │ redsom_pak v. $(VERSION) │
		@echo +—————————————————————+