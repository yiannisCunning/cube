#include "vectorFun.h"
#include "things.h"
#include <math.h>





camera::camera(int width_in, int height_in)
{
       width = width_in;
       height = height_in;


       screen = (BYTE *)calloc(width * height * 4, sizeof(BYTE));
       depthScreen = (float *)calloc(width * height, sizeof(float));

       for(int i =0; i < width*height; i++){depthScreen[i] = 999999999;}

       v = (float *)calloc(3, sizeof(float));
       h1 = (float *)calloc(3, sizeof(float));
       h2 = (float *)calloc(3, sizeof(float));
       q = (float *)calloc(3, sizeof(float));
}

camera::~camera()
{
       if(screen != NULL){
              free(screen);
       }
}

void camera::setlens(float *v_in)
{
       cpyVec(v_in, v);

       float temp[3] = {0,0,1};
       cross(v, temp, h2);
       cross(h2, v, h1);
       
       // Set size of screen - base is 1
       normalize(h1);
       normalize(h2);
       constMult(3, h1, h1);
       constMult(3, h2, h2);

       // set screen location - factor is how far away it is
       constMult(0.5, v, q);

}

void camera::clear_screen()
{
       BYTE component[4] = {0xFF, 0xFF, 0xFF, 0xFF};
       for(int i = 0; i < width*height*4; i++){
              screen[i] = component[i%4];
              if(i % 4 == 0){
                     depthScreen[i/4] = 999999999;
              }
       }
}



void camera::drawAxis()
{


       BYTE clr[4] = {0x00, 0x00, 0x00, 0xFF};
       bool res;

       float point[3] = {0,0,0};
       float cordOrigin[3] = {0,0,0};
       float cord2[3] = {0,0,0};
       cord_from_vector(point, cordOrigin);


       for(int i = 0; i < 6; i++){
              point[0] = 0;
              point[1] = 0;
              point[2] = 0;
              point[i%3] = 3;
              if(i>=3){point[i%3] = -3;}
              res = cord_from_vector(point, cord2);
              if(res){
                     if(i>=3){paint_line(cordOrigin, cord2, clr, 1);}
                     else{paint_line(cordOrigin, cord2, clr, 1);}
              }
       }
       
}




void camera::draw_cube(cube c)
{
       float cordA[3] = {0,0,0};
       float cordB[3] = {0,0,0};
       float cordC[3] = {0,0,0};
       float cordD[3] = {0,0,0};

       float point[3] = {0,0,0};
       float center[3] = {0,0,0};
       float hp1[3] = {0,0,0};
       float hp2[3] = {0,0,0};
       int normalDirec = 0;

       BYTE clr[4] = {0x00, 0x00, 0x00, 0xFF};
       BYTE clr2[4] = {0x00, 0x00, 0x00, 0xFF};
       // cube uses 0 - red, 1 - orange, 2 - blue, 3 - green, 4 - yellow, 5 - white
       for(int i = 0; i < 27; i++)
       {
              for(int q = 0; q < 6; q++)
              {
                     if(cosAngle(c.sqrs[i]->pointers[q], c.sqrs[i]->posVec) < 1.57)
                     {      
                            // 0) set color
                            setColor(clr, c.sqrs[i]->cols[q]);

                            // 1) find the center of the square
                            constMult(0.5, c.sqrs[i]->pointers[q], center);
                            addVec(center, c.sqrs[i]->posVec, center);

                            normalDirec = abs(c.sqrs[i]->pointers[q][1]) + abs(c.sqrs[i]->pointers[q][2])*2;

                            // 2) Find h1 and h2 for the square 
                            setVector(hp1, 0,0,0);
                            setVector(hp2, 0,0,0);
                            hp1[(normalDirec + 1)%3] = 0.5;
                            hp2[(normalDirec + 2)%3] = 0.5;
                     
                            // 3) find each cord of the corners
                            addVec(center, hp1, point);
                            addVec(point, hp2, point);
                            cord_from_vector(point, cordA);

                            addVec(center, hp1, point);
                            subVec(point, hp2, point);
                            cord_from_vector(point, cordB);

                            subVec(center, hp1, point);
                            subVec(point, hp2, point);
                            cord_from_vector(point, cordC);

                            subVec(center, hp1, point);
                            addVec(point, hp2, point);
                            cord_from_vector(point, cordD);

                            paint_square(cordA, cordB, cordC, cordD, clr, 1);
                            outline_sqaure(cordA, cordB, cordC, cordD, clr2, 2);
                     }
              }
       }

}


















void camera::screen_cords(float *w, float *depth, float *cords)
{


       // temp is vector going to corner of screen
       float corner[3] = {0,0,0};
       float wOnScreen[3] = {0,0,0};
       float yComp[3] = {0,0,0};

       addVec(h1, h2, corner);
       addVec(corner, q, corner);
       subVec(w, corner, wOnScreen);

       float *yaxis = corner;      // ( just changing name )
       constMult(-2, h1, yaxis);

       // To find y cord -> want to project onto -2 * h1
       project(wOnScreen, yaxis, yComp);
       cords[1] = height * vecMag(yComp)/vecMag(yaxis);



       float *xaxis = yaxis;
       float *xComp = yComp;       // ( just changing name )
       constMult(-2, h2, xaxis);

       // To find x cord -> want to project onto -2 * h2
       project(wOnScreen, xaxis, xComp);
       cords[0] = width * vecMag(xComp)/vecMag(xaxis);

       cords[2] = *depth;
}




bool camera::point_from_vector(float *w, float *ans, float *depth)
{
       // 1) make sure that it is infront of screen
       float qw[3] = {0,0,0};
       float projqwq[3] = {0,0,0};
       subVec(w, q, qw);
       project(qw, q, projqwq);

              // will be infront of the 'Q' plane if the dot product is negative
       float similarity = dotProduct(q, projqwq);
       if(similarity >= 0){
              return false;
       }


       // 2) find the ratio of (normal distance to q plane)/(normal distance to v plane)
       float distQ = vecMag(projqwq);

       float *vw = qw;
       float *projvwv = projqwq;

       subVec(w, v, vw);
       project(vw, v, projvwv);
       float distV = vecMag(projvwv);


       // 3) the point on the plane is w + wv*ratio -> store in ans
       float *wv = vw;
       float *toAdd = projqwq;
       subVec(v, w, wv);
       constMult(distQ/distV, wv, toAdd);

       addVec(w, toAdd, ans);

       // 4) set the depth/distance from eye: = mag(wv)
       *depth = vecMag(wv);

       return true;
}


bool camera::point_from_vector_other(float *w, float *ans, float *depth)
{
       float qw[3] = {0,0,0};
       float projqwq[3] = {0,0,0};
       subVec(w, q, qw);
       project(qw, q, projqwq);
       
       float similarity = dotProduct(q, projqwq);
       if(similarity >= 0){return false;}

       subVec(qw, projqwq, qw);
       addVec(qw, q, ans);

       *depth = vecMag(projqwq);
       return true;
}




bool camera::in_range(float *cords, int height, int width){
       if(cords[0] < 0 || cords[0] > width - 1){
              return false;
       }

       if(cords[1] < 0 || cords[1] > height - 1){
              return false;
       }
       return true;
}



bool camera::cord_from_vector(float *w, float *cords)
{
       // Get the point on the plane of the screen
       float p_on_screen[3] = {0,0,0};
       float depth = 0;
       bool inVeiw = point_from_vector(w, p_on_screen, &depth);
       if(!inVeiw){return false;}

       // Get the cords for that point
       screen_cords(p_on_screen, &depth, cords);
       if(!in_range(cords, height, width)){return false;}

       return true;
}






void camera::paint_line(float *cord_i, float *cord_f, BYTE *clr, int size)
{      
       // paint a whole bunch of points along the line
       

       float dx = cord_f[0] - cord_i[0];
       float dy = cord_f[1] - cord_i[1];
       float dz = cord_f[2] - cord_i[2];
       float mag = sqrt(dx*dx + dy*dy);
       float pos[3] = {0,0,0};
       float frac;
       for(int i = 0; i <= ceil(mag); i++){
              frac = i/mag;
              pos[0] = frac*dx + cord_i[0];
              pos[1] = frac*dy + cord_i[1];
              pos[2] = frac*dz + cord_i[2];

              paint_cord(pos, size, clr);
       }
}

void camera::paint_square(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size)
{
       // Start at a -> move towards b-> draw a line from each point to 
       float dxAB = cordA[0] - cordB[0];
       float dyAB = cordA[1] - cordB[1];
       float dzAB = cordA[2] - cordB[2];
       float magAB = sqrt(dxAB*dxAB + dyAB*dyAB);
       float dxCD = cordD[0] - cordC[0];
       float dyCD = cordD[1] - cordC[1];
       float dzCD = cordD[2] - cordC[2];
       float magCD = sqrt(dxCD*dxCD + dyCD*dyCD);


       float pos[3] = {0,0,0};
       float pos2[3] = {0,0,0};
       float frac;
       float length;

       length = fmax(magAB, magCD);

       for(int i = 0; i <= ceil(length); i++){
              frac = i/length;
              pos[0] = frac*dxAB + cordB[0];
              pos[1] = frac*dyAB + cordB[1];
              pos[2] = frac*dzAB + cordB[2];

              pos2[0] = frac*dxCD + cordC[0];
              pos2[1] = frac*dyCD + cordC[1];
              pos2[2] = frac*dzCD + cordC[2];

              paint_line(pos, pos2, clr, size);
       }       


}











void camera::paint_cord(float *cord, int size, BYTE *clr)
{
       // Paint a cord on 'screen', also put relative depth for each pixel on 'depthScreen'
       float temp[3] = {0,0,0};
       int n;
       for(int dx = -1*size; dx <= size; dx++){
              for(int dy = -1*size; dy <= size; dy++){
                     temp[0] = cord[0] + dx;
                     temp[1] = cord[1] + dy;
                     if(in_range(temp, height, width))
                     {
                            n = ((int)round(temp[0]) + (int)round(temp[1])*width);
                            if(depthScreen[n] >= cord[2]){
                                   depthScreen[n] = cord[2];
                                   screen[n*4] = clr[0];
                                   screen[n*4+1] = clr[1];
                                   screen[n*4+2] = clr[2];
                                   screen[n*4+3] = clr[3];
                            }
                     }
              }
       }
}




// cube uses 0 - red, 1 - orange, 2 - blue, 3 - green, 4 - yellow, 5 - white
void camera::setColor(BYTE *clr, int clrNum)
{
       clr[0] = 0x0;
       clr[1] = 0x0;
       clr[2] = 0x0;
       clr[3] = 0xFF;

       switch (clrNum)
       {
       case 0:
              clr[2] = 0xFF;
              break;
       case 1:
              clr[1] = 128;
              clr[2] = 0xFF;
              break;
       case 2:
              clr[0] = 0xFF;
              break;
       case 3:
              clr[1] = 0xFF;
              break;
       case 4:
              clr[1] = 0xFF;
              clr[2] = 0xFF;
              break;
       case 5:
              clr[0] = 150;
              clr[1] = 150;
              clr[2] = 150;
              break;
       default:
              break;
       }


}




void camera::outline_sqaure(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size)
{
       paint_line(cordA, cordB, clr, size);
       paint_line(cordB, cordC, clr, size);
       paint_line(cordC, cordD, clr, size);
       paint_line(cordD, cordA, clr, size);
}