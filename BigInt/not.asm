
include	BigInt.inc


;****************************************************************
;*		�R�[�h						*
;****************************************************************
;---------------------------------------------------------------|
;		�r�b�g���]					|
;---------------------------------------------------------------|
;�y�����z							|
;	_Dst �� ~_Src						|
;�y�����z							|
;	_Dst:		ptr BigInt	�r�b�g���]		|
;	_Src:		ptr BigInt	�r�b�g���]		|
;�y���Ӂz							|
;	_Dst��_Src�́A�����T�C�Y�̃o�b�t�@������		|
;---------------------------------------------------------------|
.code
@BigInt_Not@8	proc	syscall	uses	ebx edi esi
;	_Dst:		ptr BigInt,
;	_Src:		ptr BigInt

	assume	eax:ptr BigInt,		;_Dst
		edx:ptr BigInt		;_Src

	mov	eax, ecx
	mov	ecx, [edx].m_sz		;_Src.m_sz

ifdef	_DEBUG
	.if	(ecx != [eax].m_sz)
		push	ecx
		print	"BigInt_Not()	difference size  SRC:%d",0dh,0ah
		pop	ecx
		jmp	BigInt_Not_End
	.endif
endif
	mov	esi, [edx].m_pt
	mov	edi, [eax].m_pt
	mov	edx, ecx
	shr	ecx, 2
	.if	(ecx != 0)
	   .repeat
		lodsd
		not	eax
		stosd
	   .untilcxz
	.endif
	and	edx, 3
	.if	!(zero?)
	   mov	ecx, edx
	   .repeat
		lodsb
		not	al
		stosb
	   .untilcxz
	.endif

BigInt_Not_End:

	ret
@BigInt_Not@8	endp
;****************************************************************
	end
