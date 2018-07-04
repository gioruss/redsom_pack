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

int get_param(int* row, int* column, int* unit, int* learning, int* topo, int* init, int* epochs){
  
  char topostr[STRLENGHT], learningstr[STRLENGHT], initstr[STRLENGHT];
  FILE* conf_file;
  int param_number=0;
  
  conf_file=fopen(CONF_FILE, "r");
  if(conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, CONF_FILE);
    return 0;
  }
  
  memset(topostr, 0, sizeof(char)*STRLENGHT);
  memset(learningstr, 0, sizeof(char)*STRLENGHT);
  
  if(!feof(conf_file)) 
    param_number=fscanf(conf_file, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %s\nMAP_LEARNING: %s\nMAP_INIT: %s\nEPOCHS: %d", row, column, topostr, learningstr, initstr, epochs);
  
  fclose(conf_file);
  
  if( (param_number<6) || (*unit=(*row)*(*column)) <=0  || *epochs <=0){
    fprintf(stderr, ERR_CONF_FILE, CONF_FILE);
    return 0;
  }
  
  if(strcmp(topostr, "HEXA")==0) *topo=HEXA;
    else if(strcmp(topostr, "RECT")==0) *topo=RECT;
    else{
      fprintf(stderr, ERR_CONF_FILE, CONF_FILE);
      return 0;
    }
  
  if(strcmp(learningstr, "BATCH")==0) *learning=BATCH;
     else if(strcmp(learningstr, "INCREMENTAL")==0) *learning=INCREMENTAL;
     else{
       fprintf(stderr, ERR_CONF_FILE, CONF_FILE);
       return 0;
     }
     
  if(strcmp(initstr, "RANDOM")==0) *init=RANDOM;
     else if(strcmp(initstr, "FROM_FILE")==0) *init=FROM_FILE;
     else{
       fprintf(stderr, ERR_CONF_FILE, CONF_FILE);
       return 0;
     }
     
     return 1;	     
     
}

int get_data_dim(int* data_dim, char* data_file_name){
  
  int param_number=0;
  FILE* data_file;
  
  data_file=fopen(data_file_name, "r");
  
  if(data_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, data_file_name);
    return 0;
  }
  
  if(!feof(data_file))
    param_number=fscanf(data_file, "VECTOR_DIM: %d", data_dim);
  
  fclose(data_file);
  
  if(param_number<1||(*data_dim)<=0){
    fprintf(stderr, ERR_DATA_FILE, data_file_name);
    return 0;
  }
    
  return 1;
}

int get_data_number(char* data_file_name){
  
  int param_number=0;
  int data_dim=0;
  register int i=0, count=0;
  float input;
  
  FILE* data_file;
  
  data_file=fopen(data_file_name, "r");
  
  if(data_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, data_file_name);
    return 0;
  }
  
  if(!feof(data_file)) param_number=fscanf(data_file, "VECTOR_DIM: %d", &data_dim);
  
  if(param_number<1||data_dim<=0){
    fprintf(stderr, ERR_DATA_FILE, data_file_name);
    fclose(data_file);
    return 0;
  }
  
  fscanf(data_file, "\n\n");
  
  while(!feof(data_file)){
    
    for(i=0;i<data_dim;i++){
      if(fscanf(data_file, "%f", &input)!=1){
	fprintf(stderr, ERR_DATA_FILE, data_file_name);
	fclose(data_file);
	return 0;
      }
 
    }
    
    count++;
    
    fscanf(data_file, "\n");
  }
  
  fclose(data_file);
  
  if(count==0){
    fprintf(stderr, ERR_DATA_FILE, data_file_name);
    return 0;
  }
  
  return count;
  
}
  
int get_data_min_max(char* data_file_name, float* m_array, int value_to_find){
  
  FILE* data_file;
  
  int param_number=0;
  int data_dim=0;
  float input;
  register int i=0;
  int first_row_flag=1;
  
  data_file=fopen(data_file_name, "r");
  
  if(data_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, data_file_name);
    return 0;
  }
  
  if(!feof(data_file)) param_number=fscanf(data_file, "VECTOR_DIM: %d", &data_dim);
  
  if(param_number<1||data_dim<=0){
    fprintf(stderr, ERR_DATA_FILE, data_file_name);
    fclose(data_file);
    return 0;
  }
  
  fscanf(data_file, "\n\n");
  
  while(!feof(data_file)){
    
    for(i=0;i<data_dim;i++){
      if(fscanf(data_file, "%f", &input)!=1){
	fprintf(stderr, ERR_DATA_FILE, data_file_name);
	fclose(data_file);
	return 0;
      }
      else if(first_row_flag==1) m_array[i]=input;
      else if(value_to_find==MAX_VALUE && input>m_array[i]) m_array[i]=input;
      else if(value_to_find==MIN_VALUE && input<m_array[i]) m_array[i]=input;
 
    }
    
    first_row_flag=0;
    fscanf(data_file, "\n");
  }
  
  fclose(data_file);
  
  return 1;
  
}

int get_w_file_name(char* dst_str){
  
  register int i=0;
  
  fprintf(stdout, W_FILE_REQUEST);
  if(fgets(dst_str, FILENAME_MAX, stdin)==NULL) {
    fprintf(stdout, ERR_INPUT_STR);
    return 0;
  }
  
  for(i=0;i<FILENAME_MAX;i++) {
    if(dst_str[i]=='\n'){
      dst_str[i]='\0';
      break;
    }
  }
  
  return 1;
  
}

int get_w_line(char* w_file_name, int unit, int dim, float* w_array){
  
  static FILE* w_file=NULL;
  register int i;
  int param_number=0, row, column, codebook_dim;
  static int count=0;
  
  if(w_file==NULL){
    w_file=fopen(w_file_name, "r");
    
    if(w_file==NULL){
      fprintf(stderr, ERR_OPEN_FILE, w_file_name);
      return 0;
    }
    
    if(!feof(w_file))
      param_number=fscanf(w_file, "MAP_ROW: %d\nMAP_COLUMN: %d\nMAP_TOPOLOGY: %*s\nCODEBOOK_DIM: %d\n\n", &row, &column, &codebook_dim);
    
    if(param_number<3 || row*column!=unit || codebook_dim!=dim){
      fprintf(stderr, ERR_W_FILE, w_file_name);
      fclose(w_file);
      return 0;
    }
  }
  
  if(!feof(w_file)){
    for(i=0;i<dim;i++){
      if(fscanf(w_file, "%f", &(w_array[i]))!=1){
	fprintf(stderr, ERR_W_FILE, w_file_name);
	fclose(w_file);
	return 0;
      }
    }
    
    fscanf(w_file, "\n");
    count++;
    
    if(count==unit){
      fclose(w_file);
      w_file=NULL;
      count=0;
    }
    
    return 1;
  }
  
  else{
    fclose(w_file);
    fprintf(stderr, ERR_W_FILE, w_file_name);
    return 0;
  }
  
}

int get_data_line(char* data_file_name, float* in_vect){
  
  static int dim;
  register int i;
  float input;
  static FILE* data_file=NULL;
  
  if(data_file==NULL){
    data_file=fopen(data_file_name, "r");
    fscanf(data_file, "VECTOR_DIM: %d\n\n", &dim);
  }
   
  if(data_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, data_file_name);
    return 0;
  }
    
  if(!feof(data_file)){
    
    for(i=0;i<dim;i++){
      if(fscanf(data_file, "%f", &input)!=1){
	fprintf(stderr, ERR_DATA_FILE, data_file_name);
	fclose(data_file);
	return 0;
      }
      in_vect[i]= input;
    }
    
    fscanf(data_file, "\n");
    
    return 1;
            
  }
  
  else{
    fclose(data_file);
    data_file=NULL;
    return END_OF_DATA;
  }
 
}

int out_file_create(char* out_file_name){
  
  FILE* out_file;
  
  if((out_file=fopen(out_file_name, "w"))==NULL){
        fprintf(stderr, ERR_CREATE_FILE, out_file_name);
    return 0;
  }
  else{
    fclose(out_file);
    return 1;
  }
  
}

int write_output(char* out_file_name, unit_data* u, int unit){
  
  FILE* out_file;
  register int i,j;
  int dim=0;
    
  if((out_file=fopen(out_file_name, "a"))==NULL){
    fprintf(stderr, ERR_OPEN_FILE, out_file_name);
    return 0;
  }
  
  else{
    
    fprintf(out_file, "MAP_ROW: %d\nMAP_COLUMN: %d\n", u[0].row, u[0].column);
    fprintf(out_file, "MAP_TOPOLOGY: ");
    if(u[0].topo==HEXA) fprintf(out_file, "HEXA\n");
    else if(u[0].topo==RECT) fprintf(out_file, "RECT\n");
    fprintf(out_file, "CODEBOOK_DIM: %d\n\n", u[0].data_dim);

    dim=u[0].data_dim;
    
    for(i=0;i<unit;i++){
      for(j=0;j<dim;j++) fprintf(out_file, "%f ", u[i].w_array[j]);
      fprintf(out_file, "\n");
      
    }
    
    fclose(out_file);
    return 1;
  }
  
}

int get_lrate_type(){
  
  FILE* fine_conf_file;
  
  char typestr[STRLENGHT]=NOT_DONE_STR;
  char err_str[FILENAME_MAX];
  
  memset(typestr, 0, sizeof(char)*STRLENGHT);
  
  fine_conf_file=fopen(FINE_CONF_FILE, "r");
  if(fine_conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_OPEN_FILE, FINE_CONF_FILE);
    log_write("get_lrate_type", err_str);
    log_write("get_lrate_type", USING_DEFAULT);
    log_write("get_lrate_type", DEFAULT_CONF_PARAM);
    return DEFAULT_LRATE_TYPE;
  }
  
  while(!feof(fine_conf_file) && fscanf(fine_conf_file, "LRATE TYPE: %s", typestr)!=1)
    fgetc(fine_conf_file);
  
  fclose(fine_conf_file);
  
  if(strcmp(typestr, NOT_DONE_STR)==0){
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_lrate_type", err_str);
    log_write("get_lrate_type", USING_DEFAULT);
    log_write("get_lrate_type", DEFAULT_CONF_PARAM);
    return DEFAULT_LRATE_TYPE;
  }
  else if(strcmp(typestr, "STEPS")==0) return STEPS;
  else if(strcmp(typestr, "TIME_INVERSE")==0) return TIME_INVERSE;
  else{
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_lrate_type", err_str);
    log_write("get_lrate_type", USING_DEFAULT);
    log_write("get_lrate_type", DEFAULT_CONF_PARAM);
    return DEFAULT_LRATE_TYPE;
  }
  
}

int get_neighborhood_func(){
  
  FILE* fine_conf_file;
  
  char typestr[STRLENGHT]=NOT_DONE_STR;
  char err_str[FILENAME_MAX];
  
  memset(typestr, 0, sizeof(char)*STRLENGHT);
  
  fine_conf_file=fopen(FINE_CONF_FILE, "r");
  if(fine_conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_OPEN_FILE, FINE_CONF_FILE);
    log_write("get_neighborhood_func", err_str);
    log_write("get_neighborhood_func", USING_DEFAULT);
    log_write("get_neighborhood_func", DEFAULT_CONF_PARAM);
    return DEFAULT_NEIGHBORHOOD_FUNC;
  }
  
  while(!feof(fine_conf_file) && fscanf(fine_conf_file, "NEIGHBORHOOD FUNCTION: %s", typestr)!=1)
    fgetc(fine_conf_file);
  
  fclose(fine_conf_file);
  
  if(strcmp(typestr, NOT_DONE_STR)==0){
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_neighborhood_func", err_str);
    log_write("get_neighborhood_func", USING_DEFAULT);
    log_write("get_neighborhood_func", DEFAULT_CONF_PARAM);
    return DEFAULT_NEIGHBORHOOD_FUNC;
  }
  else if(strcmp(typestr, "GAUSSIAN")==0) return GAUSSIAN;
  else if(strcmp(typestr, "BUBBLE")==0) return BUBBLE;
  else if(strcmp(typestr, "CUT_GAUSSIAN")==0) return CUT_GAUSSIAN;
  else{
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_neighborhood_func", err_str);
    log_write("get_neighborhood_func", USING_DEFAULT);
    log_write("get_neighborhood_func", DEFAULT_CONF_PARAM);
    return DEFAULT_NEIGHBORHOOD_FUNC;
  }
  
}

float get_1_lrate(){
  
  FILE* fine_conf_file;
  
  char err_str[FILENAME_MAX];
  
  float rate=-1;
  
  fine_conf_file=fopen(FINE_CONF_FILE, "r");
  if(fine_conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_OPEN_FILE, FINE_CONF_FILE);
    log_write("get_1_lrate", err_str);
    log_write("get_1_lrate", USING_DEFAULT);
    log_write("get_1_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_1_LRATE;
  }
  
  while(!feof(fine_conf_file) && fscanf(fine_conf_file, "FIRST PHASE LRATE: %f", &rate)!=1)
    fgetc(fine_conf_file);
  
  fclose(fine_conf_file);
  
  if(rate<0 || rate >1){
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_1_lrate", err_str);
    log_write("get_1_lrate", USING_DEFAULT);
    log_write("get_1_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_1_LRATE;
  }
  else return rate;
  
}
  
float get_2_lrate(){
  
  FILE* fine_conf_file;
  
  char err_str[FILENAME_MAX];
  
  float rate=-1;
  
  fine_conf_file=fopen(FINE_CONF_FILE, "r");
  if(fine_conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_OPEN_FILE, FINE_CONF_FILE);
    log_write("get_2_lrate", err_str);
    log_write("get_2_lrate", USING_DEFAULT);
    log_write("get_2_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_2_LRATE;
  }
  
  while(!feof(fine_conf_file) && fscanf(fine_conf_file, "SECOND PHASE LRATE: %f", &rate)!=1)
    fgetc(fine_conf_file);
  
  fclose(fine_conf_file);
  
  if(rate<0 || rate >1){
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_2_lrate", err_str);
    log_write("get_2_lrate", USING_DEFAULT);
    log_write("get_2_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_2_LRATE;
  }
  else return rate;
  
}

float get_3_lrate(){
  
  FILE* fine_conf_file;
  
  char err_str[FILENAME_MAX];
  
  float rate=-1;
  
  fine_conf_file=fopen(FINE_CONF_FILE, "r");
  if(fine_conf_file==NULL){
    fprintf(stderr, ERR_OPEN_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_OPEN_FILE, FINE_CONF_FILE);
    log_write("get_3_lrate", err_str);
    log_write("get_3_lrate", USING_DEFAULT);
    log_write("get_3_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_3_LRATE;
  }
  
  while(!feof(fine_conf_file) && fscanf(fine_conf_file, "THIRD PHASE LRATE: %f", &rate)!=1)
    fgetc(fine_conf_file);
  
  fclose(fine_conf_file);
  
  if(rate<0 || rate >1){
    fprintf(stderr, ERR_CONF_FILE, FINE_CONF_FILE);
    fprintf(stdout, USING_DEFAULT);
    sprintf(err_str, ERR_CONF_FILE, FINE_CONF_FILE);
    log_write("get_3_lrate", err_str);
    log_write("get_3_lrate", USING_DEFAULT);
    log_write("get_3_lrate", DEFAULT_CONF_PARAM);
    return DEFAULT_3_LRATE;
  }
  else return rate;
  
}

int write_norm_file(char* norm_file_name, data_vector* data_matrix, int data_number, int data_dim){
  
  FILE* norm_file;
  register int i,j;
  
  if((norm_file=fopen(norm_file_name, "w"))==NULL){
    fprintf(stderr, ERR_CREATE_FILE, norm_file_name);
    return 0;
  }
  
  else{
    
    fprintf(norm_file, "VECTOR_DIM: %d\n\n", data_dim);
        
    for(i=0;i<data_number;i++){
      for(j=0;j<data_dim;j++) fprintf(norm_file, "%f ", data_matrix[i][j]);
      fprintf(norm_file, "\n");
    }
    
    fclose(norm_file);
    return 1;
  }
  
}

int write_match_line(char* match_file_name, float* data_vector, int bmu, int data_number, map_param* map_info){
  
  static FILE* match_file=NULL;
  register int i;
  
  static int count=0;
  
  if(match_file==NULL){
    
    match_file=fopen(match_file_name, "a");
    if(match_file==NULL){
      fprintf(stderr, ERR_OPEN_FILE, match_file_name);
      return 0;
    }
    
    fprintf(match_file, "MAP_ROW: %d\nMAP_COLUMN: %d\n", map_info->row, map_info->column);
    if(map_info->topo==HEXA)
      fprintf(match_file, "MAP_TOPOLOGY: HEXA\n");
    else
      fprintf(match_file, "MAP_TOPOLOGY: RECT\n");
    fprintf(match_file, "CODEBOOK_DIM: %d\nDATA_NUMBER: %d\n\n", map_info->codebook_dim, data_number);
    
  }
  
  for(i=0;i<map_info->codebook_dim;i++)
    fprintf(match_file, "%f ", data_vector[i]);
  
  fprintf(match_file, "%d ", count);
  
  fprintf(match_file, "%d \n", bmu);
  
  count++;
  if(count==data_number) fclose(match_file);
  
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

void log_write(char* func, char* err){
  
  time_t t;
  FILE* err_log;
  
  err_log=fopen(ERR_LOG_FILE, "a");
  if(err_log==NULL){
    fprintf(stderr, ERR_CREATE_FILE, ERR_LOG_FILE);
    return;
  }
  
  fprintf(err_log, "\n");
  time(&t);
  fprintf(err_log, "%s %s:%s\n", ctime(&t), func, err);
  
  fclose(err_log);
  
}
