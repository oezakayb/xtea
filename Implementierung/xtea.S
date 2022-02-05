.intel_syntax noprefix
.global xtea_encrypt_block
.global xtea_decrypt_block
.text

xtea_encrypt_block:
    mov ecx, DWORD PTR [rdi] //v0 = blocks[0]
    mov edx, DWORD PTR [rdi + 4] //v1 = blocks[1]
    mov r8, rdi //blocks
    mov r9, rsi //keys
    xor eax, eax //sum = 0
.L2:
    mov r10d, eax //second sum for sum & 3
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum&3]
    add edi, eax
    mov r11d, edx //second v1 for ciphering v0
    shr r11d, 5
    xor r10d, r10d
    mov r10d, edx //third v1 for ciphering v0
    sal r10d, 4
    xor r10d, r11d
    add r10d, edx
    xor r10d, edi //(third v1 ^ second v1 + v1) ^ (sum + key[sum & 3])
    add ecx, r10d

    sub eax, 1640531527 //sum += delta  1640531527 is the signed 2's complement to sigma

    mov r10d, eax //r10d's current value is not needed anymore so sum is moved to r10d for ciphering v1
    shr r10d, 11
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //keys[sum >> 11 & 3]
    add edi, eax
    mov r11d, ecx //r11d's current value is not needed anymore so second v0 is moved to r11d
    shr r11d, 5
    mov r10d, ecx //r10d's current value is not needed anymore so third v0 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, ecx
    xor r10d, edi //((third v0 ^ second) + v0) ^ (sum + keys[sum >> 11 & 3])
    add edx, r10d

    cmp eax, -1914802624 //sum != 0x9E3779B9 * 64 -1914802624 is the signed 2's complement to sigma * 64
    jne .L2

    mov DWORD PTR [r8], ecx
    mov DWORD PTR [r8 + 4], edx
    ret


xtea_decrypt_block:

    mov ecx, DWORD PTR [rdi] //v0 = blocks[0]
    mov edx, DWORD PTR [rdi + 4] //v1 = blocks[1]
    mov r8, rdi //blocks
    mov r9, rsi //keys
    mov eax, -1914802624 //sum = 0x9E3779B9 * 64 -1914802624 is the signed 2's complement to sigma * 64
.L6:
    mov r10d, eax //second sum for sum >> 11
    shr r10d, 11
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum >> 11 & 3]
    add edi, eax
    mov r11d, ecx //second v0 for ciphering v1
    shr r11d, 5
    mov r10d, ecx //r10d's current value is not needed anymore so third v0 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, ecx
    xor r10d, edi //(second v0 ^ third v0) + v0) ^ (sum + key[sum >> 11 & 3])
    sub edx, r10d

    add eax, 1640531527 // sum -= 0x9E3779B9 1640531527 is the signed 2's complementary to - 0x9E3779B9

    mov r10d, eax //r10d's current value is not needed anymore so sum is moved to r10d
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum & 3]
    add edi, eax
    mov r11d, edx //r11d's current value is not needed anymore so second v1 is moved to r11d
    shr r11d, 5
    mov r10d, edx //r10d's current value is not needed anymore so third v1 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, edx
    xor r10d, edi //(second v1 ^ third v1 + v1) ^ (key[sum & 3] + sum)
    sub ecx, r10d

    test eax, eax //check if eax == 0 (test sets zf to 0 if regDest & regSrc == 0)
    jne .L6

    mov DWORD PTR [r8], ecx
    mov DWORD PTR [r8 + 4], edx
    ret