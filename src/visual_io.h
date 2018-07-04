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

int get_2_map_files(char* file1_name, char* file2_name);

int get_2_map_param(char* file1_name, char* file2_name, map_param* map_ptr);

int load_codebook(char* map_file_name, void* map_ptr, int map_type);

int ask_image_save();

int ask_red_file_save();

int get_2_image_files(char* file1_name, char* file2_name);

int get_2_red_files(char* file1_name, char* file2_name);

int write_red_file(char* red_file_name, red_unit* map, map_param* map_info);

int write_image_file(char* image_file_name, IplImage *map_img);

int get_map_file(char* file_name);

int get_map_param(char* file_name, map_param* map_ptr);

int get_image_file(char* file_name);


