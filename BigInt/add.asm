
include	BigInt.inc


;****************************************************************
;*		コード						*
;****************************************************************
;---------------------------------------------------------------|
;		加算						|
;---------------------------------------------------------------|
;【処理】							|
;	_Dst ＝ _Dst ＋ _Src					|
;【引数】							|
;	_Dst:		ptr BigInt	足される数		|
;	_Src:		ptr BigInt	足す数			|
;【注意】							|
;	_Dstは、_Srcよりも大きいサイズのバッファを持つ事(Carry)	|
;---------------------------------------------------------------|
.code
@BigInt_Add@8	proc	syscall	uses	ebx edi esi
;	_Dst:		ptr BigInt,
;	_Src:		ptr BigInt

	assume	eax:ptr BigInt,		;_Dst
		edx:ptr BigInt		;_Src

	mov	eax, ecx
	mov	ecx, [edx].m_sz		;_Src.m_sz
	mov	ebx, [eax].m_sz		;_Dst.m_sz
	sub	ebx, ecx		;ebx = _Dst.m_sz - _Src.m_sz

ifdef	_DEBUG
	push	ecx
	inc	ecx
	.if	(SDWORD ptr ebx < 1)
		print	"BigInt_Add()	small _Result's size  SRC:%d",0dh,0ah
		jmp	BigInt_Add_End
	.endif
	pop	ecx
endif

	;-----------
	;加算処理
	mov	esi, [edx].m_pt
	mov	edi, [eax].m_pt
	mov	edx, ecx
	movsx	eax, byte ptr [esi + ecx - 1]
	sar	eax, 7
	push	eax			;符号
	shr	ecx, 2
	and	edx, 3
	.if	(zero?)
		.if	(ecx != 0)
		  clc
		  .repeat
			lodsd
			adc	eax, dword ptr [edi]
			stosd
		  .untilcxz
		.endif
	.else
		.if	(ecx != 0)
		  clc
		  .repeat
			lodsd
			adc	eax, dword ptr [edi]
			stosd
		  .untilcxz
		.endif
		mov	ecx, edx
		.repeat
			lodsb
			adc	al, byte ptr [edi]
			stosb
		.untilcxz
	.endif

	mov	ecx, ebx
	pop	eax
	.repeat
		adc	byte ptr [edi], al
		inc	edi
	.untilcxz

BigInt_Add_End:

	ret
@BigInt_Add@8	endp
;****************************************************************
	end
