;=======================================================================|
;									|
;		Big Integer						|
;									|
;					Programmed by			|
;						A.Watanabe		|
;									|
;-----------------------------------------------------------------------|
;Revision:								|
;	2012. 2.24	
;=======================================================================|

	.586p
	.model	flat,stdcall

;***********************************************************************|
;*		型定義							|
;***********************************************************************|
BigInt	struc
m_sz	dd	?
m_pt	dd	?
BigInt	ends



;***********************************************************************|
;*		プロトタイプ宣言					|
;***********************************************************************|
@BigInt_Copy@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_Add@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_Sub@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_Not@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_And@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_Or@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt
@BigInt_Xor@8	proto	syscall	;	_Dst:ptr BigInt,	_Src:ptr BigInt

@BigInt_Shl@8	proto	syscall	;	_Src:ptr BigInt,	_Cnt:DWORD
@BigInt_Shr@8	proto	syscall	;	_Src:ptr BigInt,	_Cnt:DWORD

@BigInt_Shl1@4	proto	syscall	;	_Src:ptr BigInt
@BigInt_Shr1@4	proto	syscall	;	_Src:ptr BigInt


;BigInt_Add3	proto	syscall	;	_Result:ptr BigInt,	_Dst:ptr BigInt,	_Src:ptr BigInt
;BigInt_Sub3	proto	syscall	;	_Result:ptr BigInt,	_Dst:ptr BigInt,	_Src:ptr BigInt

ifdef	_DEBUG
printf		proto	c	_Format:ptr byte, var:VARARG 
endif



;***********************************************************************|
;*		マクロ定義						|
;***********************************************************************|
ifdef	_DEBUG
;---------------------------------------------------------------|
;	文字列表示						|
;---------------------------------------------------------------|
;	引数							|
;		str	表示文字列（文字列、変数、レジスタ可）	|
;	返り値							|
;		無し						|
;---------------------------------------------------------------|
print	macro	str:VARARG

	local	msg

	;◆文字列を渡された場合
	if	(@InStr(1, <str>, <!">) EQ 1)
		.data
		msg	db	str, 0

		.code
		invoke	printf,	addr msg

	;◆その他
	else

		invoke	printf,	str

	endif

	endm
;---------------------------------------------------------------|

endif
