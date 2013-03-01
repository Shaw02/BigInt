#include "StdAfx.h"

//==============================================================
//			累乗（バイナリー法：上位桁から計算）
//--------------------------------------------------------------
//	●引数
//		BigInt*	_Result	結果の数値を入れるオブジェクトのポインタ
//		BigInt&	_Dst	累乗される数のオブジェクト（参照）
//		BigInt&	_Src	累乗する数のオブジェクト（参照）
//	●返値
//						無し
//	●処理
//		_Dst ＝ _Src1 sup _Src2
//==============================================================
bool	BigInt_Power(BigInt* _Result,BigInt& _Dst, BigInt& _Src)
{
	*_Result = 1;

	if(_Src != 0){
					char	sign_s	= _Src.Get_Sign();
					BigInt	temp	= ((sign_s & 0x80)? -_Src : _Src );
		unsigned	int		k		= temp.Get_Byte_Length();
					char*	pt		= temp.Get_Ptr();
		unsigned	char	i		= 0x80;

		//計算結果想定される長さにリサイズ（何回もnew/deleteを呼ばないように）
		_Result->Buff_resize(_Dst.Get_Byte_Length() << temp.Get_Bit_Length());

		do{
			if(pt[k-1] & i){
				i >>= 1;
				*_Result = _Dst;
				break;
			}
			i >>= 1;
		} while (i > 0);

		do{
			k--;
			while (i > 0){
				*_Result *= *_Result;
				if(pt[k] & i){
					*_Result *= _Dst;
				}
				i >>= 1;
			};
			i = 0x80;
		} while (k > 0);

		if(sign_s & 0x80){
			return(BigInt_Div(_Result, &temp, 1, *_Result));
		} else {
			return(true);
		}
	}
}

//==============================================================
//			累乗（バイナリー法：下位桁から計算）
//--------------------------------------------------------------
//	●引数
//		BigInt*	_Result	結果の数値を入れるオブジェクトのポインタ
//		BigInt	_Dst	累乗される数のオブジェクト（実体）
//		int		_Src	累乗する数のオブジェクト（実体）
//	●返値
//						無し
//	●処理
//		_Dst ＝ _Src1 sup _Src2
//==============================================================
bool	BigInt_Power(BigInt* _Result,BigInt _Dst, int _Src)
{

	int		temp	= ((_Src < 0)? -_Src : _Src );
	BigInt	_temp;

	//計算結果想定される長さにリサイズ（何回もnew/deleteを呼ばないように）
	_Result->Buff_resize(_Dst.Get_Byte_Length() * temp);

	if(temp & 0x01){
		*_Result = _Dst;
	} else {
		*_Result = 1;
	}

	while (temp > 0){
		_Dst *= _Dst;
		temp >>= 1;
		if(temp & 0x01){
			*_Result *= _Dst;
		}
	};

	if(_Src < 0){
		return(BigInt_Div(_Result, &_temp, 1, *_Result));
	} else {
		return(true);
	}
}

