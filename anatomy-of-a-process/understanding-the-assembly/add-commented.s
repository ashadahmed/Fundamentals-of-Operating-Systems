	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 4
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16 ; -- (CPU) Allocate 16 bytes on the stack by moving the stack pointer downwards
	.cfi_def_cfa_offset 16
	mov	w0, #0 ; -- (CPU)
	str	wzr, [sp, #12] ; -- (MEMW) store 0 at addr sp - #12
	mov	w8, #1 ; -- (CPU)
	str	w8, [sp, #8] ; -- (MEMR) store 1 at addr sp - #8
	mov	w8, #2 ; -- (CPU)
	str	w8, [sp, #4] ; -- (MEMW) store 2 at addr sp - #4
	ldr	w8, [sp, #8] ; -- (MEMR) load sp - #8 (1) into w8
	ldr	w9, [sp, #4]; --  (MEMR) load sp - #4 (2) into w9
	add	w8, w8, w9 ; -- (CPU) add 1 + 2, store into w8 (3)
	str	w8, [sp] ; -- (MEMW) store 3 at addr sp addr
	add	sp, sp, #16 ; -- (CPU) Deallocated previous 16 bytes on the stack
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
