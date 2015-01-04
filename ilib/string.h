//Filename:		string.h
//Author:		Ihsoh
//Date:			2014-1-29
//Descriptor:	String functions

#ifndef	_STRING_H_
#define	_STRING_H_


//Standard:
#define	strlen(str) (_strlen((str)))
extern unsigned int _strlen(const char * str);
#define strncpy(dst, src, count) (_memcpy((dst), (src), (count)))
#define memcpy(dst, src, count) (_memcpy((dst), (src), (count)))
extern void * _memcpy(void * dst, const void * src, unsigned int count);
#define	strcpy(dst, src) (_strcpy((dst), (src)))
extern char * _strcpy(char * dst, const char * src);
#define	strcat(dst, src) (_strcat((dst), (src)))
extern char * _strcat(char * dst, const char * src);
#define strrev(str) (_strrev((str)))
extern char * _strrev(char * str);
#define strcmp(str1, str2) (_strcmp((str1), (str2)))
extern int _strcmp(const char * str1, const char * str2);
#define strcmpn(str1, str2, n) (_strcmpn((str1), (str2), (n)))
extern int _strcmpn(const char * str1, const char * str2, unsigned int n);

//Extersion:
extern char * uctohexs(char * str, unsigned char n);
extern char * ustohexs(char * str, unsigned short n);
extern char * uitohexs(char * str, unsigned int n);
#define ultohexs(str, n) (uitohexs((str), (n)))
extern char * ultos(char * str, unsigned long n);
#define	uitos(str, n) (ultos((str), (unsigned long)(n)))
extern char * ltos(char * str, long n);
#define itos(str, n) (ltos((str), (long)(n)))
extern char * dtos(char * str, double n);
#define	ftos(str, n) (dtos((str), (n)))
extern unsigned long stoul(char * str);
#define	stoui(str) ((uint)stoul(str))
extern long stol(char * str);
#define	stoi(str) ((int)stol(str))
extern double stod(char * str);
#define	stof(str) ((float)stod(str))
extern void ltrim(char * str);
extern void rtrim(char * str);
extern void trim(char * str);
extern int is_valid_uint(char * str);

#endif
