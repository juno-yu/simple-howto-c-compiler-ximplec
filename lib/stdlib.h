#pragma once

void *malloc(int size);
void *calloc(int nmemb, int size);
void *realloc(void *ptr, int size);
void free(void *ptr);
void exit(int status);
void abort();

int atoi(const char *s);
long atol(const char *s);
long strtol(const char *nptr, void *endptr, int base);
unsigned long strtoul(const char *nptr, void *endptr, int base);

int abs(int x);
long labs(long x);
