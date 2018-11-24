#ifndef source_h
#define source_h

#include <stdio.h>
#include "image.h"

class Source{
protected:
   Image image;
   virtual void Execute() = 0;
public:
   virtual void Update();
   virtual const char* SourceName() = 0;
   Image* GetOutput(){return &image;};
};

#endif
