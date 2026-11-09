#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <elf.h>
#include <stdio.h>

int ft_memcmp(void *m1, void *m2, int n)
{
    if (!m1 || !m2)
        return -1;
    unsigned char *mem1 = (unsigned char *)m1; // casto i void * ad unsigned char per poterli dereferenziare una volta assegnatogli un tipo
    unsigned char *mem2 = (unsigned char *)m2;
    int i = 0;
    while ((i < n) && mem1[i] == mem2[i])
        i++;
    if (i == n)
        return 0;             // buffer di memoria uguali per n bytes
    return mem1[i] - mem2[i]; // diversi,ritorno diff
}

int h_parser(Elf64_Ehdr *header)
{
    unsigned char magic[4] = {127, 'E', 'L', 'F'}; // magic number,primi 4 bytes dell'header di un file ELF
    if (ft_memcmp(header->e_ident, magic, 4))      // verifico che il magic number sia presente
        return 0;
    if ((header->e_ident[4] != ELFCLASS64 || header->e_ident[5] != ELFDATA2LSB)) // verifico che sia una eseguibile per sistemi a 64bit con little endianess(lsb con indirizzi > di msb)
        return 0;
    if (header->e_type != ET_EXEC && header->e_type != ET_DYN) // verifico che sia un eseguibile statico o dinamico
        return 0;
    if (header->e_machine != EM_X86_64) // verifico che sia un eseguibile per architettura x86-64
        return 0;
    return 1;
}

int p_parser(Elf64_Phdr *ptable, int segments, off_t end, off_t *pt_load_offset, size_t *pt_load_filesz)
{
    int i = 0;
    while (i < segments)
    {
        int seg_size = ptable[i].p_offset + ptable[i].p_filesz;
        if (seg_size > end)
            return 0;
        if (ptable[i].p_type != PT_LOAD)
            continue;
        if (ptable[i].p_type == PT_LOAD && ptable[i].p_flags == PF_X)
        {
            *pt_load_offset = ptable[i].p_offset;
            *pt_load_filesz = ptable[i].p_filesz;
            return 1;
        }
        i++;
    }
}

int main(int c, char **v)
{
    if (c != 2)
        return 1;
    int fd = open(v[1], O_RDWR); // apro il file con permessi di lettura/scrittura perche' avro' in seguito bisogno di modificarlo iniettandovi lo stub
    if (fd < 0)
        return 1;
    off_t end = lseek(fd, 0, SEEK_END);        // non posso usare fstat perche' vietata,quindi calcolo la size del file sul disco usando lseek da un offset 0 a EOF
    if (end == -1 || end < sizeof(Elf64_Ehdr)) // se la size del file e' inferiore a 64 bytes,non puo' essere per certo un elf valido
        return 1;
    void *mapped = mmap(NULL, (size_t)end, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); // mappo il file in ram con permessi RW e modifiche private(cioe' limitate alla ram)
    if (mapped == MAP_FAILED)                                                           // mapping fallito
    {
        close(fd);
        return 1;
    }
    Elf64_Ehdr *header = (Elf64_Ehdr *)mapped;
    if (!h_parser(header))
    {
        munmap(mapped, (size_t)end);
        close(fd);
        return 1;
    }
    Elf64_Phdr *ph_table_start = (Elf64_Phdr *)mapped + header->e_phoff;
    // un header malformato potrebbe dichiarare falsamente un numero maggiorato di struct e portarmi a leggere out-of-bounds in memoria
    size_t ph_table_end = header->e_phoff + (header->e_phnum * sizeof(Elf64_Phdr)); // verifico che la fine della Program Table sia contenuta nei limiti della size del file
    if (ph_table_end > (size_t)end)
    {
        munmap(mapped, (size_t)end);
        close(fd);
        return 1;
    }
    munmap(mapped, (size_t)end);
    close(fd);
    return 0;
}
