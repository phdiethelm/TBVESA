extrn	DrawCircleAbsolute:proc
extrn	DrawArcAbsolute:proc
extrn	DrawSolidLineAbsolute:proc
extrn	DrawSolidLineRelative:proc
extrn	DrawLineFunction:proc
extrn	DrawDashedLineAbsolute:proc
extrn	FillRectangle:proc
extrn	PutPixel_:proc

extrn	_Scale32:dword
extrn	_Scale32Div2:dword
extrn	_DrawingPositionX:word
extrn	_DrawingPositionY:word
extrn	_WindowStartX:word
extrn	_WindowStartY:word
extrn	_WindowEndX:word
extrn	_WindowEndY:word
extrn	_WindowOriginX:word
extrn	_WindowOriginY:word
extrn	_WindowStartX32:dword
extrn	_WindowStartY32:dword
extrn	_WindowEndX32:dword
extrn	_WindowEndY32:dword
extrn	_LineDrawingMode:word

public	DrawWideLineAbsolute32

DGROUP		GROUP _DATA, _TEXT

_DATA		segment byte public use16 'DATA'

outcode		db 0
outcode2	db 0
word_4A2_3A	dw 0
word_4A2_14C	dw 0
word_4A2_14E	dw 0
word_4A2_152	dw 0
word_4A2_154	dw 0
word_4A2_180	dw 0
word_4A2_182	dw 0
word_4A2_184	dw 0
word_4A2_186	dw 0
word_4A2_188	dw 0
word_4A2_18A	dw 0
word_4A2_18C	dw 0
word_4A2_18E	dw 0
word_4A2_190	dw 0
word_4A2_192	dw 0
word_4A2_194	dw 0
word_4A2_196	dw 0
word_4A2_198	dw 0
word_4A2_19A	dw 0
word_4A2_19C	dw 0
word_4A2_19E	dw 0
word_4A2_1A0	dw 0
word_4A2_1A2	dw 0
word_4A2_1A4	dw 0
word_4A2_1A6	dw 0
word_4A2_1A8	dw 0
word_4A2_1AA	dw 0
word_4A2_1AC	dw 0
word_4A2_1AE	dw 0
word_4A2_1B0	dw 0
word_4A2_1B2	dw 0
word_4A2_1B4	dw 0
word_4A2_1B6	dw 0
word_4A2_1B8	dw 0
word_4A2_1BA	dw 0
word_4A2_1BC	dw 0
word_4A2_1BE	dw 0
word_4A2_1C0	dw 0
word_4A2_1C2	dw 0
word_4A2_1C4	dw 0
word_4A2_1C6	dw 0
word_4A2_1C8	dw 0
word_4A2_1CA	dw 0
word_4A2_1CC	dw 0
word_4A2_1CE	dw 0
word_4A2_1D0	dw 0
word_4A2_1D2	dw 0
word_4A2_1D4	dw 0
word_4A2_1D6	dw 0
word_4A2_1D8	dw 0
word_4A2_1DA	dw 0
word_4A2_1DC	dw 0

dword_4A2_2C8	dd 0
dword_4A2_2CC	dd 0
dword_4A2_2D0	dd 0
dword_4A2_2D4	dd 0
dword_4A2_2D8	dd 0
dword_4A2_2DC	dd 0
dword_4A2_2E0	dd 0
dword_4A2_2E4	dd 0

_DATA		ends

_TEXT		segment byte public use16 'CODE'
		assume cs:DGROUP, ds:DGROUP

.386

DrawWideLineAbsolute32 proc far
		push	bp
		mov	bp, sp

		push	bx
		push	cx
		push	si
		push	di
		push	ds

		mov	ax, seg	DGROUP
		mov	ds, ax

		mov	al, [bp+6]              	; fillType
		cmp	al, 0FFh
		jz	loc_0_2713

		mov	edx, [bp+8]                     ; thickness
		mov	eax, [_WindowStartX32]
		sub	eax, edx
		mov	dword_4A2_2C8, eax

		mov	eax, [_WindowStartY32]
		sub	eax, edx
		mov	dword_4A2_2CC, eax

		mov	eax, [_WindowEndX32]
		add	eax, edx
		mov	dword_4A2_2D0, eax

		mov	eax, [_WindowEndY32]
		add	eax, edx
		mov	dword_4A2_2D4, eax

		mov	outcode2, 0
		mov	eax, [bp+0Ch]           	; yb
		cmp	eax, dword_4A2_2D4
		jle	loc_0_250D

		mov	outcode2, 8
		jmp	short loc_0_2519

loc_0_250D:
		cmp	eax, dword_4A2_2CC
		jge	loc_0_2519
		mov	outcode2, 4

loc_0_2519:
		mov	eax, [bp+10h]			; xb
		cmp	eax, dword_4A2_2D0
		jle	loc_0_252B

		or	outcode2, 2
		jmp	short loc_0_2537

loc_0_252B:
		cmp	eax, dword_4A2_2C8
		jge	loc_0_2537
		or	outcode2, 1

loc_0_2537:
		mov	outcode, 0
		mov	eax, [bp+14h]			; ya
		cmp	eax, dword_4A2_2D4
		jle	loc_0_254E

		mov	outcode, 8
		jmp	short loc_0_255A

loc_0_254E:
		cmp	eax, dword_4A2_2CC
		jge	loc_0_255A

		mov	outcode, 4

loc_0_255A:
		mov	eax, [bp+18h]			; xa
		cmp	eax, dword_4A2_2D0
		jle	loc_0_256C

		or	outcode, 2
		jmp	short loc_0_2578

loc_0_256C:
		cmp	eax, dword_4A2_2C8
		jge	loc_0_2578

		or	outcode, 1

loc_0_2578:
		mov	al, outcode
		or	al, outcode2
		jz	loc_0_2696

		mov	al, outcode
		test	outcode2, al
		jnz	loc_0_270A

		cmp	outcode, 0
		jnz	loc_0_25B8

		mov	eax, [bp+18h]			; xa
		xchg	eax, [bp+10h]			; xb
		mov	[bp+18h], eax			; xa
		mov	eax, [bp+14h]			; ya
		xchg	eax, [bp+0Ch]			; yb
		mov	[bp+14h], eax			; ya
		mov	al, outcode2
		mov	outcode, al
		mov	outcode2, 0

loc_0_25B8:
		test	outcode, 8
		jz	loc_0_25EF

		mov	eax, [bp+10h]			; xb
		sub	eax, [bp+18h]			; xa
		mov	edx, dword_4A2_2D4
		mov	ecx, [bp+14h]			; ya
		sub	edx, ecx
		imul	edx
		mov	ebx, [bp+0Ch]			; yb
		sub	ebx, ecx
		idiv	ebx
		add	[bp+18h], eax                   ; xa
		mov	eax, dword_4A2_2D4
		mov	[bp+14h], eax			; ya
		jmp	loc_0_2537

loc_0_25EF:
		test	outcode, 4
		jz	loc_0_2626
		mov	eax, [bp+10h]			; xb
		sub	eax, [bp+18h]			; xa
		mov	edx, dword_4A2_2CC
		mov	ecx, [bp+14h]			; ya
		sub	edx, ecx
		imul	edx
		mov	ebx, [bp+0Ch]			; yb
		sub	ebx, ecx
		idiv	ebx
		add	[bp+18h], eax			; xa
		mov	eax, dword_4A2_2CC
		mov	[bp+14h], eax			; ya
		jmp	loc_0_2537

loc_0_2626:
		test	outcode, 2
		jz	loc_0_265D
		mov	eax, [bp+0Ch]
		sub	eax, [bp+14h]
		mov	edx, dword_4A2_2D0
		mov	ecx, [bp+18h]
		sub	edx, ecx
		imul	edx
		mov	ebx, [bp+10h]
		sub	ebx, ecx
		idiv	ebx
		add	[bp+14h], eax
		mov	eax, dword_4A2_2D0
		mov	[bp+18h], eax
		jmp	loc_0_2537

loc_0_265D:
		test	outcode, 1
		jz	loc_0_2537
		mov	eax, [bp+0Ch]
		sub	eax, [bp+14h]
		mov	edx, dword_4A2_2C8
		mov	ecx, [bp+18h]
		sub	edx, ecx
		imul	edx
		mov	ebx, [bp+10h]
		sub	ebx, ecx
		idiv	ebx
		add	[bp+14h], eax
		mov	eax, dword_4A2_2C8
		mov	[bp+18h], eax
		jmp	loc_0_2537

loc_0_2696:
		mov	ecx, [_Scale32]
		mov	ebx, [_Scale32Div2]
		mov	eax, [bp+18h]
		sub	eax, [_WindowStartX32]
		cdq
		add	eax, ebx
		adc	edx, 0
		idiv	ecx
		push	ax
		mov	eax, [bp+14h]
		sub	eax, [_WindowStartY32]
		cdq
		add	eax, ebx
		adc	edx, 0
		idiv	ecx
		push	ax
		mov	eax, [bp+10h]
		sub	eax, [_WindowStartX32]
		cdq
		add	eax, ebx
		adc	edx, 0
		idiv	ecx
		push	ax
		mov	eax, [bp+0Ch]
		sub	eax, [_WindowStartY32]
		cdq
		add	eax, ebx
		adc	edx, 0
		idiv	ecx
		push	ax
		mov	eax, [bp+8]
		cdq
		idiv	ecx
		push	ax
		push	word ptr [bp+6]
		call	sub_0_32D4

loc_0_270A:
		pop	ds
		pop	di
		pop	si
		pop	cx
		pop	bx
		pop	bp
		retf	16h

loc_0_2713:
		mov	eax, [bp+8]
		mov	ebx, [bp+0Ch]
		call	loc_0_1155
		jmp	short loc_0_270A
DrawWideLineAbsolute32 endp

loc_0_1155:
		mov	dword_4A2_2E0, eax
		mov	dword_4A2_2E4, ebx
		mov	eax, [_WindowStartX32]
		imul	ebx
		mov	dword_4A2_2C8, eax
		mov	eax, [_WindowStartY32]
		imul	ebx
		mov	dword_4A2_2CC, eax
		mov	eax, [_WindowEndX32]
		imul	ebx
		mov	dword_4A2_2D0, eax
		mov	eax, [_WindowEndY32]
		imul	ebx
		mov	dword_4A2_2D4, eax
		;call	SetPlaneAndModeAndES
		mov	eax, dword_4A2_2CC

loc_0_1191:
		mov	dword_4A2_2DC, eax
		cdq
		idiv	dword_4A2_2E0
		imul	dword_4A2_2E0
		idiv	dword_4A2_2E4
		cmp	eax, [_WindowEndY32]
		jg	locret_0_1226
		sub	eax, [_WindowStartY32]
		jl	loc_0_121A
		cdq
		add	eax, [_Scale32Div2]
		adc	edx, 0
		idiv	[_Scale32]
		add	ax, [_WindowOriginY]
		mov	word_4A2_154, ax
		mov	eax, dword_4A2_2C8

loc_0_11CF:
		mov	dword_4A2_2D8, eax
		cdq
		idiv	dword_4A2_2E0
		imul	dword_4A2_2E0
		idiv	dword_4A2_2E4
		cmp	eax, [_WindowEndX32]
		jg	loc_0_121A
		sub	eax, [_WindowStartX32]
		jl	loc_0_120F
		cdq
		add	eax, [_Scale32Div2]
		adc	edx, 0
		idiv	[_Scale32]
		add	ax, [_WindowOriginX]
		mov	cx, ax
		mov	bx, word_4A2_154
		mov	ax, cx
		mov	dx, bx
		call	PutPixel_

loc_0_120F:
		mov	eax, dword_4A2_2D8
		add	eax, dword_4A2_2E0
		jmp	short loc_0_11CF

loc_0_121A:
		mov	eax, dword_4A2_2DC
		add	eax, dword_4A2_2E0
		jmp	loc_0_1191

locret_0_1226:
		retn

sub_0_32D4 proc far
		push	bp
		mov	bp, sp

		push	bx
		push	cx
		push	si
		push	di
		push	ds

		mov	ax, seg	DGROUP
		mov	ds, ax

		mov	al, [bp+6]
		test	al, 4
		mov	bx, offset cs:DrawSolidLineAbsolute
		mov	_LineDrawingMode, 0
		jz	loc_0_32F0

		mov	bx, offset DrawDashedLineAbsolute
		mov	_LineDrawingMode, 1

loc_0_32F0:
		;mov	word ptr [DrawLineFunction], bx
		and	al, 3
		mov	[bp+6],	al
		mov	ax, [bp+10h]
		mov	bx, [bp+0Eh]
		mov	cx, [bp+0Ch]
		mov	dx, [bp+0Ah]
		push	cx
		push	dx
		mov	si, [bp+8]
		or	si, 1
		mov	[bp+8],	si
		shr	si, 1
		cmp	si, 0
		jnz	loc_0_331A
		jmp	loc_0_3906

loc_0_331A:
		mov	word_4A2_3A, si
		cmp	ax, cx
		jle	loc_0_3323
		xchg	ax, cx

loc_0_3323:
		cmp	bx, dx
		jle	loc_0_3329
		xchg	bx, dx

loc_0_3329:
		sub	ax, si
		cmp	ax, [_WindowEndX]
		jg	loc_0_334B
		add	cx, si
		cmp	cx, [_WindowStartX]
		jl	loc_0_334B
		sub	bx, si
		cmp	bx, [_WindowEndY]
		jg	loc_0_334B
		add	dx, si
		cmp	dx, [_WindowStartY]
		jl	loc_0_334B
		jmp	short loc_0_334E

loc_0_334B:
		jmp	loc_0_36E1

loc_0_334E:
		mov	ax, [bp+10h]
		mov	bx, [bp+0Eh]
		mov	cx, [bp+0Ch]
		mov	dx, [bp+0Ah]
		mov	si, cx
		sub	si, ax
		mov	di, dx
		sub	di, bx
		jnz	loc_0_3367
		jmp	loc_0_3911

loc_0_3367:
		or	si, si
		jnz	loc_0_336E
		jmp	loc_0_39B2

loc_0_336E:
		mov	ax, [bp+10h]
		mov	cx, [bp+0Ch]
		cmp	ax, cx
		jle	loc_0_338A
		mov	[bp+10h], cx
		mov	[bp+0Ch], ax
		mov	ax, [bp+0Eh]
		mov	cx, [bp+0Ah]
		mov	[bp+0Eh], cx
		mov	[bp+0Ah], ax

loc_0_338A:
		mov	ax, [bp+10h]
		sub	ax, [bp+0Eh]
		jns	loc_0_3394
		neg	ax

loc_0_3394:
		mov	cx, [bp+0Ch]
		sub	cx, [bp+0Ah]
		jns	loc_0_339E
		neg	cx

loc_0_339E:
		cmp	ax, cx
		jnz	loc_0_33B5
		mov	ax, [bp+8]
		mov	cx, 1B9Fh
		mul	cx
		mov	cx, 2710h
		div	cx
		mov	si, ax
		mov	di, ax
		jmp	short loc_0_33CC

loc_0_33B5:
		call	sub_0_3C0B
		mov	di, word_4A2_1C2
		or	di, di
		jns	loc_0_33C2
		neg	di

loc_0_33C2:
		mov	si, word_4A2_1C4
		or	si, si
		jns	loc_0_33CC
		neg	si

loc_0_33CC:
		shr	di, 1
		shr	si, 1
		cmp	byte ptr [bp+6], 3
		jnz	loc_0_33D9
		call	sub_0_3D32

loc_0_33D9:
		mov	ax, [bp+10h]
		mov	bx, [bp+0Eh]
		mov	cx, [bp+0Ch]
		mov	dx, [bp+0Ah]
		cmp	bx, dx
		jle	loc_0_3430
		mov	word_4A2_1D6, ax
		mov	word_4A2_1D8, bx
		mov	word_4A2_1DA, cx
		mov	word_4A2_1DC, dx
		add	ax, si
		mov	word_4A2_1C6, ax
		add	bx, di
		mov	word_4A2_1C8, bx
		sub	ax, si
		sub	ax, si
		mov	word_4A2_1CA, ax
		sub	bx, di
		sub	bx, di
		mov	word_4A2_1CC, bx
		add	cx, si
		mov	word_4A2_1CE, cx
		add	dx, di
		mov	word_4A2_1D0, dx
		sub	cx, si
		sub	cx, si
		mov	word_4A2_1D2, cx
		sub	dx, di
		sub	dx, di
		mov	word_4A2_1D4, dx
		jmp	short loc_0_3475

loc_0_3430:
		mov	word_4A2_1DA, ax
		mov	word_4A2_1DC, bx
		mov	word_4A2_1D6, cx
		mov	word_4A2_1D8, dx
		sub	cx, si
		mov	word_4A2_1C6, cx
		add	dx, di
		mov	word_4A2_1C8, dx
		sub	ax, si
		mov	word_4A2_1CA, ax
		add	bx, di
		mov	word_4A2_1CC, bx
		add	cx, si
		add	cx, si
		mov	word_4A2_1CE, cx
		sub	dx, di
		sub	dx, di
		mov	word_4A2_1D0, dx
		add	ax, si
		add	ax, si
		mov	word_4A2_1D2, ax
		sub	bx, di
		sub	bx, di
		mov	word_4A2_1D4, bx

loc_0_3475:
		cmp	byte ptr [bp+6], 1
		jz	loc_0_34DD
		cmp	byte ptr [bp+6], 2
		jz	loc_0_34DD
		cmp	byte ptr [bp+6], 3
		jz	loc_0_34DA
		push	word_4A2_1C6
		push	word_4A2_1C8
		push	word_4A2_1CA
		push	word_4A2_1CC
		push	cs
		call	DrawLineFunction
		push	word_4A2_1CA
		push	word_4A2_1CC
		push	word_4A2_1D2
		push	word_4A2_1D4
		push	cs
		call	DrawLineFunction
		push	word_4A2_1D2
		push	word_4A2_1D4
		push	word_4A2_1CE
		push	word_4A2_1D0
		push	cs
		call	DrawLineFunction
		push	word_4A2_1CE
		push	word_4A2_1D0
		push	word_4A2_1C6
		push	word_4A2_1C8
		push	cs
		call	DrawLineFunction
		jmp	loc_0_36E1

loc_0_34DA:
		jmp	loc_0_36FA

loc_0_34DD:
		mov	ax, word_4A2_1C8
		cmp	ax, word_4A2_1CC
		jnz	loc_0_3541
		mov	cx, word_4A2_1CA
		mov	word_4A2_180, cx
		mov	ax, word_4A2_1D2
		sub	ax, cx
		mov	word_4A2_18C, ax
		mov	word_4A2_190, 1
		mov	word_4A2_184, ax
		shl	ax, 1
		mov	word_4A2_188, ax
		mov	ax, word_4A2_1CC
		mov	word_4A2_182, ax
		mov	cx, word_4A2_1D4
		sub	cx, ax
		mov	word_4A2_18E, cx
		or	cx, cx
		jns	loc_0_351A
		neg	cx

loc_0_351A:
		mov	word_4A2_186, cx
		shl	cx, 1
		mov	word_4A2_18A, cx
		mov	ax, word_4A2_184
		cmp	ax, word_4A2_186
		jle	loc_0_3535
		sub	cx, ax
		mov	word_4A2_192, cx
		jmp	loc_0_35B0

loc_0_3535:
		mov	ax, word_4A2_188
		sub	ax, word_4A2_186
		mov	word_4A2_192, ax
		jmp	loc_0_35B0

loc_0_3541:
		mov	cx, word_4A2_1C6
		mov	word_4A2_180, cx
		mov	ax, word_4A2_1CA
		sub	ax, cx
		mov	word_4A2_18C, ax
		mov	word_4A2_190, 0FFFFh
		neg	ax
		mov	word_4A2_184, ax
		shl	ax, 1
		mov	word_4A2_188, ax
		mov	ax, word_4A2_1C8
		mov	word_4A2_182, ax
		mov	cx, word_4A2_1CC
		sub	cx, ax
		mov	word_4A2_18E, cx
		or	cx, cx
		jns	loc_0_3577
		neg	cx

loc_0_3577:
		mov	word_4A2_186, cx
		shl	cx, 1
		mov	word_4A2_18A, cx
		mov	ax, word_4A2_184
		cmp	ax, word_4A2_186
		jle	loc_0_3592
		sub	cx, ax
		mov	word_4A2_192, cx
		jmp	short loc_0_359C

loc_0_3592:
		mov	ax, word_4A2_188
		sub	ax, word_4A2_186
		mov	word_4A2_192, ax

loc_0_359C:
		cmp	word_4A2_192, 0
		jns	loc_0_35B0
		dec	word_4A2_180
		mov	ax, word_4A2_18A
		add	word_4A2_192, ax
		jmp	short loc_0_359C

loc_0_35B0:
		mov	ax, word_4A2_1C8
		cmp	ax, word_4A2_1D0
		jnz	loc_0_3619
		mov	ax, word_4A2_1CE
		mov	word_4A2_194, ax
		mov	cx, word_4A2_1D2
		sub	cx, ax
		mov	word_4A2_1A0, cx
		mov	word_4A2_1A4, 0FFFFh
		mov	ax, word_4A2_1A0
		neg	ax
		mov	word_4A2_198, ax
		shl	ax, 1
		mov	word_4A2_19C, ax
		mov	ax, word_4A2_1D0
		mov	word_4A2_196, ax
		mov	cx, word_4A2_1D4
		sub	cx, ax
		mov	word_4A2_1A2, cx
		or	cx, cx
		jns	loc_0_35F2
		neg	cx

loc_0_35F2:
		mov	word_4A2_19A, cx
		shl	cx, 1
		mov	word_4A2_19E, cx
		mov	ax, word_4A2_198
		cmp	ax, word_4A2_19A
		jle	loc_0_360D
		sub	cx, ax
		mov	word_4A2_1A6, cx
		jmp	loc_0_3689

loc_0_360D:
		mov	ax, word_4A2_19C
		sub	ax, word_4A2_19A
		mov	word_4A2_1A6, ax
		jmp	loc_0_3689

loc_0_3619:
		mov	cx, word_4A2_1C6
		mov	word_4A2_194, cx
		mov	ax, word_4A2_1CE
		sub	ax, cx
		mov	word_4A2_1A0, ax
		mov	word_4A2_1A4, 1
		mov	ax, word_4A2_1A0
		mov	word_4A2_198, ax
		shl	ax, 1
		mov	word_4A2_19C, ax
		mov	ax, word_4A2_1C8
		mov	word_4A2_196, ax
		mov	cx, word_4A2_1D0
		sub	cx, ax
		mov	word_4A2_1A2, cx
		or	cx, cx
		jns	loc_0_3650
		neg	cx

loc_0_3650:
		mov	word_4A2_19A, cx
		shl	cx, 1
		mov	word_4A2_19E, cx
		mov	ax, word_4A2_198
		cmp	ax, word_4A2_19A
		jle	loc_0_366B
		sub	cx, ax
		mov	word_4A2_1A6, cx
		jmp	short loc_0_3675

loc_0_366B:
		mov	ax, word_4A2_19C
		sub	ax, word_4A2_19A
		mov	word_4A2_1A6, ax

loc_0_3675:
		cmp	word_4A2_1A6, 0
		jns	loc_0_3689
		inc	word_4A2_194
		mov	ax, word_4A2_19E
		add	word_4A2_1A6, ax
		jmp	short loc_0_3675

loc_0_3689:
		push	word_4A2_180
		push	word_4A2_182
		push	word_4A2_194
		push	word_4A2_196
		push	cs
		call	DrawLineFunction
		mov	ax, word_4A2_182
		cmp	ax, word_4A2_1D4
		jz	loc_0_36AE
		call	sub_0_3A53
		call	sub_0_3B2F
		jmp	short loc_0_3689

loc_0_36AE:
		cmp	byte ptr [bp+6], 2
		jnz	loc_0_36E1
		mov	ax, word_4A2_3A
		cmp	ax, 2
		jl	loc_0_36E1
		push	word ptr [bp+0Ch]
		push	word ptr [bp+0Ah]
		push	ax
		push	ax
		mov	ax, 1
		push	ax
		push	cs
		call	DrawCircleAbsolute
		push	word ptr [bp+10h]
		push	word ptr [bp+0Eh]
		mov	ax, word_4A2_3A
		push	ax
		push	ax
		mov	ax, 1
		push	ax
		push	cs
		call	DrawCircleAbsolute

loc_0_36E1:
		pop	[_DrawingPositionY]
		pop	[_DrawingPositionX]
		mov	bx, offset DrawSolidLineAbsolute
		mov	_LineDrawingMode, 0
		;mov	word ptr [DrawLineFunction], bx
		pop	ds
		pop	di
		pop	si
		pop	cx
		pop	bx
		pop	bp
		retf	0Ch

loc_0_36FA:
		push	word_4A2_1D6
		push	word_4A2_1D8
		push	word_4A2_1C6
		push	word_4A2_1C8
		push	word_4A2_1D6
		mov	ax, word_4A2_1D8
		add	ax, word_4A2_3A
		push	ax
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1DA
		push	word_4A2_1DC
		push	word_4A2_1D2
		push	word_4A2_1D4
		push	word_4A2_1DA
		mov	ax, word_4A2_1DC
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		mov	ax, word_4A2_1DA
		cmp	ax, word_4A2_1D6
		jge	loc_0_3758
		jmp	loc_0_382F

loc_0_3758:
		push	word_4A2_1D6
		push	word_4A2_1D8
		push	word_4A2_1D6
		mov	ax, word_4A2_1D8
		add	ax, word_4A2_3A
		push	ax
		mov	ax, word_4A2_1D6
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_1D8
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1D6
		push	word_4A2_1D8
		push	word_4A2_1CA
		push	word_4A2_1CC
		mov	ax, word_4A2_1D6
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_1D8
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1DA
		push	word_4A2_1DC
		push	word_4A2_1CE
		push	word_4A2_1D0
		mov	ax, word_4A2_1DA
		add	ax, word_4A2_3A
		push	ax
		push	word_4A2_1DC
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1DA
		push	word_4A2_1DC
		mov	ax, word_4A2_1DA
		add	ax, word_4A2_3A
		push	ax
		push	word_4A2_1DC
		push	word_4A2_1DA
		mov	ax, word_4A2_1DC
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1C6
		push	word_4A2_1C8
		push	word_4A2_1CE
		push	word_4A2_1D0
		push	cs
		call	DrawLineFunction
		push	word_4A2_1CA
		push	word_4A2_1CC
		push	word_4A2_1D2
		push	word_4A2_1D4
		push	cs
		call	DrawLineFunction
		jmp	loc_0_36E1

loc_0_382F:
		push	word_4A2_1D6
		push	word_4A2_1D8
		push	word_4A2_1D6
		mov	ax, word_4A2_1D8
		add	ax, word_4A2_3A
		push	ax
		mov	ax, word_4A2_1D6
		add	ax, word_4A2_3A
		push	ax
		push	word_4A2_1D8
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1D6
		push	word_4A2_1D8
		push	word_4A2_1CE
		push	word_4A2_1D0
		mov	ax, word_4A2_1D6
		add	ax, word_4A2_3A
		push	ax
		push	word_4A2_1D8
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1DA
		push	word_4A2_1DC
		push	word_4A2_1CA
		push	word_4A2_1CC
		mov	ax, word_4A2_1DA
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_1DC
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1DA
		push	word_4A2_1DC
		mov	ax, word_4A2_1DA
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_1DC
		push	word_4A2_1DA
		mov	ax, word_4A2_1DC
		sub	ax, word_4A2_3A
		push	ax
		push	word_4A2_3A
		mov	ax, 1
		push	ax
		push	cs
		call	DrawArcAbsolute
		push	word_4A2_1C6
		push	word_4A2_1C8
		push	word_4A2_1CA
		push	word_4A2_1CC
		push	cs
		call	DrawLineFunction
		push	word_4A2_1CE
		push	word_4A2_1D0
		push	word_4A2_1D2
		push	word_4A2_1D4
		push	cs
		call	DrawLineFunction
		jmp	loc_0_36E1

loc_0_3906:
		push	ax
		push	bx
		push	cx
		push	dx
		push	cs
		call	DrawLineFunction
		jmp	loc_0_36E1

loc_0_3911:
		or	si, si
		jns	loc_0_3924
		xchg	ax, cx
		xchg	bx, dx
		mov	[bp+10h], ax
		mov	[bp+0Eh], bx
		mov	[bp+0Ch], cx
		mov	[bp+0Ah], dx

loc_0_3924:
		mov	si, word_4A2_3A
		sub	bx, si
		cmp	byte ptr [bp+6], 0
		jz	loc_0_3944
		cmp	byte ptr [bp+6], 3
		jz	loc_0_398A
		add	dx, si
		push	ax
		push	bx
		push	cx
		push	dx
		push	cs
		call	FillRectangle
		jmp	loc_0_36AE

loc_0_3944:
		mov	[_DrawingPositionX], ax
		mov	[_DrawingPositionY], bx
		mov	ax, [bp+0Ch]
		sub	ax, [bp+10h]
		push	ax
		xor	ax, ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		xor	ax, ax
		push	ax
		mov	ax, word_4A2_3A
		add	ax, ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		mov	ax, [bp+10h]
		sub	ax, [bp+0Ch]
		push	ax
		xor	ax, ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		xor	ax, ax
		push	ax
		mov	ax, word_4A2_3A
		add	ax, ax
		neg	ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		jmp	loc_0_36E1

loc_0_398A:
		mov	ax, [bp+0Ch]
		add	ax, [bp+10h]
		sar	ax, 1
		push	ax
		mov	ax, [bp+0Eh]
		push	ax
		mov	ax, [bp+0Ch]
		sub	ax, [bp+10h]
		jns	loc_0_39A1
		neg	ax

loc_0_39A1:
		sar	ax, 1
		add	ax, si
		push	ax
		push	si
		xor	ax, ax
		push	ax
		push	cs
		call	DrawCircleAbsolute
		jmp	loc_0_36E1

loc_0_39B2:
		or	di, di
		jns	loc_0_39C5
		xchg	ax, cx
		xchg	bx, dx
		mov	[bp+10h], ax
		mov	[bp+0Eh], bx
		mov	[bp+0Ch], cx
		mov	[bp+0Ah], dx

loc_0_39C5:
		mov	si, word_4A2_3A
		sub	ax, si
		cmp	byte ptr [bp+6], 0
		jz	loc_0_39E5
		cmp	byte ptr [bp+6], 3
		jz	loc_0_3A2B
		add	cx, si
		push	ax
		push	bx
		push	cx
		push	dx
		push	cs
		call	FillRectangle
		jmp	loc_0_36AE

loc_0_39E5:
		mov	[_DrawingPositionX], ax
		mov	[_DrawingPositionY], bx
		xor	ax, ax
		push	ax
		mov	ax, [bp+0Ah]
		sub	ax, [bp+0Eh]
		push	ax
		push	cs
		call	DrawSolidLineRelative
		mov	ax, word_4A2_3A
		add	ax, ax
		push	ax
		xor	ax, ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		xor	ax, ax
		push	ax
		mov	ax, [bp+0Eh]
		sub	ax, [bp+0Ah]
		push	ax
		push	cs
		call	DrawSolidLineRelative
		mov	ax, word_4A2_3A
		add	ax, ax
		neg	ax
		push	ax
		xor	ax, ax
		push	ax
		push	cs
		call	DrawSolidLineRelative
		jmp	loc_0_36E1

loc_0_3A2B:
		mov	ax, [bp+10h]
		push	ax
		mov	ax, [bp+0Ah]
		add	ax, [bp+0Eh]
		sar	ax, 1
		push	ax
		push	si
		mov	ax, [bp+0Ah]
		sub	ax, [bp+0Eh]
		jns	loc_0_3A43
		neg	ax

loc_0_3A43:
		sar	ax, 1
		add	ax, si
		push	ax
		xor	ax, ax
		push	ax
		push	cs
		call	DrawCircleAbsolute
		jmp	loc_0_36E1
sub_0_32D4 endp

sub_0_3A53 proc near
		mov	ax, word_4A2_184
		cmp	ax, word_4A2_186
		jle	loc_0_3AA3
		mov	di, word_4A2_182

loc_0_3A60:
		cmp	di, word_4A2_182
		jnz	loc_0_3A88
		cmp	word_4A2_192, 0
		js	loc_0_3A78
		dec	word_4A2_182
		mov	ax, word_4A2_188
		sub	word_4A2_192, ax

loc_0_3A78:
		mov	ax, word_4A2_190
		add	word_4A2_180, ax
		mov	ax, word_4A2_18A
		add	word_4A2_192, ax
		jmp	short loc_0_3A60

loc_0_3A88:
		cmp	word_4A2_190, 0FFFFh
		jnz	loc_0_3AC3

loc_0_3A8F:
		cmp	word_4A2_192, 0
		jns	loc_0_3AC3
		dec	word_4A2_180
		mov	ax, word_4A2_18A
		add	word_4A2_192, ax
		jmp	short loc_0_3A8F

loc_0_3AA3:
		cmp	word_4A2_192, 0
		js	loc_0_3AB8
		mov	ax, word_4A2_190
		add	word_4A2_180, ax
		mov	ax, word_4A2_18A
		sub	word_4A2_192, ax

loc_0_3AB8:
		dec	word_4A2_182
		mov	ax, word_4A2_188
		add	word_4A2_192, ax

loc_0_3AC3:
		mov	ax, word_4A2_182
		cmp	ax, word_4A2_1CC
		jnz	locret_0_3B2E
		mov	ax, word_4A2_1CA
		mov	word_4A2_180, ax
		mov	cx, word_4A2_1D2
		sub	cx, ax
		mov	word_4A2_18C, cx
		mov	word_4A2_190, 1
		mov	ax, word_4A2_18C
		or	ax, ax
		jns	loc_0_3AEB
		neg	ax

loc_0_3AEB:
		mov	word_4A2_184, ax
		shl	ax, 1
		mov	word_4A2_188, ax
		mov	ax, word_4A2_1CC
		mov	word_4A2_182, ax
		mov	cx, word_4A2_1D4
		sub	cx, ax
		mov	word_4A2_18E, cx
		or	cx, cx
		jns	loc_0_3B09
		neg	cx

loc_0_3B09:
		mov	word_4A2_186, cx
		shl	cx, 1
		mov	word_4A2_18A, cx
		mov	ax, word_4A2_186
		cmp	ax, word_4A2_186
		jle	loc_0_3B24
		sub	cx, ax
		mov	word_4A2_192, cx
		jmp	short locret_0_3B2E

loc_0_3B24:
		mov	ax, word_4A2_188
		sub	ax, word_4A2_186
		mov	word_4A2_192, ax

locret_0_3B2E:
		retn
sub_0_3A53 endp

sub_0_3B2F proc near
		mov	ax, word_4A2_198
		cmp	ax, word_4A2_19A
		jle	loc_0_3B7F
		mov	di, word_4A2_196

loc_0_3B3C:
		cmp	di, word_4A2_196
		jnz	loc_0_3B64
		cmp	word_4A2_1A6, 0
		js	loc_0_3B54
		dec	word_4A2_196
		mov	ax, word_4A2_19C
		sub	word_4A2_1A6, ax

loc_0_3B54:
		mov	ax, word_4A2_1A4
		add	word_4A2_194, ax
		mov	ax, word_4A2_19E
		add	word_4A2_1A6, ax
		jmp	short loc_0_3B3C

loc_0_3B64:
		cmp	word_4A2_1A4, 1
		jnz	loc_0_3B9F

loc_0_3B6B:
		cmp	word_4A2_1A6, 0
		jns	loc_0_3B9F
		inc	word_4A2_194
		mov	ax, word_4A2_19E
		add	word_4A2_1A6, ax
		jmp	short loc_0_3B6B

loc_0_3B7F:
		cmp	word_4A2_1A6, 0
		js	loc_0_3B94
		mov	ax, word_4A2_1A4
		add	word_4A2_194, ax
		mov	ax, word_4A2_19E
		sub	word_4A2_1A6, ax

loc_0_3B94:
		dec	word_4A2_196
		mov	ax, word_4A2_19C
		add	word_4A2_1A6, ax

loc_0_3B9F:
		mov	ax, word_4A2_196
		cmp	ax, word_4A2_1D0
		jnz	locret_0_3C0A
		mov	ax, word_4A2_1CE
		mov	word_4A2_194, ax
		mov	cx, word_4A2_1D2
		sub	cx, ax
		mov	word_4A2_1A0, cx
		mov	word_4A2_1A4, 0FFFFh
		mov	ax, word_4A2_1A0
		or	ax, ax
		jns	loc_0_3BC7
		neg	ax

loc_0_3BC7:
		mov	word_4A2_198, ax
		shl	ax, 1
		mov	word_4A2_19C, ax
		mov	ax, word_4A2_1D0
		mov	word_4A2_196, ax
		mov	cx, word_4A2_1D4
		sub	cx, ax
		mov	word_4A2_1A2, cx
		or	cx, cx
		jns	loc_0_3BE5
		neg	cx

loc_0_3BE5:
		mov	word_4A2_19A, cx
		shl	cx, 1
		mov	word_4A2_19E, cx
		mov	ax, word_4A2_198
		cmp	ax, word_4A2_19A
		jle	loc_0_3C00
		sub	cx, ax
		mov	word_4A2_1A6, cx
		jmp	short locret_0_3C0A

loc_0_3C00:
		mov	ax, word_4A2_19C
		sub	ax, word_4A2_19A
		mov	word_4A2_1A6, ax

locret_0_3C0A:
		retn
sub_0_3B2F endp

sub_0_3C0B proc near
		mov	ax, [bp+8]
		mul	ax
		mov	word_4A2_1BE, ax
		mov	word_4A2_1C0, dx
		mov	bx, 1
		mov	ax, [bp+10h]
		mov	word_4A2_1A8, ax
		mov	cx, ax
		mov	ax, [bp+0Ch]
		sub	ax, cx
		mov	word_4A2_1B4, ax
		jns	loc_0_3C2E
		neg	bx

loc_0_3C2E:
		mov	word_4A2_1B8, bx
		or	ax, ax
		jns	loc_0_3C38
		neg	ax

loc_0_3C38:
		mov	word_4A2_1AC, ax
		mov	si, ax
		shl	ax, 1
		mov	word_4A2_1B0, ax
		mov	bx, 1
		mov	ax, [bp+0Eh]
		mov	word_4A2_1AA, ax
		mov	cx, ax
		mov	ax, [bp+0Ah]
		sub	ax, cx
		mov	word_4A2_1B6, ax
		jns	loc_0_3C59
		neg	bx

loc_0_3C59:
		mov	word_4A2_1BA, bx
		or	ax, ax
		jns	loc_0_3C63
		neg	ax

loc_0_3C63:
		mov	word_4A2_1AE, ax
		mov	di, ax
		shl	ax, 1
		mov	word_4A2_1B2, ax
		cmp	si, di
		jle	loc_0_3CBC
		sub	ax, si
		mov	word_4A2_1BC, ax

loc_0_3C76:
		call	sub_0_3D0D
		cmp	dx, word_4A2_1C0
		jnz	loc_0_3C83
		cmp	ax, word_4A2_1BE

loc_0_3C83:
		jnb	loc_0_3CD3
		mov	ax, word_4A2_1A8
		sub	ax, [bp+10h]
		mov	word_4A2_1C2, ax
		mov	ax, word_4A2_1AA
		sub	ax, [bp+0Eh]
		mov	word_4A2_1C4, ax
		cmp	word_4A2_1BC, 0
		js	loc_0_3CAC
		mov	ax, word_4A2_1BA
		add	word_4A2_1AA, ax
		mov	ax, word_4A2_1B0
		sub	word_4A2_1BC, ax

loc_0_3CAC:
		mov	ax, word_4A2_1B8
		add	word_4A2_1A8, ax
		mov	ax, word_4A2_1B2
		add	word_4A2_1BC, ax
		jmp	short loc_0_3C76

loc_0_3CBC:
		mov	ax, word_4A2_1B0
		sub	ax, word_4A2_1AE
		mov	word_4A2_1BC, ax

loc_0_3CC6:
		call	sub_0_3D0D
		cmp	dx, word_4A2_1C0
		jnz	loc_0_3CD3
		cmp	ax, word_4A2_1BE

loc_0_3CD3:
		jnb	locret_0_3D0C
		mov	ax, word_4A2_1A8
		sub	ax, [bp+10h]
		mov	word_4A2_1C2, ax
		mov	ax, word_4A2_1AA
		sub	ax, [bp+0Eh]
		mov	word_4A2_1C4, ax
		cmp	word_4A2_1BC, 0
		js	loc_0_3CFC
		mov	ax, word_4A2_1B8
		add	word_4A2_1A8, ax
		mov	ax, word_4A2_1B2
		sub	word_4A2_1BC, ax

loc_0_3CFC:
		mov	ax, word_4A2_1BA
		add	word_4A2_1AA, ax
		mov	ax, word_4A2_1B0
		add	word_4A2_1BC, ax
		jmp	short loc_0_3CC6

locret_0_3D0C:
		retn
sub_0_3C0B endp

sub_0_3D0D proc near
		mov	ax, word_4A2_1A8
		sub	ax, [bp+10h]
		or	ax, ax
		jns	loc_0_3D19
		neg	ax

loc_0_3D19:
		mul	ax
		mov	cx, ax
		mov	di, dx
		mov	ax, word_4A2_1AA
		sub	ax, [bp+0Eh]
		or	ax, ax
		jns	loc_0_3D2B
		neg	ax

loc_0_3D2B:
		mul	ax
		add	ax, cx
		adc	dx, di
		retn
sub_0_3D0D	endp


sub_0_3D32 proc near
		push	bp
		mov	bp, sp
		mov	cx, word_4A2_3A
		mov	ax, cx
		mul	ax
		mov	word_4A2_1BE, ax
		mov	word_4A2_1C0, dx
		mov	word_4A2_1B0, ax
		mov	word_4A2_1B2, dx
		mov	word_4A2_152, 0
		mov	word_4A2_154, 0
		mov	word_4A2_14C, 0

loc_0_3D5D:
		cmp	word_4A2_14C, 1
		jle	loc_0_3D67
		jmp	loc_0_3DEF

loc_0_3D67:
		mov	word_4A2_14E, 0

loc_0_3D6D:
		mov	cx, word_4A2_14E
		cmp	cx, 1
		jg	loc_0_3DE8
		mov	ax, word_4A2_14C
		add	ax, si
		mul	ax
		add	cx, di
		xchg	ax, cx
		mov	bx, dx
		mul	ax
		add	ax, cx
		adc	dx, bx
		mov	word_4A2_1B8, ax
		mov	word_4A2_1BA, dx
		cmp	word_4A2_1C0, dx
		jnz	loc_0_3D99
		cmp	word_4A2_1BE, ax

loc_0_3D99:
		jnz	loc_0_3DA5
		add	si, word_4A2_14C
		add	di, word_4A2_14E
		pop	bp
		retn

loc_0_3DA5:
		mov	dx, word_4A2_1C0
		mov	ax, word_4A2_1BE
		mov	bx, word_4A2_1BA
		mov	cx, word_4A2_1B8
		cmp	dx, bx
		jnz	loc_0_3DBA
		cmp	ax, cx

loc_0_3DBA:
		jnb	loc_0_3DBF
		xchg	ax, cx
		xchg	dx, bx

loc_0_3DBF:
		sub	ax, cx
		sbb	dx, bx
		cmp	dx, word_4A2_1B2
		jnz	loc_0_3DCD
		cmp	ax, word_4A2_1B0

loc_0_3DCD:
		jnb	loc_0_3DE2
		mov	word_4A2_1B0, ax
		mov	word_4A2_1B2, dx
		mov	ax, word_4A2_14C
		mov	word_4A2_152, ax
		mov	ax, word_4A2_14E
		mov	word_4A2_154, ax

loc_0_3DE2:
		inc	word_4A2_14E
		jmp	loc_0_3D6D

loc_0_3DE8:
		inc	word_4A2_14C
		jmp	loc_0_3D5D

loc_0_3DEF:
		add	si, word_4A2_152
		add	di, word_4A2_154
		pop	bp
		retn
sub_0_3D32	endp

_TEXT		ends
		end
