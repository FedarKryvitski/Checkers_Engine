.DATA

InitB     BYTE  1, 0, 1, 0, 0, 0, 2, 0
          BYTE  0, 1, 0, 0, 0, 2, 0, 2 
          BYTE  1, 0, 1, 0, 0, 0, 2, 0 
     	  BYTE  0, 1, 0, 0, 0, 2, 0, 2 
          BYTE  1, 0, 1, 0, 0, 0, 2, 0
     	  BYTE  0, 1, 0, 0, 0, 2, 0, 2 
    	  BYTE  1, 0, 1, 0, 0, 0, 2, 0 
    	  BYTE  0, 1, 0, 0, 0, 2, 0, 2

AssessD   BYTE  14, 0,  8,  0,  8,  0,  8,  0
		  BYTE  0,  12, 0,  6,  0,  6,  0,  8
		  BYTE  8,  0,  12, 0,  6,  0,  6,  0
		  BYTE  0,  6,  0,  12, 0,  6,  0,  8
		  BYTE  8,  0,  6,  0,  12, 0,  6,  0 
		  BYTE  0,  6,  0,  6,  0,  12, 0,  8
		  BYTE  8,  0,  6,  0,  6,  0,  12, 0 
		  BYTE  0,  8,  0,  8,  0,  8,  0,  14

AssessS   BYTE  8,  0,  9,  0,  9,  0,  12, 0
		  BYTE  0,  8,  0,  9,  0,  10, 0,  10
		  BYTE  8,  0,  10, 0,  11, 0,  14, 0
		  BYTE  0,  9,  0,  11, 0,  12, 0,  10
	  	  BYTE  12, 0,  10, 0,  11, 0,  14, 0
		  BYTE  0,  9,  0,  11, 0,  12, 0,  10
		  BYTE  12, 0,  9,  0,  9,  0,  14, 0
		  BYTE  0,  8,  0,  9,  0,  10, 0,  10


.CODE

CheckCoord PROC
	mov rax, 0
	
	cmp cl, 0
	jl Ending
	cmp cl, 7
	jg Ending

	cmp dl, 0
	jl Ending
	cmp dl, 7
	jg Ending

	mov rax, 1

Ending:

	ret
CheckCoord ENDP


CheckCoord4 PROC
push rbp
mov rbp, rsp

xor rax, rax

cmp cl, 0
jl Ending
cmp cl, 7
jg Ending

cmp dl, 0
jl Ending
cmp dl, 7
jg Ending

cmp r8b, 0
jl Ending
cmp r8b, 7
jg Ending

cmp r9b, 0
jl Ending
cmp r9b, 7
jg Ending

mov rax, 1

Ending:

pop rbp
ret
CheckCoord4 ENDP


GetMode PROC
	push rbp
	mov rbp, rsp

	movsx rax, byte ptr[rbp + 48]

	test rax, rax
	jz Mode0

	cmp rax, 2
	je Mode23

	cmp rax, 3
	je Mode23

	jmp Mode14

Mode0:
	
	cmp rdx, r9
	jng m0y1notBiggery2

		cmp rcx, r8
		jng m0x1notBiggerx2
		mov rax, 4
		jmp Ending

	m0x1notBiggerx2:

		mov rax, 3
		jmp Ending

m0y1notBiggery2:
		
		cmp rcx, r8
		jng m02x1notBiggerx2
		mov rax, 2
		jmp Ending

	m02x1notBiggerx2:

		mov rax, 1
		jmp Ending

Mode14:
	sub rcx, rdx
	sub r8, r9
	cmp rcx, r8
	jg m14Bigger
	jl m14Lower
	jmp Ending

	m14Bigger:
		mov rax, 2
		jmp Ending
	m14Lower:
		mov rax, 3
		jmp Ending
Mode23:
	add rcx, rdx
	add r8, r9
	cmp rcx, r8
	jg m23Bigger
	jl m23Lower
	jmp Ending

	m23Bigger:
		mov rax, 4
		jmp Ending
	m23Lower:
		mov rax, 1

Ending:
	
	pop rbp
	ret
GetMode ENDP

;---------------------------------------------- BInit 

BInit PROC

mov rdi, rcx
lea rsi, OFFSET InitB

mov rcx, 8
rep movsq

ret
BInit ENDP

;---------------------------------------------- BInit END


;---------------------------------------------- memcpy 

BCopy PROC

mov rdi, rcx
mov rsi, rdx
mov rcx, 8
rep movsq

ret
BCopy ENDP

;---------------------------------------------- memcpy END


;---------------------------------------------- PosDamka

PosDamka PROC

mov rsi, rcx
mov rcx, 4

metka:
	cmp	byte ptr[rsi], 2
	jnz noteq1
	mov byte ptr[rsi], 4
noteq1:
	add rsi, 15
	cmp byte ptr[rsi], 1
	jnz noteq2
	mov byte ptr[rsi], 3
noteq2:
	inc rsi
	loop metka
	
ret
PosDamka ENDP

;---------------------------------------------- PosDamka END

;---------------------------------------------- asmMove

Move PROC
push rbp
mov rbp, rsp

and rdx, 0FFh
and r8, 0FFh
and r9, 0FFh
movsx r10, byte ptr[rbp + 48] 

mov rsi, rcx  
mov rdi, rcx

shl rdx, 3     ;imul rdx, 8
add rsi, rdx
add rsi, r8

shl r9, 3      ;imul r9, 8
add rdi, r9
add rdi, r10

mov al, byte ptr[rsi]
mov byte ptr[rdi], al
mov byte ptr[rsi], 0

call PosDamka

pop rbp
ret
Move ENDP

;---------------------------------------------- asmMove END

;---------------------------------------------- asmBeat

Beat PROC
push rbp
mov rbp, rsp

and rdx, 0FFh
and r8, 0FFh
and r9, 0FFh
movsx r10, byte ptr[rbp + 48] 

mov rsi, rcx  
mov rdi, rcx

shl rdx, 3        ;imul rdx, 8
add rsi, rdx
add rsi, r8

shl r9, 3         ;imul r9, 8
add rdi, r9
add rdi, r10

mov al, byte ptr[rsi]
mov byte ptr[rdi], al
mov byte ptr[rsi], 0

cmp rdx, r9
jg Bigger
jmp Lower

Bigger:
	cmp r8, r10
	jg BBig
	jmp BLow
	BBig:
		sub rsi, 9
		jmp Ending
	BLow:
		sub rsi, 7
		jmp Ending
Lower:
	cmp r8, r10
	jg LBig
	jmp LLow
	LBig:
		add rsi, 7	
		jmp Ending
	LLow:
		add rsi, 9

Ending:
	mov byte ptr [rsi], 0

	call PosDamka


pop rbp
ret
Beat ENDP

;---------------------------------------------- asmBeat END

;---------------------------------------------- SMCheck

SMCheck PROC
push rbp
mov rbp, rsp

xor rax, rax

mov rsi, rcx
mov rcx, [rbp + 48]

call CheckCoord4
test rax, rax
jz Ending

and r9, 0FFh
xor rax, rax

shl r9, 3            ;imul r9, 8
add r9b, cl
add rsi, r9

cmp byte ptr[rsi], 0
jnz Ending
mov rax, 1

Ending:

pop rbp
ret
SMCheck ENDP

;---------------------------------------------- SMCheck END

;---------------------------------------------- SBCheck END

SBCheck PROC
push rbp
mov rbp, rsp

mov rsi, rcx                    ;field
mov cl, byte ptr [rbp + 48]     ;y2
mov rdi, rsi

call CheckCoord4
test rax, rax
jz Ending

xor rax, rax

mov r10b, byte ptr [rbp + 56]	;turn
inc r10b
 

and r9, 0FFh
mov r13, r9
shl r13, 3                ;imul r13, 8
add r13b, cl
add rdi, r13
mov r12b, byte ptr[rdi]
test r12b, r12b
jnz Ending

;x1 - dl
;y1 - r8b
;x2 - r9b
;y2 - cl
;temp - r11b
;x0 - r12b
;y0 - r13b


mov r12b, dl
mov r13b, r8b 

cmp dl, r9b
jg B
inc r12b

cmp r8b, cl
jg LB
inc r13b
jmp LOR

LB:
dec r13b
jmp LOR

B:
dec r12b
cmp r8b, cl
jg BB
inc r13b
jmp LOR

BB:
dec r13b

LOR:
and r12, 0FFh
shl r12, 3;           imul r12, 8
add r12b, r13b
add rsi, r12

mov r12b, byte ptr[rsi]
cmp r12b, r10b
je Okey
add r10b, 2
cmp r12b, r10b
je Okey
jmp Ending

Okey:
mov rax, 1

Ending:


pop rbp
ret
SBCheck ENDP

;---------------------------------------------- SBCheck END


;---------------------------------------------- getAssess 

getAssess PROC
push rbp
mov rbp, rsp

mov rbx, rcx
mov rsi, OFFSET AssessS
mov rdi, OFFSET AssessD
mov r15, rsi
add r15, 63

xor r8, r8
xor r9, r9
xor r10, r10
xor r11, r11
xor rax, rax

mov rcx, 64
LoopMetka:

mov dl, byte ptr[rbx]
test dl, dl
jz Next

cmp dl, 1
je Piece1

cmp dl, 2
je Piece2

cmp dl, 3
je Piece3

jmp Piece4

Piece1:
add r8b, byte ptr[rsi]
jmp Next

Piece2:
add r9b, byte ptr[r15]
jmp Next

Piece3:
add r10b, byte ptr[rdi]
jmp Next

Piece4:
add r11b, byte ptr[rdi]

Next:
inc rsi
inc rdi
dec r15
inc rbx
loop LoopMetka

xor r12, r12
add r12, r8 
add r12, r10
mov rax, -100
test r12, r12
jz Ending

xor r12, r12
add r12, r9 
add r12, r11
mov rax, 100
test r12, r12
jz Ending

xor r12, r12
mov r12, r10
sub r12, r11
mov r13, r12      ;imul prepare
shl r12, 2        ;imul r12, 5
add r12, r13      ;imul end
add r12, r8
sub r12, r9
mov rax, r12

Ending:

pop rbp
ret
getAssess ENDP

;---------------------------------------------- getAssess END

;---------------------------------------------- NTBDamka

NTBDamka PROC
push rbp
mov rbp, rsp

;r15 - temp
;r14 - y0
;r13 - x0

;r12 - y
;r11 - x
;r10 - mode
;rsi - field
;rdi - field[xi][yi]


mov rsi, rcx
mov rdi, rcx

xor r15, r15
mov r15b, r9b
inc r15

mov r11b, dl
mov r12b, r8b
and r11, 0FFh
and r12, 0FFh

mov r13, r11
mov r14, r12
mov r10b, byte ptr[rbp+48]

cmp r10b, 4
je next4

xor rax, rax
mov al, r11b		;mov al, 8
shl al, 3			;mul r11b
add al, r12b
add rdi, rax

while4:
	inc r11b
	inc r12b

	cmp r11b, 7
	jg endwhile4

	cmp r12b, 7
	jg endwhile4

	add rdi, 9
	cmp byte ptr[rdi], 0
	jne endwhile4
	jmp while4

endwhile4:

	cmp r11b, 7
	jge next4
	cmp r12b, 7
	jge next4
		
	mov r9b, r15b
	cmp byte ptr[rdi], r9b
	je nextif4

	add r9b, 2
	cmp byte ptr[rdi], r9b
	je nextif4
	jmp next4
	
	nextif4:
		
		add rdi, 9
		cmp byte ptr[rdi], 0
		jne next4
		mov rax, 1
		jmp Ending

next4:

cmp r10b, 3
je next3

mov r11, r13
mov r12, r14
mov rdi, rsi

xor rax, rax
mov al, r11b		;mov al, 8
shl al, 3   		;mul r11b
add al, r12b
add rdi, rax

while3:
	dec r11b
	inc r12b

	cmp r11b, 0
	jl endwhile3

	cmp r12b, 7
	jg endwhile3

	sub rdi, 7
	cmp byte ptr[rdi], 0
	jne endwhile3
	jmp while3

endwhile3:

	cmp r11b, 0
	jle next3
	cmp r12b, 7
	jge next3
		
	mov r9b, r15b
	cmp byte ptr[rdi], r9b
	je nextif3

	add r9b, 2
	cmp byte ptr[rdi], r9b
	je nextif3
	jmp next3
	
	nextif3:
		
		sub rdi, 7
		cmp byte ptr[rdi], 0
		jne next3
		mov rax, 1
		jmp Ending

next3:

cmp r10b, 2
je next2

mov r11, r13
mov r12, r14
mov rdi, rsi

xor rax, rax
mov al, r11b		;mov al, 8
shl al, 3		    ;mul r11b
add al, r12b
add rdi, rax

while2:
	inc r11b
	dec r12b

	cmp r11b, 7
	jg endwhile2

	cmp r12b, 0
	jl endwhile2

	add rdi, 7
	cmp byte ptr[rdi], 0
	jne endwhile2
	jmp while2

endwhile2:

	cmp r11b, 7
	jge next2
	cmp r12b, 0
	jle next2

	mov r9b, r15b
	cmp byte ptr[rdi], r9b
	je nextif2

	add r9b, 2
	cmp byte ptr[rdi], r9b
	je nextif2
	jmp next2
	
	nextif2:
		
		add rdi, 7
		cmp byte ptr[rdi], 0
		jne next2
		mov rax, 1
		jmp Ending

next2:

cmp r10b, 1
je next1

mov r11, r13
mov r12, r14
mov rdi, rsi

xor rax, rax
mov al, r11b		;mov al, 8
shl al, 3           ;mul r11b
add al, r12b
add rdi, rax

while1:
	dec r11b
	dec r12b

	cmp r11b, 0
	jl endwhile1

	cmp r12b, 0
	jl endwhile1

	sub rdi, 9
	cmp byte ptr[rdi], 0
	jne endwhile1
	jmp while1

endwhile1:

	cmp r11b, 0
	jle next1
	cmp r12b, 0
	jle next1
		
	mov r9b, r15b
	cmp byte ptr[rdi], r9b
	je nextif1

	add r9b, 2
	cmp byte ptr[rdi], r9b
	je nextif1
	jmp next1
	
	nextif1:
		
		sub rdi, 9
		cmp byte ptr[rdi], 0
		jne next1
		mov rax, 1
		jmp Ending

next1:

mov rax, 0
Ending:

pop rbp
ret
NTBDamka ENDP

;---------------------------------------------- NTBDamka END


;---------------------------------------------- DamkaBeat

DamkaBeat PROC
push rbp
mov rbp, rsp

;r15 - mode
;r14 - y2
;r13 - x2
;r12 - y1
;r11 - x1

mov r15b, byte ptr[rbp + 56]
mov r14b, byte ptr[rbp + 48]
mov r13b, r9b
mov r12b, r8b
mov r11b, dl
mov rsi, rcx
mov rdi, rcx
xor rcx, rcx

xor rax, rax
mov al, r11b		;mov al, 8
shl al, 3    		;mul r11b
add al, r12b
add rsi, rax

xor rax, rax
mov al, r13b		;mov al, 8
shl al, 3			;mul r13b
add al, r14b
add rdi, rax 

mov al, byte ptr[rsi]
mov byte ptr[rdi], al

cmp r15b, 1
jne next1

mov cl, r13b
add cl, r14b
sub cl, r11b
sub cl, r12b

loop1:
	cmp cl, 0
	jle next
	mov byte ptr[rsi], 0
	add rsi, 9
	inc r11b
	inc r12b
	sub cl, 2
	jmp loop1

next1:
cmp r15b, 2
jne next2

mov cl, r11b
add cl, r14b
sub cl, r12b
sub cl, r13b

loop2:	
	cmp cl, 0
	jle next
	mov byte ptr[rsi], 0
	sub rsi, 7
	dec r11b
	inc r12b
	sub cl, 2
	jmp loop2

next2:
cmp r15b, 3
jne next3

mov cl, r13b
add cl, r12b
sub cl, r11b
sub cl, r14b

loop3:
	cmp cl, 0
	jle next
	mov byte ptr[rsi], 0
	add rsi, 7
	inc r11b
	dec r12b
	sub cl, 2
	jmp loop3

next3:
cmp r15b, 4
jne next

mov cl, r11b
add cl, r12b
sub cl, r13b
sub cl, r14b

loop4:
	cmp cl, 0
	jle next
	mov byte ptr[rsi], 0
	sub rsi, 9
	dec r11b
	dec r12b
	sub cl, 2
	jmp loop4

next:
	xor rcx, rcx
	cmp r11b, r13b
	jl x2
	cmp r12b, r14b
	jl x1y2

	mov cl, r11b
	sub cl, r13b
loop01:
	test cl, cl
	jz Ending
	mov byte ptr[rsi], 0
	sub rsi, 9
	dec cl
	jmp loop01

x1y2:

	mov cl, r11b
	sub cl, r13b
loop02:
	test cl, cl
	jz Ending
	mov byte ptr[rsi], 0
	sub rsi, 7
	dec cl
	jmp loop02

	
x2:
	cmp r12b, r14b
	jl x2y2

	mov cl, r13b
	sub cl, r11b
loop03:
	test cl, cl
	jz Ending
	mov byte ptr[rsi], 0
	add rsi, 7
	dec cl
	jmp loop03

x2y2:
	mov cl, r13b
	sub cl, r11b
loop04:
	test cl, cl
	jz Ending
	mov byte ptr[rsi], 0
	add rsi, 9
	dec cl
	jmp loop04
	

Ending:

pop rbp
ret
DamkaBeat ENDP

;---------------------------------------------- DamkaBeat END


;---------------------------------------------- amountOfDamka

amountOfDamka PROC
push rbp
mov rbp, rsp
mov rsi, rcx

xor rax, rax
mov rcx, 64
Metka:
cmp byte ptr[rsi], 2
jle notDamka

inc rax

notDamka:
inc rsi
loop Metka

pop rbp
ret
amountOfDamka ENDP

;---------------------------------------------- amountOfDamka END

END
