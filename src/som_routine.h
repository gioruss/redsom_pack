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

void exit_procedure(unit_data* u_dat_array, match* match_l, pthread_t* pthread_array, float* in_vector, int unit, int data_number, int return_code);

int u_dat_array_alloc(unit_data** u_dat_array, int unit);

int pthread_array_alloc(pthread_t** pthread_array, int unit);

int in_vector_alloc(float** in_vector, int data_dim);
  
int w_alloc(unit_data* u_dat_ptr);

int in_alloc(unit_data* u_dat_ptr);

int aux_alloc(unit_data* u_dat_ptr);

int match_alloc(match** match_l, int data_number, int data_dim);

int w_from_file_init(unit_data* u_dat_ptr);

int w_rand_init(unit_data* u_dat_ptr, char* data_file_name);

int u_dat_struct_init(unit_data* u_d_ptr, int row, int column, int topo, int learning, int data_dim, int id, int epochs, int init, char* data_file_name);

