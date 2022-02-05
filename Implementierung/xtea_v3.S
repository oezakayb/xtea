.intel_syntax noprefix
.global xtea_encrypt_block_v3
.global xtea_decrypt_block_v3
.text

//this implementation was not programmed by us. it was compiled via Godbolt Online Compiler from our own implementation
//of V2 in C with Intirinsics optimization -O2

xtea_encrypt_block_v3:
    mov     rcx, rdi //rcx = &sums
    mov     r9, rsi //r9 = &blocks
    mov     edi, DWORD PTR [rsi] //v0 = blocks[0]
    mov     esi, DWORD PTR [rsi+4] //v1 = blocks[1]
    mov     r8, rdx //r8 = &keys
    lea     r10, [rcx+512] //r10 = &sums[64] for for loop comparison
.Lfor_begin:
    mov     rax, QWORD PTR [rcx] //rax = sums[rcx]
    mov     r11d, esi //second v1 for ciphering v0
    add     rcx, 8
    shr     r11d, 5
    mov     rdx, rax
    and     edx, 3 //sums[rcx] & 3
    add     eax, DWORD PTR [r8+rdx*4] //sums[rcx] + keys[sums[rcx] & 3]
    mov     edx, esi //third v1 for ciphering v0
    sal     edx, 4
    xor     edx, r11d
    add     edx, esi //second v1 ^ third v1 + v1
    xor     eax, edx
    add     edi, eax //v0 += second v1 ^ third v1 + v1 ^ sums[rcx] + keys[sums[rcx] & 3]
    mov     rax, QWORD PTR [rcx] //rax = sums[rcx + 1]
    mov     r11d, edi //second v0 for ciphering v1
    mov     rdx, rax
    shr     r11d, 5
    shr     rdx, 11 //sums[rcx + 1] >> 11
    and     edx, 3
    add     eax, DWORD PTR [r8+rdx*4] //eax = sums[rcx] + keys[sums[rcx] >> 11 & 3]
    mov     edx, edi //second v0 for ciphering v1
    sal     edx, 4
    xor     edx, r11d
    add     edx, edi //second v0 ^ third v0 + v0
    xor     eax, edx //second v0 ^ third v0 + v0 ^ sums[rcx] + keys[sums[rcx] >> 11 & 3]
    add     esi, eax //v1 += second v0 ^ third v0 + v0 ^ sums[rcx] + keys[sums[rcx] >> 11 & 3]
    cmp     r10, rcx
    jne     .Lfor_begin
    mov     DWORD PTR [r9], edi //blocks[0] = v0
    mov     DWORD PTR [r9+4], esi //blocks[1] = v1
    ret

xtea_decrypt_block_v3:
    mov     r10, rsi //r10 = &blocks
    push    rbx
    mov     esi, DWORD PTR [rsi] //esi = blocks[0]
    mov     r9, rdi //r9 = &sums
    mov     ecx, DWORD PTR [r10+4] //ecx = blocks[1]
    mov     r8, rdx //r8 = &keys
    lea     rdi, [rdi+504] //sums = &sums[63]
.Lfor_begin2:
    mov     rax, QWORD PTR [rdi+8] //rax = sums[i + 1]
    mov     ebx, esi //second v0 for deciphering v1
    mov     r11, rdi //r11 = &sums
    sub     rdi, 8 //sums--
    shr     ebx, 5
    mov     rdx, rax //rdx = sums[i + 1]
    shr     rdx, 11
    and     edx, 3
    add     eax, DWORD PTR [r8+rdx*4] //sums[i + 1] += keys[sums[i + 1] >> 11 & 3]
    mov     edx, esi //third v0 for deciphering v1
    sal     edx, 4
    xor     edx, ebx
    add     edx, esi //second v0 ^ third v0 + v0
    xor     eax, edx //second v0 ^ third v0 + v0 sums[i + 1] + keys[sums[i + 1] >> 11 & 3]
    sub     ecx, eax //v1 -= second v0 ^ third v0 + v0 sums[i + 1] + keys[sums[i + 1] >> 11 & 3]
    mov     rax, QWORD PTR [rdi+8]
    mov     ebx, ecx
    mov     rdx, rax
    shr     ebx, 5
    and     edx, 3
    add     eax, DWORD PTR [r8+rdx*4]
    mov     edx, ecx
    sal     edx, 4
    xor     edx, ebx
    add     edx, ecx
    xor     eax, edx
    sub     esi, eax
    cmp     r9, r11
    jne     .Lfor_begin2
    mov     DWORD PTR [r10], esi
    pop     rbx
    mov     DWORD PTR [r10+4], ecx
    ret