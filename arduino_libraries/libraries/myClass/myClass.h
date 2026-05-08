#ifndef MYCLASS_H
#define MYCLASS_H
#include <Arduino.h>

class myClass
{
  public:
    myClass();
    ~myClass() {};
    void printInternalValue();
    void setInternalValue(int val);

  private:
    int internalValue=0;
};

#endif