
include	BigInt.inc


;****************************************************************
;*		コード						*
;****************************************************************
;---------------------------------------------------------------|
;		右シフト					|
;---------------------------------------------------------------|
;【引数】							|
;	esi	m_pt						|
;	eax	処理長						|
;	ebx	m_sz-1						|
;	edx	cnt						|
;【返り値】							|
;	ecx		破壊 ＝ ０				|
;	edx	cnt	破壊 ＝ ０				|
;	edi		破壊					|
;	ebp =	(ebp << edx) | シフトして溢れたビット		|
;	※旧Versionの名残。呼び出し元では使っていないが、	|
;	　この関数の処理上では必要なので、残しておく。		|
;---------------------------------------------------------------|
.code
_Shr	proc	uses	ebx esi
	add	esi, ebx			;最後尾へ

	.while	(edx != 0)
		;ポインターと桁数のカウンターを設定
		mov	edi, esi
		lea	ebx, [eax - 1]
		mov	ecx, ebx
		shr	ebx, 2
		and	ecx, 3

		;先ず、Byte単位のシフト
		.if	(ecx == 0)
			sar	byte ptr [edi], 1
		.else
			sar	byte ptr [edi], 1
			.repeat
				dec	edi
				rcr	byte ptr [edi], 1
			.untilcxz
		.endif
		rcr	ebp, 1		;キャリーを保存

		;DWORD(4Byte)単位のシフト
		.if	(ebx != 0)
			shl	ebp, 1	;キャリーを戻す
			mov	ecx, ebx
			.repeat
				dec	edi
				dec	edi
				dec	edi
				dec	edi
				rcr	dword ptr [edi], 1
			.untilcxz
			rcr	ebp, 1	;桁あふれを入れる。
		.endif

		;回数カウンターのデクリメント
		dec	edx
	.endw
	
	ret
_Shr	endp
;---------------------------------------------------------------|
;		1bit 右シフト					|
;---------------------------------------------------------------|
;【処理】							|
;	_Src >>= 1						|
;								|
;	少々高速化版（但し、1bit シフト）			|
;	　　・余計な条件分岐を削除				|
;	　　・引数を１つだけにする				|
;	　　・esiレジスタは未使用				|
;								|
;【引数】							|
;	_Src:		ptr BigInt	シフトするオブジェクト	|
;【返り値】							|
;	unsigned int	シフトして溢れたやつ(32bit分まで)	|
;---------------------------------------------------------------|
.code
@BigInt_Shr1@4	proc	syscall	uses	ebp ebx edi
;	_Src:		ptr BigInt,	;(ecx)

	assume	ecx:ptr BigInt

	mov	eax, [ecx].m_sz		;eax = _Src.m_sz
	mov	edi, [ecx].m_pt		;edi = _Src.m_pt

	xor	ebp, ebp

	;ポインターと桁数のカウンターを設定
	lea	ebx, [eax - 1]		;ebx = m_sz - 1
	add	edi, ebx		;edi = m_pt + m_sz - 1
	mov	ecx, ebx
	shr	ebx, 2
	and	ecx, 3

	;先ず、Byte単位のシフト
	.if	(ecx == 0)
		sar	byte ptr [edi], 1
	.else
		sar	byte ptr [edi], 1
		.repeat
			dec	edi
			rcr	byte ptr [edi], 1
		.untilcxz
	.endif
	rcr	ebp, 1		;キャリーを保存

	;DWORD(4Byte)単位のシフト
	.if	(ebx != 0)
		shl	ebp, 1	;キャリーを戻す
		mov	ecx, ebx
		.repeat
			dec	edi
			dec	edi
			dec	edi
			dec	edi
			rcr	dword ptr [edi], 1
		.untilcxz
		rcr	ebp, 1	;桁あふれを入れる。
	.endif

	mov	eax, ebp

	ret
@BigInt_Shr1@4	endp
;---------------------------------------------------------------|
;		右シフト					|
;---------------------------------------------------------------|
;【処理】							|
;	_Src >>= _Cnt						|
;【引数】							|
;	_Src:		ptr BigInt	シフトするオブジェクト	|
;	_Cnt		unsigned int	シフト量		|
;【返り値】							|
;	unsigned int	シフトして溢れたやつ(32bit分まで)	|
;---------------------------------------------------------------|
.code
@BigInt_Shr@8	proc	syscall	uses	ebp ebx edi esi
;	_Src:		ptr BigInt,	;(ecx)
;	_Cnt:		unsigned int	;(edx)

	.if	(edx != 0)			;シフト量が０だったら、終わり。

		assume	ecx:ptr BigInt

		mov	eax, [ecx].m_sz		;eax = _Src.m_sz
		or	eax, eax
		jz	@BigInt_Shr@8_Exit	;サイズが０だったら、処理しない。

		mov	esi, [ecx].m_pt		;esi = _Src.m_pt

		mov	ecx, edx
		shr	ecx, 3			;ecx = Byte単位のシフト量

		lea	ebx, [eax - 1]		;ebx = m_sz - 1
		sub	eax, ecx		;eax = m_sz - (_Cnt >> 3)	Bit単位のシフト量

		;※	eax, ebxは破壊しないので、push/popしない。
		push	ecx
		.if	(ecx < 4)
			;_Cnt(edx) < 32　の場合
			mov	ecx, 32
			mov	ebp, dword ptr [esi]
			sub	ecx, edx		;ecx = 32 - edx
			shl	ebp, cl
			.if	(sdword ptr eax > 0)
				;バッファ内でシフトができる場合
				and	edx, 0007h		;edx = Bit 単位のシフト量
				push	ebp
				invoke	_Shr
				pop	ebp
			.endif
		.else
			;_Cnt(edx) >= 32　の場合
			add	eax, 4			;eax = 処理するサイズに４を加算(返り値(ebp)へ保存用)
			.if	(sdword ptr eax > 0)
				;バッファ内でシフトができる場合
				and	edx, 0007h	;edx = Bit 単位のシフト量
				invoke	_Shr
				mov	ecx, ebx
				sub	ecx, eax
				mov	ebp, dword ptr [esi + ecx + 1]
		;	.else
		;		;バッファを越えるシフト量の場合
		;		;（処理すべきサイズが、０以下の場合。）
		;		xor	ebp, ebp
		;		↑以降に、符号拡張があるので、不要
		;
			.endif
			sub	eax, 4
		.endif
		pop	ecx
		;※	eax, ebxは破壊しないので、push/popしない。

		;Byte単位のシフト量が０だったら処理しない。
		.if	(ecx != 0)
			.if	(sdword ptr eax > 0)
				;Byte単位のシフト量がm_sz未満の場合。
				mov	edi, esi			;送り元 = esi
				mov	edx, ecx			;edx = Byte単位のシフト量
				add	esi, ecx			;送り先 = esi + ecx(Byte単位のシフト量)
				mov	ecx, eax			;ecx = 転送量
				rep	movsb
				lea	edi, [esi - 1]			;edi = 最後のポインタ
				mov	ecx, edx
				movsx	eax, byte ptr [edi]		;
				sar	eax, 7				;eax = 符号
			.else
				;Byte単位のシフト量がm_sz以上の場合。
				lea	edi, [esi + ebx]		;edi = 最後のポインタ
				mov	edx, eax			;edx = Bit単位でシフトした量
				movsx	eax, byte ptr [edi]
				lea	ecx, [ebx + 1]			;ecx = ０クリアする量（＝m_sz）
				sar	eax, 7				;eax = 符号
				mov	ebx, eax
				;符号拡張
				.if	(sdword ptr edx <= -4)
					mov	ebp, ebx
				.elseif	(sdword ptr edx == -3)
					and	ebp, 0000000FFh
					and	ebx, 0FFFFFF00h
					or	ebp, ebx
				.elseif	(sdword ptr edx == -2)
					and	ebp, 00000FFFFh
					and	ebx, 0FFFF0000h
					or	ebp, ebx
				.elseif	(sdword ptr edx == -1)
					and	ebp, 000FFFFFFh
					and	ebx, 0FF000000h
					or	ebp, ebx
				.endif
			.endif
			std
			rep	stosb
			cld			;inc mode
		.endif
		mov	eax, ebp
	.else
		xor	eax, eax
	.endif

@BigInt_Shr@8_Exit:

	ret
@BigInt_Shr@8	endp
;****************************************************************
	end
