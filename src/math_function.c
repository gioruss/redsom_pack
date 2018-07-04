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
#include "math_function.h"

float euclidean_dist(float* vector1, float* vector2, int dim){
  
  register int i;
  float sum=0;
  
  for(i=0;i<dim;i++)
    sum = sum + ((vector1[i]-vector2[i])*(vector1[i]-vector2[i]));
    
  return ((float)sqrt(sum));
  
}

float steps_l_rate(int step, int epochs, float l1r, float l2r, float l3r){

  if((float)step<(float)epochs/3) return l1r;
  else if((float)step<((float)epochs*2)/3) return l2r;
  else return l3r;  

}

float time_inv_l_rate(int step, int epochs){
  
  float time = (float)step/(float)epochs;
  
  return 0.7/((80*time) + 1);
  
}

float l_rate(int step, int epochs, int type, float l1r, float l2r, float l3r){
    
  if(type==STEPS) return steps_l_rate(step, epochs, l1r, l2r, l3r);
  
  else if(type==TIME_INVERSE) return time_inv_l_rate(step, epochs);
  
  return 0;
  
}

float sigma(int step, int epochs, int map_row, int map_column){
  
  float start_neighborhood, return_neighborhood;
  
  if(map_row<=map_column)
    start_neighborhood = (float)map_row/2;
  else
    start_neighborhood = (float)map_column/2;
  
  if((float)(step)<=((float)(epochs))/4)
    return_neighborhood = start_neighborhood;
  else if((float)(step)<=((float)(epochs))/2)
    return_neighborhood = (start_neighborhood*3)/4;
  else if((float)(step)<=(((float)(epochs))*3)/4)
    return_neighborhood = start_neighborhood/2;
  else
    return_neighborhood = start_neighborhood/4;
  
  if(return_neighborhood<NEIGHBOR_2D_DIST)
    return NEIGHBOR_2D_DIST;
  else
    return return_neighborhood;
  
}

void find_location(int unit, int map_topo, int map_row, int map_column, float* location){
 
  if(map_topo==RECT){
    location[1]=(unit/map_column)*NEIGHBOR_2D_DIST;
    location[0]=(unit%map_column)*NEIGHBOR_2D_DIST;
  }
  
  else if(map_topo==HEXA){
    
    int row, column;
    
    row=unit/map_column;
    column=unit%map_column;
        
    if(row%2==0){
      location[1]=(row*(sqrt(3)/2))*NEIGHBOR_2D_DIST;
      location[0]=((float)column)*NEIGHBOR_2D_DIST;
    }
    else{
      location[1]=(row*(sqrt(3)/2))*NEIGHBOR_2D_DIST;
      location[0]=((float)column+0.5)*NEIGHBOR_2D_DIST;
    }
    
  }
  
}

float gaussian_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column){
  
  float bmu_location[2];
  float current_unit_location[2];
  float s, dist;
  
  if(bmu==current_unit) return 1;
  if((s=sigma(step, epochs, map_row, map_column))==0) return 0;
  
  find_location(current_unit, map_topo, map_row, map_column, current_unit_location);
  find_location(bmu, map_topo, map_row, map_column, bmu_location);
  
  dist=euclidean_dist(bmu_location, current_unit_location, 2);
  
  return exp( - ((dist*dist) / (2*s*s)) );
  
}

float bubble_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column){

  float bmu_location[2];
  float current_unit_location[2];
  float s, dist;
  
  if(bmu==current_unit) return 1;
  if((s=sigma(step, epochs, map_row, map_column))==0) return 0;
  
  find_location(current_unit, map_topo, map_row, map_column, current_unit_location);
  find_location(bmu, map_topo, map_row, map_column, bmu_location);
  
  dist=euclidean_dist(bmu_location, current_unit_location, 2);
  
  if(dist>s+NEIGHBOR_2D_DIST_ERROR) return 0;
  else return 1;
  
}

float cut_gaussian_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column){
  
  return
  bubble_func(bmu, current_unit, map_topo, step, epochs, map_row, map_column)*
  gaussian_func(bmu, current_unit, map_topo, step, epochs, map_row, map_column);
  
}

float neighborhood_func(int bmu, int current_unit, int map_topo, int step, int epochs, int map_row, int map_column, int type){
  
  if(type==GAUSSIAN)
    return gaussian_func(bmu, current_unit, map_topo, step, epochs, map_row, map_column);
  
  else if(type==BUBBLE)
    return bubble_func(bmu, current_unit, map_topo, step, epochs, map_row, map_column);
  
  else if(type==CUT_GAUSSIAN)
    return cut_gaussian_func(bmu, current_unit, map_topo, step, epochs, map_row, map_column);

  return 0;
  
}

float third_quartile(red_unit* map, map_param* map_info){
  
  register int i, j, dist_count=0;
  int neighbor_num;
  float unit_location[2], remote_location[2];
  float* dist_array=NULL;
  float th_quart;
  
  if(map_info->topo==HEXA) neighbor_num=HEXAPOINT;
  else neighbor_num=SQUAREPOINT;
  
  dist_array=malloc(sizeof(float)*(map_info->unit_num)*neighbor_num);
  if(dist_array==NULL){
    fprintf(stderr, ERR_ALLOC);
    return -1;
  }
  memset(dist_array, 0, sizeof(float)*(map_info->unit_num)*neighbor_num);
  
  for(i=0;i<(map_info->unit_num);i++){
    
    find_location(map[i].unit_id, map_info->topo, map_info->row, map_info->column, unit_location);
    
    for(j=0;j<(map_info->unit_num);j++){
      
      find_location(map[j].unit_id, map_info->topo, map_info->row, map_info->column, remote_location);
      
      if(euclidean_dist(unit_location, remote_location, 2)<=NEIGHBOR_2D_DIST+NEIGHBOR_2D_DIST_ERROR && map[i].unit_id!=map[j].unit_id){
	dist_array[dist_count]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	dist_count++;
      }
    }
  }
  
  qsort((void*)dist_array, dist_count, sizeof(float), float_cmp);
  
  th_quart =
  dist_array[(int)(floor((dist_count+1)*((float)3/4))) - 1] +
  ( dist_array[(int)(ceil((dist_count+1)*((float)3/4))) -1] - dist_array[(int)(floor((dist_count+1)*((float)3/4))) - 1] ) *
  ( ((dist_count+1)*((float)3/4)) - floor((dist_count+1)*((float)3/4)) );
  
  free(dist_array);
  
  return th_quart;
  
}

void neighbor_dist_calc(dist_unit* map, map_param* map_info){
  
  register int i, j;
  float unit_location[2], remote_location[2];
    
  for(i=0;i<(map_info->unit_num);i++){
    
    find_location(map[i].unit_id, map_info->topo, map_info->row, map_info->column, unit_location);
    
    for(j=0;j<6;j++) map[i].neighbor_dist[j]=NOT_DISTANCE;
    
    for(j=0;j<(map_info->unit_num);j++){
      
      find_location(map[j].unit_id, map_info->topo, map_info->row, map_info->column, remote_location);
      
      if(euclidean_dist(unit_location, remote_location, 2)<=NEIGHBOR_2D_DIST+NEIGHBOR_2D_DIST_ERROR && map[i].unit_id!=map[j].unit_id){
	
	if(map_info->topo==HEXA){
	  if(remote_location[0]<unit_location[0] && remote_location[1]<unit_location[1])
	    map[i].neighbor_dist[0]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(map[i].unit_id==map[j].unit_id+1)
	    map[i].neighbor_dist[1]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(remote_location[0]<unit_location[0] && remote_location[1]>unit_location[1])
	    map[i].neighbor_dist[2]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(remote_location[0]>unit_location[0] && remote_location[1]>unit_location[1])
	    map[i].neighbor_dist[3]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(map[i].unit_id==map[j].unit_id-1)
	    map[i].neighbor_dist[4]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(remote_location[0]>unit_location[0] && remote_location[1]<unit_location[1])
	    map[i].neighbor_dist[5]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	}
	else{
	  if(remote_location[1]<unit_location[1])
	    map[i].neighbor_dist[0]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(map[i].unit_id==map[j].unit_id-1)
	    map[i].neighbor_dist[1]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(remote_location[1]>unit_location[1])
	    map[i].neighbor_dist[2]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	  else if(map[i].unit_id==map[j].unit_id+1)
	    map[i].neighbor_dist[3]=euclidean_dist(map[i].codebook, map[j].codebook, map_info->codebook_dim);
	}
      }
    }
    
  }
  
}

int float_cmp(const void* elem1, const void* elem2){
  
  if( *((float*)elem1) < *((float*)elem2) ) return -1;
  else if( *((float*)elem1) == *((float*)elem2) ) return 0;
  else return 1;
  
}

float float_average(float n1, float n2){
  
  return (n1+n2)/2;
  
}

int density_calc(red_unit* map1, red_unit* map2, map_param* map_info){
  
  register int i, j;
  float radius, th_quart1, th_quart2;
  
  if((th_quart1=third_quartile(map1, map_info))==-1) return 0;
  if((th_quart2=third_quartile(map2, map_info))==-1) return 0;
  radius=float_average(th_quart1, th_quart2);
  
  for(i=0;i<map_info->unit_num;i++)
    for(j=0;j<map_info->unit_num;j++)
      map1[i].dens_on_map1 += exp(-(euclidean_dist(map1[i].codebook, map1[j].codebook, map_info->codebook_dim)/(2*radius)));
      
  for(i=0;i<map_info->unit_num;i++)
    for(j=0;j<map_info->unit_num;j++)
      map1[i].dens_on_map2 += exp(-(euclidean_dist(map1[i].codebook, map2[j].codebook, map_info->codebook_dim)/(2*radius)));
      
  for(i=0;i<map_info->unit_num;i++)
    for(j=0;j<map_info->unit_num;j++)
      map2[i].dens_on_map1 += exp(-(euclidean_dist(map2[i].codebook, map1[j].codebook, map_info->codebook_dim)/(2*radius)));
      
  for(i=0;i<map_info->unit_num;i++)
    for(j=0;j<map_info->unit_num;j++)
      map2[i].dens_on_map2 += exp(-(euclidean_dist(map2[i].codebook, map2[j].codebook, map_info->codebook_dim)/(2*radius)));
      
  return 1;
    
}

float log_2base(float x){
  
  return log(x)/log(2);
  
}

void re_density_calc(red_unit* map, map_param* map_info){
  
  register int i;
  
  for(i=0;i<map_info->unit_num;i++)
    map[i].re_dens = log_2base( map[i].dens_on_map2 / map[i].dens_on_map1 );
  
}

float max_re_density(red_unit* map, map_param* map_info){
  
  register int i;
  float max=0;
  
  for(i=0;i<map_info->unit_num;i++)
    if(map[i].re_dens>max || i==0)
      max=map[i].re_dens;
    
  return max;
  
}

float max_neighbor_distance(dist_unit* map, map_param* map_info){
  
  register int i, j;
  float max=0;
  int max_flag=0;
  
  for(i=0;i<map_info->unit_num;i++){
    for(j=0;j<6;j++){
      if((map[i].neighbor_dist[j]>max && map[i].neighbor_dist[j]!=NOT_DISTANCE) || (max_flag==0 && map[i].neighbor_dist[j]!=NOT_DISTANCE)){
	max_flag=1;
	max=map[i].neighbor_dist[j];
      }
    }
  }
  
  return max;
  
}

float min_re_density(red_unit* map, map_param* map_info){
  
  register int i;
  float min=0;
  
  for(i=0;i<map_info->unit_num;i++)
    if(map[i].re_dens<min || i==0)
      min=map[i].re_dens;
    
  return min;
  
}

float min_neighbor_distance(dist_unit* map, map_param* map_info){
  
  register int i, j;
  float min=0;
  int min_flag=0;
  
  for(i=0;i<map_info->unit_num;i++){
    for(j=0;j<6;j++){
      if((map[i].neighbor_dist[j]<min && map[i].neighbor_dist[j]!=NOT_DISTANCE) || (min_flag==0 && map[i].neighbor_dist[j]!=NOT_DISTANCE)){
	min_flag=1;
	min=map[i].neighbor_dist[j];
      }
    }
  }
  
  return min;
  
}

void find_mean(data_vector* data_matrix, int data_number, int data_dim, data_vector mean_vector){
  
  float sum=0;
  register int i, j;
  
  for(i=0;i<data_dim;i++){
    
    for(sum=0, j=0;j<data_number;j++) sum+=data_matrix[j][i];
    
    mean_vector[i]=sum/data_number;
  
  }
  
}

void find_std_dev(data_vector* data_matrix, int data_number, int data_dim, data_vector std_dev_vector){
  
  float sum=0, mean=0, quad_diff_summ=0;
  register int i, j;
  
  for(i=0;i<data_dim;i++){
    
    for(sum=0, j=0;j<data_number;j++) sum+=data_matrix[j][i];
    
    mean=sum/data_number;
    
    for(quad_diff_summ=0, j=0;j<data_number;j++) quad_diff_summ+=(data_matrix[j][i]-mean)*(data_matrix[j][i]-mean);
    
    std_dev_vector[i]=sqrt(quad_diff_summ/data_number);
  }
  
}
