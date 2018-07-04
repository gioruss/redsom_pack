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
#include "global.h"
#include "som_io.h"
#include "unit.h"
#include "math_function.h"
#include "flag_management.h"
#include "som_routine.h"

#include <pthread.h>

int main(int argc, char *argv[]){
  
  int row, column, unit, learning, topo, init, epochs, data_dim, data_number, bmu;
  
  float bmu_dist=0;
  
  unit_data* u_dat_array=NULL;
  pthread_t* pthread_array=NULL;
  
  float* in_vector=NULL;
  
  register int i=0, step=0, get_data_result=0;
  
  fprintf(stdout, START_STR, REDSOM_VERSION);
  
  init_global_flags();
  
  row=column=unit=learning=topo=init=data_dim=epochs=data_number=bmu=0;
  
  if(argc<3){
    fprintf(stderr, ARG_MISS);
    fprintf(stdout, SOM_USAGE);
    exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  }
  
  fprintf(stdout, READING_PARAM_NOTIFICATION);
  
  if(get_param(&row, &column, &unit, &learning, &topo, &init, &epochs)==0)
    exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  if(get_data_dim(&data_dim, argv[1])==0)
    exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  
  if(out_file_create(argv[2])==0)
    exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  
  fprintf(stdout, ALLOC_NOTIFICATION);
  
  if(u_dat_array_alloc(&u_dat_array, unit)==0 ||
    pthread_array_alloc(&pthread_array, unit)==0 ||
    in_vector_alloc(&in_vector, data_dim)==0)
      exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  
  if(learning==BATCH){
    if((data_number=get_data_number(argv[1]))==0)
      exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
    
    if(match_alloc(&match_list, data_number, data_dim)==0)
      exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
    
  }
  
  fprintf(stdout, INIT_STRUCT_NOTIFICATION);
  
  for(i=0;i<unit;i++)
    if(u_dat_struct_init(&(u_dat_array[i]), row, column, topo, learning, data_dim, i, epochs, init, argv[1])==0)
      exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
  
  fprintf(stdout, START_THREADS_NOTIFICATION);
    
  for(i=0;i<unit;i++){  
    if(pthread_create (&(pthread_array[i]), NULL, (void *(*)(void*))(&unit_main), (void*)&(u_dat_array[i]))!=0) {
      fprintf(stderr, ERR_THREAD);
      exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 0);
    }    
  }
  
  for(step=1;step<=epochs;step++){
    
    while((get_data_result=get_data_line(argv[1], in_vector))==1){
      
      for(i=0;i<unit;i++)
	memcpy(u_dat_array[i].input_vector, in_vector, sizeof(float)*data_dim);
      
      send_broadcast(&data_flag, NEXT_DATA, &data_flag_mutex, &data_flag_cv);
      
      for(i=0,bmu=-1,bmu_dist=0;i<unit;i++){
	wait_flag(&u_dat_array[i].read_flag, DATA_READED_DIST_CALCULATED, &(u_dat_array[i].read_flag_mutex), &(u_dat_array[i].read_flag_cv));
	
	u_dat_array[i].read_flag=0;
	
	if(u_dat_array[i].dist<=bmu_dist || i==0){
	  
	  bmu=i;
	  bmu_dist=u_dat_array[i].dist;
	  
	}
      }
      
      fprintf(stdout, STEP_BMU_NOTIFICATION, step, bmu);
      
      if(learning==INCREMENTAL){
	
	for(i=0;i<unit;i++) u_dat_array[i].bmu=bmu;
	send_broadcast(&bmu_flag, BMU_PRESENT, &bmu_flag_mutex, &bmu_flag_cv);
	
	for(i=0;i<unit;i++){
	wait_flag(&(u_dat_array[i].w_update_flag), WEIGHTS_UPDATED, &(u_dat_array[i].w_update_flag_mutex), &(u_dat_array[i].w_update_flag_cv));
	
	u_dat_array[i].w_update_flag=0;
	}
      }
      else if(learning==BATCH){
	send_broadcast(&bmu_flag, BMU_FOUND, &bmu_flag_mutex, &bmu_flag_cv);
	
	pthread_mutex_lock(&(match_list[data_flag].match_mutex));
	pthread_mutex_lock(&data_flag_mutex);
	{

	  match_list[data_flag].bmu=bmu;
	  
	  for(i=0;i<data_dim;i++)
	    match_list[data_flag].data_vector[i]=in_vector[i];
	  
	}
	pthread_mutex_unlock(&data_flag_mutex);
	pthread_mutex_unlock(&(match_list[data_flag].match_mutex));
	
	for(i=0;i<unit;i++){
	wait_flag(&(u_dat_array[i].w_update_flag), READY_TO_NEXT_DATA, &(u_dat_array[i].w_update_flag_mutex), &(u_dat_array[i].w_update_flag_cv));
	
	u_dat_array[i].w_update_flag=0;
	}
      }
      
      bmu_flag=0;
    }
     
    if(get_data_result==0) exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, THREADS_EXECUTING);
    else if(get_data_result==END_OF_DATA){
       
      if(learning==BATCH) fprintf(stdout, BATCH_UPDATE_NOTIFICATION);
      
      send_broadcast(&data_flag, END_OF_DATA, &data_flag_mutex, &data_flag_cv);
       
      for(i=0;i<unit;i++){
	wait_flag(&(u_dat_array[i].read_flag), EOD_READED, &(u_dat_array[i].read_flag_mutex), &(u_dat_array[i].read_flag_cv));
	 
	u_dat_array[i].read_flag=0;
      }
    }
  }
  
  if(write_output(argv[2], u_dat_array, unit)==0) exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, THREADS_EXECUTING);
  
  fprintf(stdout, OUTPUT_WRITTEN);
    
  send_broadcast(&finish_flag, FINISH, &finish_flag_mutex, &finish_flag_cv);
  
  for(i=0;i<unit;i++) pthread_join(pthread_array[i], NULL);
   
  exit_procedure(u_dat_array, match_list, pthread_array, in_vector, unit, data_number, 1);
  
  return 1;
  
}
