#include "StdAfx.h"

//==============================================================
//			乗算（MUL） with FFT（高速フーリエ変換）
//--------------------------------------------------------------
//	●引数
//		BigInt&	_Dst	結果の数値を入れるオブジェクトのポインタ
//		BigInt	_Src1	かけられる数のオブジェクト（実体）
//		BigInt	_Src2	かける数のオブジェクト（実体）
//	●返値
//						無し
//	●処理
//		_Dst ＝ _Src1 × _Src2
//==============================================================
void	BigInt_Mul_FFT(BigInt* _Dst, BigInt _Src1, BigInt _Src2)
{
	unsigned	int		_sz1	= _Src1.m_sz;
	unsigned	int		_sz2	= _Src2.m_sz;
				char	s1		= _Src1.Get_Sign();
				char	s2		= _Src2.Get_Sign();

	//符号をチェックし、正の数へ
	if(s1 & 0x80){
		_Src1 = - _Src1;
	}
	if(s2 & 0x80){
		_Src2 = - _Src2;
	}

	//ＦＦＴ


	//乗算


	//ＦＦＴ-1


	//符号を戻す
	if((s1 ^ s2) & 0x80){
		*_Dst = -*_Dst;
	}
	_Dst->resize();
}
