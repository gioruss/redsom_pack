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

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "common.h"
#include "math_function.h"
#include "drawing.h"

IplImage* draw_red_map(red_unit* map, map_param* map_info){
  
  if(map_info->topo==HEXA) return draw_red_hexa(map, map_info);
  else return draw_red_rect(map, map_info);

}

IplImage* draw_red_hexa(red_unit* map, map_param* map_info){
  
  IplImage* img=NULL;
  
  register int i;
  
  const int num_point=HEXAPOINT;
  
  CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
  CvPoint point_array[HEXAPOINT];
  CvPoint* hexagon;
  
  CvFont font;
  double font_horiz_scale = 0.3;
  double font_vert_scale = 0.3;
  int font_line_width = 1;
  char num_str[STRLENGHT];
    
  float color_scale;
  
  float location[2];
  float center[2];
  
  float max_re_dens, min_re_dens, max_abs_re_dens;
  
  int offset=UNIT_DIST/sqrt(3)+BORDER_DIM;
  int x_dim=(map_info->column-0.5)*UNIT_DIST+offset*2;
  int y_dim=(map_info->row-1)*sqrt(3)/2*UNIT_DIST+offset*2+DENSITY_BAR_Y_DIM+BORDER_DIM;
  
  max_re_dens=max_re_density(map, map_info);
  min_re_dens=min_re_density(map, map_info);
  
  img = cvCreateImage(cvSize(x_dim, y_dim), IPL_DEPTH_8U, 3);
  if(img==NULL){
    fprintf(stderr, ERR_ALLOC_IMG);
    return NULL;
  }
  cvSet(img, BACKGROUND_COLOR, NULL);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);
  
  max_abs_re_dens=fabs(max_re_dens);
  if(fabs(min_re_dens)>max_abs_re_dens) max_abs_re_dens=fabs(min_re_dens);
    
  draw_density_bar(img, x_dim, y_dim, max_abs_re_dens);
  
  for(i=0;i<map_info->unit_num;i++){
    
    find_location(i, HEXA, map_info->row, map_info->column, location);
    
    center[0]=location[0]*UNIT_DIST+offset;
    center[1]=location[1]*UNIT_DIST+offset;
    
    pt1.x=center[0];
    pt1.y=center[1]-UNIT_DIST/sqrt(3);
    pt2.x=center[0]-UNIT_DIST/2;
    pt2.y=center[1]-UNIT_DIST/sqrt(3)/2;
    pt3.x=center[0]-UNIT_DIST/2;
    pt3.y=center[1]+UNIT_DIST/sqrt(3)/2;
    pt4.x=center[0];
    pt4.y=center[1]+UNIT_DIST/sqrt(3);
    pt5.x=center[0]+UNIT_DIST/2;
    pt5.y=center[1]+UNIT_DIST/sqrt(3)/2;
    pt6.x=center[0]+UNIT_DIST/2;
    pt6.y=center[1]-UNIT_DIST/sqrt(3)/2;
    
    point_array[0]=pt1; 
    point_array[1]=pt2; 
    point_array[2]=pt3;
    point_array[3]=pt4; 
    point_array[4]=pt5;
    point_array[5]=pt6;
    
    hexagon=point_array;
    
    if(map[i].re_dens>0){
      color_scale=(map[i].re_dens/max_abs_re_dens)*255;
      cvFillConvexPoly(img, hexagon, num_point, REDENS_POSITIVE_COLOR, CV_AA, 0);
    }
    else if(map[i].re_dens<0){
      color_scale=(fabs(map[i].re_dens)/max_abs_re_dens)*255;
      cvFillConvexPoly(img, hexagon, num_point, REDENS_NEGATIVE_COLOR, CV_AA, 0);
    }
    else if(map[i].re_dens==0)
      cvFillConvexPoly(img, hexagon, num_point, REDENS_ZERO_COLOR, CV_AA, 0);
    
    cvPolyLine(img, &hexagon, &num_point, 1, 1, LINE_COLOR, 2, CV_AA, 0);
    
    memset(num_str, 0, sizeof(char)*STRLENGHT);
    sprintf(num_str, "%d", i);
    cvPutText(img, num_str, cvPoint(center[0]-UNIT_DIST/2+3, center[1]+3), &font, LINE_COLOR);
        
  }
    
  return img;

}

IplImage* draw_red_rect(red_unit* map, map_param* map_info){
  
  IplImage* img=NULL;
  
  register int i;
  
  const int num_point=SQUAREPOINT;
  
  CvPoint pt1, pt2, pt3, pt4;
  CvPoint point_array[SQUAREPOINT];
  CvPoint* square;
  
  CvFont font;
  double font_horiz_scale = 0.3;
  double font_vert_scale = 0.3;
  int font_line_width = 1;
  char num_str[STRLENGHT];
  
  float color_scale;
  
  float location[2];
  float center[2];
  
  float max_re_dens, min_re_dens, max_abs_re_dens;
  
  int offset=UNIT_DIST/2+BORDER_DIM;
  int x_dim=(map_info->column-1)*UNIT_DIST+offset*2;
  int y_dim=(map_info->row-1)*UNIT_DIST+offset*2+DENSITY_BAR_Y_DIM+BORDER_DIM;
  
  max_re_dens=max_re_density(map, map_info);
  min_re_dens=min_re_density(map, map_info);
  
  img = cvCreateImage(cvSize(x_dim, y_dim), IPL_DEPTH_8U, 3);
  if(img==NULL){
    fprintf(stderr, ERR_ALLOC_IMG);
    return NULL;
  }
  cvSet(img, BACKGROUND_COLOR, NULL);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);
  
  max_abs_re_dens=fabs(max_re_dens);
  if(fabs(min_re_dens)>max_abs_re_dens) max_abs_re_dens=fabs(min_re_dens);
    
  draw_density_bar(img, x_dim, y_dim, max_abs_re_dens);
  
  for(i=0;i<map_info->unit_num;i++){
    
    find_location(i, RECT, map_info->row, map_info->column, location);
    
    center[0]=location[0]*UNIT_DIST+offset;
    center[1]=location[1]*UNIT_DIST+offset;
    
    pt1.x=center[0]-UNIT_DIST/2;
    pt1.y=center[1]-UNIT_DIST/2;
    pt2.x=center[0]+UNIT_DIST/2;
    pt2.y=center[1]-UNIT_DIST/2;
    pt3.x=center[0]+UNIT_DIST/2;
    pt3.y=center[1]+UNIT_DIST/2;
    pt4.x=center[0]-UNIT_DIST/2;
    pt4.y=center[1]+UNIT_DIST/2;
    
    point_array[0]=pt1; 
    point_array[1]=pt2; 
    point_array[2]=pt3;
    point_array[3]=pt4; 
     
    square=point_array;
    
    if(map[i].re_dens>0){
      color_scale=(map[i].re_dens/max_abs_re_dens)*255;
      cvFillConvexPoly(img, square, num_point, REDENS_POSITIVE_COLOR, CV_AA, 0);
    }
    else if(map[i].re_dens<0){
      color_scale=(fabs(map[i].re_dens)/max_abs_re_dens)*255;
      cvFillConvexPoly(img, square, num_point, REDENS_NEGATIVE_COLOR, CV_AA, 0);
    }
    else if(map[i].re_dens==0)
      cvFillConvexPoly(img, square, num_point, REDENS_ZERO_COLOR, CV_AA, 0);
    
    cvPolyLine(img, &square, &num_point, 1, 1, LINE_COLOR, 2, CV_AA, 0);
    
    memset(num_str, 0, sizeof(char)*STRLENGHT);
    sprintf(num_str, "%d", i);
    cvPutText(img, num_str, cvPoint(center[0]-UNIT_DIST/2+3, center[1]+3), &font, LINE_COLOR);
        
  }
  
  return img;

}

void draw_density_bar(IplImage* img, int x_dim, int y_dim, float max_abs_re_dens){
  
  register int i;
  
  float color_scale;
  
  CvPoint pt1, pt2;
  
  CvFont font;
  double font_horiz_scale = 0.5;
  double font_vert_scale = 0.5;
  int font_line_width = 1;
  char num_str[STRLENGHT];
  
  memset(num_str, 0, sizeof(char)*STRLENGHT);
  
  pt1.y=y_dim-BORDER_DIM-DENSITY_BAR_Y_DIM;
  pt2.y=y_dim-BORDER_DIM;
  
  pt1.x=pt2.x=BORDER_DIM+1;
  
  for( i=x_dim/2-BORDER_DIM ; pt1.x<x_dim/2+1 ; pt1.x++, pt2.x++, i-- ){
    
    color_scale=((float)i/((float)x_dim/2-BORDER_DIM))*255;
    cvLine(img, pt1, pt2, REDENS_NEGATIVE_COLOR, 1, CV_AA, 0);
  
  }
  
  pt1.x=pt2.x=x_dim-BORDER_DIM;
  
  for( i=x_dim/2-BORDER_DIM ; pt1.x>x_dim/2 ; pt1.x--, pt2.x--, i-- ){
    
    color_scale=((float)i/((float)x_dim/2-BORDER_DIM))*255;
    cvLine(img, pt1, pt2, REDENS_POSITIVE_COLOR, 1, CV_AA, 0);
  
  }
  
  pt1.x=BORDER_DIM+1;
  pt2.x=x_dim-BORDER_DIM;
  pt1.y=y_dim-BORDER_DIM-DENSITY_BAR_Y_DIM;
  pt2.y=y_dim-BORDER_DIM;
  
  cvRectangle(img, pt1, pt2, LINE_COLOR, 2, CV_AA, 0);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);
  
  sprintf(num_str, "%.2f", -max_abs_re_dens);
  cvPutText(img, num_str, cvPoint(BORDER_DIM+4, y_dim-BORDER_DIM-DENSITY_BAR_Y_DIM/2+4), &font, LINE_COLOR);
  
  sprintf(num_str, "%.2f", max_abs_re_dens);
  cvPutText(img, num_str, cvPoint(x_dim-BORDER_DIM-40, y_dim-BORDER_DIM-DENSITY_BAR_Y_DIM/2+4), &font, LINE_COLOR);
  
  sprintf(num_str, "%.2f", (float)0);
  cvPutText(img, num_str, cvPoint(x_dim/2-25, y_dim-BORDER_DIM-DENSITY_BAR_Y_DIM/2+4), &font, LINE_COLOR);
  
}

IplImage* draw_umatrix_map(dist_unit* map, map_param* map_info){
  
  if(map_info->topo==HEXA) return draw_umatrix_hexa(map, map_info);
  else return draw_umatrix_rect(map, map_info);

}

IplImage* draw_umatrix_hexa(dist_unit* map, map_param* map_info){
  
  IplImage* img=NULL;
  
  register int i, j, k;
  float sum;
  int dist_num;
  
  const int num_point=HEXAPOINT;
  
  CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
  CvPoint point_array[HEXAPOINT];
  CvPoint triangle[3];
  CvPoint* hexagon;
  
  CvFont font;
  double font_horiz_scale = 0.3;
  double font_vert_scale = 0.3;
  int font_line_width = 1;
  char num_str[STRLENGHT];
    
  float color_scale;
  
  float location[2];
  float center[2];
  
  float max_neighbor_dist, min_neighbor_dist;
  
  int offset=UNIT_DIST/sqrt(3)+BORDER_DIM;
  int x_dim=(map_info->column-0.5)*UNIT_DIST+offset*2;
  int y_dim=(map_info->row-1)*sqrt(3)/2*UNIT_DIST+offset*2+DISTANCE_BAR_Y_DIM+BORDER_DIM;
  
  max_neighbor_dist=max_neighbor_distance(map, map_info);
  min_neighbor_dist=min_neighbor_distance(map, map_info);
  
  img = cvCreateImage(cvSize(x_dim, y_dim), IPL_DEPTH_8U, 3);
  if(img==NULL){
    fprintf(stderr, ERR_ALLOC_IMG);
    return NULL;
  }
  cvSet(img, BACKGROUND_COLOR, NULL);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);

  draw_distance_bar(img, x_dim, y_dim, max_neighbor_dist, min_neighbor_dist);
  
  for(i=0;i<map_info->unit_num;i++){
    
    find_location(i, HEXA, map_info->row, map_info->column, location);
    
    center[0]=location[0]*UNIT_DIST+offset;
    center[1]=location[1]*UNIT_DIST+offset;
    
    pt1.x=center[0];
    pt1.y=center[1]-UNIT_DIST/sqrt(3);
    pt2.x=center[0]-UNIT_DIST/2;
    pt2.y=center[1]-UNIT_DIST/sqrt(3)/2;
    pt3.x=center[0]-UNIT_DIST/2;
    pt3.y=center[1]+UNIT_DIST/sqrt(3)/2;
    pt4.x=center[0];
    pt4.y=center[1]+UNIT_DIST/sqrt(3);
    pt5.x=center[0]+UNIT_DIST/2;
    pt5.y=center[1]+UNIT_DIST/sqrt(3)/2;
    pt6.x=center[0]+UNIT_DIST/2;
    pt6.y=center[1]-UNIT_DIST/sqrt(3)/2;
    
    point_array[0]=pt1; 
    point_array[1]=pt2; 
    point_array[2]=pt3;
    point_array[3]=pt4; 
    point_array[4]=pt5;
    point_array[5]=pt6;
    
    hexagon=point_array;

    for(j=0;j<num_point;j++){
      
      triangle[0]=point_array[j];
      if(j==num_point-1)
	triangle[1]=point_array[0];
      else
	triangle[1]=point_array[j+1];
      triangle[2]=cvPoint(center[0], center[1]);
      
      if(map[i].neighbor_dist[j]!=NOT_DISTANCE){
	color_scale=((max_neighbor_dist-map[i].neighbor_dist[j]) / (max_neighbor_dist-min_neighbor_dist))*255;
      }
      else{
	for(k=0, sum=0, dist_num=0;k<num_point;k++){
	  if(map[i].neighbor_dist[k]!=NOT_DISTANCE){
	    sum+=map[i].neighbor_dist[k];
	    dist_num++;
	  }
	}
	color_scale=((max_neighbor_dist-sum/dist_num) / (max_neighbor_dist-min_neighbor_dist))*255;
      }
      
      cvFillConvexPoly(img, triangle, 3, UMATRIX_COLOR, CV_AA, 0);
    }
    
    cvPolyLine(img, &hexagon, &num_point, 1, 1, BLACK_COLOR, 1, CV_AA, 0);
    
    memset(num_str, 0, sizeof(char)*STRLENGHT);
    sprintf(num_str, "%d", i);
    cvPutText(img, num_str, cvPoint(center[0]-UNIT_DIST/2+3, center[1]+3), &font, BLACK_COLOR);
        
  }
    
  return img;

}

IplImage* draw_umatrix_rect(dist_unit* map, map_param* map_info){
  
  IplImage* img=NULL;
  
  register int i, j, k;
  float sum;
  int dist_num;
  
  const int num_point=SQUAREPOINT;
  
  CvPoint pt1, pt2, pt3, pt4;
  CvPoint point_array[SQUAREPOINT];
  CvPoint* square;
  CvPoint triangle[3];
  
  CvFont font;
  double font_horiz_scale = 0.3;
  double font_vert_scale = 0.3;
  int font_line_width = 1;
  char num_str[STRLENGHT];
  
  float color_scale;
  
  float location[2];
  float center[2];
  
  float max_neighbor_dist, min_neighbor_dist;
  
  int offset=UNIT_DIST/2+BORDER_DIM;
  int x_dim=(map_info->column-1)*UNIT_DIST+offset*2;
  int y_dim=(map_info->row-1)*UNIT_DIST+offset*2+DISTANCE_BAR_Y_DIM+BORDER_DIM;
  
  max_neighbor_dist=max_neighbor_distance(map, map_info);
  min_neighbor_dist=min_neighbor_distance(map, map_info);
  
  img = cvCreateImage(cvSize(x_dim, y_dim), IPL_DEPTH_8U, 3);
  if(img==NULL){
    fprintf(stderr, ERR_ALLOC_IMG);
    return NULL;
  }
  cvSet(img, BACKGROUND_COLOR, NULL);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);
  
  draw_distance_bar(img, x_dim, y_dim, max_neighbor_dist, min_neighbor_dist);
  
  for(i=0;i<map_info->unit_num;i++){
    
    find_location(i, RECT, map_info->row, map_info->column, location);
    
    center[0]=location[0]*UNIT_DIST+offset;
    center[1]=location[1]*UNIT_DIST+offset;
    
    pt1.x=center[0]-UNIT_DIST/2;
    pt1.y=center[1]-UNIT_DIST/2;
    pt2.x=center[0]+UNIT_DIST/2;
    pt2.y=center[1]-UNIT_DIST/2;
    pt3.x=center[0]+UNIT_DIST/2;
    pt3.y=center[1]+UNIT_DIST/2;
    pt4.x=center[0]-UNIT_DIST/2;
    pt4.y=center[1]+UNIT_DIST/2;
    
    point_array[0]=pt1; 
    point_array[1]=pt2; 
    point_array[2]=pt3;
    point_array[3]=pt4; 
     
    square=point_array;
    
    for(j=0;j<num_point;j++){
      
      triangle[0]=point_array[j];
      if(j==num_point-1)
	triangle[1]=point_array[0];
      else
	triangle[1]=point_array[j+1];
      triangle[2]=cvPoint(center[0], center[1]);
      
      if(map[i].neighbor_dist[j]!=NOT_DISTANCE){
	color_scale=((max_neighbor_dist-map[i].neighbor_dist[j]) / (max_neighbor_dist-min_neighbor_dist))*255;
      }
      else{
	for(k=0, sum=0, dist_num=0;k<num_point;k++){
	  if(map[i].neighbor_dist[k]!=NOT_DISTANCE){
	    sum+=map[i].neighbor_dist[k];
	    dist_num++;
	  }
	}
	color_scale=((max_neighbor_dist-sum/dist_num) / (max_neighbor_dist-min_neighbor_dist))*255;
      }
      
      cvFillConvexPoly(img, triangle, 3, UMATRIX_COLOR, CV_AA, 0);
    }
    
    cvPolyLine(img, &square, &num_point, 1, 1, BLACK_COLOR, 1, CV_AA, 0);
    
    memset(num_str, 0, sizeof(char)*STRLENGHT);
    sprintf(num_str, "%d", i);
    cvPutText(img, num_str, cvPoint(center[0]-UNIT_DIST/2+3, center[1]+3), &font, BLACK_COLOR);
        
  }
  
  return img;

}

void draw_distance_bar(IplImage* img, int x_dim, int y_dim, float max_neighbor_dist, float min_neighbor_dist){
  
  register int i;
  
  float color_scale;
  
  CvPoint pt1, pt2;
  
  CvFont font;
  double font_horiz_scale = 0.5;
  double font_vert_scale = 0.5;
  int font_line_width = 1;
  char num_str[STRLENGHT];
  
  memset(num_str, 0, sizeof(char)*STRLENGHT);
  
  pt1.y=y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM;
  pt2.y=y_dim-BORDER_DIM;
  
  pt1.x=pt2.x=BORDER_DIM+1;
  
  for( i=x_dim-BORDER_DIM ; pt1.x<x_dim-BORDER_DIM ; pt1.x++, pt2.x++, i-- ){
    
    color_scale=((float)i/((float)x_dim-(BORDER_DIM*2)))*255;
    cvLine(img, pt1, pt2, UMATRIX_COLOR, 1, CV_AA, 0);
  
  }
  
  pt1.x=BORDER_DIM+1;
  pt2.x=x_dim-BORDER_DIM;
  pt1.y=y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM;
  pt2.y=y_dim-BORDER_DIM;
  
  cvRectangle(img, pt1, pt2, BLUE_COLOR, 2, CV_AA, 0);
  
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, font_horiz_scale, font_vert_scale, 0, font_line_width, CV_AA);
  
  sprintf(num_str, "%.2f", min_neighbor_dist);
  cvPutText(img, num_str, cvPoint(BORDER_DIM+4, y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM/2+4), &font, BLACK_COLOR);
  
  if(max_neighbor_dist<10){
    sprintf(num_str, "%.2f", max_neighbor_dist);
    cvPutText(img, num_str, cvPoint(x_dim-BORDER_DIM-40, y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM/2+4), &font, WHITE_COLOR);
  }
  else if(max_neighbor_dist<100){
    sprintf(num_str, "%.2f", max_neighbor_dist);
    cvPutText(img, num_str, cvPoint(x_dim-BORDER_DIM-50, y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM/2+4), &font, WHITE_COLOR);
  }
  else if(max_neighbor_dist<1000){
    sprintf(num_str, "%.2f", max_neighbor_dist);
    cvPutText(img, num_str, cvPoint(x_dim-BORDER_DIM-60, y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM/2+4), &font, WHITE_COLOR);
  }
  else{
    sprintf(num_str, "%.2fK", max_neighbor_dist/1000);
    cvPutText(img, num_str, cvPoint(x_dim-BORDER_DIM-60, y_dim-BORDER_DIM-DISTANCE_BAR_Y_DIM/2+4), &font, WHITE_COLOR);
  }
  
}
