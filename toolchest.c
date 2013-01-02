
/* toolchest.c
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* toolchest.c contains all of the redundant algorithms used by brain.c */

#include "cv.h"
#include "highgui.h"
#include "brain.h"

#define WAIT_TIME 1000

/**************************************************************
 *  BINARIZATION ALGORITHMS
 *
 *  These algorithms convert the grayscale target image into a
 *  black-and-white, binary image. Black=data that is meaningful;
 *  white=data that isn't. There are many ways to make this
 *  distinction, with varying levels of automation, but they all
 *  generally involve filtering out noise, taking image derivatives,
 *  and thresholding the image.
 *
 **************************************************************/




/* A very simple binarization algorithm-- 
   applies a bilateral filter for noise reduction and then some threshold */

void binarize_threshold(void){

  printf("Running algorithm=binarize_threshold.\n");

  /* Set default values, if none provided */
  if(!val1) val1=22;  /* color sigma */
  if(!val2) val2=70;  /* spatial sigma */ 
  if(!val3) val3=127;  /* threshold */

  /* cvSmooth bilateral can't process images in place, so we create an
     image buffer */  
  IplImage* buffer = cvCreateImage( cvSize(target->width,target->height), 
				    8, 1 );

  /* Bilateral filter for noise reduction */
  cvSmooth(target, buffer, CV_BILATERAL, 10, 0, val1, val2);

  /* We aren't taking image derivates here. */
  
  /* This step is gratifying, so show it to the user */
  if(visualize){
    /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", buffer);
    cvWaitKey(WAIT_TIME);
  }

  /* Binarization stage */
  cvThreshold(buffer, target, val3, 255, CV_THRESH_BINARY);

  /* We no longer need/want buffer */
  cvReleaseImage(&buffer);
}


/* A slightly better binarization algorithm */
void binarize_adaptive_threshold(void){
  printf("Running algorithm=binarize_adaptive_threshold.\n");

  /* Set default values, if none provided */
  if(!val1) val1=22;  /* color sigma */
  if(!val2) val2=70;  /* spatial sigma */ 
  if(!val3) val3=22;  /* offset */

  /* cvSmooth bilateral can't process images in place, so we create an
     image buffer */  
  IplImage* buffer = cvCreateImage( cvSize(target->width,target->height), 
				    8, 1 );

  /* Bilateral filter for noise reduction */
  cvSmooth(target, buffer, CV_BILATERAL, 10, 0, val1, val2);

  /* We aren't taking image derivates here. */
  
  /* This step is gratifying, so show it to the user */
  if(visualize){
    /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", buffer);
    cvWaitKey(WAIT_TIME);
  }

  /* Binarization stage */
  cvAdaptiveThreshold(buffer, target, 255, 
		      CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 3, val3);

  /* We no longer need/want buffer */
  cvReleaseImage(&buffer);

}


/* A more sophisticated binarization algorithm-- applies a Canny
   filter. This performs really well on solid boundaries, but
   relatively poorly on thin edges. */
void binarize_canny(void){


  /* Set default values, if none provided */
  if(!val1) val1=230;  /* Edge linking */
  if(!val2) val2=250;  /* 'Strongest' segments */


  printf("Running algorithm=binarize_canny.\n");

  cvCanny(target, target, val1, val2, 3);
}


/* A more sophisticated binarization algorithm-- 
   median noise reduction (to reduce speckling) then a Canny filter */

void binarize_median_canny(void){


  /* Set default values, if none provided */
  if(!val1) val1=230;  /* Edge linking */
  if(!val2) val2=250;  /* 'Strongest' segments */
  if(!val3) val3=75;  /* thus, val4=3 */
    

  /* val3 is special since it can only take 1,3,5,7... */
  int val4 = val3*10/255;
  if(!(val4 % 2)) val4++;

  printf("Running algorithm=binarize_median_canny.\n");


  /* cvSmooth bilateral can't process images in place, so we create an
     image buffer */  
  IplImage* buffer = cvCreateImage( cvSize(target->width,target->height), 
				    8, 1 );
  /* Median filter for speckle reduction */
  cvSmooth(target, buffer, CV_MEDIAN, val4, val4, 0, 0);
 
  /* This step is gratifying, so show it to the user */
  if(visualize){
    /* Wait for user acknowledgement */		 
    cvShowImage("image2dxf", buffer);
    cvWaitKey(WAIT_TIME);
  }

  cvCanny(buffer, target, val1, val2, 3);

  /* We no longer need/want buffer */
  cvReleaseImage(&buffer);

}



/**************************************************************
 *  CONTOURIZATION ALGORITHMS
 *
 *  These algorithms convert the binary image into a sequence of
 *  points, which I can draw on screen in a manner which is very
 *  pretty. OpenCV isn't particularly transparent regarding these
 *  functions, sadly. However, they work well enough for me to not
 *  have to bother writing my own.
 *
 **************************************************************/


/* A "simple" contourizing algorithm. I don't know that much about how
   it works. But it does. */
void contourize_simple(void){
  
  /* Allocate contour storage */
  contour = NULL;
  storage = cvCreateMemStorage(0);  

  /* Magic happens here */
  cvFindContours( target, storage, &contour, sizeof(CvContour),
		  CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
 
  if(quality<100){
  float precision = 1.0/quality;
    /* Approximate them so that our file doesn't become too enormous */
      contour = cvApproxPoly( contour, sizeof(CvContour), 
    			  storage, CV_POLY_APPROX_DP, precision, 1 );
  }
 }


