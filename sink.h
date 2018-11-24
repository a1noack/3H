#ifndef sink_h
#define sink_h

#include <stdio.h>
#include "image.h"

class Sink{
protected:
   Image *image1, *image2;
public:
   Sink(){image1 = 0;image2 = 0;};
   void SetInput(Image *image1){this->image1 = image1;};
   void SetInput2(Image *image2){this->image2 = image2;};
   virtual const char* SinkName() = 0;
};

#endif
