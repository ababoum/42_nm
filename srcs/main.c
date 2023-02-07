#include "../inc/nm.h"


void nm(char *ptr)
{
    int magic_number;

    magic_number = *(int *)ptr;

    printf("magic_number: %x\n", magic_number);
    if (magic_number == 0x464c457f)
    {
        printf("The file is an ELF file (x64)\n");
    }
}



int main(int ac, char **av)
{
    char *filepath;

    if (ac == 1) {
        filepath = "a.out";
    } else if (ac == 2) {
        filepath = av[1];
    } else {
        print_err("Usage: ./my_nm [flags] [filepath]\n");
        return EXIT_FAILURE;
    }

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        print_err("Error: unable to open the file\n");
        return EXIT_FAILURE;
    }

    struct stat st;

    if (fstat(fd, &st) < 0) {
        print_err("Error: unable to get the file size\n");
        return EXIT_FAILURE;
    }

    char *ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (ptr == MAP_FAILED) {
        print_err("Error: unable to map the file\n");
        return EXIT_FAILURE;
    }

    nm(ptr);

    return EXIT_SUCCESS;
}