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
#include "som_io.h"
#include "math_function.h"
#include "normalizer_routine.h"

int main(int argc, char *argv[]){
  
  int file_number=0, data_dim=0, next_data_dim=0, total_data_number=0, num=0, get_data_result=0;
  register int i, j, k;
  
  data_vector* data_matrix=NULL;
  data_vector mean_vector=NULL;
  data_vector std_dev_vector=NULL;
  data_vector read_vector=NULL;
  
  int* data_number=NULL;
  
  char norm_file_name[FILENAME_MAX];
  
  fprintf(stdout, START_STR, REDSOM_VERSION);
  
  memset(norm_file_name, 0, sizeof(char)*FILENAME_MAX);
  
  if(argc<2){
    fprintf(stderr, ARG_MISS);
    fprintf(stdout, NORMALIZER_USAGE);
    exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
  }
  
  file_number=argc-1;
  
  data_number=malloc(sizeof(int)*file_number);
  if(data_number==NULL){
    fprintf(stderr, ERR_ALLOC);
    exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
  }
  
  for(i=0;i<file_number;i++){
    if(get_data_dim(&next_data_dim, argv[i+1])==0)
      exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
    if(i==0) data_dim=next_data_dim;
    else if(data_dim!=next_data_dim)
      exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
    
    if((num=get_data_number(argv[i+1]))==0)
      exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
    else{
      total_data_number+=num;
      data_number[i]=num;
    }
  }
  
  data_matrix=malloc(sizeof(data_vector)*total_data_number);
  mean_vector=malloc(sizeof(float)*data_dim);
  std_dev_vector=malloc(sizeof(float)*data_dim);
  read_vector=malloc(sizeof(float)*data_dim);
  if(mean_vector==NULL || std_dev_vector==NULL || read_vector==NULL || data_matrix==NULL){
    fprintf(stderr, ERR_ALLOC);
    exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
  }
  else{
    memset(data_matrix, 0, sizeof(data_vector)*total_data_number);
    memset(mean_vector, 0, sizeof(float)*data_dim);
    memset(std_dev_vector, 0, sizeof(float)*data_dim);
    memset(read_vector, 0, sizeof(float)*data_dim);
  }
  
  for(i=0;i<total_data_number;i++){
    data_matrix[i]=malloc(sizeof(float)*data_dim);
    if(data_matrix[i]==NULL){
      fprintf(stderr, ERR_ALLOC);
      exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
    }
    else memset(data_matrix[i], 0, sizeof(float)*data_dim);
  }
  
  for(j=0,i=0;i<file_number;i++){
    while((get_data_result=get_data_line(argv[i+1], read_vector))!=END_OF_DATA){
      
      if(get_data_result==0)
	exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
      else{
	for(k=0;k<data_dim;k++)
	  data_matrix[j][k]=read_vector[k];
	j++;
      }
    }
  }

  find_mean(data_matrix, total_data_number, data_dim, mean_vector);
  find_std_dev(data_matrix, total_data_number, data_dim, std_dev_vector);
  
  for(i=0;i<data_dim;i++)
    for(j=0;j<total_data_number;j++)
      data_matrix[j][i] = ( data_matrix[j][i] - mean_vector[i] ) / std_dev_vector[i];
    
  for(i=0,j=0;i<file_number;i++,j+=data_number[i]){
    
    norm_file_name_create(argv[i+1], norm_file_name);
    
    if(write_norm_file(norm_file_name, &data_matrix[j], data_number[i], data_dim)==0)
      exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 0);
    else
      fprintf(stdout, NORM_FILE_WRITTEN, norm_file_name);
    
  }
  
  exit_procedure(data_matrix, mean_vector, std_dev_vector, read_vector, data_number, total_data_number, 1);
  
  return 1;
}

