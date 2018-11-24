#include "PNMwriter.h"
#include "logging.h"

void PNMwriter::Write(char *filename){
   // open file for writing
   FILE *f_out = fopen(filename, "wb");
   if (f_out == NULL)
   {
      char msg[1024];
      sprintf(msg, "%s: error opening file", SinkName());
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   
   // write metadata to file
   fprintf(f_out, "%s\n%d %d\n%d\n", this->image1->GetMagicNum(), this->image1->GetWidth(), this->image1->GetHeight(), this->image1->GetMaxVal());
   
   // print each pixel to file
   for(int r = 0; r < this->image1->GetHeight(); r++){
      for(int c = 0; c < this->image1->GetWidth(); c++){
         Pixel p = this->image1->GetPixel(r, c);
         fwrite(p.rgb, sizeof(unsigned char), sizeof(p.rgb), f_out);
      }
   }
   
   // close file
   fclose(f_out);
}

const char* PNMwriter::SinkName(){
   char msg[128];
   sprintf(msg, "PNMwriter");
   return strdup(msg);
}
