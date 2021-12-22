#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "cube.h"





/*camera in the scene*/
/* Holds image in byte array of h by w.*/


class camera{


       float *v;
       float *h1;
       float *h2;
       float *q;
       float *depthScreen;

public:
       BYTE *screen;
       int width;
       int height;

       camera(int height_in, int width_in);
       ~camera();
       
       /* Sets the perpective of the window into the graphical region                      */
       void setlens(float *v_in);

       /* Draws the xyz axises                                                             */
       void drawAxis();

       /* clears the screen                                                                */
       void clear_screen();

       /*                                                                                  */
       void draw_cube(cube c);





       /* Gives the w/x, h/y cords of screen given a vector on the plane of the screen     */
       void screen_cords(float *w, float *depth, float *cords);

       /* Gives the point on the screen from a vector                                      */
       /*     Returns false if outside the screen. True otherwise                          */
       bool point_from_vector(float *w, float *ans, float *depth);

       bool point_from_vector_other(float *w, float *ans, float *depth);

       /* Gives the position on the screen from a vector/point                             */
       bool cord_from_vector(float *w, float *cords);

       /* Returns true iff cords is withing range of 0-width, 0-height;                    */
       static bool in_range(float *cords, int height, int width);

       /* Paints a line from cord_i to cord_f with clor clr and size size*/
       void paint_line(float *cord_i, float *cord_f, BYTE *clr, int size);

       void paint_square(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);

       void outline_sqaure(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);

       void paint_cord(float *cord, int size, BYTE *clr);

       void setColor(BYTE *clr, int clrNum);

};