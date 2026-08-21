section .text

; definisco la funzione strlen,per la quale ho bisogno di usare i :
; trattandosi di una leaf function (una funzione che non effettua alcuna call verso altre funzioni), non ho bisogno di allocare stack frame o preoccuparmi dell'allineamento a 16 byte.
; per prima cosa azzero il registro rax che incrementero' come accumulatore per calcolare la lunghezza della stringa
; e che alla fine con ret ritornero' al chiamante(il main in cui la funzione verra' evocata)
; potrei azzerarlo scrivendo 0 in rax con mov rax , 0  ,ma questo mnemonico verrebbe tradotto in una opcode di 7 bytes
; e' molto piu' efficiente sfruttare il comportamento dei registri general purpose a 64 bit quando si eseguono scritture
; nei sottoregistri a 32 bit,come eax di rax, caso in cui i 32 bit alti vengono azzerati immediatamente
; quindi facendo lo xor di eax con se stesso,e sfruttando il fatto che xorare un numero con se stesso restituisce zero,scrivo
; zero nel registro rax con un opcode di soli 2 bytes
; perche' devo azzerare rax all'inizio di una funzione? all'ingresso di una funzione i registri non vengono mai azzerati automaticamente dall'hw o dall'os
; contengono spazzatura,ovvero i dati residui lasciate dalle istruzioni eseguite prima della call fatta dal chiamante
; i registri infatti sono locazioni fisiche condivise, un unico set per tutti,e quando il chiamante esegue call ft_strlen il controllo passa alla label della funzione
; ma lo stato fisico del chip resta immutato: in RAX rimane qualunque cosa il chiamasse stesse manipolando prima,e Rax e' un registro volatile(caller-saved),per cui il chiamante assume
; che il dev possa sovrascriverlo liberamente e non si occupa di pulirlo prima di invocare la funzione

; una volta azzerato rax, definisco la label loop,che eseguira' di seguito le seguenti operazioni. E' una local label,cioe' un etichetta locale,legata alla piu' recente etichetta globale definita
; sopra di essa,in questo caso ft_strlen,quindi deve essere preceduta dal punto: per l'assembler .loop diventa internamente ft_strlen.loop
; la label compara un byte del valore puntato da rdi(dove viene caricato il primo argomento della funzione) spostato di rax bytes
; e legge quel valore come byte singolo,cioe' essenzialmente come un char,per poi compararlo con zero,cioe' con il \0
; jz(jump if zero) e' uno mnemonico che esegue un salto condizionale alla label indicata solo se il registro ZF(zero flags) e' impostato a 1
; ossia se il compare precedente ha dato 0 come risultato(cioe' dst e src sono uguali)
; in tal caso infatti ho trovato il null terminator e devo uscire dal loop,eseguendo un salto alla etichetta .done,che nasconde l'indirizzo
; della mnemonica ret, che ritorna al chiamante il valore di [rax], che e' proprio l'accumulatore che abbiamo usato per contare la lunghezza
; se invece ZF e' 0,cioe' il compare ha dato un valore diverso da 0, allora jz viene ignorata,rax incrementato di 1 con inc e poi si continua
; il ciclo saltando all'etichetta loop

ft_strlen:
    xor eax , eax
.loop:
    cmp byte [rdi + rax], 0
    jz .done
    inc rax
    jmp .loop

.done:
    ret

; questo e' solo uno dei due modi di gestire il loop: l'altro e' usare un puntatore mobile incrementando direttamente RDI ad ogni ciclo(inc rdi)
; e usando un secondo registro per contare quanti passaggi ho fatto


; implementazione alternativa dove,invece di eseguire due jump ad ogni iterazione,sposto la
; condizione alla fine del blocco, risparmiando un istruzione di salto per iterazione(un trucco noto come bottom-test loop)

;ft_strlen:
;    xor eax, eax
    jmp .check
;.loop:
;    inc rax
;.check:
;    cmp byte [rdi + rax], 0
;    jnz .loop
;    ret