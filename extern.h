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

extern match* match_list;

extern int data_flag;
extern pthread_mutex_t data_flag_mutex;
extern pthread_cond_t data_flag_cv;

extern int bmu_flag;
extern pthread_mutex_t bmu_flag_mutex;
extern pthread_cond_t bmu_flag_cv;

extern int finish_flag;
extern pthread_mutex_t finish_flag_mutex;
extern pthread_cond_t finish_flag_cv;
