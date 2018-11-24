#ifndef pnmwriter_h
#define pnmwriter_h

#include <stdio.h>
#include "sink.h"

class PNMwriter : public Sink{
public:
   void Write(char *filename);
   const char *SinkName();
};

#endif
