	.text
	.global	bitrev
	.type	bitrev, @function
bitrev:

# Algorytmem jest ciag operacji (podobne zadanie wykonywalismy przygotowujac sie do cwiczen):
# zamienienie kolejnoscia bitow wewnatrz kolejnych par bitow, kolejnych czworek bitow, itd.
# Odpowiada to kodowi: 
# unsigned long bitrev(unsigned long rdi){
#    rdi = ((rdi & 0x5555555555555555) << 1 ) | ((rdi & 0xAAAAAAAAAAAAAAAA) >> 1 );
#    rdi = ((rdi & 0x3333333333333333) << 2 ) | ((rdi & 0xCCCCCCCCCCCCCCCC) >> 2 );
#    rdi = ((rdi & 0x0F0F0F0F0F0F0F0F) << 4 ) | ((rdi & 0xF0F0F0F0F0F0F0F0) >> 4 );
#    rdi = ((rdi & 0x00FF00FF00FF00FF) >> 8 ) | ((rdi & 0xFF00FF00FF00FF00) << 8 );
#    rdi = ((rdi & 0x0000FFFF0000FFFF) << 16 ) | ((rdi & 0xFFFF0000FFFF0000) >> 16 );
#    rdi = (rdi << 32) | (rdi >> 32);
#    return rdi;
# }
# Obserwacja: w pojedynczym przypisaniu powyzszej funkcji jedna maske mozna uzyskac z drugiej poprzez przesuniecie bitowe (choc rozwiazanie straci wtedy na czytelnosci).


mov %rdi, %rax
mov $0x5555555555555555, %rsi
and %rsi, %rax
sal %rax
shr %rdi
and %rsi, %rdi
or  %rax, %rdi

mov %rdi, %rax
mov $0x3333333333333333, %rsi
and %rsi, %rax
sal $2, %rax
shr $2, %rdi
and %rsi, %rdi
or  %rax, %rdi

mov %rdi, %rax
mov $0x0F0F0F0F0F0F0F0F, %rsi
and %rsi, %rax
sal $4, %rax
shr $4, %rdi
and %rsi, %rdi
or  %rax, %rdi

mov %rdi, %rax
mov $0x00FF00FF00FF00FF, %rsi
and %rsi, %rax
sal $8, %rax
shr $8, %rdi
and %rsi, %rdi
or  %rax, %rdi

mov %rdi, %rax
mov $0x0000FFFF0000FFFF, %rsi
#sal $16, %rsi
and %rsi, %rax
#mov $0xFFFF0000FFFF0000, %rsi
sal  $16, %rax
shr $16, %rdi
and %rsi, %rdi
#shr $16, %rdi
or %rax, %rdi

mov %rdi, %rax
sal $32, %rax
shr $32, %rdi
or  %rdi, %rax

# mov %rdi, %rax

ret 

	.size	bitrev, .-bitrev

