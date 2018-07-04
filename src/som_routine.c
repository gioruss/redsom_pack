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

#include "common.h"
#include "extern.h"
#include "som_io.h"

#include <pthread.h>

void exit_procedure(unit_data* u_dat_array, match* match_l, pthread_t* pthread_array, float* in_vector, int unit, int data_number, int return_code){
  
  register int i;
  
  if(return_code==THREADS_EXECUTING){
    for(i=0;i<unit;i++) pthread_cancel(pthread_array[i]);
    sleep(2);
    return_code=0;
  }
  
  if(pthread_array!=NULL)
    free(pthread_array);
  
  if(u_dat_array!=NULL){
    for(i=0;i<unit;i++) if(u_dat_array[i].w_array!=NULL) free(u_dat_array[i].w_array);
    for(i=0;i<unit;i++) if(u_dat_array[i].input_vector!=NULL) free(u_dat_array[i].input_vector);
    for(i=0;i<unit;i++) if(u_dat_array[i].aux_vector!=NULL) free(u_dat_array[i].aux_vector);
    free(u_dat_array);  
  }
  
  if(in_vector!=NULL) free(in_vector);
  
  if(match_l!=NULL){
    for(i=0;i<data_number;i++) if(match_l[i].data_vector!=NULL) free(match_l[i].data_vector);
    free(match_l);  
  }
  
  fprintf(stdout, EXITING, return_code);
  
  exit(return_code);
}

int u_dat_array_alloc(unit_data** u_dat_array, int unit){
  
  *u_dat_array=malloc(sizeof(unit_data)*unit);
  if(*u_dat_array==NULL){
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  else{
    memset(*u_dat_array, 0, sizeof(unit_data)*unit);
    return 1;
  }
  
}

int pthread_array_alloc(pthread_t** pthread_array, int unit){
  
  *pthread_array=malloc(sizeof(pthread_t)*unit);
  if(*pthread_array==NULL){
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  else{
    memset(*pthread_array, 0, sizeof(pthread_t)*unit);
    return 1;
  }
  
}

int in_vector_alloc(float** in_vector, int data_dim){
  
  *in_vector=malloc(sizeof(float)*data_dim);
  if(*in_vector==NULL){
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  else{
    memset(*in_vector, 0, sizeof(float)*data_dim);
    return 1;
  }
  
}

int w_alloc(unit_data* u_dat_ptr){
  
  u_dat_ptr->w_array=malloc(sizeof(float)*u_dat_ptr->data_dim);
  if(u_dat_ptr->w_array==NULL){ 
    fprintf(stderr, ERR_ALLOC);
    return 0;    
  } else {
    memset(u_dat_ptr->w_array, 0, sizeof(float)*u_dat_ptr->data_dim);
    return 1;
  }
  
}

int in_alloc(unit_data* u_dat_ptr){
  
  u_dat_ptr->input_vector=malloc(sizeof(float)*u_dat_ptr->data_dim);
  if(u_dat_ptr->input_vector==NULL){ 
    fprintf(stderr, ERR_ALLOC);
    return 0;    
  } else {
    memset(u_dat_ptr->input_vector, 0, sizeof(float)*u_dat_ptr->data_dim);
    return 1;
  }
  
}

int aux_alloc(unit_data* u_dat_ptr){
  
  u_dat_ptr->aux_vector=malloc(sizeof(float)*u_dat_ptr->data_dim);
  if(u_dat_ptr->aux_vector==NULL){ 
    fprintf(stderr, ERR_ALLOC);
    return 0;    
  } else {
    memset(u_dat_ptr->aux_vector, 0, sizeof(float)*u_dat_ptr->data_dim);
    return 1;
  }
  
}

int match_alloc(match** match_l, int data_number, int data_dim){
  
  register int i=0;
  
  *match_l=malloc(sizeof(match)*data_number);
  
  if(*match_l==NULL){ 
    fprintf(stderr, ERR_ALLOC);
    return 0;    
  } else {
    memset(*match_l, 0, sizeof(match)*data_number);
    
    for(i=0;i<data_number;i++){
      (*match_l)[i].data_vector=(float*)malloc(sizeof(float)*data_dim);
      if((*match_l)[i].data_vector==NULL){ 
	fprintf(stderr, ERR_ALLOC);
	return 0;    
      } else{
	memset((*match_l)[i].data_vector, 0, sizeof(float)*data_dim);
	
	pthread_mutex_init(&((*match_l)[i].match_mutex), NULL);
      }
    }
  }
  
  return 1;
  
}

int w_from_file_init(unit_data* u_dat_ptr){
  
  static char w_file_name[FILENAME_MAX];
  static int w_file_known_flag=0;
  
  static int dim=0;
  static int unit=0;
  
  float* w_array=malloc(sizeof(float)*(u_dat_ptr->data_dim));
  
  register int i=0;
  
  if(w_array==NULL){ 
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  
  if(!w_file_known_flag){
    if(get_w_file_name(w_file_name)==0) return 0;
    w_file_known_flag=W_FILE_KNOWN;
    
    dim=u_dat_ptr->data_dim;
    unit=(u_dat_ptr->row)*(u_dat_ptr->column);
  }
  
  if(get_w_line(w_file_name, unit, dim, w_array)==0) return 0;
  
  for(i=0;i<dim;i++) u_dat_ptr->w_array[i]=w_array[i];
  
  free(w_array);

  return 1;
}

int w_rand_init(unit_data* u_dat_ptr, char* data_file_name){
  
  register int i;
  
  static int randf_init_flag=0, min_max_found_flag=0;
  
  static float* min_array;
  static float* max_array;
  
  if(min_max_found_flag==0){
    min_array=malloc(sizeof(float)*(u_dat_ptr->data_dim));
    max_array=malloc(sizeof(float)*(u_dat_ptr->data_dim));
    
    if(min_array==NULL || max_array==NULL){ 
      fprintf(stderr, ERR_ALLOC);
      return 0;
    }
    else{
      memset(min_array, 0, sizeof(float)*(u_dat_ptr->data_dim));
      memset(max_array, 0, sizeof(float)*(u_dat_ptr->data_dim));
    }
    
    if(get_data_min_max(data_file_name, min_array, MIN_VALUE)==0 || get_data_min_max(data_file_name, max_array, MAX_VALUE)==0)
      return 0;
    
    min_max_found_flag=1;
  }
  
  if(randf_init_flag!=RANDF_INITIALIZED) {
    srand(time(NULL));
    randf_init_flag=RANDF_INITIALIZED;
  }
  
  for (i=0;i<u_dat_ptr->data_dim;i++)
    (u_dat_ptr->w_array)[i] = min_array[i] + (max_array[i]-min_array[i])*(((float)rand())/RAND_MAX);
  
  if((u_dat_ptr->row*u_dat_ptr->column)==(u_dat_ptr->my_id+1)){
    free(min_array);
    free(max_array);
    min_max_found_flag=0;
  }
  
  return 1;
 
}

int u_dat_struct_init(unit_data* u_d_ptr, int row, int column, int topo, int learning, int data_dim, int id, int epochs, int init, char* data_file_name){
  
  u_d_ptr->row=row;
  u_d_ptr->column=column;
  u_d_ptr->topo=topo;
  u_d_ptr->learning=learning;
  u_d_ptr->data_dim=data_dim;
  u_d_ptr->dist=0;
  u_d_ptr->bmu=0;
  u_d_ptr->my_id=id;
  u_d_ptr->epochs=epochs;
  
  if(w_alloc(u_d_ptr)==0) return 0;
  if(in_alloc(u_d_ptr)==0) return 0;
  if(aux_alloc(u_d_ptr)==0) return 0;

  if(init==RANDOM) if(w_rand_init(u_d_ptr, data_file_name)==0) return 0;
  if(init==FROM_FILE) if(w_from_file_init(u_d_ptr)==0) return 0;
  
  u_d_ptr->fine_conf.l_rate_type=get_lrate_type();
  u_d_ptr->fine_conf.l1r=get_1_lrate();
  u_d_ptr->fine_conf.l2r=get_2_lrate();
  u_d_ptr->fine_conf.l3r=get_3_lrate();
  u_d_ptr->fine_conf.neighborhood_func=get_neighborhood_func();
  
  u_d_ptr->read_flag=0;
  pthread_mutex_init(&(u_d_ptr->read_flag_mutex), NULL);
  pthread_cond_init(&(u_d_ptr->read_flag_cv), NULL);
  
  u_d_ptr->w_update_flag=0;
  pthread_mutex_init(&(u_d_ptr->w_update_flag_mutex), NULL);
  pthread_cond_init(&(u_d_ptr->w_update_flag_cv), NULL);
  
  u_d_ptr->main=pthread_self();

  return 1;
  
}
