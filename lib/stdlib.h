#pragma once

#define NULL ((void*)0)

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void *malloc(int size);
void *calloc(int nmemb, int size);
void *realloc(void *ptr, int size);
void free(void *ptr);
void exit(int status);
void abort();

int atoi(const char *s);
long atol(const char *s);
long strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

int abs(int x);
long labs(long x);

int rand();
void srand(unsigned int seed);
