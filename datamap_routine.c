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

void exit_procedure(float* data_vector, float* codebook_vector, int return_code){
  
  if(data_vector!=NULL) free(data_vector);
  if(codebook_vector!=NULL) free(codebook_vector);
  
  fprintf(stdout, EXITING, return_code);
  
  exit(return_code);
  
}
