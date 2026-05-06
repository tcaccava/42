#include <stdio.h>
#include "./utf8_decode.c"
// Controlla se un buffer e' composto da caratteri utf-8 validi.
// Esiste un range di valori proibiti,i cosidetti surrogati,2048 valori da U+D800 a U+DFFF,
// che sono ad uso esclusivo di UTF-16 e sono vietati per evitare ambiguita' e problemi di conversione.
// Ovviamente poi bisogna verificare che il byte inizi con gli header corretti, che i byte di continuazione
// inizino con 10,che non sia superato il limite massimale di U+10FFF,e infine che sia rispettato il limite della cd
// minimalita' ,ovvero che il carattere sia scritto col mino numero di bytes possibile,cioe' che non sia overlong:
// non si tratta solo di una questione di efficienza,ma anche di sicurezza, perche' l'overlong encoding e' stato usato
// storicamente per bypassare i filtri di sicurezza: immaginiamo un server che controlla che nel testo inviato ci sia il carattere /(0x2f)
// per impedire attacchi path traversal. Il parser cerca il byte 0x2F, l'hacker invia il carattere / codificato in due bytes
// C0(che sarebbe 11000000,cioe' il byte iniziale) e AF(cioe' 10101111),che sarebbe il bytes di continuazione
// con il carattere /,che e' 47,il parser non se ne accorge e lascia passare la stringa,quindi il db o l'os decodificano e ottengono
// il carattere /,e si ha un Directory Traversal. Per verificare l'overlong,bisogna controllare che il codepoint sia abbastanza grande
// per il contenitore in cui si trova,ovvero se il carattere usa due bytes,allora il suo codepoint deve essere maggiore di 0x7F,cioe' deve
// essere grande almeno 0x80,e cosi se 3 bytes almeno 0x800,e se 4 bytes almeno 0x10000.

int is_in_range(int codepoint) // verifichiamo che il codepoint non sia compreso nei surrogati proibiti e che non superi il limite massimo di Unicode
{
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF || codepoint > 0x10FFFF)
        return 0;
    return 1;
}

int is_valid_ascii(unsigned char c)
{
    if (c & 128) // se e' un unicode single byte,quindi un ascii,l'MSB deve essere 0
        return 0;
    return 1;
}

int is_valid_two(unsigned char bytes[2])
{
    if (!(bytes[0] >> 5 ^ 6) && (!(bytes[1] >> 6 ^ 2))) // il byte 0 deve iniziare con 110 e il byte 1 con 10
        return 1;
    return 0;
}

int is_valid_three(unsigned char bytes[3])
{
    if (!(bytes[0] >> 4 ^ 14) && (!(bytes[1] >> 6 ^ 2)) && (!(bytes[2] >> 6 ^ 2))) // il bytes 0 inizia con 1110,l'1 con 10,e anche lo zero
        return 1;
    return 0;
}

int is_valid_four(unsigned char bytes[4])
{
    if (!(bytes[0] >> 3 ^ 30) && (!(bytes[1] >> 6 ^ 2)) && (!(bytes[2] >> 6 ^ 2)) && (!(bytes[3] >> 6 ^ 2))) // il byte zero deve iniziare con 11110
        return 1;
    return 0;
}

int is_valid_utf8(unsigned char **s, size_t n) // passiamo il doppio puntatore perche' avremo bisogno di spostare il puntatore stesso del buffer da analizzare
{
    int codepoint = 0;

    while (n)
    {
        int charlen = utf8_charlen(**s);
        if (charlen > n)
            return 0;
        if (charlen < 1 || charlen > 4)
            return 0;
        if (charlen == 1)
        {
            if (!(is_valid_ascii(**s)))
                return 0;
            utf8_decode(*s, &codepoint);
            if (!(is_in_range(codepoint)))
                return 0;
            if (codepoint >= 0x80)
                return 0;
            *s += 1;
            n--;
        }
        else if (charlen == 2)
        {
            if (!(is_valid_two(*s)))
                return 0;
            utf8_decode(*s, &codepoint);
            if (!(is_in_range(codepoint)))
                return 0;
            if (codepoint < 0x80)
                return 0;
            *s += 2;
            n -= 2;
        }
        else if (charlen == 3)
        {
            if (!(is_valid_three(*s)))
                return 0;
            utf8_decode(*s, &codepoint);
            if (!(is_in_range(codepoint)))
                return 0;
            if (codepoint < 0x800)
                return 0;
            *s += 3;
            n -= 3;
        }
        else if (charlen == 4)
        {
            if (!(is_valid_four(*s)))
                return 0;
            utf8_decode(*s, &codepoint);
            if (!(is_in_range(codepoint)))
                return 0;
            if (codepoint < 0x10000)
                return 0;
            *s += 4;
            n -= 4;
        }
    }
    return 1;
}
// oppure ancora piu' concisa,anche se meno leggibile,condensando il tutto con un solo controllo per iterazione del ciclo
// sfruttando il fatto che tutti i bytes di continuazione cominciano con 10
// int is_valid_utf8(unsigned char **s, size_t n)
// {
//     int cp, len;
//     // Tabelle per validare il primo byte in base alla lunghezza
//     unsigned char masks[] = {0x80, 0xE0, 0xF0, 0xF8};
//     unsigned char expect[] = {0x00, 0xC0, 0xE0, 0xF0};
//     unsigned int limits[] = {0x0, 0x80, 0x800, 0x10000};

//     while (n > 0)
//     {
//         len = utf8_charlen(**s);

//         // 1. Validità lunghezza e spazio nel buffer
//         if (len < 1 || len > 4 || (size_t)len > n)
//             return 0;

//         // 2. Validità del PRIMO byte
//         // Applichiamo la maschera e verifichiamo che i bit di controllo siano giusti
//         if ((**s & masks[len - 1]) != expect[len - 1])
//             return 0;

//         // 3. Validità dei byte di CONTINUAZIONE (dal secondo in poi)
//         for (int i = 1; i < len; i++)
//         {
//             if (((*s)[i] >> 6) != 2)
//                 return 0;
//         }

//         // 4. Decodifica e controlli di sicurezza (Overlong e Range)
//         utf8_decode(*s, (unsigned int *)&cp);

//         if (cp < (int)limits[len - 1] || !is_in_range(cp))
//             return 0;

//         *s += len;
//         n -= len;
//     }
//     return 1;
// }
int main()
{
    //valide
    unsigned char *s = "Hello";
    unsigned char *multibyte = "A©€";
    unsigned char max_codepoint[4] = {0xF4, 0x8F, 0xBF, 0xBF};

    unsigned char *mc = max_codepoint;

    //invalide
    unsigned char i[2] = {0xC2, 0x41};
    unsigned char i2[3] = {0xE0, 0x80, 0xAF};
    unsigned char i3[3] = {0xEd, 0xA0, 0x80};
    unsigned char i4[4] = {0xF5, 0x80, 0x80, 0x80};
    unsigned char i5[5] = {0xF8, 0x80, 0x80, 0x80, 0x80};

    unsigned char *p1 = i;
    unsigned char *p2 = i2;
    unsigned char *p3 = i3;
    unsigned char *p4 = i4;
    unsigned char *p5 = i5;
    printf("%d\n", is_valid_utf8(&s, 5));         // 1
    printf("%d\n", is_valid_utf8(&multibyte, 6)); // 1
    printf("%d\n", is_valid_utf8(&mc, 4));        // 1
    printf("%d\n", is_valid_utf8(&p1, 2));        // 0
    printf("%d\n", is_valid_utf8(&p2, 3));        // 0
    printf("%d\n", is_valid_utf8(&p3, 3));        // 0
    printf("%d\n", is_valid_utf8(&p4, 4));        // 0
    printf("%d\n", is_valid_utf8(&p5, 5));        // 0
}