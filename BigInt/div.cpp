#include "StdAfx.h"

//==============================================================
//			除算（Div）	※回復法
//--------------------------------------------------------------
//	●引数
//		BigInt&	_Result	結果（商）
//		BigInt&	_Mod	結果（余）
//		BigInt	_Drc	割る数のオブジェクト（実体）
//		BigInt	_Src	割られる数のオブジェクト（実体）
//	●返値
//		bool			true	ok
//						false	0で除算
//	●処理
//		回復法により、商と余を求めます。
//		_Result	＝ _Dst ÷ _Src
//		_Mod	＝ _Dst mod _Src
//==============================================================
bool	BigInt_Div(BigInt* _Result, BigInt* _Mod, BigInt _Dst, BigInt _Src)
{
	//0除算のチェック
	if(_Src == 0){
#ifdef	Zero_Div_Err
		//０除算割り込みを発生させる（例外になりエラーになる）。
		__asm{	int	0	}
#endif
		return(false);
	}

	char	sign_d	= _Dst.Get_Sign();
	char	sign_s	= _Src.Get_Sign();

	//負の数だったら、正の数にする
	if(sign_d & 0x80){
		_Dst = -_Dst;
	}

	if(sign_s & 0x80){
		_Src = -_Src;
	}

	_Dst.resize();
	_Src.resize();

	//筆算の要領で割り算
	*_Result	= 0;
	if(_Dst.m_sz >= _Src.m_sz){
		//割る数の桁数の方が小さい
		unsigned	int		dt		= (_Dst.m_sz - _Src.m_sz + 1) * 8;
		BigInt				_temp	= 0x01;
		BigInt				_cmp;

		_Src <<= dt;
		_temp <<= dt;
		_Result->resize(_temp.m_sz);		//_Result と _temp を同じサイズにする。
		do{
			BigInt_Shr1(&_Src);		//リサイズは省略する。
			BigInt_Shr1(&_temp);	//リサイズは省略（リサイズすると、BigInt_OR()関数で狂う）。
			_cmp = _Dst - _Src;
			if (!(_cmp.Chk_Sign() & 0x80)){
				BigInt_Or(_Result, &_temp);		//_Result |= _temp; 
				BigInt_Copy(&_Dst, &_cmp);		//__Dst    = _cmp;
			}
			dt--;
		} while(dt > 0);
	}

	_Result->resize();		//リサイズ

	//割る数、割られる数のどちらかが負数だったら、結果を負数へ。
	if((sign_d ^ sign_s) & 0x80){
		*_Result = -*_Result;		//符号を反転する
		//余りがある場合
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
	_Mod->resize();			//リサイズ

	return(true);
}
