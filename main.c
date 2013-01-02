
/* main.c
 * Nathan W Brei
 * 6.087 Final Project: image2dxf
 * 26 January 2010
 */

/* main.c manages the (minimal) command-line interface, setting up
   files and global variables as needed, and then launching the
   appropriate edge detection algorithm onto the user-specified
   image. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "scribe.h"
#include "brain.h"


/* void help_user(void)
   Print instructions for use */
void help_user(void){

  printf("Usage: image2dxf [-a algorithm][-1 val -2 val -3 val][-o outfile][-i][-v|-m] infile\n");
  printf("Where: -a <algorithm>   The desired algorithm:\n");
  printf("          0             Bilateral noise reduction and thresholding\n");
  printf("                             -1 <color_sigma> -2 <spatial_sigma> -3 <threshold>\n");
  printf("          1             Bilateral noise reduction and adaptive thresholding\n");
  printf("                             -1 <color_sigma> -2 <spatial_sigma> -3 <offset>\n");
  printf("          2             Canny Filter\n");
  printf("                             -1 <edge link threshold> -2 <strongest signal threshold>\n");
  printf("          3             Canny Filter with median filter noise reduction\n");
  printf("                             -1 <edge link> -2 <strongest signal> -3 <median size>\n");
  printf("       -o <outfile>     The DXF output file\n");
  printf("       -q <quality>     1-100, where 100 is highest (and largest, and nastiest...)\n");
  printf("       -i               Invert (For light-on-dark images)\n");
  printf("       -v               Visualize the entire process\n");
  printf("       -m               Manually control process, and find sensible parameters.\n");
  printf("          <infile>      An image in JPEG or PNG format\n"); 
  printf("\nimage2dxf by Nathan W Brei\n");
}


int main(int argc, char** argv){

  char c;               /* Dummy variable for while loop */
  char* output_filename = "outfile.dxf";
  int manual = 0;       /* Whether or not user wants manual control */
  /* Default output filename */

  /* Get user's options */
  /* The skeleton for this code came from the glibc manual */

  printf("\n");  /* Nice formatting for startup */

  opterr = 0;
  while ((c = getopt (argc, argv, "a:1:2:3:n:o:q:ivm")) != -1)
    switch (c)
      {
      case 'a':  /* User invokes a specific algorithm */ 
        algorithm = atoi(optarg); break;

      case '1':  /* User specifies a magic number */
        val1 = atoi(optarg); break;

      case '2':  /* User specifies a magic number */
        val2 = atoi(optarg); break;

      case '3':  /* User specifies a magic number */
        val3 = atoi(optarg); break;

      case 'o':  /* User specifies an output file name */
        output_filename = optarg; break;

      case 'q':  /* User specifies contour precision */
        quality = atoi(optarg); break;

      case 'i':  /* User specifies invert */
        invert = 1; break;

      case 'v':  /* User wants a visualization */
	visualize=1; break;

      case 'm':  /* User wants manual control */
	manual=1; break;

      default:   /* Print an error */
	fflush(stdout);
	fprintf(stderr, "\n\e[91mError: Regarding command line options.\n\e[0m");
	fflush(stderr);
	help_user();
        return 22;
      }

  /* If user didn't specify the input file, help them */
  if (argc <= optind){ 
    fprintf(stderr, "\n\e[91mError: Missing input file.\n\e[0m");
    help_user();
    return 22;
  }


  /* Open image */
  image_from_file(argv[optind]);

  /* Open output file */
  dxf_start (output_filename);


  /* Run either manual or automatic process */
  if(manual) run_manual();
  else run_automatic();

  /* Close file */
  dxf_end ();

  return 0;
}
