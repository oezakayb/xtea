.intel_syntax noprefix
.global xtea_encrypt_block
.global xtea_decrypt_block
.text

xtea_encrypt_block:
    mov eax, DWORD PTR [rdi] //v0 = blocks[0]
    mov ecx, DWORD PTR [rdi + 4] //v1 = blocks[1]
    mov r8, rdi //blocks
    mov r9, rsi //keys
    xor edx, edx //sum = 0
.Lfor_begin:
    mov r10d, edx //second sum for sum & 3
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum&3]
    add edi, edx
    mov r11d, ecx //second v1 for ciphering v0
    shr r11d, 5
    xor r10d, r10d
    mov r10d, ecx //third v1 for ciphering v0
    sal r10d, 4
    xor r10d, r11d
    add r10d, ecx
    xor r10d, edi //(third v1 ^ second v1 + v1) ^ (sum + key[sum & 3])
    add eax, r10d

    //we could not find a solution for working with a 64-bit int (sigma is nearly at the limit of 32-bit int and sigma * 64 is way over it)
    //because of that we compiled our C implementation with godbolt online compiler with Intirinsics -O2 and then let ourselves get inspired
    //by how the compiler handled it. it used the bottom 32 bits of the 2's complement for the sum values so we did the same

    sub edx, 1640531527 //sum += delta  1640531527 is the signed 2's complement to sigma

    mov r10d, edx //r10d's current value is not needed anymore so sum is moved to r10d for ciphering v1
    shr r10d, 11
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //keys[sum >> 11 & 3]
    add edi, edx
    mov r11d, eax //r11d's current value is not needed anymore so second v0 is moved to r11d
    shr r11d, 5
    mov r10d, eax //r10d's current value is not needed anymore so third v0 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, eax
    xor r10d, edi //((third v0 ^ second) + v0) ^ (sum + keys[sum >> 11 & 3])
    add ecx, r10d

    cmp edx, -1914802624 //sum != 0x9E3779B9 * 64 -1914802624 is the signed 2's complement to sigma * 64 's bottom 32-bits
    jne .Lfor_begin

    mov DWORD PTR [r8], eax
    mov DWORD PTR [r8 + 4], ecx
    ret


xtea_decrypt_block:

    mov eax, DWORD PTR [rdi] //v0 = blocks[0]
    mov ecx, DWORD PTR [rdi + 4] //v1 = blocks[1]
    mov r8, rdi //blocks
    mov r9, rsi //keys
    mov edx, -1914802624 //sum = 0x9E3779B9 * 64 -1914802624 is the signed 2's complement to sigma * 64 's bottom 32-bits
.Lfor_begin2:
    mov r10d, edx //second sum for sum >> 11
    shr r10d, 11
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum >> 11 & 3]
    add edi, edx
    mov r11d, eax //second v0 for ciphering v1
    shr r11d, 5
    mov r10d, eax //r10d's current value is not needed anymore so third v0 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, eax
    xor r10d, edi //(second v0 ^ third v0) + v0) ^ (sum + key[sum >> 11 & 3])
    sub ecx, r10d

    add edx, 1640531527 // sum -= 0x9E3779B9 1640531527 is the signed 2's complementary to - 0x9E3779B9

    mov r10d, edx //r10d's current value is not needed anymore so sum is moved to r10d
    and r10d, 3
    mov edi, DWORD PTR [r9 + r10 * 4] //key[sum & 3]
    add edi, edx
    mov r11d, ecx //r11d's current value is not needed anymore so second v1 is moved to r11d
    shr r11d, 5
    mov r10d, ecx //r10d's current value is not needed anymore so third v1 is moved to r10d
    sal r10d, 4
    xor r10d, r11d
    add r10d, ecx
    xor r10d, edi //(second v1 ^ third v1 + v1) ^ (key[sum & 3] + sum)
    sub eax, r10d

    cmp edx, 0
    jne .Lfor_begin2

    mov DWORD PTR [r8], eax
    mov DWORD PTR [r8 + 4], ecx
    ret