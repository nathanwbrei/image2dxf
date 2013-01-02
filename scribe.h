
/* writer.h
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* Header file for scribe.c */

#ifndef SCRIBE_H_
#define SCRIBE_H_

extern FILE* fout;     /* The output file */


/* void dxf_start (char* filename) 
   Open the output file, and write the initial header data. This is
   supposed to be data such as units, line colors, etc. However, we
   don't either don't have any of that information, or don't care, so
   long as our file is readable by most CAD software. 
*/
void dxf_start (char* filename);



/* void dxf_end (void)
   Write any remaining DXF nonsense, and close file. 
*/
void dxf_end (void);



/* void dxf_point (float x, float y)
   Write the data for a point to our DXF file. This is pretty intuitive. 
*/
void dxf_point (float x, float y);



/* void dxf_line (float x1, float y1, float x2, float y2)
   Write the data for a line to our DXF file. This is pretty intuitive. 
*/
void dxf_line (float x1, float y1, float x2, float y2);



void dxf_face (void);


#endif /* SCRIBE_H_ */
