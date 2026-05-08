#include "myClass.h"

myClass::myClass()
{
  char str[1000];
  sprintf(str,"In constructor\n");
  Serial.write(str);
}
void myClass::printInternalValue()
{
  char str[1000];
  sprintf(str,"I am in the library version of printInternalValue function %d\n",internalValue);
  Serial.write(str);
}

void myClass::setInternalValue(int val)
{
    internalValue = val;
}
