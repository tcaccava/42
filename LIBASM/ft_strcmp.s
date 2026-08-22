global ft_strcmp

segment .text

ft_strcmp :
xor eax,eax    ; in realta' superfluo perche' movzx, scrivendo 32 bit su eax,resettera' automaticamente anche i 32bit msb di rax
.loop :
    cmp byte [rdi], 0  ; verifico se *s1 e' \0
    jz .end            ; in tal caso esco e restituisco la differenza tra i due caratteri correnti castati ad unsigned char
    cmp byte [rsi], 0  ; stesso per *s2
    jz .end
    movzx eax, byte [rdi]   ; sposto con zero extension un byte da rdi(s1) ad eax
    movzx ecx, byte [rsi]   ; sposto con zero extension un byte da rsi(s2) ad ecx
    cmp eax ,ecx            ; comparo i due bytes estratti
    jnz .end                ; se differenti restituisco la differenza lessicografica e ritorno
    inc rdi                 ; se uguali avanzo i puntatori delle due stringhe
    inc rsi
    jmp .loop               ; rientro nel loop

.end :
    ; se ho raggiunto questa label vuol dire che o ho raggiunto il terminatore di una delle stringhe o ho trovato una differenza(la prima) nei caratteri
    movzx eax, byte [rdi]   ; in entrambi i casi devo calcolare la differenza e ritornare, uso eax cosi' che sub salvi la differenza direttamente nel registro di ritorno
    movzx ecx, byte [rsi]   ; carico l'altro valore in ecx perche' volatile,quindi caller saved,e posso disporne a piacimento
    sub eax, ecx            ; sub calcola la differenza(che sia positiva o negativa) e la piazza in eax
    ret                     ; ritorno al chiamante che intercettera' il valore della differenza dal registro rax


