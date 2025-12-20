.DATA


.CODE

Check PROC
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
Check ENDP

;---------------------------------------------- asmAdd

asmAdd PROC
push rbp
mov rbp, rsp

xor rbx, rbx
mov bl, byte ptr [rdx]
mov al, bl
imul rbx, 4
add rcx, rbx
mov r10b, byte ptr[rbp+48]
mov r11b, byte ptr[rbp+56]


mov byte ptr[rcx], r8b
inc rcx
mov byte ptr[rcx], r9b
inc rcx
mov byte ptr[rcx], r10b
inc rcx
mov byte ptr[rcx], r11b

inc al
mov byte ptr[rdx], al

pop rbp
ret
asmAdd ENDP

;---------------------------------------------- asmAdd END


;---------------------------------------------- FillDamkaMoves

FillDamkaMoves PROC
push rbp
mov rbp, rsp

;x - r8
;y - r9
;x0 - r10
;y0 - r11
;*len - r12
;len - r13
;temp field - r15
;field - rsi
;AllMoves - rdi

mov rsi, rcx
mov rdi, rdx
mov r12, r8
mov r13b, byte ptr[r8]
mov r8, r9
mov r9, [rbp+48]
and r8, 0FFh
and r9, 0FFh
and r13, 0FFh

mov r10, r8
mov r11, r9

mov rax, 8
mul r8b
add rax, r9
add rsi, rax

mov rax, 4
mul r13b
add rdi, rax

mov r15, rsi

loop1:
	inc r8
	inc r9

	mov rcx, r8
	mov rdx, r9
	call Check
	test rax, rax
	jz next1

	add rsi, 9
	cmp byte ptr[rsi], 0
	jnz next1

	mov byte ptr[rdi], r10b
	inc rdi
	mov byte ptr[rdi], r11b
	inc rdi
	mov byte ptr[rdi], r8b
	inc rdi
	mov byte ptr[rdi], r9b
	inc rdi

	inc r13
	jmp loop1

next1:
	mov r8, r10
	mov r9, r11


mov rsi, r15

loop2:
	dec r8
	inc r9

	mov rcx, r8
	mov rdx, r9
	call Check
	test rax, rax
	jz next2

	sub rsi, 7
	cmp byte ptr[rsi], 0
	jnz next2

	mov byte ptr[rdi], r10b
	inc rdi
	mov byte ptr[rdi], r11b
	inc rdi
	mov byte ptr[rdi], r8b
	inc rdi
	mov byte ptr[rdi], r9b
	inc rdi

	inc r13
	jmp loop2

next2:
	mov r8, r10
	mov r9, r11


mov rsi, r15

loop3:
	inc r8
	dec r9

	mov rcx, r8
	mov rdx, r9
	call Check
	test rax, rax
	jz next3

	add rsi, 7
	cmp byte ptr[rsi], 0
	jnz next3

	mov byte ptr[rdi], r10b
	inc rdi
	mov byte ptr[rdi], r11b
	inc rdi
	mov byte ptr[rdi], r8b
	inc rdi
	mov byte ptr[rdi], r9b
	inc rdi

	inc r13
	jmp loop3

next3:
	mov r8, r10
	mov r9, r11


mov rsi, r15

loop4:
	dec r8
	dec r9

	mov rcx, r8
	mov rdx, r9
	call Check
	test rax, rax
	jz next4

	sub rsi, 9
	cmp byte ptr[rsi], 0
	jnz next4

	mov byte ptr[rdi], r10b
	inc rdi
	mov byte ptr[rdi], r11b
	inc rdi
	mov byte ptr[rdi], r8b
	inc rdi
	mov byte ptr[rdi], r9b
	inc rdi

	inc r13
	jmp loop4

next4:
	mov byte ptr [r12], r13b

pop rbp
ret
FillDamkaMoves ENDP

;---------------------------------------------- FillDamkaMoves END

END