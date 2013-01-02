

/* A very simple binarization algorithm-- 
   applies a bilateral filter for noise reduction and then some threshold */
void binarize_threshold(void);


/* A slightly better binarization algorithm*/
void binarize_adaptive_threshold(void);


/* A more sophisticated binarization algorithm-- 
   applies a Canny filter */
void binarize_canny(void);


/* A more sophisticated binarization algorithm-- 
   median noise reduction (to reduce speckling) then a Canny filter */
void binarize_median_canny(void);



/* A "simple" contourizing algorithm. I don't know that much about how
   it works. But it does. */
void contourize_simple(void);

