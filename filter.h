#ifndef filter_h
#define filter_h

#include <stdio.h>
#include "sink.h"
#include "source.h"

class Filter : public Sink, public Source{
private:
   int max_depth = 1000;
   static int current_depth;
protected:
   virtual void Update();
   virtual void Execute() = 0;
   void CheckImage1();
   void CheckImage2();
public:
   Filter();
   virtual const char* FilterName() = 0;
   const char* SourceName() { return FilterName(); };
   const char* SinkName() { return FilterName(); };
};

class Mirror : public Filter{
public:
   void Execute();
   const char* FilterName();
};

class Rotate : public Filter{
public:
   void Execute();
   const char* FilterName();
};

class Grayscale : public Filter{
public:
   void Execute();
   const char* FilterName();
};

class Blur : public Filter{
public:
   void Execute();
   const char* FilterName();
};

class Subtract : public Filter{
public:
   void Execute();
   void Update();
   const char* FilterName();
};

class Blender : public Filter{
private:
   float factor;
public:
   void Execute();
   void SetFactor(float factor){this->factor = factor;};
   float GetFactor(){return factor;};
   void Update();
   const char* FilterName();
};

class TBCombine : public Filter{
public:
   void Execute();
   void Update();
   const char* FilterName();
};

class LRCombine : public Filter{
public:
   void Execute();
   void Update();
   const char* FilterName();
};

class Shrinker : public Filter{
public:
   void Execute();
   const char* FilterName();
};

class Color : public Source{
public:
   Color(int width, int height, int r, int g, int b);
   
   const char *SourceName();
   
//   void Update();
   void Execute(){};
};

class CheckSum : public Sink{
public:
   void OutputCheckSum(char *filename);
   const char* SinkName();
};

#endif
