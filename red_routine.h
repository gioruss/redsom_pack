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

int map_alloc(red_unit** map_ptr, map_param* map_info);

void map_free(red_unit* map_ptr, map_param* map_info);

void exit_procedure(red_unit* map1_ptr, red_unit* map2_ptr, map_param* map_info, IplImage *map1_img, IplImage *map2_img, int return_code);