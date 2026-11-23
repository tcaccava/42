#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <elf.h> // per accedere ai tipi delle struct contenute nelle Tables
#include <stdio.h>
#include "stub.h" // header generato dal xxd -i sul sorgente assembly dello stub
#include <stdint.h>

int ft_memcmp(void *m1, void *m2, int n)
{
    if (!m1 || !m2)
        return -1;
    unsigned char *mem1 = (unsigned char *)m1; // casto i void * ad unsigned char per poterli dereferenziare una volta assegnatogli un tipo(non si puo' dereferenziare un void* perche' il compilatore non conosce la dimensione del tipo che dovrebbe leggere)
    unsigned char *mem2 = (unsigned char *)m2;
    int i = 0;
    while ((i < n) && mem1[i] == mem2[i])
        i++;
    if (i == n)
        return 0;             // buffer di memoria uguali per n bytes
    return mem1[i] - mem2[i]; // diversi,ritorno diff
}

int h_parser(Elf64_Ehdr *header) // verifica la conformita' dell'header agli standard ABI System V
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
    return 1; // header validato
}

int p_parser(Elf64_Phdr *ptable, int segments, off_t end, Elf64_Phdr **exec_phdr) // isola il solo segmento pt_load eseguibile
{
    for (int i = 0; i < segments; i++) // itero su tutti i segmenti della Program Table
    {
        if (ptable[i].p_offset > (size_t)end || ptable[i].p_filesz > (size_t)end - ptable[i].p_offset) // verifiche basilari per cercare un segmento evidentemente malformato
            return 0;
        if (ptable[i].p_type == PT_LOAD && (ptable[i].p_flags & PF_X))
        {
            *exec_phdr = &ptable[i]; // salvo il puntatore al segmento ptload eseguibile,l'unico che verra' mappato in memoria dal kernel con permessi di esecuzione,e quindi adatto ad ospitare lo stub
            return 1;                // la ricerca ha avuto successo
        }
    }
    return 0; // la ricerca e' fallita
}

off_t min(off_t a, off_t b) // ricerca branchless dell'offset minimo
{
    return b ^ ((a ^ b) & -(a < b));
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{

    if (!dst && !src)
        return (NULL);
    size_t i = 0;
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    while (i < n)
    {
        d[i] = s[i];
        i++;
    }
    return (dst);
}

int main(int c, char **v)
{
    if (c != 2) // l'eseguibile accettera' un solo argomento,cioe' il binario in cui iniettare lo stub
        return 1;
    int fd = open(v[1], O_RDONLY); // apro il file con permessi di sola lettura perche' iniettero' lo stub sulla copia privata creata in ram senza alterare l'eseguibile su disco
    if (fd < 0)
        return 1;
    off_t end = lseek(fd, 0, SEEK_END);               // non posso usare fstat perche' vietata da subject,quindi calcolo la size del file sul disco usando lseek da un offset 0 a EOF
    if (end == -1 || end < (off_t)sizeof(Elf64_Ehdr)) // il cast di size ad off_t e' necessario perche' off_t e' un signed int 64,mentre sizeof restituisce sempre un unsigned int 64bit
    {                                                 // se la size del file e' inferiore a 64 bytes,non puo' essere per certo un elf valido,perche' non avrebbe lo spazio nemmeno per il solo header elf
        close(fd);
        return 1;
    }
    void *mapped = mmap(NULL, (size_t)end, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); // mappo il file in ram con permessi RW e modifiche private(cioe' limitate alla ram,senza alcuna modifca all'eseguibile sul disco)
    close(fd);                                                                          // il fd sorgente non serve più una volta fatto mmap
    if (mapped == MAP_FAILED)                                                           // mapping fallito
        return 1;
    Elf64_Ehdr *header = (Elf64_Ehdr *)mapped;
    if (!h_parser(header)) // parsing header fallito
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    // controllo preliminare sulla program table: se il suo offset e' oltre la EOF o lo spazio destinato ai suoi segmenti(numero di segmenti * size della singola struct) supera
    // la distanza tra l'offset della phtable e l'EOF,allora l'elf e' grossolanamente malformato
    if (header->e_phoff > (size_t)end || (header->e_phnum * sizeof(Elf64_Phdr)) > (size_t)end - header->e_phoff)
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    unsigned char *cast = (unsigned char *)mapped + header->e_phoff; // casto ad unsigned char* l'indirizzo dell'header spostato in avanti dell'offset della ph table
    // l'offset e' necessario per fare aritmetica dei puntatori,che devo fare a byte level,cosa non possibile con un puntatore ad una struttura Elf64_ehdr,dove ogni spostamento avverrebbe a colpi di 64 bytes alla volta
    Elf64_Phdr *ph_table_start = (Elf64_Phdr *)cast; // ricasto a elf64phdr l'indirizzo appena spostato
    Elf64_Phdr *exec_phdr;
    if (!p_parser(ph_table_start, header->e_phnum, end, &exec_phdr)) // ricerca del pt_load eseguibile fallita
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    off_t pt_load_end = exec_phdr->p_offset + exec_phdr->p_filesz; // calcolo l'offset della fine del segmento pt_load eseguibile
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
    // ma il problema e' che il segmento pt_load eseguibile RX non e' l'unico ad essere caricato in memoria,ma vengono caricati tutti gli altri
    // Dopo la fine del segmento eseguibile(pt_load_end) possono dunque succedere due cose sul file sul fisco:
    // 1) CASO NORMALE : il file sul disco non finisce a pt_load_end,quindi quando il segmento pt_load eseguibile viene mappato in ram,tra la sua end e la fine della pagina
    // in cui solo lui e' mappato,il linker lascia lo zero padding affinche' i segmenti pt_load successivi possano iniziare con la pagina gia' allineata a multipli di 4096 bytes.
    // in sintesi pt_load_end < page_boundary < EOF
    // 2) EDGE CASE : lo pt_load eseguibile e' l'ultimo segmento del file,quindi pt_load_end < EOF < page_boundary; quando il kernel carica l'eseguibile per eseguirlo
    // allochera' comunque fino a page_boundary,riempendo con zeri l'intervallo tra eof e boundary. Ma il mio packer ha mappato solo i bytes fino a EOF(end) e riscrivera' nel nuovo eseguibile
    // solo end bytes. Tutto quello che sta dopo end non puo' essere toccato in mmap (darebbe SIGBUS) e sopratutto non deve esistere nel nuovo binario woody.
    // Calcolare il min tra il boundary e l'EOF serve nel caso in cui end < page_boundary(come in molti binari stripped privi di section header table,dove il segmento eseguibile
    // termina esattamente a EOF,cioe' end),caso in cui,visto che mmap mappa un oggetto lungo end bytes,se provassi a leggere o scrivere oltre end il kernel finirebbe per uccidere il processo
    // con SIGBUS;il calcolo di min garantisce che codecave size corrisponda esattamente ai bytes che esistono fisicamente nel file mappato
    if (codecave_size < stub_len) // la codecave e' troppo piccola da poter contenere lo stub,eventualita' rarissima che per adesso non prendo in considerazione
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    uint64_t stub_vaddr = exec_phdr->p_vaddr + exec_phdr->p_filesz;     // l'indirizzo a cui devo iniettare lo stub corrisponde esattamente alla fine del segmento pt_load eseguibile
    int64_t oep_delta = (int64_t)header->e_entry - (int64_t)stub_vaddr; // la distanza,ovviamente negativa,tra l'OEP originale e il punto di iniezione dello stub

    unsigned char *cave_ptr = (unsigned char *)mapped + pt_load_end; // per eseguire la copia dallo stub.h all'inizio della codecave ho bisogno di procedere byte level,da qui il cast ad unsigned char
    for (unsigned int i = 0; i < stub_len; i++)                      // stub injection
        cave_ptr[i] = stub[i];

    uint64_t magic = 0xAAAAAAAAAAAAAAAAULL; // vado alla ricerca del placeholder che ho piazzato nello stub.s e che ho appena copiato nell'eseguibile in ram
    unsigned int i = 0;
    while (i <= stub_len - sizeof(uint64_t)) // Scorro lo stub appena copiato nel codecave byte per byte alla ricerca degli 8 byte sentinella
    {
        if (!ft_memcmp(cave_ptr + i, &magic, sizeof(uint64_t))) // trovato l'offset esatto del placeholder
        {
            ft_memcpy(cave_ptr + i, &oep_delta, sizeof(int64_t)); // sostituisco il placeholder con l'oep delta,cioe' la distanza relativa a 64bit che lo stub sommera'
            // a runtime al proprio vaddr base per ricavare l'oep originale
            break;
        }
        i++;
    }

    exec_phdr->p_filesz += stub_len; // perche' modifico anche filesz se l'iniezione dello stub e' avvenuta solo nella memoria ram mappata da mmap?
    // perche' io devo creare un NUOVO eseguibile,woody,dove lo stub fisicamente si trova anche sul disco al posto di alcuni degli zeri di padding
    // quando lancero' woody ,il kernel lo carichera' dal disco in ram leggendo l'header elf: filesz dira' al kernel quanti bytes leggere dal disco e copiare
    // in ram,mentre memsz dira' quanta memoria riservare a quel segmento. Se aumentassi solo memsz lasciando filesz invariato,il kernel ignorerebbe i bytes dello
    // stub salvati nella codecave,e,trovando memsz > filesz,riempirebbe automaticaente la differenza con degli 0 di padding(e' lo stesso meccanismo con cui nasce la sezione .bss)
    exec_phdr->p_memsz += stub_len;
    header->e_entry = stub_vaddr; // nuovo entry point: ricordo che il vecchio entrypoint NON corrisponde al primo byte dell'eseguibile,che fa parte dell'header elf
    // e non e' eseguibile; l'OEP e' l'indirizzo virtuale della funzione _start dell'ambiente di bootstrap della libc che inizializza l'ambiente e poi chiama main
    // quando lancero' il nuovo eseguibile woody il kernel carichera' il programma ad un indirizzo base random e fara' partire l'instruction pointer da base + stub_vaddr
    // Ma terminata l'esecuzione dello stub(che stampa su stdout) la cpu dovra' proseguire col programma originale,ma avendo modificato l'entry point dell'eseguibile sul disco non puo'
    // piu' ricavarlo dall'header elf,per cui l'unico modo e' che il packer abbia salvato nello stub stesso quale e' l'offset tra lo stub_vaddr e l'OEP(lo _start della libc),cioe' l'oep_delta
    int output = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0755); // creo l'eseguibile woody con permessi rwxr-xr-x
    if (output < 0)
    {
        munmap(mapped, (size_t)end);
        return 1;
    }
    ssize_t written = write(output, mapped, (size_t)end); // scrivo in woody il codice del vecchio eseguibile mappato in ram (ma modificato col codice dello stub) ma con OEP modificato
    close(output);
    munmap(mapped, (size_t)end);
    if (written != (ssize_t)end) // verifico di aver copiato nel nuovo eseguibile l'esatta interezza del vecchio
        return 1;
    return 0;
}
