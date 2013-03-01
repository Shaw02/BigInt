
include	BigInt.inc


;****************************************************************
;*		コード						*
;****************************************************************
;---------------------------------------------------------------|
;		コピー with Zero expand				|
;---------------------------------------------------------------|
;【処理】							|
;	_Dst ＝ _Src						|
;【引数】							|
;	_Src:		ptr BigInt,	コピー元		|
;	_Dst:		ptr BigInt,	コピー先		|
;【返り値】							|
;	無し							|
;---------------------------------------------------------------|
.code
@BigInt_Copy@8	proc	syscall	uses	ebx edi esi
;	_Dst:		ptr BigInt,
;	_Src:		ptr BigInt

	assume	eax:ptr BigInt,		;_Dst
		edx:ptr BigInt		;_Src

	mov	eax, ecx
	mov	ecx, [edx].m_sz		;_Src.m_sz
	mov	ebx, [eax].m_sz		;_Dst.m_sz
	sub	ebx, ecx		;サイズの差

ifdef	_DEBUG
	.if	(carry?)
		print	"BigInt_Copy()	small _Dst's size",0dh,0ah
		jmp	BigInt_Copy_End
	.endif
endif

	;-----------
	;コピー処理
	mov	esi, [edx].m_pt
	mov	edi, [eax].m_pt
	movsx	eax, byte ptr [esi + ecx - 1]
	mov	edx, ecx
	shr	ecx, 2
	and	edx, 3
	sar	eax, 7
	push	eax

	.if	(ecx != 0)
	  rep	movsd
	.endif

	.if	(edx != 0)
	  mov	ecx, edx
	  rep	movsb
	.endif

	;-----------
	;0 expand
	pop	eax
	.if	(ebx != 0)
	  mov	ecx, ebx
	  rep	stosb
	.endif

BigInt_Copy_End:

	ret
@BigInt_Copy@8	endp
;****************************************************************
	end
