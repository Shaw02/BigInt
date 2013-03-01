#include "StdAfx.h"

//==============================================================
//			��Z�iMUL�j	Radix-16 Booth Mul
//--------------------------------------------------------------
//	������
//		BigInt&	_Dst	���ʂ̐��l������I�u�W�F�N�g�̃|�C���^
//		BigInt	_Src1	�������鐔�̃I�u�W�F�N�g�i���́j
//		BigInt	_Src2	�����鐔�̃I�u�W�F�N�g�i���́j
//	���Ԓl
//						����
//	������
//		_Dst �� _Src1 �~ _Src2
//	������
//		�E���̃A���S���Y���̃����b�g�́A���̒l�ւ̕������]���s�v�B
//		�@�������Q�̕␔�ŕ\������Ă�����A���̂܂܌v�Z�ł���B
//		�E���ǔłł́A�\�߂������鐔�̂Q�{�����߂Ă����A
//		�@�V�t�g���Z�́A��C��2bit�V�t�g���A
//		�@�]���̃u�[�X�̏�Z���A�����Z�̉񐔂�񔼕��Ɍ��炷�B
//==============================================================
void	BigInt_Mul(BigInt* _Dst, BigInt _Src1, BigInt _Src2)
{
	unsigned	int		_sz1	= _Src1.m_sz;
	unsigned	int		_sz2	= _Src2.m_sz;
	unsigned	int		i		= _sz2 * 8;
	unsigned	int		b = 0;
	unsigned	int		m;
	BigInt	_n[9];			//�\�߁A_Src1�ɑ΂���1�`8�{�������l�����߂�B

	*_Dst = 0;
	_Dst->resize(_sz1 + _sz2 + 5);
	memcpy(_Dst->m_pt, _Src2.m_pt, _sz2);

	_Src1 <<= i;

	_n[0]	=	_Src1;

	_n[1]	=	_Src1;
	_n[1]	<<= 1;		//*2

	_n[2]	=	_n[1];
	_n[2]	+=	_Src1;	//*3 (2+1)

	_n[3]	=	_n[1];
	_n[3]	<<=	1;		//*4

	_n[4]	=	_n[3];
	_n[4]	+=	_Src1;	//*5 (4+1)

	_n[5]	=	_n[3];
	_n[5]	+=	_n[1];	//*6 (4+2)

	_n[6]	=	_n[3];
	_n[6]	+=	_n[2];	//*7 (4+3)

	_n[7]	=	_n[3];
	_n[7]	<<=	1;		//*8


	i = i >> 2;
	while(i > 0){
		b >>= 4;
		b |= BigInt_Shr(_Dst,4);
		m = b>>27;
		if((m & 0x10) == 0){
			if(m != 0){
				BigInt_Add(_Dst, &_n[(m-1)/2]);
			}
		} else {
			m ^= 0x1F;
			if(m != 0){
				BigInt_Sub(_Dst, &_n[(m-1)/2]);
			}
		}
		i--;
	}
	*_Dst >>= 4;		//���T�C�Y���Ă����B
}

//==============================================================
//			��Z�iMUL�j	Radix-16 Booth Mul
//--------------------------------------------------------------
//	������
//		BigInt&	_Dst	���ʂ̐��l������I�u�W�F�N�g�̃|�C���^
//		BigInt	_Src1	�������鐔�̃I�u�W�F�N�g�i���́j
//		int		_Src2	�����鐔�̃I�u�W�F�N�g�i���́j
//	���Ԓl
//						����
//	������
//		_Dst �� _Src1 �~ _Src2
//==============================================================
void	BigInt_Mul(BigInt* _Dst, BigInt _Src1, int _Src2)
{
	unsigned	int		i		= sizeof(int) * 8;
	unsigned	int		m;
	BigInt	_n[9];		//�\�߁A_Src1�ɑ΂���1�`8�{�������l�����߂�B

	*_Dst = 0;
	_Dst->resize(_Src1.m_sz + sizeof(int) + 3);

	_Src1 <<= i;

	_n[0]	=	_Src1;

	_n[1]	=	_Src1;
	_n[1]	<<= 1;		//*2

	_n[2]	=	_n[1];
	_n[2]	+=	_Src1;	//*3 (2+1)

	_n[3]	=	_n[1];
	_n[3]	<<=	1;		//*4

	_n[4]	=	_n[3];
	_n[4]	+=	_Src1;	//*5 (4+1)

	_n[5]	=	_n[3];
	_n[5]	+=	_n[1];	//*6 (4+2)

	_n[6]	=	_n[3];
	_n[6]	+=	_n[2];	//*7 (4+3)

	_n[7]	=	_n[3];
	_n[7]	<<=	1;		//*8

	m = (_Src2 << 1) & 0x1F;
	_Src2 >>= 3;
	i = i >> 2;
	while(i > 0){
		if((m & 0x10) == 0){
			if(m != 0){
				BigInt_Add(_Dst, &_n[(m-1)/2]);
			}
		} else {
			m ^= 0x1F;
			if(m != 0){
				BigInt_Sub(_Dst, &_n[(m-1)/2]);
			}
		}
		BigInt_Shr(_Dst,4);
		m = _Src2 & 0x1F;
		_Src2 >>= 4;
		i--;
	}

	_Dst->resize();
}
