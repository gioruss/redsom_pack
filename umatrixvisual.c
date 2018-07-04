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
#include "visual_io.h"
#include "umatrix_routine.h"
#include "drawing.h"
#include "math_function.h"

int main(int argc, char *argv[]){
  
  char file_name[FILENAME_MAX];
  
  int response=0;

  IplImage *map_img=NULL;

  dist_unit* map=NULL;
  map_param map_info;
  
  fprintf(stdout, START_STR, REDSOM_VERSION);
  
  memset(&map_info, 0, sizeof(map_param));

  if(get_map_file(file_name)==0)
    exit_procedure(map, &map_info, map_img, 0);
  
  if(get_map_param(file_name, &map_info)==0)
    exit_procedure(map, &map_info, map_img, 0);
  
  if(map_alloc(&map, &map_info)==0)
    exit_procedure(map, &map_info, map_img, 0);
  
  if(load_codebook(file_name, map, MAP_UMATRIX)==0)
    exit_procedure(map, &map_info, map_img, 0);
  
  neighbor_dist_calc(map, &map_info);

  if((map_img=draw_umatrix_map(map, &map_info))==NULL)
    exit_procedure(map, &map_info, map_img, 0);

  if((response=ask_image_save())==NO_RESPONSE);
  else if(response==YES_RESPONSE){
    if(get_image_file(file_name)==0)
      exit_procedure(map, &map_info, map_img, 0);
    else if(write_image_file(file_name, map_img)==0)
      exit_procedure(map, &map_info, map_img, 0);
  }
  else
    exit_procedure(map, &map_info, map_img, response);
  
  cvNamedWindow("U-MATRIX MAP", CV_WINDOW_AUTOSIZE);
  cvShowImage("U-MATRIX MAP", map_img);

  cvWaitKey(0);

  exit_procedure(map, &map_info, map_img, 1);
  
  return 1;
  
}
