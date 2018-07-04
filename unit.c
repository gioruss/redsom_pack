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
#include "math_function.h"
#include "flag_management.h"
#include "unit.h"

#include <pthread.h>

void unit_main(void* my_param_ptr){
  
  unit_data* param_ptr=my_param_ptr;
  
  register int step=1, data_count=0;
  int eod_flag=END_OF_DATA;
    
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  
  while(step<=param_ptr->epochs){
        
    pthread_mutex_lock(&data_flag_mutex);
    
    while((data_flag!=(data_count) && data_flag!=END_OF_DATA) || (data_flag==END_OF_DATA && eod_flag==END_OF_DATA)) 
      pthread_cond_wait(&data_flag_cv, &data_flag_mutex);
    
    if(data_flag==END_OF_DATA){
      pthread_mutex_unlock(&data_flag_mutex);
       
      if(param_ptr->learning==BATCH) batch_update(param_ptr, step, data_count);
	
      eod_flag=END_OF_DATA;
      data_count=0;
      step++;
      
      send_signal(&(param_ptr->read_flag), EOD_READED, &(param_ptr->read_flag_mutex), &(param_ptr->read_flag_cv));
      
    }
    
    else{
      pthread_mutex_unlock(&data_flag_mutex);
      eod_flag=0;
      
      data_count++;
      
      param_ptr->dist = euclidean_dist(param_ptr->w_array, param_ptr->input_vector, param_ptr->data_dim);
  
      send_signal(&(param_ptr->read_flag), DATA_READED_DIST_CALCULATED, &(param_ptr->read_flag_mutex), &(param_ptr->read_flag_cv));
      
      if(param_ptr->learning==INCREMENTAL){
	wait_flag(&bmu_flag, BMU_PRESENT, &bmu_flag_mutex, &bmu_flag_cv);
	
	incremental_update(param_ptr, step);
	
	send_signal(&(param_ptr->w_update_flag), WEIGHTS_UPDATED, &(param_ptr->w_update_flag_mutex), &(param_ptr->w_update_flag_cv));
      }
      else if(param_ptr->learning==BATCH){
	wait_flag(&bmu_flag, BMU_FOUND, &bmu_flag_mutex, &bmu_flag_cv);
	
	send_signal(&(param_ptr->w_update_flag), READY_TO_NEXT_DATA, &(param_ptr->w_update_flag_mutex), &(param_ptr->w_update_flag_cv));
      }
      
    }

  }
  
  wait_flag(&finish_flag, FINISH, &finish_flag_mutex, &finish_flag_cv);
  
  pthread_exit(NULL);
  
}

void incremental_update(unit_data* param_ptr, int step){
  
  register int i=0;
  
  for(i=0;i<param_ptr->data_dim;i++)
    param_ptr->w_array[i] = param_ptr->w_array[i] +
    (l_rate(step, param_ptr->epochs, param_ptr->fine_conf.l_rate_type, param_ptr->fine_conf.l1r, param_ptr->fine_conf.l2r, param_ptr->fine_conf.l3r)*
    neighborhood_func(param_ptr->bmu, param_ptr->my_id, param_ptr->topo, step, param_ptr->epochs, param_ptr->row, param_ptr->column, param_ptr->fine_conf.neighborhood_func)*
    (param_ptr->input_vector[i]-param_ptr->w_array[i]));
  
}

void batch_update(unit_data* param_ptr, int step, int data_number){
  
  register int i=0, j=0, circolar_counter;
  int current_bmu=0;
  float weighted_data_number=0;
  
  memset(param_ptr->aux_vector, 0, sizeof(float)*(param_ptr->data_dim));
  
  srand(param_ptr->my_id);
  circolar_counter=(float)((float)rand()/RAND_MAX)*(float)(data_number-1); 
  
  for(i=0;i<data_number;i++){
    
    pthread_mutex_lock(&(match_list[circolar_counter].match_mutex));
    {
      memcpy(param_ptr->input_vector, match_list[circolar_counter].data_vector, sizeof(float)*(param_ptr->data_dim));
      current_bmu=match_list[circolar_counter].bmu;
    }
    pthread_mutex_unlock(&(match_list[circolar_counter].match_mutex));
    
    if(circolar_counter==(data_number-1)) circolar_counter=0;
    else circolar_counter++;
    
    weighted_data_number += 
    neighborhood_func(current_bmu, param_ptr->my_id, param_ptr->topo, step, param_ptr->epochs, param_ptr->row, param_ptr->column, param_ptr->fine_conf.neighborhood_func);
    
    for(j=0;j<param_ptr->data_dim;j++)
      param_ptr->aux_vector[j] += (param_ptr->input_vector[j])*
      neighborhood_func(current_bmu, param_ptr->my_id, param_ptr->topo, step, param_ptr->epochs, param_ptr->row, param_ptr->column, param_ptr->fine_conf.neighborhood_func);
    
  }
  
  if(weighted_data_number==0);
  else{
    for(j=0;j<param_ptr->data_dim;j++){
      param_ptr->aux_vector[j] /= weighted_data_number;
      param_ptr->w_array[j] = param_ptr->aux_vector[j];
    }
  }

}
