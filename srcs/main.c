#include "../inc/nm.h"

char detect_symbol_type(Elf64_Sym *symbol)
{
    unsigned char type = ELF64_ST_TYPE(symbol->st_info);
    unsigned char bind = ELF64_ST_BIND(symbol->st_info);

    printf("type: %d, bind: %d\n", type, bind);

    int is_global = bind == STB_GLOBAL;

    switch (type)
    {
    case STT_NOTYPE:
        return ('?');
        break;
    case STT_OBJECT:
        return (is_global ? 'D' : 'd');
        break;
    case STT_FUNC:
        return (is_global ? 'T' : 't');
        break;
    default:
        return ('?');
        break;
    }
}

void handle_elf64(char *ptr)
{
    int number_of_sections;
    Elf64_Ehdr *header;
    Elf64_Shdr *sections_list;
    Elf64_Shdr *string_table_header;
    Elf64_Shdr *section;
    char *section_content;
    Elf64_Sym *symbol_list;

    header = (Elf64_Ehdr *)ptr;
    number_of_sections = header->e_shnum;
    sections_list = (Elf64_Shdr *)(ptr + header->e_shoff);
    string_table_header = &sections_list[header->e_shstrndx - 1];
    char *string_table_content = (char *)(ptr + string_table_header->sh_offset);

    // printf("The number of sections: %d\n", number_of_sections);
    // printf("The string table is at index: %d\n", header->e_shstrndx);

    for (int i = 0; i < number_of_sections; ++i)
    {
        section = (Elf64_Shdr *)(ptr + header->e_shoff + (i * header->e_shentsize));
        section_content = (char *)(ptr + section->sh_offset);

        if (section->sh_type == SHT_SYMTAB)
        {
            // printf("\nSection number %d is a symbols table\n", i);

            int sym_to_print_count = 0;

            int symtab_entries = section->sh_size / sizeof(Elf64_Sym);
            symbol_list = (Elf64_Sym *)section_content;
            for (int j = 0; j < symtab_entries; j++)
            {
                if (symbol_list[j].st_name == 0)
                    continue;
                if (symbol_list[j].st_shndx == SHN_UNDEF ||
                    symbol_list[j].st_shndx == SHN_ABS ||
                    symbol_list[j].st_shndx == SHN_COMMON ||
                    symbol_list[j].st_shndx == SHN_HIRESERVE)
                    continue;
                char *sym_name = (char *)(string_table_content + symbol_list[j].st_name);
                printf("%016lx %c %s\n", symbol_list[j].st_value, detect_symbol_type(&symbol_list[j]), sym_name);
                sym_to_print_count++;
                printf("The symbol is in the section: %s\n", string_table_content + sections_list[symbol_list[j].st_shndx].sh_name);
            }
            printf("The number of symbols printed: %d\n", sym_to_print_count);
        }
    }
}

void nm(char *ptr)
{
    Elf64_Ehdr *header;

    header = (Elf64_Ehdr *)ptr;

    if (header->e_ident[0] == ELFMAG0 &&
        header->e_ident[1] == ELFMAG1 &&
        header->e_ident[2] == ELFMAG2 &&
        header->e_ident[3] == ELFMAG3) // ELF x64
    {
        if (header->e_ident[4] == ELFCLASS64)
            handle_elf64(ptr);
        // else if (header->e_ident[4] == ELFCLASS32)
        //     handle_elf32(ptr);
        // else
        // handle error
    }
}

int main(int ac, char **av)
{
    char *filepath;

    if (ac == 1)
    {
        filepath = "a.out";
    }
    else if (ac == 2)
    {
        filepath = av[1];
    }
    else
    {
        print_err("Usage: ./my_nm [flags] [filepath]\n");
        return EXIT_FAILURE;
    }

    int fd = open(filepath, O_RDONLY);
    if (fd < 0)
    {
        print_err("Error: unable to open the file\n");
        return EXIT_FAILURE;
    }

    struct stat st;

    if (fstat(fd, &st) < 0)
    {
        print_err("Error: unable to get the file size\n");
        return EXIT_FAILURE;
    }

    char *ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (ptr == MAP_FAILED)
    {
        print_err("Error: unable to map the file\n");
        return EXIT_FAILURE;
    }

    nm(ptr);

    return EXIT_SUCCESS;
}