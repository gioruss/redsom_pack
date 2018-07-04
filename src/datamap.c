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
#include "datamap_routine.h"

int main(int argc, char *argv[]){
  
  register int i;
  
  map_param map_info;
  int data_dim;
  
  int get_data_result, dist, bmu, min_dist, data_number;
  
  float* codebook_vector=NULL;
  float* data_vector=NULL;
  
  fprintf(stdout, START_STR, REDSOM_VERSION);
  
  if(argc<4){
    fprintf(stderr, ARG_MISS);
    fprintf(stdout, DATAMAP_USAGE);
    exit_procedure(data_vector, codebook_vector, 0);
  }
  
  if(get_map_param(argv[2], &map_info)==0) exit_procedure(data_vector, codebook_vector, 0);
  if(get_data_dim(&data_dim, argv[1])==0) exit_procedure(data_vector, codebook_vector, 0);
  if((data_number=get_data_number(argv[1]))==0) exit_procedure(data_vector, codebook_vector, 0);
  if(out_file_create(argv[3])==0) exit_procedure(data_vector, codebook_vector, 0);
  
  if(data_dim!=map_info.codebook_dim){
    fprintf(stderr, ERR_DATA_DIM_DIFF, argv[1], argv[2]);
    exit_procedure(data_vector, codebook_vector, 0);
  }
  
  if((data_vector=malloc(sizeof(float)*data_dim))==NULL || (codebook_vector=malloc(sizeof(float)*(map_info.codebook_dim)))==NULL){
    fprintf(stderr, ERR_ALLOC);
    exit_procedure(data_vector, codebook_vector, 0);
  }
  
  fprintf(stdout, DATAMAP_NOTIFICATION);
  
  while((get_data_result=get_data_line(argv[1], data_vector))==1){
    
    for(i=0, min_dist=0, bmu=0;i<map_info.unit_num;i++){
      
      if(get_w_line(argv[2], map_info.unit_num, map_info.codebook_dim, codebook_vector)==0) exit_procedure(data_vector, codebook_vector, 0);
      
      dist=euclidean_dist(codebook_vector, data_vector, data_dim);
      
      if(dist<min_dist || i==0){
	min_dist=dist;
	bmu=i;
      }
    }
    
    if(write_match_line(argv[3], data_vector, bmu, data_number, &map_info)==0) exit_procedure(data_vector, codebook_vector, 0);
    
  }
  
  if(get_data_result==0) exit_procedure(data_vector, codebook_vector, 0);
  
  exit_procedure(data_vector, codebook_vector, 1);
  
  return 1;
  
}
