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
;*		�^��`							|
;***********************************************************************|
BigInt	struc
m_sz	dd	?
m_pt	dd	?
BigInt	ends



;***********************************************************************|
;*		�v���g�^�C�v�錾					|
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
;*		�}�N����`						|
;***********************************************************************|
ifdef	_DEBUG
;---------------------------------------------------------------|
;	������\��						|
;---------------------------------------------------------------|
;	����							|
;		str	�\��������i������A�ϐ��A���W�X�^�j	|
;	�Ԃ�l							|
;		����						|
;---------------------------------------------------------------|
print	macro	str:VARARG

	local	msg

	;���������n���ꂽ�ꍇ
	if	(@InStr(1, <str>, <!">) EQ 1)
		.data
		msg	db	str, 0

		.code
		invoke	printf,	addr msg

	;�����̑�
	else

		invoke	printf,	str

	endif

	endm
;---------------------------------------------------------------|

endif