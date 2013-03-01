
include	BigInt.inc


;****************************************************************
;*		コード						*
;****************************************************************
;---------------------------------------------------------------|
;		1bit 左シフト					|
;---------------------------------------------------------------|
;【処理】							|
;	_Src <<= _Cnt						|
;								|
;	少々高速化版（但し、1bit シフト）			|
;	　　・余計な条件分岐を削除				|
;	　　・引数を１つだけにする				|
;	　　・esiレジスタは未使用				|
;								|
;【引数】							|
;	_Src:		ptr BigInt	シフトするオブジェクト	|
;	_Cnt		unsigned int	シフト量		|
;---------------------------------------------------------------|
.code
@BigInt_Shl1@4	proc	syscall	uses	edi
;	_Src:		ptr BigInt,	;(ecx)

	assume	ecx:ptr BigInt

	mov	edx, [ecx].m_sz		;edx = _Src.m_sz
	mov	edi, [ecx].m_pt		;edi = _Src.m_pt

	;ビット単位のシフト
	mov	ecx, edx
	shr	ecx, 2
	and	edx, 3
	.if	(zero?)
		.if	(ecx != 0)
		  clc
		  .repeat
			rcl	dword ptr [edi], 1
			inc	edi
			inc	edi
			inc	edi
			inc	edi
		  .untilcxz
		.endif
	.else
		.if	(ecx != 0)
		  clc
		  .repeat
			rcl	dword ptr [edi], 1
			inc	edi
			inc	edi
			inc	edi
			inc	edi
		  .untilcxz
		.endif
		mov	ecx, edx
		.repeat
			rcl	byte ptr [edi], 1
			inc	edi
		.untilcxz
	.endif

	ret
@BigInt_Shl1@4	endp
;---------------------------------------------------------------|
;		左シフト					|
;---------------------------------------------------------------|
;【処理】							|
;	_Src <<= _Cnt						|
;【引数】							|
;	_Src:		ptr BigInt	シフトするオブジェクト	|
;	_Cnt		unsigned int	シフト量		|
;---------------------------------------------------------------|
.code
@BigInt_Shl@8	proc	syscall	uses	ebx edi esi
;	_Src:		ptr BigInt,	;(ecx)
;	_Cnt:		unsigned int	;(edx)

	assume	ecx:ptr BigInt

	mov	esi, [ecx].m_pt		;esi = _Src.m_pt
	mov	eax, [ecx].m_sz		;eax = _Src.m_sz

	mov	ecx, edx
	push	eax
	shr	ecx, 3			;ecx = バイト単位のシフト量（後で）
	and	edx, 0007h		;edx = ビット単位のシフト量（ = 0～7 の範囲）
	sub	eax, ecx		;eax = ビットシフトする長さ（後でシフトする分は除外）

	push	ecx

	lea	edi, [esi + eax - 1]	;edx == 0だった時の為に代入しておく。

	;ビット単位のシフト
	.while	(edx != 0)
		mov	ecx, eax
		mov	ebx, eax
		shr	ecx, 2
		mov	edi, esi
		and	ebx, 3
		.if	(zero?)
			.if	(ecx != 0)
			  clc
			  .repeat
				rcl	dword ptr [edi], 1
				inc	edi
				inc	edi
				inc	edi
				inc	edi
			  .untilcxz
			.endif
		.else
			.if	(ecx != 0)
			  clc
			  .repeat
				rcl	dword ptr [edi], 1
				inc	edi
				inc	edi
				inc	edi
				inc	edi
			  .untilcxz
			.endif
			mov	ecx, ebx
			.repeat
				rcl	byte ptr [edi], 1
				inc	edi
			.untilcxz
		.endif
		dec	edi
		dec	edx
	.endw

	;Byte(8bit)単位のシフト
	pop	eax		;eax = バイト単位のシフト量
	mov	ebx, esi	;ebx = &m_pt[0]
	pop	ecx		;ecx = this.m_sz
	mov	esi, edi	;esi = 転送元
	sub	ecx, eax	;ecx = 転送回数
	add	edi, eax	;edi = 転送先（最後尾）

	;転送
	.if	(ecx != 0)
		push	eax
		std			;dec mode
		rep	movsb
		cld			;inc mode
		pop	eax
	.endif

	;転送した分、0x00で埋める。
	.if	(eax != 0)
		mov	ecx, eax
		xor	eax, eax
		mov	edi, ebx
		rep	stosb
	.endif
	
	ret
@BigInt_Shl@8	endp
;****************************************************************
	end
