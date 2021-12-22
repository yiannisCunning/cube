#include "vectorFun.h"
#include "things.h"
#include <stdio.h>
#include "cube.h"


int notmain(){
       printf("hello world");
       camera c = camera(500, 500);

       float v[3] = {1,1,1};
       float xa[3] = {1,0,0};
       float cords[3] = {0.1,0.1,0};

       c.setlens(v);
       c.clear_screen();
       bool a = c.cord_from_vector(xa, cords);
       c.drawAxis();

       cube cubeT = cube();
	for(int i = 0; i < 27; i++){
              printf("%d\n", i);
		printf("%f, %f, %f \n", cubeT.sqrs[i]->posVec[0], cubeT.sqrs[i]->posVec[1], cubeT.sqrs[i]->posVec[2]);
	}


       if(a){
              printf("\n%f, %f, %f", cords[0], cords[1], cords[2]);
       }
       return 0;
}