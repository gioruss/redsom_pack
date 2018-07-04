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

int get_2_map_files(char* file1_name, char* file2_name){
  
  register int i=0;
  
  fprintf(stdout, MAP_FILE_REQUEST, 1);
  if(fgets(file1_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file1_name[i]=='\n'){
      file1_name[i]='\0';
      break;
    }
  }
  
  fprintf(stdout, MAP_FILE_REQUEST, 2);
  if(fgets(file2_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file2_name[i]=='\n'){
      file2_name[i]='\0';
      break;
    }
  }
  
  return 1;

}

int get_2_map_param(char* file1_name, char* file2_name, map_param* map_ptr){
  
  char topostr1[STRLENGHT], topostr2[STRLENGHT];
  FILE *map1, *map2;
  int param_number=0;
  
  int row1, column1, dim1;
  int row2, column2, dim2;
  
  map1=fopen(file1_name, "r");
  if(map1==NULL){
    fprintf(stderr, ERR_OPEN_FILE, file1_name);
    return 0;
  }
  
  map2=fopen(file2_name, "r");
  if(map2==NULL){
    fprintf(stderr, ERR_OPEN_FILE, file2_name);
    return 0;
  }
  
  memset(topostr1, 0, sizeof(char)*STRLENGHT);
  memset(topostr2, 0, sizeof(char)*STRLENGHT);
 
  if(!feof(map1))
    param_number=fscanf(map1, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %s\nCODEBOOK_DIM: %d\n\n", &row1, &column1, topostr1, &dim1);
  
  fclose(map1);
  
  if(param_number<4){
    fprintf(stderr, ERR_CONF_FILE, file1_name);
    return 0;
  }
  
  if(!feof(map2)) 
    param_number=fscanf(map2, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %s\nCODEBOOK_DIM: %d\n\n", &row2, &column2, topostr2, &dim2);
  
  fclose(map2);
  
  if(param_number<4){
    fprintf(stderr, ERR_CONF_FILE, file2_name);
    return 0;
  }
  
  if((strcmp(topostr1, "HEXA")!=0 && strcmp(topostr1, "RECT")!=0) ||
    ((row1*column1)<=0)||
    (dim1<=0)){
    fprintf(stderr, ERR_CONF_FILE, file1_name);
    return 0;
  }
  
  if((strcmp(topostr2, "HEXA")!=0 && strcmp(topostr2, "RECT")!=0) ||
    ((row2*column2)<=0)||
    (dim2<=0)){
    fprintf(stderr, ERR_CONF_FILE, file2_name);
    return 0;
  }
  
  if((strcmp(topostr1, topostr2)!=0) || (row1!=row2) || (column1!=column2) || (dim1!=dim2)){
    fprintf(stderr, MAP_FILE_DIFF, file1_name, file2_name);
    return 0;
  }
  
  map_ptr->row=row1;
  map_ptr->column=column1;
  map_ptr->codebook_dim=dim1;
  map_ptr->unit_num=row1*column1;
  if(strcmp(topostr1, "HEXA")==0) map_ptr->topo=HEXA;
  else map_ptr->topo=RECT;
     
  return 1;	     
  
}

int load_codebook(char* map_file_name, void* map_ptr, int map_type){
  
  FILE* map_file;
  int param_number=0;
  int row, column, dim, unit_num;
  char topostr[STRLENGHT];
  register int i, count=0;
 
  map_file=fopen(map_file_name, "r");
  if(map_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, map_file_name);
    return 0;
  }
  
  if(!feof(map_file)) 
    param_number=fscanf(map_file, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %s\nCODEBOOK_DIM: %d\n\n", &row, &column, topostr, &dim);
  
  
  if(param_number<4){
    fprintf(stderr, ERR_CONF_FILE, map_file_name);
    return 0;
  }
  
  unit_num=row*column;
  
  if(map_type==MAP_RED){
    while(!feof(map_file)){
      for(i=0;i<dim;i++){
	if(fscanf(map_file, "%f", &(((red_unit*)map_ptr)[count].codebook[i]))!=1){
	  fprintf(stderr, ERR_CONF_FILE, map_file_name);
	  fclose(map_file);
	  return 0;
	}
	((red_unit*)map_ptr)[count].unit_id=count;
      }
      
      fscanf(map_file, "\n");
      count++;
    }
  }
  else if(map_type==MAP_UMATRIX){
    while(!feof(map_file)){
      for(i=0;i<dim;i++){
	if(fscanf(map_file, "%f", &(((dist_unit*)map_ptr)[count].codebook[i]))!=1){
	  fprintf(stderr, ERR_CONF_FILE, map_file_name);
	  fclose(map_file);
	  return 0;
	}
	((dist_unit*)map_ptr)[count].unit_id=count;
      }
      
      fscanf(map_file, "\n");
      count++;
    }
  }
  else{
    fclose(map_file);
    return 0;
  }
  
  fclose(map_file);
  
  if(count==unit_num) return 1;
  else{
    fprintf(stderr, ERR_CONF_FILE, map_file_name);
    return 0;
  }

}

int ask_image_save(){
  
  char response[FILENAME_MAX];
  register int i;
  
  while(1){
    
    fprintf(stdout, ASK_IMAGE_SAVE);
    
    if(fgets(response, FILENAME_MAX, stdin)==NULL){
      fprintf(stderr, ERR_INPUT_STR);
      return 0;
    }

    for(i=0;i<FILENAME_MAX;i++) {
      if(response[i]=='\n'){
	response[i]='\0';
	break;
      }
      else if(islower(response[i]))
	response[i]=toupper(response[i]);
    }
    
    if(strcmp(response, "YES")==0) return YES_RESPONSE;
    else if(strcmp(response, "NO")==0) return NO_RESPONSE;

  }
}

int ask_red_file_save(){
  
  char response[FILENAME_MAX];
  register int i;
  
  while(1){
    
    fprintf(stdout, ASK_REDFILE_SAVE);
    
    if(fgets(response, FILENAME_MAX, stdin)==NULL){
      fprintf(stderr, ERR_INPUT_STR);
      return 0;
    }

    for(i=0;i<FILENAME_MAX;i++) {
      if(response[i]=='\n'){
	response[i]='\0';
	break;
      }
      else if(islower(response[i]))
	response[i]=toupper(response[i]);
    }
    
    if(strcmp(response, "YES")==0) return YES_RESPONSE;
    else if(strcmp(response, "NO")==0) return NO_RESPONSE;

  }
}

int get_2_image_files(char* file1_name, char* file2_name){
  
  register int i=0;
  
  fprintf(stdout, IMAGEFILE_REQUEST, 1);
  if(fgets(file1_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file1_name[i]=='\n'){
      file1_name[i]='\0';
      break;
    }
  }
  
  fprintf(stdout, IMAGEFILE_REQUEST, 2);
  if(fgets(file2_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file2_name[i]=='\n'){
      file2_name[i]='\0';
      break;
    }
  }
  
  return 1;
  
}
  
int get_2_red_files(char* file1_name, char* file2_name){
  
  register int i=0;
  
  fprintf(stdout, REDFILE_REQUEST, 1);
  if(fgets(file1_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file1_name[i]=='\n'){
      file1_name[i]='\0';
      break;
    }
  }
  
  fprintf(stdout, REDFILE_REQUEST, 2);
  if(fgets(file2_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file2_name[i]=='\n'){
      file2_name[i]='\0';
      break;
    }
  }
  
  return 1;
  
}

int write_red_file(char* red_file_name, red_unit* map, map_param* map_info){
  
  FILE* red_file;
  register int i;
      
  if((red_file=fopen(red_file_name, "w"))==NULL){
    fprintf(stderr, ERR_CREATE_FILE, red_file_name);
    return 0;
  }
  
  else{
    
    fprintf(red_file, "MAP_ROW: %d\nMAP_COLUMN: %d\n", map_info->row, map_info->column);
    fprintf(red_file, "MAP_TOPOLOGY: ");
    if(map_info->topo==HEXA) fprintf(red_file, "HEXA\n\n");
    else if(map_info->topo==RECT) fprintf(red_file, "RECT\n\n");
    
    for(i=0;i<map_info->unit_num;i++){
      fprintf(red_file, "%f %f %f", map[i].dens_on_map1, map[i].dens_on_map2, map[i].re_dens);
      fprintf(red_file, "\n");
    }
    
    fclose(red_file);
    return 1;
  }
}

int write_image_file(char* image_file_name, IplImage *map_img){
  
  char file_name[FILENAME_MAX];
  memset(file_name, 0, sizeof(char)*FILENAME_MAX);
  
  sprintf(file_name, "%s%s", image_file_name, IMAGE_TYPE);
  
  if(cvSaveImage(file_name, map_img, NULL)==0){
    fprintf(stderr, ERR_CREATE_FILE, file_name);
    return 0;
  }
  else return 1;

}

int get_map_file(char* file_name){
  
  register int i=0;
  
  fprintf(stdout, MAP_FILE_REQUEST, 0);
  if(fgets(file_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file_name[i]=='\n'){
      file_name[i]='\0';
      break;
    }
  }
  
  return 1;

}

int get_map_param(char* file_name, map_param* map_ptr){
  
  char topostr[STRLENGHT];
  FILE *map;
  int param_number=0;
  
  int row, column, dim;
  
  map=fopen(file_name, "r");
  if(map==NULL){
    fprintf(stderr, ERR_OPEN_FILE, file_name);
    return 0;
  }
 
  memset(topostr, 0, sizeof(char)*STRLENGHT);
 
  if(!feof(map))
    param_number=fscanf(map, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %s\nCODEBOOK_DIM: %d\n\n", &row, &column, topostr, &dim);
  
  fclose(map);
  
  if(param_number<4){
    fprintf(stderr, ERR_CONF_FILE, file_name);
    return 0;
  }
    
  if((strcmp(topostr, "HEXA")!=0 && strcmp(topostr, "RECT")!=0) ||
    ((row*column)<=0)||
    (dim<=0)){
    fprintf(stderr, ERR_CONF_FILE, file_name);
    return 0;
  }
  
  map_ptr->row=row;
  map_ptr->column=column;
  map_ptr->codebook_dim=dim;
  map_ptr->unit_num=row*column;
  if(strcmp(topostr, "HEXA")==0) map_ptr->topo=HEXA;
  else map_ptr->topo=RECT;
     
  return 1;	     
  
}

int get_image_file(char* file_name){
  
  register int i=0;
  
  fprintf(stdout, IMAGEFILE_REQUEST, 0);
  if(fgets(file_name, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(file_name[i]=='\n'){
      file_name[i]='\0';
      break;
    }
  }
   
  return 1;
  
}
