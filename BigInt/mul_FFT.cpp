#include "StdAfx.h"

//==============================================================
//			��Z�iMUL�j with FFT�i�����t�[���G�ϊ��j
//--------------------------------------------------------------
//	������
//		BigInt&	_Dst	���ʂ̐��l������I�u�W�F�N�g�̃|�C���^
//		BigInt	_Src1	�������鐔�̃I�u�W�F�N�g�i���́j
//		BigInt	_Src2	�����鐔�̃I�u�W�F�N�g�i���́j
//	���Ԓl
//						����
//	������
//		_Dst �� _Src1 �~ _Src2
//==============================================================
void	BigInt_Mul_FFT(BigInt* _Dst, BigInt _Src1, BigInt _Src2)
{
	unsigned	int		_sz1	= _Src1.m_sz;
	unsigned	int		_sz2	= _Src2.m_sz;
				char	s1		= _Src1.Get_Sign();
				char	s2		= _Src2.Get_Sign();

	//�������`�F�b�N���A���̐���
	if(s1 & 0x80){
		_Src1 = - _Src1;
	}
	if(s2 & 0x80){
		_Src2 = - _Src2;
	}

	//�e�e�s


	//��Z


	//�e�e�s-1


	//������߂�
	if((s1 ^ s2) & 0x80){
		*_Dst = -*_Dst;
	}
	_Dst->resize();
}
