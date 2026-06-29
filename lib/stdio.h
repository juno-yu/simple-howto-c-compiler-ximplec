#pragma once

#define NULL ((void*)0)
#define EOF (-1)

typedef struct FILE FILE;

FILE *stdin;
FILE *stdout;
FILE *stderr;

int printf(const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);
int snprintf(char *str, int n, const char *fmt, ...);
int scanf(const char *fmt, ...);

FILE *fopen(const char *pathname, const char *mode);
int fclose(FILE *stream);
int fread(void *ptr, int size, int nmemb, FILE *stream);
int fwrite(const void *ptr, int size, int nmemb, FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int fputs(const char *s, FILE *stream);
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int fflush(FILE *stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
