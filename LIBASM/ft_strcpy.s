global ft_strcpy

section .text

ft_strcpy:
mov rax, rdi         ; salvo l'indirizzo iniziale della stringa dst,che all'inizio della funzione si trova
                     ; caricato nel registro rdi,trattandosi del primo argomento della funzione

.loop:
    cmp byte [rsi], 0 ; comparo ogni byte della src con \0 alla ricerca della fine della stringa
    jz .done          ; salto alla fine solo se ho trovato il null terminator
    mov cl, [rsi]     ; salvo il valore di rsi in un registro di appoggio a 8 bit,scelgo cl al posto di al perche' quest'ultimo
                      ; sovrascriverebbe rax,del cui valore ho bisogno nel ret
    mov [rdi], cl     ; trasferisco il byte letto in dst
    inc rdi           ; avanzo i puntatori delle due stringhe
    inc rsi
    jmp .loop         ; itero nel loop

.done:
    mov cl, [rsi]    ; dst deve essere nulla terminata,quindi copio anche il null terminator
    mov [rdi], cl
    ret              ; restituisco il controllo al chiamante,e ret gli ritornera' un indirizzo che punta all'inizio di dst,che ho salvato in rax
                     ; all'inizio della funzione