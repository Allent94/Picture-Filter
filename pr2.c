#include <stdio.h>
#include <string.h>

/* This program changes the coloring of the image.
   Each pixel is made from a combination of Red, Green, Blue.
   For each of these 3 colors, there is a number indicating the intensity or brightness.
   These numbers range from 0 (none) to 255 (full).  The program interchanges 
   the intensities of the green and blue components. 
   The blue sky becomes green.   The green tree becomes purple/blue.
   The red things remain red.
*/      



/*

Allen Tran
Kevin Nguyen
TCSSS 333 HW 2

EXTRA CREDIT - Darker()!

*/

#define HEADER_SIZE 54
#define HEIGHT 132
#define WIDTH 88

void brightening(char filename[], int height, int width);
void contrast(char filename[], int height, int width);
void scaledownrepeat(char filename[], int height, int width);
void rotation180(char filename[], int height, int width);
void darker(char filename[], int height, int width);
int getMinimizedDimension(int dimension);


// Main fuction that gets user inputs of image and 
// genernates 5 different versions of given image.
int main(void) {

    char filename[100];
    char filenameBMP[100];
    
    int height;
    int width;
    char newLine;

    /* Getting file information */
    printf("Enter the file name: ");
    scanf("%s" , filename);
    scanf("%c", &newLine);

    printf("Enter height and width (in pixels): ");
    scanf("%u%c%u%c" , &height, &newLine, &width, &newLine);

    char extension[] = ".bmp";
    strcat(filename, extension);

    /* Function class */
    brightening(filename, height, width);
    contrast(filename, height, width);
    scaledownrepeat(filename, height, width);
    rotation180(filename, height, width);
    darker(filename, height, width);
    printf("Done. Check the generated images. \n");

    return 0;

}

/**
 * Contrasts the image. 
 * filename is the name of the file
 * height is the height of the image in pixels
 * width is the width of the image in pixels
 */ 
void contrast(char filename[], int height, int width){

    FILE *infile = fopen(filename, "rb"); 
    FILE *outfile = fopen("test2.bmp", "wb");  
    
    char header[HEADER_SIZE]; 
    unsigned char pixels[height][width * 3];


    fread( header, 1 , HEADER_SIZE , infile);
    fread( pixels, 1 , height * height * 3 , infile);

    int r,c;

    unsigned int maxSaturation = 255;
    unsigned int minSaturation = 0;
    double ratio = 2.9695;

    for (r = 0; r < height; r++) {
        for (c = 0; c < width * 3; c++) {
          
          // Changing the  contrast
          pixels[r][c] = ratio * (pixels[r][c] - 128) + 128;
          
          // Checking for valid pixel contrast values
          // and changing values if invalid
          if (pixels[r][c] > maxSaturation) {
            pixels[r][c] = maxSaturation;
          } else if (pixels[r][c] < minSaturation) {
            pixels[r][c] = minSaturation;
          }   
      }
    }

    fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile); 
    fwrite( pixels, sizeof(char)  , height * width * 3  ,  outfile); 

    fclose(infile);
    fclose(outfile);


}

/**
 * This fuction brigthten the picture by 50 BRG
 * filename is the name of the file
 * height is the height of the image in pixels
 * width is the width of the image in pixels
 */ 
void brightening(char filename[], int height, int width){

    FILE *infile = fopen(filename, "rb"); 
    FILE *outfile = fopen("test1.bmp", "wb");  
    
    char header[HEADER_SIZE]; 
    unsigned char pixels[height][width * 3];


    fread( header, 1 , HEADER_SIZE , infile);
    fread( pixels, 1 , height * width * 3 , infile);

    int r,c;

    for(r = 0; r < height; r++){
        for( c = 0; c < width * 3; c++){
            if((pixels[r][c]+50) >= 255){
                pixels[r][c] = 255;
            } else if ((pixels[r][c]+50) < 255){
                pixels[r][c] = pixels[r][c]+50;
            }
        }
    }

    fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile);
    fwrite( pixels, sizeof(char)  , height * width * 3  ,  outfile); 

    fclose(infile);
    fclose(outfile);



}

/**
 * Rotates the given image by 180 degrees.
 * filename is the name of the file
 * height is the height of the image in pixels
 * width is the width of the image in pixels
 */ 
void rotation180(char filename[], int height, int width){

    FILE *infile = fopen(filename, "rb"); 
    FILE *outfile = fopen("test3.bmp", "wb");  
    
    char header[HEADER_SIZE]; 
    unsigned char pixels[height][width * 3];
    unsigned char pixels2[height][width * 3];


    fread( header, 1 , HEADER_SIZE , infile);
    fread( pixels, 1 , height * width * 3 , infile);

     int r1, c1, r2, c2;

     /* Going across original array, copying into copy array */
     for(r1 = 0 , r2 = height-1; r1 < height-1; r1++, r2--){
        for(c1 = 0, c2 = width*3; c1 < width*3; c1+=3, c2-=3){
            pixels2[r2][c2] = pixels[r1][c1+2];
            pixels2[r2][c2-1] = pixels[r1][c1+1];
            pixels2[r2][c2-2] = pixels[r1][c1];

        }
     }

    fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile); 
    fwrite( pixels2, sizeof(char)  , height * width * 3  ,  outfile); 

    fclose(infile);
    fclose(outfile);





}

/**
 * This fuction scales down the picthre and repeat the picture everywhere.
 * filename is the name of the file
 * height is the height of the image in pixels
 * width is the width of the image in pixels
 */ 
void scaledownrepeat(char filename[], int height, int width) {
    FILE *infile = fopen(filename, "rb"); 
    FILE *outfile = fopen("test4.bmp", "wb");  
    
    char header[HEADER_SIZE]; 
    unsigned char pixels[height][width * 3];
    unsigned char copyPixel[height][width * 3];


    fread( header, 1 , HEADER_SIZE , infile);
    fread( pixels, 1 , height * width * 3 , infile);

    int r,c;

    int h = getMinimizedDimension(height);
    int w = getMinimizedDimension(width);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width * 3; c++) {
            int newC = getMinimizedDimension(c);
            int newR = getMinimizedDimension(r);
            copyPixel[newR][newC] = pixels[r][c];
        }
    }

    for (r = 0; r < height; r++) {
        for (c = 0; c < width * 3; c++) {
            // int newC = getMinimizedDimension(c);
            // int newR = getMinimizedDimension(r);
            // copyPixel[newR + h][newC + w] = pixels[r][c];
            copyPixel[r/2 + h][c/2 + w] = pixels[r][c];
        }
    }


    fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile); 
    fwrite( copyPixel, sizeof(char)  , height * width * 3  ,  outfile);

    fclose(infile);
    fclose(outfile);


    

}

/**
 * Gets the new dimension of an image
 * dimension is the original dimension.
 * Returns the new dimension.
 */
int getMinimizedDimension(int dimension) {
    if (dimension % 2 == 0 ) {
        return dimension / 2;
    } else {
        return (dimension / 2) + 1;
    }
}


/**
 * Darkens the given image.
 * filename is the name of the file
 * height is the height of the image in pixels
 * width is the width of the image in pixels
 */ 
void darker(char filename[], int height, int width){

    FILE *infile = fopen(filename, "rb"); 
    FILE *outfile = fopen("test5.bmp", "wb");  
    
    char header[HEADER_SIZE]; 
    unsigned char pixels[height][width * 3];


    fread( header, 1 , HEADER_SIZE , infile);
    fread( pixels, 1 , height * width * 3 , infile);

    int r,c;

        for (r = 0; r < height; r++) {
            for (c = 0; c < width * 3; c++) {
                if((pixels[r][c]-50) <= 0){
                    pixels[r][c] = 0;
                } else if ((pixels[r][c]-50) > 0){
                    pixels[r][c] = pixels[r][c]-50;
                }
            }
        }

    fwrite( header, sizeof(char)  , HEADER_SIZE  ,  outfile); 
    fwrite( pixels, sizeof(char)  , height * width * 3  ,  outfile); 
    fclose(infile);
    fclose(outfile);
   
}