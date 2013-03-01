
include	BigInt.inc


;****************************************************************
;*		�R�[�h						*
;****************************************************************
;---------------------------------------------------------------|
;		�E�V�t�g					|
;---------------------------------------------------------------|
;�y�����z							|
;	esi	m_pt						|
;	eax	������						|
;	ebx	m_sz-1						|
;	edx	cnt						|
;�y�Ԃ�l�z							|
;	ecx		�j�� �� �O				|
;	edx	cnt	�j�� �� �O				|
;	edi		�j��					|
;	ebp =	(ebp << edx) | �V�t�g���Ĉ�ꂽ�r�b�g		|
;	����Version�̖��c�B�Ăяo�����ł͎g���Ă��Ȃ����A	|
;	�@���̊֐��̏�����ł͕K�v�Ȃ̂ŁA�c���Ă����B		|
;---------------------------------------------------------------|
.code
_Shr	proc	uses	ebx esi
	add	esi, ebx			;�Ō����

	.while	(edx != 0)
		;�|�C���^�[�ƌ����̃J�E���^�[��ݒ�
		mov	edi, esi
		lea	ebx, [eax - 1]
		mov	ecx, ebx
		shr	ebx, 2
		and	ecx, 3

		;�悸�AByte�P�ʂ̃V�t�g
		.if	(ecx == 0)
			sar	byte ptr [edi], 1
		.else
			sar	byte ptr [edi], 1
			.repeat
				dec	edi
				rcr	byte ptr [edi], 1
			.untilcxz
		.endif
		rcr	ebp, 1		;�L�����[��ۑ�

		;DWORD(4Byte)�P�ʂ̃V�t�g
		.if	(ebx != 0)
			shl	ebp, 1	;�L�����[��߂�
			mov	ecx, ebx
			.repeat
				dec	edi
				dec	edi
				dec	edi
				dec	edi
				rcr	dword ptr [edi], 1
			.untilcxz
			rcr	ebp, 1	;�����ӂ������B
		.endif

		;�񐔃J�E���^�[�̃f�N�������g
		dec	edx
	.endw
	
	ret
_Shr	endp
;---------------------------------------------------------------|
;		1bit �E�V�t�g					|
;---------------------------------------------------------------|
;�y�����z							|
;	_Src >>= 1						|
;								|
;	���X�������Łi�A���A1bit �V�t�g�j			|
;	�@�@�E�]�v�ȏ���������폜				|
;	�@�@�E�������P�����ɂ���				|
;	�@�@�Eesi���W�X�^�͖��g�p				|
;								|
;�y�����z							|
;	_Src:		ptr BigInt	�V�t�g����I�u�W�F�N�g	|
;�y�Ԃ�l�z							|
;	unsigned int	�V�t�g���Ĉ�ꂽ���(32bit���܂�)	|
;---------------------------------------------------------------|
.code
@BigInt_Shr1@4	proc	syscall	uses	ebp ebx edi
;	_Src:		ptr BigInt,	;(ecx)

	assume	ecx:ptr BigInt

	mov	eax, [ecx].m_sz		;eax = _Src.m_sz
	mov	edi, [ecx].m_pt		;edi = _Src.m_pt

	xor	ebp, ebp

	;�|�C���^�[�ƌ����̃J�E���^�[��ݒ�
	lea	ebx, [eax - 1]		;ebx = m_sz - 1
	add	edi, ebx		;edi = m_pt + m_sz - 1
	mov	ecx, ebx
	shr	ebx, 2
	and	ecx, 3

	;�悸�AByte�P�ʂ̃V�t�g
	.if	(ecx == 0)
		sar	byte ptr [edi], 1
	.else
		sar	byte ptr [edi], 1
		.repeat
			dec	edi
			rcr	byte ptr [edi], 1
		.untilcxz
	.endif
	rcr	ebp, 1		;�L�����[��ۑ�

	;DWORD(4Byte)�P�ʂ̃V�t�g
	.if	(ebx != 0)
		shl	ebp, 1	;�L�����[��߂�
		mov	ecx, ebx
		.repeat
			dec	edi
			dec	edi
			dec	edi
			dec	edi
			rcr	dword ptr [edi], 1
		.untilcxz
		rcr	ebp, 1	;�����ӂ������B
	.endif

	mov	eax, ebp

	ret
@BigInt_Shr1@4	endp
;---------------------------------------------------------------|
;		�E�V�t�g					|
;---------------------------------------------------------------|
;�y�����z							|
;	_Src >>= _Cnt						|
;�y�����z							|
;	_Src:		ptr BigInt	�V�t�g����I�u�W�F�N�g	|
;	_Cnt		unsigned int	�V�t�g��		|
;�y�Ԃ�l�z							|
;	unsigned int	�V�t�g���Ĉ�ꂽ���(32bit���܂�)	|
;---------------------------------------------------------------|
.code
@BigInt_Shr@8	proc	syscall	uses	ebp ebx edi esi
;	_Src:		ptr BigInt,	;(ecx)
;	_Cnt:		unsigned int	;(edx)

	.if	(edx != 0)			;�V�t�g�ʂ��O��������A�I���B

		assume	ecx:ptr BigInt

		mov	eax, [ecx].m_sz		;eax = _Src.m_sz
		or	eax, eax
		jz	@BigInt_Shr@8_Exit	;�T�C�Y���O��������A�������Ȃ��B

		mov	esi, [ecx].m_pt		;esi = _Src.m_pt

		mov	ecx, edx
		shr	ecx, 3			;ecx = Byte�P�ʂ̃V�t�g��

		lea	ebx, [eax - 1]		;ebx = m_sz - 1
		sub	eax, ecx		;eax = m_sz - (_Cnt >> 3)	Bit�P�ʂ̃V�t�g��

		;��	eax, ebx�͔j�󂵂Ȃ��̂ŁApush/pop���Ȃ��B
		push	ecx
		.if	(ecx < 4)
			;_Cnt(edx) < 32�@�̏ꍇ
			mov	ecx, 32
			mov	ebp, dword ptr [esi]
			sub	ecx, edx		;ecx = 32 - edx
			shl	ebp, cl
			.if	(sdword ptr eax > 0)
				;�o�b�t�@���ŃV�t�g���ł���ꍇ
				and	edx, 0007h		;edx = Bit �P�ʂ̃V�t�g��
				push	ebp
				invoke	_Shr
				pop	ebp
			.endif
		.else
			;_Cnt(edx) >= 32�@�̏ꍇ
			add	eax, 4			;eax = ��������T�C�Y�ɂS�����Z(�Ԃ�l(ebp)�֕ۑ��p)
			.if	(sdword ptr eax > 0)
				;�o�b�t�@���ŃV�t�g���ł���ꍇ
				and	edx, 0007h	;edx = Bit �P�ʂ̃V�t�g��
				invoke	_Shr
				mov	ecx, ebx
				sub	ecx, eax
				mov	ebp, dword ptr [esi + ecx + 1]
		;	.else
		;		;�o�b�t�@���z����V�t�g�ʂ̏ꍇ
		;		;�i�������ׂ��T�C�Y���A�O�ȉ��̏ꍇ�B�j
		;		xor	ebp, ebp
		;		���ȍ~�ɁA�����g��������̂ŁA�s�v
		;
			.endif
			sub	eax, 4
		.endif
		pop	ecx
		;��	eax, ebx�͔j�󂵂Ȃ��̂ŁApush/pop���Ȃ��B

		;Byte�P�ʂ̃V�t�g�ʂ��O�������珈�����Ȃ��B
		.if	(ecx != 0)
			.if	(sdword ptr eax > 0)
				;Byte�P�ʂ̃V�t�g�ʂ�m_sz�����̏ꍇ�B
				mov	edi, esi			;���茳 = esi
				mov	edx, ecx			;edx = Byte�P�ʂ̃V�t�g��
				add	esi, ecx			;����� = esi + ecx(Byte�P�ʂ̃V�t�g��)
				mov	ecx, eax			;ecx = �]����
				rep	movsb
				lea	edi, [esi - 1]			;edi = �Ō�̃|�C���^
				mov	ecx, edx
				movsx	eax, byte ptr [edi]		;
				sar	eax, 7				;eax = ����
			.else
				;Byte�P�ʂ̃V�t�g�ʂ�m_sz�ȏ�̏ꍇ�B
				lea	edi, [esi + ebx]		;edi = �Ō�̃|�C���^
				mov	edx, eax			;edx = Bit�P�ʂŃV�t�g������
				movsx	eax, byte ptr [edi]
				lea	ecx, [ebx + 1]			;ecx = �O�N���A����ʁi��m_sz�j
				sar	eax, 7				;eax = ����
				mov	ebx, eax
				;�����g��
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
