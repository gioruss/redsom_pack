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
#include "red_routine.h"
#include "math_function.h"
#include "drawing.h"

int main(int argc, char *argv[]){
  
  char file1_name[FILENAME_MAX], file2_name[FILENAME_MAX];
  
  int response=0;

  IplImage *map1_img=NULL, *map2_img=NULL;

  red_unit* map1=NULL;
  red_unit* map2=NULL;
  map_param map_info;
  
  fprintf(stdout, START_STR, REDSOM_VERSION);
  
  memset(&map_info, 0, sizeof(map_param));

  if(get_2_map_files(file1_name, file2_name)==0)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  
  if(get_2_map_param(file1_name, file2_name, &map_info)==0)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  
  if(map_alloc(&map1, &map_info)==0 || map_alloc(&map2, &map_info)==0)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  
  if(load_codebook(file1_name, map1, MAP_RED)==0 || load_codebook(file2_name, map2, MAP_RED)==0)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  
  if(density_calc(map1, map2, &map_info)==0)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  
  re_density_calc(map1, &map_info);
  re_density_calc(map2, &map_info);
  
  if((map1_img=draw_red_map(map1, &map_info))==NULL || (map2_img=draw_red_map(map2, &map_info))==NULL)
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
      
  if((response=ask_red_file_save())==NO_RESPONSE);
  else if(response==YES_RESPONSE){
    if(get_2_red_files(file1_name, file2_name)==0)
      exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
    else if(write_red_file(file1_name, map1, &map_info)==0 || write_red_file(file2_name, map2, &map_info)==0)
      exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  }
  else
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, response);
  
  if((response=ask_image_save())==NO_RESPONSE);
  else if(response==YES_RESPONSE){
    if(get_2_image_files(file1_name, file2_name)==0)
      exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
    else if(write_image_file(file1_name, map1_img)==0 || write_image_file(file2_name, map2_img)==0)
      exit_procedure(map1, map2, &map_info, map1_img, map2_img, 0);
  }
  else
    exit_procedure(map1, map2, &map_info, map1_img, map2_img, response);
  
  cvNamedWindow("RED MAP 1", CV_WINDOW_AUTOSIZE);
  cvShowImage("RED MAP 1", map1_img);
  cvNamedWindow("RED MAP 2", CV_WINDOW_AUTOSIZE);
  cvShowImage("RED MAP 2", map2_img);
  
  cvWaitKey(0);

  exit_procedure(map1, map2, &map_info, map1_img, map2_img, 1);
  
  return 1;
  
}
