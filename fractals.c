// file name sierpmain.c
// author: Grace Odmark



// sierpmain.c
// main driver to test the Sierpinski fractal
// NOTE: you may only use this code if you fully understand how it works.

#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 );
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 );
void shrinkingSquares(int x1, int y1, int x2, int y2);
void drawSquare(int x1, int y1, int x2, int y2);
void spiralSquares(int x1, int y1, int r, int theta);
void circularLace(int x, int y, int radius);
void snowflake(int x, int y, int length); 
void tree(float x, float y, float length, float angle);
void fern(float x, float y, float length, float angle);
void spiralOfSpirals(float x, float y, float r, float theta); 
void drawSpiral(int x1, int y1, int r, int theta);




int main()
{
  int width = 700, height = 700, mrgn = 20;
  char c;

  gfx_open(width, height, "Sierpinski's Triangle");

  while(1) {
    c = gfx_wait();
   
    if ( c  == 'q' ) break;
    else if (c == '1') 
    {
	    gfx_clear();
	    sierpinski(mrgn, mrgn, width-mrgn, mrgn, width/2, height-mrgn);
    }
    else if (c == '2') {
    	gfx_clear();
	shrinkingSquares(width/4, height/4, (3 * width)/4, (3 * height)/4);
    }
    else if (c == '3') {
	    gfx_clear();
	    spiralSquares(width/2, height/2, 10,  M_PI);
    }
    else if (c == '4') {
	    gfx_clear();
	    circularLace(width/2, height/2, width/3);
    }
    else if (c == '5') {
	    gfx_clear();
	    snowflake(width/2 - mrgn, height/2, width/3);
    }
    else if (c == '6') {
	    gfx_clear();
	    tree(width/2, height - mrgn, width / 3, M_PI / 2);
    }
    else if (c == '7') {
	    gfx_clear();
	    fern(width/2, height - mrgn, width * .7, M_PI / 2);
    }
    else if (c == '8') {
	    gfx_clear();
	    spiralOfSpirals(width/2, height/2, width/2, M_PI/5);
    }

 
  }
}

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 )
{
  // Base case. 
  if( abs(x2-x1) < 5 ) return;

  // Draw a triangle
  drawTriangle( x1, y1, x2, y2, x3, y3 );

  // Recursive calls
  sierpinski( x1, y1, (x1+x2)/2, (y1+y2)/2, (x1+x3)/2, (y1+y3)/2 );
  sierpinski( (x1+x2)/2, (y1+y2)/2, x2, y2, (x2+x3)/2, (y2+y3)/2 );
  sierpinski( (x1+x3)/2, (y1+y3)/2, (x2+x3)/2, (y2+y3)/2, x3, y3 );
}

void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3 )
{
  gfx_line(x1,y1,x2,y2);
  gfx_line(x2,y2,x3,y3);
  gfx_line(x3,y3,x1,y1);
}

void shrinkingSquares(int x1, int y1, int x2, int y2)
{
	if (abs(y2-y1) <= .5) return;
	drawSquare(x1, y1, x2, y2);


	shrinkingSquares((x1-((x2-x1)/5)), (y1-((y2-y1)/5)), (x1+((x2-x1)/5)), (y1+((y2-y1)/5)));
	shrinkingSquares((x1-((x2-x1)/5)), (y2-((x2-x1)/5)), (x1+((x2-x1)/5)), (y2+((y2-y1)/5)));
	shrinkingSquares((x2-((x2-x1)/5)), (y1-((y2-y1)/5)), (x2+((x2-x1)/5)), (y1+((y2-y1)/5)));
	shrinkingSquares((x2-((x2-x1)/5)), (y2-((y2-y1)/5)), (x2+((x2-x1)/5)), (y2+((y2-y1)/5)));
}

void drawSquare(int x1, int y1, int x2, int y2)
{
	gfx_line(x1, y1, x2, y1);
	gfx_line(x1, y1, x1, y2);
	gfx_line(x1, y2, x2, y2);
	gfx_line(x2, y1, x2, y2);

}



void spiralSquares(int x1, int y1, int r, int theta)
{
	if (y1 <= 0) return;
	
	theta += 2 * M_PI;
	r = theta;

	int x = 1.2 * r * cos(theta) + 700/2;
	int y = 1.2 * r * sin(theta) + 700/2;
	
	drawSquare(x1, y1, x1+(r/5), y1+(r/5));

	spiralSquares(x - (r/5), y - (r/5), r, theta);



}

void circularLace(int x, int y, int radius)
{
	if (radius <=1) return;
	gfx_circle(x, y, radius);
	
	for (int i = 1; i <= 6; i++) 
		circularLace(x + radius * cos(M_PI /3 * i), y + radius * sin(M_PI / 3 * i), radius/3);


}

void snowflake(int x, int y, int length) 
{
	if (length <=1) return;


	for (int i = 0; i < 5; i++)
	{
		int x2 =  x + length * cos(2 * M_PI * i / 5);
		int y2 = y + length * sin(2 * M_PI * i / 5);

		gfx_line(x, y, x2, y2);
		snowflake(x2, y2, length/3);
	}

	
}


void tree(float x, float y, float length, float angle)
{
	if (length <=1) return;

	float x2 = x + length * cos(angle);
	float y2 = y - length * sin(angle);
	
	gfx_line(x, y, x2, y2);

	length = length * .65;


	tree(x2, y2, length, angle - M_PI / 4);
	tree(x2, y2, length, angle + M_PI /4);

	


}


void fern(float x, float y, float length, float angle)
{

	if (length <= 1) return;
	
	float x1 = x + length * cos(angle);
	float y1 = y - length * sin(angle);
	

	gfx_line(x, y, x1, y1);	

	fern(x-(x-x1) / 4, y-(y-y1) /4, length * .25, angle + M_PI /6);
	fern(x-(x-x1) / 2, y-(y-y1) /2, length * .25, angle + M_PI /6);
	fern(x-(x-x1) * 3 / 4, y-(y-y1) * 3 /4, length * .25, angle + M_PI /6);
	fern(x1, y1, length * .25, angle + M_PI /6);
	fern(x-(x-x1) / 4, y-(y-y1) /4, length * .25, angle - M_PI /6);
	fern(x-(x-x1) / 2, y-(y-y1) /2, length * .25, angle - M_PI /6);
	fern(x-(x-x1) * 3 / 4, y-(y-y1) * 3 /4, length * .25, angle - M_PI /6);
	fern(x1, y1, length * .25, angle - M_PI /6);

}



void spiralOfSpirals(float x, float y, float r, float theta) 
{
	if (r <= .5) return;

	
	gfx_point(x + r * cos(theta), y + r * sin(theta));
 
	spiralOfSpirals(x + r * cos(theta), y + r * sin(theta), r * .25, theta);
	spiralOfSpirals(x, y, r * .9, theta + M_PI/5);


}



# Grace-Odmark-Resume-Projects
