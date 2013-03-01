#pragma once

/****************************************************************/
/*			定数定義											*/
/****************************************************************/
//0で除算時に、０除算割り込み（例外）を発生させる場合、定義して下さい。
//#define	Zero_Div_Err			

/****************************************************************/
/*			プロトタイプ宣言									*/
/****************************************************************/
class	BigInt;

//アセンブリ言語で書かれた関数
extern "C"{
	void			__fastcall	BigInt_Copy(BigInt* _dst,	BigInt* _src);	//符号拡張付きコピー
	void			__fastcall	BigInt_Add(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Sub(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Not(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_And(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Or(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Xor(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Shl(	BigInt* _src,	unsigned int _Cnt);
	unsigned	int	__fastcall	BigInt_Shr(	BigInt* _src,	unsigned int _Cnt);

	void			__fastcall	BigInt_Shl1(BigInt* _src);	//高速1bitシフト
	unsigned	int	__fastcall	BigInt_Shr1(BigInt* _src);	//高速1bitシフト
}


/****************************************************************/
/*			クラス定義											*/
/****************************************************************/
class BigInt
{
private:
	//--------------
	//メンバー変数
	unsigned	int		m_sz;			//計算用のサイズ
				char*	m_pt;			//バッファのポインタ
	unsigned	int		Buff_Size;		//実際に確保したメモリサイズ

public:
	//--------------
	//関数
	BigInt(void);									// 0 で初期化
	BigInt(BigInt&	_Src);							//指定された多倍長整数で初期化
	BigInt(int		_Src);							//指定された数値で初期化
	BigInt(char* _ptr, unsigned int _sz);			//任意長の数値で初期化
	~BigInt(void);									//

	BigInt&			Set(char* _ptr, unsigned int _sz);	//代入

private:
	void			Buff_resize();					//メモリ・バッファを最小限に確保しなおす。
	void			Buff_resize(unsigned int _sz);	//メモリ・バッファを指定値に確保しなおす。

public:
	void			resize();						//計算サイズを最小限に設定する。
	void			resize(unsigned int _sz);		//計算サイズを指定値に設定する。

	void			Print_Dec();					//10進数表示を標準出力に出力
	void			Print_Hex();					//16進数表示を標準出力に出力

	char*			Get_Ptr();						//整数のポインタを取得する。
	unsigned int	Get_Byte_Length();				//整数のバイト長を取得する
	unsigned long	Get_Bit_Length();				//整数のビット長を取得する
	char			Get_Sign();						//整数の符号を取得する
	void			Get_Ascii_Dec(string* _asc);	//10進文字列を取得する

//--------------
//operator 演算子
public:
	//代入
	BigInt&	operator=(BigInt	_Src);	//代入
	BigInt&	operator=(int		_Src);

	//四則演算
	BigInt&	operator+=(BigInt	_Src);	//加算（単項）
	BigInt&	operator+=(int		_Src);
	BigInt&	operator++(				);
	BigInt	operator++(int			);	//（※実体を返す）
	BigInt	operator+(BigInt&	_Src);	//加算（２項）
	BigInt	operator+(int		_Src);

	BigInt&	operator-=(BigInt	_Src);	//減算（単項）
	BigInt&	operator-=(int		_Src);
	BigInt&	operator--(				);
	BigInt	operator--(int			);	//（※実体を返す）
	BigInt	operator-(BigInt&	_Src);	//減算（２項）
	BigInt	operator-(int		_Src);

	BigInt&	operator*=(BigInt&	_Src);	//乗算
	BigInt&	operator*=(int		_Src);
	BigInt	operator*(BigInt&	_Src);	//
	BigInt	operator*(int		_Src);

	BigInt&	operator/=(BigInt&	_Src);	//除算
	BigInt&	operator/=(int		_Src);
	BigInt	operator/(BigInt&	_Src);	//
	BigInt	operator/(int		_Src);

	BigInt&	operator%=(BigInt&	_Src);	//Mod
	BigInt&	operator%=(int		_Src);
	BigInt	operator%(BigInt&	_Src);	//
	BigInt	operator%(int		_Src);

	//補数
	BigInt	operator-(void);			//NEG
	BigInt	operator~(void);			//NOT

	//論理演算
	BigInt&	operator&=(BigInt&	_Src);	//AND
	BigInt&	operator&=(int		_Src);
	BigInt	operator&(BigInt&	_Src);
	BigInt	operator&(int		_Src);
	BigInt&	operator|=(BigInt&	_Src);	//OR
	BigInt&	operator|=(int		_Src);
	BigInt	operator|(BigInt&	_Src);
	BigInt	operator|(int		_Src);
	BigInt&	operator^=(BigInt&	_Src);	//XOR
	BigInt&	operator^=(int		_Src);
	BigInt	operator^(BigInt&	_Src);
	BigInt	operator^(int		_Src);

	//シフト
//	BigInt&	operator<<=(BigInt&	_Cnt);
//	BigInt	operator<<(BigInt&	_Cnt);
//	BigInt&	operator>>=(BigInt&	_Cnt);
//	BigInt	operator>>(BigInt&	_Cnt);
	BigInt&	operator<<=(unsigned int	_Cnt);
	BigInt	operator<<( unsigned int	_Cnt);
	BigInt&	operator>>=(unsigned int	_Cnt);
	BigInt	operator>>( unsigned int	_Cnt);

	//比較
	bool	operator==(BigInt&	_Src);
	bool	operator==(int		_Src);
	bool	operator!=(BigInt&	_Src);
	bool	operator!=(int		_Src);
	bool	operator<(BigInt&	_Src);
	bool	operator<(int		_Src);
	bool	operator<=(BigInt&	_Src);
	bool	operator<=(int		_Src);
	bool	operator>(BigInt&	_Src);
	bool	operator>(int		_Src);
	bool	operator>=(BigInt&	_Src);
	bool	operator>=(int		_Src);

	bool	Chk_Zero(){
		resize();
		return((m_sz==1) && (m_pt[0] == 0));
	}
	char	Chk_Sign(){
		return(Get_Sign());
	}

	//Math
	BigInt	Power(BigInt&	_Src);	//累乗
	BigInt	Power(int		_Src);

	//グローバルな演算子
	friend	BigInt	operator-(int _Dst, BigInt& _Src);	//Add
	friend	BigInt	operator+(int _Dst, BigInt& _Src);	//Sub
	friend	BigInt	operator*(int _Dst, BigInt& _Src);	//Mul
	friend	BigInt	operator/(int _Dst, BigInt& _Src);	//Div
	friend	BigInt	operator%(int _Dst, BigInt& _Src);	//Mod
	friend	BigInt	operator&(int _Dst, BigInt& _Src);	//And
	friend	BigInt	operator|(int _Dst, BigInt& _Src);	//Or
	friend	BigInt	operator^(int _Dst, BigInt& _Src);	//Xor
//	friend	BigInt	operator<<(int _Dst, BigInt& _Cnt);	//Shl
//	friend	BigInt	operator>>(int _Dst, BigInt& _Cnt);	//Shr
	friend	bool	operator==(int _Dst, BigInt& _Src);	//
	friend	bool	operator!=(int _Dst, BigInt& _Src);	//
	friend	bool	operator<( int _Dst, BigInt& _Src);	//
	friend	bool	operator<=(int _Dst, BigInt& _Src);	//
	friend	bool	operator>( int _Dst, BigInt& _Src);	//
	friend	bool	operator>=(int _Dst, BigInt& _Src);	//

	friend	void	BigInt_Mul_FFT(	BigInt* _Dst,	BigInt _Src1, BigInt _Src2);
	friend	void	BigInt_Mul(		BigInt* _Dst,	BigInt _Src1, BigInt _Src2);
	friend	void	BigInt_Mul(		BigInt* _Dst,	BigInt _Src1, int _Src2);
	friend	bool	BigInt_Div(		BigInt* _Result,BigInt* _Mod, BigInt _Dst, BigInt _Src);
	friend	bool	BigInt_Power(	BigInt* _Result,BigInt& _Dst, BigInt& _Src);
	friend	bool	BigInt_Power(	BigInt* _Result,BigInt  _Dst, int _Src);
};
