#include "string.h"

inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    for (; len != 0; len--) {
        *dest++ = *src++;
    }
}

inline void memset(void *dest, uint8_t val, uint32_t len)
{
    uint8_t *dst = (uint8_t *)dest;

    for ( ; len != 0; len--) {
        *dst++ = val;
    }
}

inline void bzero(void *dest, uint32_t len)
{
    memset(dest, 0, len);
}

inline int strcmp(const char *str1, const char *str2)
{
    int ret=0;
    while(!(ret=*str1-*str2)&&*str2)
	++str1,++str2;
    return ret;
}

char *strcpy(char *dest, const char *src)
{
    char *tmp=dest;
    if(dest==NULL || src==NULL)
    {
        return NULL;
    }
    while((*tmp++=*src++)!='\0');
    return dest;
}

char *strcat(char *dest, const char *src)
{
    char *tmp=dest;
    while(*tmp!='\0')
        tmp++;
    strcpy(tmp,src);
    return dest;
}

int strlen(const char *src)
{
    int len=0;
    while(*src++!='\0')
        len++;
    return len;
}
