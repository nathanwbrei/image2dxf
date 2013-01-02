
/* brain.c
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* brain.c runs through the process of noise reduction, binarization,
   and contour mapping, with redundant algorithms where
   necessary. THIS IS WHERE THE <PROVERBIAL> MAGIC HAPPENS!!! */

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "scribe.h"
#include "toolchest.h"

#define MAX_WINDOW_SIZE 600
#define WAIT_TIME 1000


/* Global variables, shared with main */
int visualize = 0;      /* Whether or not to include visualization */
int magic_number = 0;   /* Algorithm-specific behavior */
int algorithm = 0;      /* Chosen algorithm */
int invert = 0;         /* Whether or not image comes from webcam */
int val1 = 0;         /* Algorithm-specific parameters, scale from 0 to 255 */
int val2 = 0;
int val3 = 0;
int quality=1000;

IplImage* target;         /* The image we are playing with */
CvSeq* contour;       /* The contours we end up with */
CvMemStorage* storage;

/* C is really spoiling me; the Java student I once was recoils at my
   utter lack of encapsulation and willy-nilly global variable
   access. */


/* image_from_file loads the user-specified image from file. */

void image_from_file (char* image_name){
  /* Load the image */
  target=cvLoadImage(image_name, CV_LOAD_IMAGE_GRAYSCALE);

  /* If loading failed, abort. */
  if(!target){
    fprintf(stderr, "\n\e[91mError: Unable to load image.\n\e[0m");
    abort();
  }

  if(invert) cvNot(target, target);
}




/* void write_contours(void)
   Writes all of the contour information to the DXF file */

void write_contours(void){
  puts("Eventually write contours to file.");

  /*   CvSeq* p = contour->
    for( ; contour != 0; contour = contour->h_next ){
       dxf_point();
    }  

  */
}



   
/* void run_manual (void) The interactive edge-detection
   procedure. Without this, we basically have no hope of determining
   sensible algorithm parameters. Given a collection of similar
   images, we can manually find the proper parameters on a reference
   image, and from there process the rest non-interactively.

   This gets a little bit harder to read because it is event-driven. 
   Sorry about that.
*/
void on_mouse(int event, int x, int y, int flags, void* param);
void refresh_results(void);

void run_manual (void){  
  
  /* Create window showing binarization */
  cvNamedWindow("Binarized", CV_WINDOW_AUTOSIZE);

  /* Create window showing resulting contours */
  cvNamedWindow("Contourized", CV_WINDOW_AUTOSIZE);

  /* Create window showing all three sliders */
  cvNamedWindow("Adjust parameters- then click on image",CV_WINDOW_AUTOSIZE);


  cvCreateTrackbar("Value 1", "Adjust parameters- then click on image", 
		   &val1, 255, NULL);
  cvCreateTrackbar("Value 2", "Adjust parameters- then click on image",
		   &val2, 255, NULL);
  cvCreateTrackbar("Value 3", "Adjust parameters- then click on image",
		   &val3, 255, NULL);

  
  cvSetMouseCallback("Binarized", on_mouse, NULL);
  cvSetMouseCallback("Contourized", on_mouse, NULL);

  /* Invert image, obviously */
  cvNot(target, target);

  /* Put results into windows */
  refresh_results();  

  cvSetTrackbarPos("Value 1", "Adjust parameters- then click on image", val1);
  cvSetTrackbarPos("Value 2", "Adjust parameters- then click on image", val2);
  cvSetTrackbarPos("Value 3", "Adjust parameters- then click on image", val3);


  /* Window sizing and placement */
  int max = (target->width > target->height) ? 
    target->width : target->height;
  cvResizeWindow("Binarized", (target->width*400/max), 
		 (target->height*400/max));
  cvResizeWindow("Contourized", (target->width*400/max), 
		 (target->height*400/max));
  cvResizeWindow("Adjust parameters- then click on image", 850, 150);
  cvMoveWindow("Binarized",100,100);
  cvMoveWindow("Contourized",550,100);
  cvMoveWindow("Adjust parameters- then click on image",100,550);


  /* Until the user presses a key, do nothing. */
  char c = -1;
  while (c==-1) c = cvWaitKey(0);   

  /* Program flow passes to slider callback function */

  /* Write contours to file */
  write_contours();

  /* Deallocate last resources */
  cvReleaseMemStorage(&storage);
  cvDestroyWindow("Binarize");
  cvDestroyWindow("Contourize");
  cvDestroyWindow("Adjust parameters- then click on image");

  /* Print values for future reference */
  printf("Algorithm %d, val1=%d val2=%d val3=%d\n",algorithm,
	 val1,val2,val3);

}


void refresh_results(void){
 
    /* Create resultant images */
    IplImage* backuptarget = cvCloneImage(target);
    IplImage* contourtarget = 
      cvCreateImage(cvSize(target->width,target->height), 8, 3);
    cvZero( contourtarget );
    
    /* Run some binarize algorithm */
    switch(algorithm){
    case 0:  binarize_threshold(); break;
    case 1:  binarize_adaptive_threshold(); break;
    case 2:  binarize_canny(); break;
    case 3:  binarize_median_canny();  break;
    default: binarize_threshold(); break;
    }

    /* Write binarized image to screen */
    cvShowImage("Binarized", target);
    
     /* Run some contourize algorithm */
    contourize_simple();

    /* Write contours to screen */
    CvSeq* p = contour;
    for( ; p != 0; p = p->h_next ){

      /* Trippy colors differentiate each contour level */
      CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 );

      cvDrawContours( contourtarget, p, color, color, -1, 
		      CV_FILLED, 8,cvPoint(0,0) );
    }
 
    cvShowImage("Contourized", contourtarget);
 
    cvReleaseImage(&contourtarget);
    cvReleaseImage(&target);
    target = cvCloneImage(backuptarget);
    cvReleaseImage(&backuptarget);

}


/* void on_mouse(...)
   callback function for mouse events.
   We use this instead of the trackbar event because we want to
   refresh in discrete, occasional intervals */

void on_mouse(int event, int x, int y, int flags, void* param){
  /* on mouseUp, set vals based on trackbar vals.  */  

  if(event==CV_EVENT_LBUTTONUP){
    printf("Running with values %d %d %d\n",val1,val2,val3);
    refresh_results();
  }
}



/* void run_automatic (void) The non-interactive edge-detection
   procedure. This is the real heart of the program, managing all of
   the different algorithms as well as notable output.
*/

void run_automatic (void){  

  if(visualize) {

    /* Create window, show original */
    cvNamedWindow("image2dxf", CV_WINDOW_AUTOSIZE);
    //cvSetMouseCallback("image2dxf", on_mouse, NULL);
    cvShowImage("image2dxf", target);

    /* Resize as necessary */
    int max = (target->width > target->height) ? 
      target->width : target->height;
    cvResizeWindow("image2dxf", (target->width*MAX_WINDOW_SIZE/max), 
		   (target->height*MAX_WINDOW_SIZE/max));

    /* Wait for user acknowledgement */		 
    cvWaitKey(WAIT_TIME);
  }

  /* Contourizing algorithms treat white as object of interest, black
     as background. So we have to invert. We do it at this stage so that
     we don't confuse the user. */ 
  cvNot(target, target);

  if(visualize){
    /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", target);
    cvWaitKey(WAIT_TIME);
  }

  /* Run some binarize algorithm */
  switch(algorithm){
  case 0:  binarize_threshold(); break;
  case 1:  binarize_adaptive_threshold(); break;
  case 2:  binarize_canny(); break;
  case 3:  binarize_median_canny();  break;
  default: binarize_threshold(); break;
  }

  if(visualize){
    /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", target);
    cvWaitKey(WAIT_TIME);
  }

  /* Run some contourize algorithm */
  contourize_simple();


  /* This is where it gets artistic */
  if(visualize){

    /* Black background time! Note we need to recreate target in COLOR */
    CvSize targetsize = cvSize(target->width,target->height);
    cvReleaseImage(&target);
    target=cvCreateImage( targetsize, 8, 3 );
    cvZero( target );
        
    /* Write contours to screen */
    CvSeq* p = contour;
    for( ; p != 0; p = p->h_next ){

      /* Trippy colors differentiate each contour level */
      CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 );

      cvDrawContours( target, p, color, color, -1, 
		      CV_FILLED, 8,cvPoint(0,0) );
    }
 
   /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", target);
    cvWaitKey(0);

    /* Deallocate resources */
    cvReleaseImage(&target);
    cvDestroyWindow("image2dxf");
  }

  /* Write contours to file */
  write_contours();

  /* Deallocate last resources */
  cvReleaseMemStorage(&storage);

}
