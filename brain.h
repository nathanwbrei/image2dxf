
/* brain.h
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* Headers for brain.c
*/

#include "cv.h"
#include "highgui.h"

#ifndef BRAIN_H_
#define BRAIN_H_

/* Global variables; actually defined in main */
extern int visualize;
extern int magic_number;
extern int invert;
extern int algorithm;
extern int val1;
extern int val2;
extern int val3;
extern int quality;

extern IplImage* target;  
extern CvSeq* contour;
extern CvMemStorage* storage;

/* void run_automatic (void)
   The non-interactive edge-detection procedure */
void run_automatic (void);


   
/* void run_manual (void) The interactive edge-detection
   procedure. Without this, we basically have no hope of determining
   sensible algorithm parameters. Given a collection of similar
   images, we can manually find the proper parameters on a reference
   image, and from there process the rest non-interactively.

   This gets a little bit harder to read because it is event-driven. 
   Sorry about that.
*/
void run_manual (void);
  

/* image_from_file loads the user-specified image from file. */
void image_from_file (char* image_name);


#endif /* BRAIN_H_ */
