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

int map_alloc(dist_unit** map_ptr, map_param* map_info);

void map_free(dist_unit* map_ptr, map_param* map_info);

void exit_procedure(dist_unit* map_ptr, map_param* map_info, IplImage *map_img, int return_code);
