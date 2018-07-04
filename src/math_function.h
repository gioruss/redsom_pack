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

float euclidean_dist(float* vector1, float* vector2, int dim);

float steps_l_rate(int step, int epochs, float l1r, float l2r, float l3r);

float time_inv_l_rate(int step, int epochs);

float l_rate(int step, int epochs, int type, float l1r, float l2r, float l3r);

float sigma(int step, int epochs, int map_row, int map_column);

void find_location(int unit, int map_topo, int map_row, int map_column, float* location);

float gaussian_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column);

float bubble_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column);

float cut_gaussian_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column);

float neighborhood_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column, int type);

float third_quartile(red_unit* map, map_param* map_info);

int float_cmp(const void* elem1, const void* elem2);

float float_average(float n1, float n2);

int density_calc(red_unit* map1, red_unit* map2, map_param* map_info);

void neighbor_dist_calc(dist_unit* map, map_param* map_info);

float log_2base(float x);

void re_density_calc(red_unit* map, map_param* map_info);

float max_re_density(red_unit* map, map_param* map_info);

float max_neighbor_distance(dist_unit* map, map_param* map_info);

float min_re_density(red_unit* map, map_param* map_info);

float min_neighbor_distance(dist_unit* map, map_param* map_info);

void find_mean(data_vector* data_matrix, int data_number, int data_dim, data_vector mean_vector);

void find_std_dev(data_vector* data_matrix, int data_number, int data_dim, data_vector std_dev_vector);
