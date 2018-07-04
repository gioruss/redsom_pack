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

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "common.h"

int map_alloc(dist_unit** map_ptr, map_param* map_info){
  
  register int i;
  
  *map_ptr=malloc(sizeof(dist_unit)*(map_info->unit_num));
  if(*map_ptr==NULL){
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  
  memset(*map_ptr, 0, sizeof(dist_unit)*(map_info->unit_num));
  
  for(i=0;i<map_info->unit_num;i++){
    if(((*map_ptr)[i].codebook=malloc(sizeof(float)*(map_info->codebook_dim)))==NULL){
      fprintf(stderr, ERR_ALLOC);
      free(*map_ptr);
      return 0;
    }
    memset((*map_ptr)[i].codebook, 0, sizeof(float)*(map_info->codebook_dim));
  }
  
  return 1;
}

void map_free(dist_unit* map_ptr, map_param* map_info){
  
  register int i;
  
  for(i=0;i<map_info->unit_num;i++) if((map_ptr)[i].codebook!=NULL) free((map_ptr)[i].codebook);
  
  free(map_ptr);
  
}

void exit_procedure(dist_unit* map_ptr, map_param* map_info, IplImage *map_img, int return_code){
  
  cvDestroyWindow("U-MATRIX MAP");
  
  if(map_img!=NULL)
    cvReleaseImage(&map_img);
  
  if(map_ptr!=NULL)
    map_free(map_ptr, map_info);
  
  fprintf(stdout, EXITING, return_code);
  
  exit(return_code);
  
}
