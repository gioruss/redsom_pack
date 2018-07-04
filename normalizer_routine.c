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

void norm_file_name_create(char* data_file_name, char* norm_file_name){

  char norm[]=".norm";
  
  sprintf(norm_file_name, "%s%s", data_file_name, norm);

}

void exit_procedure(data_vector* data_matrix, data_vector mean_vector, data_vector std_dev_vector, data_vector read_vector, int* data_number, int total_data_number, int return_code){
  
  register int i;
  
  if(data_matrix!=NULL){
    
    for(i=0;i<total_data_number;i++)
      if(data_matrix[i]!=NULL) free(data_matrix[i]);
      
    free(data_matrix);
  }
  
  if(mean_vector!=NULL) free(mean_vector);
  
  if(std_dev_vector!=NULL) free(std_dev_vector);
  
  if(read_vector!=NULL) free(read_vector);
  
  if(data_number!=NULL) free(data_number);
  
  fprintf(stdout, EXITING, return_code);
  
  exit(return_code);
  
}
