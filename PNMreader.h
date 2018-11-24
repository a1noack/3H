#ifndef pnmreader_h
#define pnmreader_h

#include <stdio.h>
#include "source.h"

class PNMreader : public Source{
private:
   char *filename;
public:
   PNMreader(char *filename);
   ~PNMreader();
   const char *SourceName();
   void Execute();
};

#endif
