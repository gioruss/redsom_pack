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

int map_alloc(red_unit** map_ptr, map_param* map_info){
  
  register int i;
  
  *map_ptr=malloc(sizeof(red_unit)*(map_info->unit_num));
  if(*map_ptr==NULL){
    fprintf(stderr, ERR_ALLOC);
    return 0;
  }
  
  memset(*map_ptr, 0, sizeof(red_unit)*(map_info->unit_num));
  
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

void map_free(red_unit* map_ptr, map_param* map_info){
  
  register int i;
  
  for(i=0;i<map_info->unit_num;i++) if((map_ptr)[i].codebook!=NULL) free((map_ptr)[i].codebook);
  
  free(map_ptr);
  
}

void exit_procedure(red_unit* map1_ptr, red_unit* map2_ptr, map_param* map_info, IplImage *map1_img, IplImage *map2_img, int return_code){
  
  cvDestroyWindow("RED MAP 1");
  cvDestroyWindow("RED MAP 2");
  
  if(map1_img!=NULL)
    cvReleaseImage(&map1_img);
  if(map2_img!=NULL)
    cvReleaseImage(&map2_img);
  
  if(map1_ptr!=NULL)
    map_free(map1_ptr, map_info);
  if(map2_ptr!=NULL)
    map_free(map2_ptr, map_info);
  
  fprintf(stdout, EXITING, return_code);
  
  exit(return_code);
  
}

  
  
  