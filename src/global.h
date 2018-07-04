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

#include <pthread.h>

match* match_list=NULL;

int data_flag;
pthread_mutex_t data_flag_mutex;
pthread_cond_t data_flag_cv;

int bmu_flag;
pthread_mutex_t bmu_flag_mutex;
pthread_cond_t bmu_flag_cv;

int finish_flag;
pthread_mutex_t finish_flag_mutex;
pthread_cond_t finish_flag_cv;

