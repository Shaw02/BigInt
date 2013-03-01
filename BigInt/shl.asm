
include	BigInt.inc


;****************************************************************
;*		�R�[�h						*
;****************************************************************
;---------------------------------------------------------------|
;		1bit ���V�t�g					|
;---------------------------------------------------------------|
;�y�����z							|
;	_Src <<= _Cnt						|
;								|
;	���X�������Łi�A���A1bit �V�t�g�j			|
;	�@�@�E�]�v�ȏ���������폜				|
;	�@�@�E�������P�����ɂ���				|
;	�@�@�Eesi���W�X�^�͖��g�p				|
;								|
;�y�����z							|
;	_Src:		ptr BigInt	�V�t�g����I�u�W�F�N�g	|
;	_Cnt		unsigned int	�V�t�g��		|
;---------------------------------------------------------------|
.code
@BigInt_Shl1@4	proc	syscall	uses	edi
;	_Src:		ptr BigInt,	;(ecx)

	assume	ecx:ptr BigInt

	mov	edx, [ecx].m_sz		;edx = _Src.m_sz
	mov	edi, [ecx].m_pt		;edi = _Src.m_pt

	;�r�b�g�P�ʂ̃V�t�g
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
;		���V�t�g					|
;---------------------------------------------------------------|
;�y�����z							|
;	_Src <<= _Cnt						|
;�y�����z							|
;	_Src:		ptr BigInt	�V�t�g����I�u�W�F�N�g	|
;	_Cnt		unsigned int	�V�t�g��		|
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
	shr	ecx, 3			;ecx = �o�C�g�P�ʂ̃V�t�g�ʁi��Łj
	and	edx, 0007h		;edx = �r�b�g�P�ʂ̃V�t�g�ʁi = 0�`7 �͈̔́j
	sub	eax, ecx		;eax = �r�b�g�V�t�g���钷���i��ŃV�t�g���镪�͏��O�j

	push	ecx

	lea	edi, [esi + eax - 1]	;edx == 0���������ׂ̈ɑ�����Ă����B

	;�r�b�g�P�ʂ̃V�t�g
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

	;Byte(8bit)�P�ʂ̃V�t�g
	pop	eax		;eax = �o�C�g�P�ʂ̃V�t�g��
	mov	ebx, esi	;ebx = &m_pt[0]
	pop	ecx		;ecx = this.m_sz
	mov	esi, edi	;esi = �]����
	sub	ecx, eax	;ecx = �]����
	add	edi, eax	;edi = �]����i�Ō���j

	;�]��
	.if	(ecx != 0)
		push	eax
		std			;dec mode
		rep	movsb
		cld			;inc mode
		pop	eax
	.endif

	;�]���������A0x00�Ŗ��߂�B
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
