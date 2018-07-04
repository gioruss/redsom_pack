/********************************************************************************/
/*										*/
/*	This file is part of program package 'redsom_pak'.			*/
/*	Copyright (C) 2011 by Russotto Giovanni (giorussotto@hotmail.com)	*/
/*	This program is free software; you can redistribute it and/or		*/
/*	modify it for accademic and scientific purposes, including a		*/
/*	reference to its author.						*/
/*	This program CANNOT be used, copied, included or modified		*/
/*	for commercial purposes.						*/
/*	This program is distributed in the hope that it will be useful,		*/
/*	but WITHOUT ANY WARRANTY!!						*/
/*										*/
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define REDSOM_VERSION "1.3.2"
#define START_STR "+—————————————————————+\n│ redsom_pak v. %s │\n+—————————————————————+\n\n"

#define CONF_FILE "som.conf"
#define FINE_CONF_FILE "fine_som.conf"
#define ERR_LOG_FILE "err_log.txt"

#define BACKGROUND_COLOR CV_RGB(255,255,255)
#define LINE_COLOR CV_RGB(0,0,0)
#define BLACK_COLOR CV_RGB(0,0,0)
#define WHITE_COLOR CV_RGB(255,255,255)
#define RED_COLOR CV_RGB(255,0,0)
#define BLUE_COLOR CV_RGB(0,0,255)
#define GREEN_COLOR CV_RGB(0,255,0)
/*#define UMATRIX_COLOR CV_RGB(color_scale,127+color_scale/2,127-color_scale/2)*/
/*#define UMATRIX_COLOR CV_RGB(color_scale,255,127-color_scale/2)*/
/*#define UMATRIX_COLOR CV_RGB(color_scale,255,127+color_scale/2)*/
#define UMATRIX_COLOR CV_RGB(color_scale,color_scale,color_scale)
#define REDENS_NEGATIVE_COLOR CV_RGB(255,255-color_scale,255-color_scale)
#define REDENS_POSITIVE_COLOR CV_RGB(255-color_scale,255-color_scale,255)
#define REDENS_ZERO_COLOR CV_RGB(255,255,255)

#define IMAGE_TYPE ".jpg"

#define MAP_RED 1
#define MAP_UMATRIX 2

#define NEIGHBOR_2D_DIST 1
#define NEIGHBOR_2D_DIST_ERROR 0.001

#define UNIT_DIST 30
#define DENSITY_BAR_Y_DIM 30
#define DISTANCE_BAR_Y_DIM 30
#define BORDER_DIM 10

#define NOT_DISTANCE -1

#define HEXAPOINT 6
#define SQUAREPOINT 4

#define STRLENGHT 20

#define YES_RESPONSE 1
#define NO_RESPONSE -1

#define NOT_DONE_STR "NOT_DONE\0"

#define RANDF_INITIALIZED 1

#define W_FILE_KNOWN 1

#define END_OF_DATA -1
#define NEXT_DATA data_flag+1

#define BMU_PRESENT 1
#define BMU_FOUND 2

#define WEIGHTS_UPDATED 1
#define READY_TO_NEXT_DATA 2

#define DATA_READED_DIST_CALCULATED 1
#define EOD_READED 2

#define FINISH 1

#define RECT 1
#define HEXA 2

#define BATCH 1
#define INCREMENTAL 2

#define RANDOM 1
#define FROM_FILE 2

#define MAX_VALUE 1
#define MIN_VALUE 2

#define STEPS 1
#define TIME_INVERSE 2

#define GAUSSIAN 1
#define BUBBLE 2
#define CUT_GAUSSIAN 3

#define THREADS_EXECUTING -1

typedef struct {
  
  int l_rate_type;
  
  float l1r;
  float l2r;
  float l3r;
  
  int neighborhood_func;
  
} fine_config_param;

typedef struct {
  
  int row;
  int column;
  int my_id;
  int topo;
  int learning;
  int data_dim;
  int epochs;
  
  float* w_array;
  int bmu;
  float dist;
  float* input_vector;
  float* aux_vector;
    
  fine_config_param fine_conf;
  
  int read_flag;
  pthread_mutex_t read_flag_mutex;
  pthread_cond_t read_flag_cv;
  
  int w_update_flag;
  pthread_mutex_t w_update_flag_mutex;
  pthread_cond_t w_update_flag_cv;
  
  pthread_t main;

} unit_data;

typedef struct {
  
  int bmu;
  float* data_vector;
  pthread_mutex_t match_mutex;
  
} match;

typedef struct {
  
  int topo;
  int row;
  int column;
  int unit_num;
  int codebook_dim;
  
} map_param;

typedef struct {
  
  int unit_id;
  float* codebook;
  float dens_on_map1;
  float dens_on_map2;
  float re_dens;

} red_unit;

typedef struct {
  
  int unit_id;
  float* codebook;
  float neighbor_dist[6];

} dist_unit;

typedef float* data_vector;



#define SOM_USAGE "Usage: som <data_file> <output_file>\n"
#define NORMALIZER_USAGE "Usage: normalizer <data_file_1> <data_file_2> ... <data_file_n>\n"
#define DATAMAP_USAGE "Usage: datamap <data_file> <map_file> <output_file>\n"

#define DEFAULT_CONF_PARAM "Default configuration parameters:\nLRATE TYPE: TIME_INVERSE\n\nFIRST PHASE LRATE: 0.6\nSECOND PHASE LRATE: 0.3\nTHIRD PHASE LRATE: 0.05\n\nNEIGHBORHOOD FUNCTION: GAUSSIAN\n"

#define DEFAULT_LRATE_TYPE 1
#define DEFAULT_1_LRATE 0.6
#define DEFAULT_2_LRATE 0.3
#define DEFAULT_3_LRATE 0.05
#define DEFAULT_NEIGHBORHOOD_FUNC 1

#define READING_PARAM_NOTIFICATION "Reading parameters from configuration file...\n"
#define START_THREADS_NOTIFICATION "Starting threads...\n"
#define INIT_STRUCT_NOTIFICATION "Initializing threads parameters...\n"
#define ALLOC_NOTIFICATION "Allocating memory...\n"
#define ERR_OPEN_FILE "Error opening file %s\n"
#define ERR_CREATE_FILE "Error creating file %s\n"
#define ERR_CONF_FILE "Error in configuration parameters on file %s\n"
#define MAP_FILE_DIFF "Error: file %s and %s differ in the parameters\n"
#define ERR_DATA_FILE "Error on file %s\n"
#define ERR_W_FILE "Error on the file %s containing the map configuration\n"
#define ERR_ALLOC "Can't allocate memory\n"
#define ERR_THREAD "Error creating threads\n"
#define ARG_MISS "Missing argument!!\n"
#define STEP_BMU_NOTIFICATION "STEP: %d BMU: %d\n"
#define BATCH_UPDATE_NOTIFICATION "BATCH UPDATING...\n"
#define OUTPUT_WRITTEN "Output file written correctly\n"
#define NORM_FILE_WRITTEN "Normalized file %s written correctly\n"
#define W_FILE_REQUEST "Enter the name of the file containing the map configuration:\n"
#define MAP_FILE_REQUEST "Enter the name of the file containing map %d:\n"
#define ERR_INPUT_STR "Error in input string from console\n"
#define USING_DEFAULT "Using default values...\n"
#define EXITING "Exiting with code %d\n"
#define ASK_REDFILE_SAVE "Save on file the relative density maps (YES/NO)?\n"
#define ASK_IMAGE_SAVE "Save on file the maps images (YES/NO)?\n"
#define REDFILE_REQUEST "Enter the name of the file on which will be saved map %d:\n"
#define IMAGEFILE_REQUEST "Enter the name of the file (without extension) on which will be saved image of map %d:\n"
#define ERR_ALLOC_IMG "Error creating images\n"
#define ERR_DATA_DIM_DIFF "Error: file %s and %s differ in data dimension\n"
#define DATAMAP_NOTIFICATION "Locating data on map...\n"

/*
#define READING_PARAM_NOTIFICATION "Lettura dei parametri dal file di configurazione...\n"
#define START_THREADS_NOTIFICATION "Avvio threads...\n"
#define INIT_STRUCT_NOTIFICATION "Inizializzazione parametri dei threads...\n"
#define ALLOC_NOTIFICATION "Allocazione memoria...\n"
#define ERR_OPEN_FILE "Errore nell'apertura del file %s\n"
#define ERR_CREATE_FILE "Errore nella creazione del file %s\n"
#define ERR_CONF_FILE "Errore nei parametri di configurazione sul file %s\n"
#define MAP_FILE_DIFF "Errore: i file %s e %s differiscono nei parametri\n"
#define ERR_DATA_FILE "Errore nel file %s\n"
#define ERR_W_FILE "Errore nel file contenente la configurazione della mappa %s\n"
#define ERR_ALLOC "Impossibile allocare memoria\n"
#define ERR_THREAD "Errore nella creazione del thread\n"
#define ARG_MISS "Argomento mancante!!\n"
#define STEP_BMU_NOTIFICATION "STEP: %d BMU: %d\n"
#define BATCH_UPDATE_NOTIFICATION "BATCH UPDATING...\n"
#define OUTPUT_WRITTEN "Output file scritto correttamente\n"
#define NORM_FILE_WRITTEN "File %s scritto correttamente\n"
#define W_FILE_REQUEST "Inserire il nome del file contenente la configurazione della mappa:\n"
#define MAP_FILE_REQUEST "Inserire il nome del file contenente la configurazione della mappa %d:\n"
#define ERR_INPUT_STR "Errore nella stringa di input da console\n"
#define USING_DEFAULT "Verranno utilizzati i parametri di default...\n"
#define EXITING "Uscita con il codice %d\n"
#define ASK_REDFILE_SAVE "Si desidera salvare su file le densità relative delle mappe (YES/NO)?\n"
#define ASK_IMAGE_SAVE "Si desidera salvare su file le immagini delle mappe (YES/NO)?\n"
#define REDFILE_REQUEST "Inserire il nome del file su cui salvare la mappa %d:\n"
#define IMAGEFILE_REQUEST "Inserire il nome del file (senza estensione) su cui salvare l'immagine della mappa %d:\n"
#define ERR_ALLOC_IMG "Errore nella creazione dell'immagine\n"
#define ERR_DATA_DIM_DIFF "Errore: i file %s e %s differiscono nella dimensione dei dati\n"
#define DATAMAP_NOTIFICATION "Collocazione dei dati sulla mappa...\n"
*/
