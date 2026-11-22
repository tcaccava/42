#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <elf.h>
#include <stdio.h>
#include "stub.h"
#include <stdint.h>

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
    if ((header->e_ident[4] != ELFCLASS64 || header->e_ident[5] != ELFDATA2LSB)) // verifico che sia una eseguibile per sistemi a 64bit con little endianess(lsb con indirizzi < di msb)
        return 0;
    if (header->e_type != ET_EXEC && header->e_type != ET_DYN) // verifico che sia un eseguibile statico o dinamico
        return 0;
    if (header->e_machine != EM_X86_64) // verifico che sia un eseguibile per architettura x86-64
        return 0;
    return 1;
}

int p_parser(Elf64_Phdr *ptable, int segments, off_t end, Elf64_Phdr **exec_phdr)
{
    for (int i = 0; i < segments; i++)
    {
        if (ptable[i].p_offset > (size_t)end || ptable[i].p_filesz > (size_t)end - ptable[i].p_offset)
            return 0;
        if (ptable[i].p_type == PT_LOAD && (ptable[i].p_flags & PF_X))
        {
            *exec_phdr = &ptable[i];
            return 1;
        }
    }
    return 0;
}

off_t min(off_t a, off_t b)
{
    return b ^ ((a ^ b) & -(a < b));
}

int main(int c, char **v)
{
    if (c != 2)
        return 1;
    int fd = open(v[1], O_RDONLY); // apro il file con permessi di sola lettura perche' iniettero' lo stub sulla copia privata creata in ram senza alterare l'eseguibile su disco
    if (fd < 0)
        return 1;
    off_t end = lseek(fd, 0, SEEK_END); // non posso usare fstat perche' vietata,quindi calcolo la size del file sul disco usando lseek da un offset 0 a EOF
    if (end == -1 || end < (off_t)sizeof(Elf64_Ehdr))
    { // se la size del file e' inferiore a 64 bytes,non puo' essere per certo un elf valido
        close(fd);
        return 1;
    }
    void *mapped = mmap(NULL, (size_t)end, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); // mappo il file in ram con permessi RW e modifiche private(cioe' limitate alla ram)
    close(fd);                                                                          // il fd sorgente non serve più una volta fatto mmap
    if (mapped == MAP_FAILED)                                                           // mapping fallito
        return 1;
    Elf64_Ehdr *header = (Elf64_Ehdr *)mapped;
    if (!h_parser(header)) // parsing fallito
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    if (header->e_phoff > (size_t)end || (header->e_phnum * sizeof(Elf64_Phdr)) > (size_t)end - header->e_phoff)
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    unsigned char *cast = (unsigned char *)mapped + header->e_phoff; // casto ad unsigned char* l'indirizzo dell'header spostato in avanti dell'offset della ph table
    // l'offset e' necessario per fare aritmetica dei puntatori
    Elf64_Phdr *ph_table_start = (Elf64_Phdr *)cast; // ricasto a elf64phdr l'indirizzo appena spostato
    // stesso identico controllo di pparser: check preventivo per evitare che,in caso di elf malformato,il calcolo di ph_table_end possa andare in overflow
    // con successivo wraparound che potrebbe ingannare il controllo if (ph_table_end > (size_t)end)
    Elf64_Phdr *exec_phdr;
    if (!p_parser(ph_table_start, header->e_phnum, end, &exec_phdr)) // parsing fallito
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    off_t pt_load_end = exec_phdr->p_offset + exec_phdr->p_filesz; // calcolo l'offset della fine del segmento pt_load
    // un header malformato potrebbe dichiarare falsamente un numero maggiorato di struct e portarmi a leggere out-of-bounds in memoria
    // il kernel mappa i segmenti pt_load allineando gli indirizzi a multipli di 4096
    // dal momento che le dimensioni delle pagine sono generalmente 4096 bytes(2 ^ 12),un offset o indirizzo perfettamente allineato deve avere i 12 bit LSB impostati a zero
    // il NOT bitwise di 4095 accende tutti i bit dopo il 12esimo,lasciando i 12 bit lsb impostati a 0
    // fare l'AND bitwise con ~4095 significa in sostanza azzerare i 12 bit piu' bassi di un qualsiasi numero
    // questa operazione in sostanza arrotonda per difetto al multiplo di 4096 piu' vicino
    // ma io ho bisogno di arrotondare per eccesso,cioe' ho bisogno del termine della pagina,per cui aggiungo 4095 prima di applicare la maschera
    // perche' posso usare l'offset del pt_load calcolato sul file sul disco come punto di partenza da cui trovare la fine del segmento in RAM?
    // perch' nei sistemi UNIX X86/64 il kernel impone un vincolo rigido allo standard ELF : p_vaddr % 4096 == p_offset % 4096 ,
    // ovvero la granularita' dell'allineamento deve essere uguale sul disco e in memoria,per cui il segmento ha lo stesso identico offset interno
    // alla pagina sia sul disco che quando mappato da mmap in memoria virtuale
    off_t page_boundary = (pt_load_end + 4095) & ~4095;
    size_t codecave_size = min(page_boundary, end) - pt_load_end; // il zero padding si estende dalla fine del segmento pt_load fino al prossimo limite di pagina
    if (codecave_size < stub_bin_len)
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    uint64_t stub_vaddr = exec_phdr->p_vaddr + exec_phdr->p_filesz;
    int64_t oep_delta = (int64_t)header->e_entry - (int64_t)stub_vaddr;

    unsigned char *cave_ptr = (unsigned char *)mapped + pt_load_end;
    for (unsigned int i = 0; i < stub_bin_len; i++)
        cave_ptr[i] = stub_bin[i];

    uint64_t magic = 0xAAAAAAAAAAAAAAAAULL;
    for (unsigned int i = 0; i <= stub_bin_len - sizeof(uint64_t); i++)
    {
        if (!ft_memcmp(cave_ptr + i, &magic, sizeof(uint64_t)))
        {
            unsigned char *delta_bytes = (unsigned char *)&oep_delta;
            for (size_t b = 0; b < sizeof(int64_t); b++)
                cave_ptr[i + b] = delta_bytes[b];
            break;
        }
    }

    exec_phdr->p_filesz += stub_bin_len;
    exec_phdr->p_memsz += stub_bin_len;
    header->e_entry = stub_vaddr;
    int output = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0755);
    if (output < 0)
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    ssize_t written = write(output, mapped, (size_t)end);
    close(output);
    munmap(mapped, (size_t)end);
    if (written != (ssize_t)end)
        return 1;
    return 0;
}
