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

#include <pthread.h>

void init_global_flags(){
  
  data_flag=END_OF_DATA;
  pthread_mutex_init(&data_flag_mutex, NULL);
  pthread_cond_init(&data_flag_cv, NULL);
  
  bmu_flag=0;
  pthread_mutex_init(&bmu_flag_mutex, NULL);
  pthread_cond_init(&bmu_flag_cv, NULL);
  
  finish_flag=0;
  pthread_mutex_init(&finish_flag_mutex, NULL);
  pthread_cond_init(&finish_flag_cv, NULL);
  
}

void send_broadcast(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv){
  
  pthread_mutex_lock(flag_mutex);
  *flag=flag_value;
  pthread_cond_broadcast(flag_cv);
  pthread_mutex_unlock(flag_mutex);

}

void send_signal(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv){
  
  pthread_mutex_lock(flag_mutex);
  *flag=flag_value;
  pthread_cond_signal(flag_cv);
  pthread_mutex_unlock(flag_mutex);
  
}

void wait_flag(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv){
  
  pthread_mutex_lock(flag_mutex);
  while(*flag!=flag_value) pthread_cond_wait(flag_cv, flag_mutex);
  pthread_mutex_unlock(flag_mutex);

}
