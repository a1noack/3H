#ifndef image_h
#define image_h

#include <stdio.h>
#include <string.h>

class Source;

// struct for holding the rgb values of each pixel
struct Pixel{
   unsigned char rgb[3];
};

// class for image with associated data and method declarations
class Image{
private:
   int width, height, maxval;
   char magicNum[128];
   Pixel **buffer;
   Source* source;
public:
   // constructors
   Image();
   Image(const Image &original);
   Image(char* filename);
   
   void ResetSize(int width, int height);
   
   // getters
   int GetWidth(){return width;};
   int GetHeight(){return height;};
   int GetMaxVal(){return maxval;};
   char* GetMagicNum(){char* ptr = magicNum;return ptr;};
   Pixel GetPixel(int x, int y){return buffer[x][y];};
   Source* GetSource();
   
   // setters
   void SetWidth(int w){width=w;};
   void SetHeight(int h){height=h;};
   void SetMaxVal(int mv){maxval=mv;};
   void SetMagicNum(char mn[128]){strcpy(magicNum, mn);};
   void SetPixel(Pixel p, int x, int y){buffer[x][y]=p;};
   void AllocateBufferSpace();
   void SetSource(Source* source);
   
   void Update();
   void DeallocateBufferSpace();
};

#endif
