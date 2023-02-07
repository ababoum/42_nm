#ifndef NM_H
#define NM_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <elf.h>


// UTILS (utils.c)
size_t ft_strlen(const char *str);
void print_err(char *str);

#endif