#include "def.h"

void WriteINIString(const char *section,const char *key,const char *value,const char *filename);
void GetINIString(char *section,char *key,char *def,char *dest,int destsize,
                  const char *filename);
Sint5 GetINIInt(char *section,char *key,Sint5 def,const char *filename);
void WriteINIInt(char *section,char *key,Sint5 value,const char *filename);
bool GetINIBool(char *section,char *key,bool def,const char *filename);
void WriteINIBool(char *section,char *key,bool value,const char *filename);
