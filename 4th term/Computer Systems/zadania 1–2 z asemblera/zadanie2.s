	.text
	.global	clz
	.type	clz, @function
clz:
    xor %eax, %eax
    
    # Ponizszy algorytm nie zadziala poprawnie dla zera, podajac wynik mniejszy o 1
    cmpq $0, %rdi
    je return
    
    # Jesli wiodacych zer jest co najmniej 32, to liczba jest mniejsza od $0x00000000ffffffff
    # W takim razie wynikiem bedzie 32 + liczba wiodacych zer poczawszy od 32 bitu
    # Jesli nie, wiadomo, ze prefix skladajacy sie wylacznie z zer konczy sie przed 32 bitem
    mov $0x00000000ffffffff, %rsi
    cmp %rsi, %rdi
    ja skip1
    add $32, %eax
    shl $32, %rdi
    
    # Jesli wiodacych zer jest co najmniej 16, to liczba jest mniejsza od $0x0000ffffffffffff
    # W takim razie wynikiem bedzie 16 + liczba wiodacych zer poczawszy od 16 bitu
    # Jesli nie, wiadomo, ze prefix skladajacy sie wylacznie z zer konczy sie przed 16 bitem
    skip1:
    mov $0x0000ffffffffffff, %rsi
    cmp %rsi, %rdi
    ja skip2
    add $16, %eax
    shl $16, %rdi
    
    skip2:
    mov $0x00ffffffffffffff, %rsi
    cmp %rsi, %rdi
    ja skip3
    add $8, %eax
    shl $8, %rdi
    
    
    skip3:
    mov $0x0fffffffffffffff, %rsi
    cmp %rsi, %rdi
    ja skip4
    add $4, %eax
    shl $4, %rdi  
    
    
    skip4:
    mov $0x3fffffffffffffff, %rsi
    cmp %rsi, %rdi
    ja skip5
    add $2, %eax
    shl $2, %rdi
    
    
    skip5:
    mov $0x7fffffffffffffff, %rsi
    cmp %rsi, %rdi
    ja skip6
    add $1, %eax
    
    skip6:
    ret
    
    return:
    mov $64, %eax
    ret


	.size	clz, .-clz

