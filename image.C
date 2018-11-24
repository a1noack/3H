#include "image.h"
#include <stdlib.h>
#include <string.h>
#include "source.h"

// default constructor
Image::Image(){
   width = height = maxval = 0;
   strcpy(magicNum, "P6");
   AllocateBufferSpace();
}

// copy constructor
Image::Image(const Image &original){
   width = original.width;
   height = original.height;
   maxval = original.maxval;
   strcpy(magicNum, original.magicNum);
   
   buffer = new Pixel*[height];
   for(int i = 0; i < height; i++){
      buffer[i] = new Pixel[width];
   }
   
   // copy each value to new buffer
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         memcpy(buffer[r][c].rgb, original.buffer[r][c].rgb, sizeof(buffer[r][c]));
      }
   }
}

Image::Image(char* filename){
   int width, height, maxval;
   char magicNum[128];
   
   // open file
   FILE *f_in = fopen(filename,"rb+");
   if (f_in == NULL) {
      printf("Error opening file.");
   }
   
   // read in initial pnm file data
   fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
   
   strcpy(this->magicNum, magicNum);
   this->width = width;
   this->height = height;
   this->maxval = maxval;
   this->AllocateBufferSpace();
   
   // copy values from file into buffer
   Pixel current_pixel;
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         fread(current_pixel.rgb, sizeof(unsigned char), 3, f_in);
         this->SetPixel(current_pixel, r, c);
      }
   }
   
   // close file
   fclose(f_in);
}

void Image::AllocateBufferSpace(){
   // allocate space for the buffer
   buffer = new Pixel*[height];
   for(int i = 0; i < height; i++){
      buffer[i] = new Pixel[width];
   }
}

void Image::DeallocateBufferSpace(){
   for(int r = 0; r < height; r=r+2){
      delete[] buffer[r];
   }
}

void Image::ResetSize(int width, int height){
   this->height = height;
   this->width = width;
}

void Image::SetSource(Source* source){
   this->source = source;
}

Source* Image::GetSource(){
   return source;
}

void Image::Update(){
   source->Update();
}
