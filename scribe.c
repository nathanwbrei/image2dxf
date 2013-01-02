
/* scribe.c
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* scribe.c provides procedures for committing the geometry to disk in
   the DXF format.

   AutoCAD DXF has the advantage of being simple and
   ubiquitous. Furthermore, it imports into SolidWorks (my CAD of
   choice at the moment) effortlessly and reliably. Even furthermore,
   implementing it does not require reading hundreds of pages of
   unintuitive specifications, unlike IGES.

*/

#include <stdio.h>
#include <stdlib.h>

/* Global variables we care about: */
   FILE* fout = NULL;     /* The output file, defined in scribe.h */




/* void dxf_start (char* filename) 
   Open the output file, and write the initial header data. This is
   supposed to be data such as units, line colors, etc. However, we
   don't either don't have any of that information, or don't care, so
   long as our file is readable by most CAD software. 

*/
void dxf_start (char* filename){
  printf("Opening DXF output file: Outfile=%s",filename);
  if(!(fout=fopen(filename, "w"))){
    fprintf(stderr, "\n\e[91mError: Unable to open file %s for writing.\n\e[0m", filename);
    abort();
  }
  puts("Writing header data.");
  fprintf(fout,"Header data.\n");
}



/* void dxf_end (void)
   Write any remaining DXF nonsense, and close file. 
*/
void dxf_end (void){
  puts("Writing any remaining DXF nonsense, and closing.");
  fprintf(fout,"End.\n");
  fclose(fout);
}



/* void dxf_point (float x, float y)
   Write the data for a point to our DXF file. This is pretty intuitive. 
*/
void dxf_point (float x, float y){
  printf("Writing a point to file: %f,%f\n",x,y);
  fprintf(fout, "Writing a point to file: %f,%f\n",x,y);
  
}

/* void dxf_line (float x1, float y1, float x2, float y2)
   Write the data for a line to our DXF file. This is pretty intuitive. 
*/
void dxf_line (float x1, float y1, float x2, float y2){
  printf("Writing a line to file: %f,%f -> %f,%f\n",x1,y1,x2,y2);
  fprintf(fout,"Writing a line to file: %f,%f -> %f,%f\n",x1,y1,x2,y2);
}

void dxf_face (void){
  puts("Writing a face to file. Not as easy as it looks.\n");
  fprintf(fout,"Writing a face to file. Not as easy as it looks.\n");
}


/* I might have to do something a little bit more sophisticated,
   e.g. keeping track of points for re-use. Blah. */


