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

IplImage* draw_red_map(red_unit* map, map_param* map_info);

IplImage* draw_red_hexa(red_unit* map, map_param* map_info);

IplImage* draw_red_rect(red_unit* map, map_param* map_info);

void draw_density_bar(IplImage* img, int x_dim, int y_dim, float max_abs_re_dens);

IplImage* draw_umatrix_map(dist_unit* map, map_param* map_info);

IplImage* draw_umatrix_hexa(dist_unit* map, map_param* map_info);

IplImage* draw_umatrix_rect(dist_unit* map, map_param* map_info);

void draw_distance_bar(IplImage* img, int x_dim, int y_dim, float max_neighbor_dist, float min_neighbor_dist);
