#include "pnmreader.h"
#include "logging.h"

PNMreader::PNMreader(char *filename){
   int size = strlen(filename);
   this->filename = new char[size+1];
   strcpy(this->filename, filename);
   this->image.SetSource(this);
}

PNMreader::~PNMreader(){
   delete []filename;
}

void PNMreader::Execute(){
   int width, height, maxval;
   char magicNum[128];
   
   // open file
   FILE *f_in = fopen(filename,"rb+");
   if (f_in == NULL)
   {
      char msg[1024];
      sprintf(msg, "%s: error opening file", SourceName());
      DataFlowException e(SourceName(), msg);
      throw e;
   }
   
   // read in initial pnm file data
   fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
   
   this->image.SetMagicNum(magicNum);
   this->image.SetWidth(width);
   this->image.SetHeight(height);
   this->image.SetMaxVal(maxval);
   this->image.AllocateBufferSpace();
   
   // copy values from file into buffer
   Pixel current_pixel;
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         fread(current_pixel.rgb, sizeof(unsigned char), 3, f_in);
         this->image.SetPixel(current_pixel, r, c);
      }
   }
   
   // close file
   fclose(f_in);
}

const char* PNMreader::SourceName(){
   char msg[128];
   sprintf(msg, "PNMreader");
   return strdup(msg);
}
