#include "def.h"

void WriteINIString(const char *section,const char *key,const char *value,const char *filename);
void GetINIString(const char *section, const char *key, const char *def,char *dest,int destsize,
                  const char *filename);
Sint5 GetINIInt(const char *section, const char *key,Sint5 def,const char *filename);
void WriteINIInt(const char *section, const char *key,Sint5 value,const char *filename);
bool GetINIBool(const char *section, const char *key,bool def,const char *filename);
void WriteINIBool(const char *section, const char *key,bool value,const char *filename);
