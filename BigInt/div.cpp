#include "StdAfx.h"

//==============================================================
//			���Z�iDiv�j	���񕜖@
//--------------------------------------------------------------
//	������
//		BigInt&	_Result	���ʁi���j
//		BigInt&	_Mod	���ʁi�]�j
//		BigInt	_Drc	���鐔�̃I�u�W�F�N�g�i���́j
//		BigInt	_Src	�����鐔�̃I�u�W�F�N�g�i���́j
//	���Ԓl
//		bool			true	ok
//						false	0�ŏ��Z
//	������
//		�񕜖@�ɂ��A���Ɨ]�����߂܂��B
//		_Result	�� _Dst �� _Src
//		_Mod	�� _Dst mod _Src
//==============================================================
bool	BigInt_Div(BigInt* _Result, BigInt* _Mod, BigInt _Dst, BigInt _Src)
{
	//0���Z�̃`�F�b�N
	if(_Src == 0){
#ifdef	Zero_Div_Err
		//�O���Z���荞�݂𔭐�������i��O�ɂȂ�G���[�ɂȂ�j�B
		__asm{	int	0	}
#endif
		return(false);
	}

	char	sign_d	= _Dst.Get_Sign();
	char	sign_s	= _Src.Get_Sign();

	//���̐���������A���̐��ɂ���
	if(sign_d & 0x80){
		_Dst = -_Dst;
	}

	if(sign_s & 0x80){
		_Src = -_Src;
	}

	_Dst.resize();
	_Src.resize();

	//�M�Z�̗v�̂Ŋ���Z
	*_Result	= 0;
	if(_Dst.m_sz >= _Src.m_sz){
		//���鐔�̌����̕���������
		unsigned	int		dt		= (_Dst.m_sz - _Src.m_sz + 1) * 8;
		BigInt				_temp	= 0x01;
		BigInt				_cmp;

		_Src <<= dt;
		_temp <<= dt;
		_Result->resize(_temp.m_sz);		//_Result �� _temp �𓯂��T�C�Y�ɂ���B
		do{
			BigInt_Shr1(&_Src);		//���T�C�Y�͏ȗ�����B
			BigInt_Shr1(&_temp);	//���T�C�Y�͏ȗ��i���T�C�Y����ƁABigInt_OR()�֐��ŋ����j�B
			_cmp = _Dst - _Src;
			if (!(_cmp.Chk_Sign() & 0x80)){
				BigInt_Or(_Result, &_temp);		//_Result |= _temp; 
				BigInt_Copy(&_Dst, &_cmp);		//__Dst    = _cmp;
			}
			dt--;
		} while(dt > 0);
	}

	_Result->resize();		//���T�C�Y

	//���鐔�A�����鐔�̂ǂ��炩��������������A���ʂ𕉐��ցB
	if((sign_d ^ sign_s) & 0x80){
		*_Result = -*_Result;		//�����𔽓]����
		//�]�肪����ꍇ
		if(_Dst != 0){
			*_Result -= 1;
			if(sign_s & 0x80){
				*_Mod	= _Dst - _Src;
			} else {
				*_Mod	= _Src - _Dst;
			}
		} else {
			*_Mod = 0;
		}
	} else {
		if(sign_s & 0x80){
			*_Mod	= -_Dst;
		} else {
			*_Mod	=  _Dst;
		}
	}
	_Mod->resize();			//���T�C�Y

	return(true);
}
