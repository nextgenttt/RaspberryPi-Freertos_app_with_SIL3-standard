//mem.c
//standard memory functions to avoid using incompatible libraries on ARM

#include <FreeRTOS.h>
#include <mem.h>

void *memset(void *s, int c, size_t n){
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

void *memmove(void *dest, const void *src, size_t n){
    unsigned char tmp[n];
    memcpy2(tmp,src,n);
    memcpy2(dest,tmp,n);
    return dest;
}

void *memcpy2(void *dest, const void *src, size_t n){
	/*unaligned word moves don't work
	for sizes not a multiple of 4,
	this function does not work
	just use a loop inline*/
    char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
	return dest;
}

int memcmp(const void* s1, const void* s2, size_t n){
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

char *strcpy(char *dest, const char* src){
    char *ret = dest;
    while (*dest++ = *src++)
        ;
    return ret;
}

char *strncpy(char *dest, const char *src, size_t n){
    char *ret = dest;
    do {
        if (!n--)
            return ret;
    } while (*dest++ = *src++);
    while (n--)
        *dest++ = 0;
    return ret;
}

size_t strlen(const char *s){
    size_t i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

//this is not random at all
int next = 1;
int rand(){return (int)((next = next * 1103515245 + 12345) % ((unsigned long)32767 + 1));}