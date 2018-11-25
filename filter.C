#include "filter.h"
#include "logging.h"

//Filter
//hello
Filter::Filter(){
   this->image.SetSource(this);
}

int Filter::current_depth = 0;

void Filter::Update(){
   current_depth++;
   if(current_depth > max_depth){
      char msg[1024];
      sprintf(msg, "%s: max filter depth reached!!", FilterName());
      DataFlowException e(FilterName(), msg);
      throw e;
   }
   char msg[128];
   sprintf(msg, "%s: about to update input1", SourceName());
   Logger::LogEvent(msg);
   CheckImage1();
   image1->Update();
   sprintf(msg, "%s: done updating input1", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to execute", SourceName());
   Logger::LogEvent(msg);
   this->Execute();
   sprintf(msg, "%s: done executing", SourceName());
   Logger::LogEvent(msg);
}

void Filter::CheckImage1(){
   if (image1 == 0)
   {
      char msg[1024];
      sprintf(msg, "%s: no input1!", FilterName());
      DataFlowException e(FilterName(), msg);
      throw e;
   }
   if (image1 == &image){
      char msg[1024];
      sprintf(msg, "%s: no filter loops!", FilterName());
      DataFlowException e(FilterName(), msg);
      throw e;
   }
}

void Filter::CheckImage2(){
   if (image2 == 0)
   {
      char msg[1024];
      sprintf(msg, "%s: no input2!", FilterName());
      DataFlowException e(FilterName(), msg);
      throw e;
   }
   if (image2 == &image){
      char msg[1024];
      sprintf(msg, "%s: no filter loops!", FilterName());
      DataFlowException e(FilterName(), msg);
      throw e;
   }
}

//Blender
void Blender::Execute(){
   if(factor>1 || factor<0){
      char msg[1024];
      sprintf(msg, "Invalid factor for %s: %f", FilterName(), this->factor);
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   int width1 = image1->GetWidth();
   int height1 = image1->GetHeight();
   int width2 = image2->GetWidth();
   int height2 = image2->GetHeight();
   if(width1 != width2 || height1 != height2)
   {
      char msg[1024];
      sprintf(msg, "%s: dimensions must match: image1 = %dx%d, image2 = %dx%d", SinkName(), width1, height1, width2, height2);
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   int maxval1 = image1->GetMaxVal();
   int maxval2 = image2->GetMaxVal();
   if(maxval1 > maxval2){
      image.SetMaxVal(maxval1);
   }
   else{
      image.SetMaxVal(maxval2);
   }
   image.SetMagicNum(image1->GetMagicNum());
   image.SetHeight(height1);
   image.SetWidth(width1);
   image.AllocateBufferSpace();
   for(int r = 0; r < height1; r++){
      for(int c = 0; c < width1; c++){
         Pixel p1 = image1->GetPixel(r, c);
         Pixel p2 = image2->GetPixel(r, c);
         Pixel pcombine;
         for(int d = 0; d < 3; d++){
            int val1 = int(p1.rgb[d]);
            int val2 = int(p2.rgb[d]);
            unsigned char valcombine = (unsigned char)(val1*this->GetFactor() + val2*(1-this->GetFactor()));
            pcombine.rgb[d] = valcombine;
         }
         image.SetPixel(pcombine, r, c);
      }
   }
}

void Blender::Update(){
   char msg[128];
   sprintf(msg, "%s: about to update input1", SourceName());
   Logger::LogEvent(msg);
   CheckImage1();
   image1->Update();
   sprintf(msg, "%s: done updating input1", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to update input2", SourceName());
   Logger::LogEvent(msg);
   CheckImage2();
   image2->Update();
   sprintf(msg, "%s: done updating input2", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to execute", SourceName());
   Logger::LogEvent(msg);
   this->Execute();
   sprintf(msg, "%s: done executing", SourceName());
   Logger::LogEvent(msg);
}

const char* Blender::FilterName(){
   char msg[128];
   sprintf(msg, "Blender");
   return strdup(msg);
}

//TBCombine
void TBCombine::Execute(){
   int widtht = image1->GetWidth();
   int heightt = image1->GetHeight();
   int widthb = image2->GetWidth();
   int heightb = image2->GetHeight();
   if (widtht != widthb)
   {
      char msg[1024];
      sprintf(msg, "%s: widths must match: %d, %d", SinkName(), widtht, widthb);
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   int maxval1 = image1->GetMaxVal();
   int maxval2 = image2->GetMaxVal();
   if(maxval1 > maxval2){
      image.SetMaxVal(maxval1);
   }
   else{
      image.SetMaxVal(maxval2);
   }
   image.SetMagicNum(image1->GetMagicNum());
   image.SetHeight(heightt + heightb);
   image.SetWidth(widtht);
   image.AllocateBufferSpace();
   Pixel p;
   for(int r = 0; r < heightt; r++){
      for(int c = 0; c < widtht; c++){
         p = image1->GetPixel(r, c);
         image.SetPixel(p, r, c);
      }
   }
   for(int r = 0; r < heightb; r++){
      for(int c = 0; c < widthb; c++){
         p = image2->GetPixel(r, c);
         image.SetPixel(p, r+heightt, c);
      }
   }
}

void TBCombine::Update(){
   char msg[128];
   sprintf(msg, "%s: about to update input1", SourceName());
   Logger::LogEvent(msg);
   CheckImage1();
   image1->Update();
   sprintf(msg, "%s: done updating input1", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to update input2", SourceName());
   Logger::LogEvent(msg);
   CheckImage2();
   image2->Update();
   sprintf(msg, "%s: done updating input2", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to execute", SourceName());
   Logger::LogEvent(msg);
   this->Execute();
   sprintf(msg, "%s: done executing", SourceName());
   Logger::LogEvent(msg);
}

const char* TBCombine::FilterName(){
   char msg[128];
   sprintf(msg, "TBConcat");
   return strdup(msg);
}

//LRCombine
void LRCombine::Execute(){
   int widthl = image1->GetWidth();
   int heightl = image1->GetHeight();
   int widthr = image2->GetWidth();
   int heightr = image2->GetHeight();
   if (heightl != heightr)
   {
      char msg[1024];
      sprintf(msg, "%s: heights must match: %d, %d", SinkName(), heightl, heightr);
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   int maxval1 = image1->GetMaxVal();
   int maxval2 = image2->GetMaxVal();
   if(maxval1 > maxval2){
      image.SetMaxVal(maxval1);
   }
   else{
      image.SetMaxVal(maxval2);
   }
   image.SetMagicNum(image1->GetMagicNum());
   image.SetHeight(heightl);
   image.SetWidth(widthl + widthr);
   image.AllocateBufferSpace();
   for(int r = 0; r < heightl; r++){
      for(int c = 0; c < widthl; c++){
         Pixel p = image1->GetPixel(r, c);
         image.SetPixel(p, r, c);
      }
   }
   for(int r = 0; r < heightr; r++){
      for(int c = 0; c < widthr; c++){
         Pixel p = image2->GetPixel(r, c);
         image.SetPixel(p, r, c+widthl);
      }
   }
}

void LRCombine::Update(){
   char msg[128];
   sprintf(msg, "%s: about to update input1", SourceName());
   Logger::LogEvent(msg);
   CheckImage1();
   image1->Update();
   sprintf(msg, "%s: done updating input1", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to update input2", SourceName());
   Logger::LogEvent(msg);
   CheckImage2();
   image2->Update();
   sprintf(msg, "%s: done updating input2", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to execute", SourceName());
   Logger::LogEvent(msg);
   this->Execute();
   sprintf(msg, "%s: done executing", SourceName());
   Logger::LogEvent(msg);
}

const char* LRCombine::FilterName(){
   char msg[128];
   sprintf(msg, "LRConcat");
   return strdup(msg);
}

//Shrinker
void Shrinker::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   image.SetMaxVal(this->image1->GetMaxVal());
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(int(height/2));
   image.SetWidth(int(width/2));
   image.AllocateBufferSpace();
   for(int r = 0; r < height-1; r=r+2){
      for(int c = 0; c < width-1; c=c+2){
         Pixel p = this->image1->GetPixel(r, c);
         image.SetPixel(p, r/2, c/2);
      }
   }
}

const char* Shrinker::FilterName(){
   char msg[128];
   sprintf(msg, "Shrinker");
   return strdup(msg);
}

//Mirror
void Mirror::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   image.SetMaxVal(this->image1->GetMaxVal());
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(height);
   image.SetWidth(width);
   image.AllocateBufferSpace();
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         Pixel p = this->image1->GetPixel(r, c);
         image.SetPixel(p, r, width-c-1);
      }
   }
}

const char* Mirror::FilterName(){
   char msg[128];
   sprintf(msg, "Mirror");
   return strdup(msg);
}

//Rotate
void Rotate::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   
   image.SetMaxVal(this->image1->GetMaxVal());
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(width);
   image.SetWidth(height);
   image.AllocateBufferSpace();
//   for(int r = 0; r < width; r++){
//      for(int c = height-1; c >= 0; c--){
//         p1 = this->image1->GetPixel(c, r);
//         image.SetPixel(p1, r, c);
//      }
//   }
//   for(int r = 0; r < height; r++){
//      for(int c = 0; c < width; c++){
//         image1->GetPixel(r, c);
//         image.SetPixel(p1, c, r);
//      }
//   }
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         image.SetPixel(image1->GetPixel(r, c), c, height-1-r);
      }
   }
}

const char* Rotate::FilterName(){
   char msg[128];
   sprintf(msg, "Rotate");
   return strdup(msg);
}

//Grayscale
void Grayscale::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   image.SetMaxVal(this->image1->GetMaxVal());
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(height);
   image.SetWidth(width);
   image.AllocateBufferSpace();
   Pixel p1;
   Pixel p2;
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         p1 = image1->GetPixel(r, c);
         unsigned char grayscaleval = (unsigned char)(int(p1.rgb[0]/5) + int(p1.rgb[1]/2) + int(p1.rgb[2]/4));
         p2.rgb[0] = p2.rgb[1] = p2.rgb[2] = grayscaleval;
         image.SetPixel(p2, r, c);
      }
   }
}

const char* Grayscale::FilterName(){
   char msg[128];
   sprintf(msg, "Grayscale");
   return strdup(msg);
}

//Blur
void Blur::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   image.SetMaxVal(this->image1->GetMaxVal());
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(height);
   image.SetWidth(width);
   image.AllocateBufferSpace();
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         Pixel result;
         if(r != 0 && r != height-1 && c != 0 && c != width-1){
            for(int d = 0; d < 3; d++){
               int val00 = int(image1->GetPixel(r-1, c-1).rgb[d]/8);
               int val10 = int(image1->GetPixel(r-1, c).rgb[d]/8);
               int val20 = int(image1->GetPixel(r-1, c+1).rgb[d]/8);
               
               int val01 = int(image1->GetPixel(r, c-1).rgb[d]/8);
               int val21 = int(image1->GetPixel(r, c+1).rgb[d]/8);
               
               int val02 = int(image1->GetPixel(r+1, c-1).rgb[d]/8);
               int val12 = int(image1->GetPixel(r+1, c).rgb[d]/8);
               int val22 = int(image1->GetPixel(r+1, c+1).rgb[d]/8);
               
               unsigned char valcombine = (unsigned char)(val00+val10+val20  +  val01+val21  +  val02+val12+val22);
               result.rgb[d] = valcombine;
            }
         }
         else{
            result = image1->GetPixel(r, c);
         }
         image.SetPixel(result, r, c);
      }
   }
}

const char* Blur::FilterName(){
   char msg[128];
   sprintf(msg, "Blur");
   return strdup(msg);
}

//Subtract
void Subtract::Execute(){
   int height = this->image1->GetHeight();
   int width = this->image1->GetWidth();
   
   int height2 = this->image2->GetHeight();
   int width2 = this->image2->GetWidth();
   if(width != width2 || height != height2)
   {
      char msg[1024];
      sprintf(msg, "%s: dimensions must match: image1 = %dx%d, image2 = %dx%d", SinkName(), width, height, width2, height2);
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   int maxval1 = image1->GetMaxVal();
   int maxval2 = image2->GetMaxVal();
   if(maxval1 > maxval2){
      image.SetMaxVal(maxval1);
   }
   else{
      image.SetMaxVal(maxval2);
   }
   image.SetMagicNum(this->image1->GetMagicNum());
   image.SetHeight(height);
   image.SetWidth(width);
   image.AllocateBufferSpace();
   Pixel result;
   for(int r = 0; r < height; r++){
      for(int c = 0; c < width; c++){
         for(int d = 0; d < 3; d++){
            int val1 = int(image1->GetPixel(r, c).rgb[d]);
            int val2 = int(image2->GetPixel(r, c).rgb[d]);
            int val3 = val1 - val2;
            if(val3 < 0){
               val3 = 0;
            }
            unsigned char valcombine = (unsigned char)(val3);
            result.rgb[d] = valcombine;
         }
         image.SetPixel(result, r, c);
      }
   }
}

void Subtract::Update(){
   char msg[128];
   sprintf(msg, "%s: about to update input1", SourceName());
   Logger::LogEvent(msg);
   CheckImage1();
   image1->Update();
   sprintf(msg, "%s: done updating input1", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to update input2", SourceName());
   Logger::LogEvent(msg);
   CheckImage2();
   image2->Update();
   sprintf(msg, "%s: done updating input2", SourceName());
   Logger::LogEvent(msg);
   
   sprintf(msg, "%s: about to execute", SourceName());
   Logger::LogEvent(msg);
   this->Execute();
   sprintf(msg, "%s: done executing", SourceName());
   Logger::LogEvent(msg);
}

const char* Subtract::FilterName(){
   char msg[128];
   sprintf(msg, "Subtract");
   return strdup(msg);
}

//Color
Color::Color(int width, int height, int r, int g, int b){
   this->image.SetMagicNum("P6");
   this->image.SetWidth(width);
   this->image.SetHeight(height);
   this->image.SetMaxVal(255);
   this->image.AllocateBufferSpace();
   
   Pixel current_pixel;
   for(int i = 0; i < height; i++){
      for(int c = 0; c < width; c++){
         current_pixel.rgb[0] = (unsigned char)r;
         current_pixel.rgb[1] = (unsigned char)g;
         current_pixel.rgb[2] = (unsigned char)b;
         this->image.SetPixel(current_pixel, i, c);
      }
   }
   this->image.SetSource(this);
}

const char* Color::SourceName(){
   char msg[128];
   sprintf(msg, "Color");
   return strdup(msg);
}

//void Color::Update(){
//   char msg[128];
//   sprintf(msg, "%s: updating", SourceName());
//   Logger::LogEvent(msg);
//}

//CheckSum
void CheckSum::OutputCheckSum(char *filename){
   // open file for writing
   FILE *f_out = fopen(filename, "wb");
   if (f_out == NULL)
   {
      char msg[1024];
      sprintf(msg, "%s: error opening checksum file", SinkName());
      DataFlowException e(SinkName(), msg);
      throw e;
   }
   
   int checksum[3] = {0};
   // print each pixel to file
   for(int r = 0; r < this->image1->GetHeight(); r++){
      for(int c = 0; c < this->image1->GetWidth(); c++){
         Pixel p = this->image1->GetPixel(r, c);
         for(int i = 0; i < 3; i++){
            checksum[i] += p.rgb[i];
         }
      }
   }
   fprintf(f_out, "CHECKSUM: %d, %d, %d\n", checksum[0]%256, checksum[1]%256, checksum[2]%256);
   
   
   // close file
   fclose(f_out);
}

const char* CheckSum::SinkName(){
   char msg[128];
   sprintf(msg, "CheckSum");
   return strdup(msg);
}
