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

void init_global_flags();

void send_broadcast(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv);

void send_signal(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv);

void wait_flag(int* flag, int flag_value, pthread_mutex_t* flag_mutex, pthread_cond_t* flag_cv);
