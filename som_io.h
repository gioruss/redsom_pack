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

int get_param(int* row, int* column, int* unit, int* learning, int* topo, int* init, int* epochs);

int get_data_dim(int* data_dim, char* data_file_name);

int get_data_number(char* data_file_name);

int get_data_min_max(char* data_file_name, float* m_array, int value_to_find);

int get_w_file_name(char* dst_str);

int get_w_line(char* w_file_name, int unit, int dim, float* w_array);

int get_data_line(char* data_file_name, float* in_vect);

int out_file_create(char* out_file_name);

int write_output(char* out_file_name, unit_data* u, int unit);

int get_lrate_type();

int get_neighborhood_func();

float get_1_lrate();

float get_2_lrate();

float get_3_lrate();

int write_norm_file(char* norm_file_name, data_vector* data_matrix, int data_number, int data_dim);

int write_match_line(char* match_file_name, float* data_vector, int bmu, int data_number, map_param* map_info);

int get_map_param(char* file_name, map_param* map_ptr);

void log_write(char* func, char* err);
