#include "StdAfx.h"
#include "BigInt.h"

//==============================================================
//			コンストラクタ
//==============================================================
BigInt::BigInt(void)
{
	//引数が無い場合は、バッファを確保しない。
	Buff_Size	= 0;
	m_sz		= 0;
	m_pt		= NULL;		//ヌルポインタ（「ぬるぽ」の語源！？）

//	m_pt		= new char[Buff_Size];
//	m_pt[0]		= 0;
}

//==============================================================
//			コンストラクタ
//==============================================================
BigInt::BigInt(BigInt&	_Src)
{
	Buff_Size	= _Src.Buff_Size;
	m_sz		= _Src.m_sz;
	m_pt		= new char [Buff_Size];
	memcpy(m_pt, _Src.m_pt, m_sz);
}

//==============================================================
//			コンストラクタ
//==============================================================
BigInt::BigInt(int		_Src)
{
	int* _pt;

	Buff_Size	= 16;
	m_sz		= sizeof(int);
	m_pt		= new char[Buff_Size];

	_pt = (int *)m_pt;
	_pt[0] = _Src;
}

//==============================================================
//			任意長の数値で初期化
//==============================================================
BigInt::BigInt(char* _ptr, unsigned int _sz)
{
	Buff_Size	= (_sz + 16) & 0xFFFFFFF0;
	m_sz		= _sz;
	m_pt		= new char[Buff_Size];

	memcpy(m_pt, _ptr, m_sz);
}

//==============================================================
//			デストラクタ
//==============================================================
BigInt::~BigInt(void)
{
	delete [] m_pt;
}

//==============================================================
//			バッファリサイズ（最小サイズ）
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//	●処理
//			バッファを、最小サイズにリサイズする。
//				m_pt	バッファのポインタ
//==============================================================
void	BigInt::Buff_resize()
{
	unsigned	int	_sz	=	(m_sz + 16) & 0xFFFFFFF0;

	if(Buff_Size != _sz){
		char*	_pt = new char [_sz];
		if(Buff_Size != 0){
			memcpy(_pt, m_pt, m_sz);
			delete [] m_pt;
		}
		m_pt = _pt;
		Buff_Size	= _sz;
	}
}

//==============================================================
//			バッファリサイズ（指定サイズ）
//--------------------------------------------------------------
//	●引数
//			unsigned int _sz	バイト数
//	●返値
//			無し
//	●処理
//			バッファを、指定サイズにリサイズする。
//			但し、16Byte単位にする。
//				m_pt	バッファのポインタ
//==============================================================
void	BigInt::Buff_resize(unsigned int _sz)
{
	unsigned	int		new_sz	= (_sz + 16) & 0xFFFFFFF0;
				char*	_pt		= new char [new_sz];
	
	if(m_sz > _sz){
		m_sz = _sz;
	}
	if(Buff_Size != 0){
		memcpy(_pt, m_pt, m_sz);
		delete [] m_pt;
	}
	m_pt = _pt;
	Buff_Size = new_sz;
}

//==============================================================
//			リサイズ（最小サイズ）
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//	●処理
//			多長倍整数のバッファを、最小サイズにリサイズする。
//			以下のメンバー変数を更新する。
//				m_sz	バッファのサイズ
//==============================================================
void	BigInt::resize()
{
	int		pt	= m_sz;						//ループ用ポインタ
	char	d = m_pt[--pt];					//読み込み用
	char	i = d >> 7;						//整数の符号（0x00:正／0xFF:負）

	//上位バイトが、どこまで余分になっているかチェック。
	while((d == i)&&(pt > 0)){
		d = m_pt[--pt];
	}

	//符号を消さないように、リサイズする。
	m_sz = pt + 1 + (((d^i) & 0x80)?1:0);

//ポインタを変えてしまうので、リサイズしない。
//#ifdef	_DEBUG
//	Buff_resize();
//#endif

}

//==============================================================
//			リサイズ（指定サイズ）
//--------------------------------------------------------------
//	●引数
//			unsigned int _sz	バイト数
//	●返値
//			無し
//	●処理
//			多長倍整数のバッファを、指定サイズにリサイズする。
//			以下のメンバー変数を更新する。
//				m_sz	バッファのサイズ
//==============================================================
void	BigInt::resize(unsigned int _sz)
{
	if(Buff_Size < _sz){
		Buff_resize(_sz);
	}

	if(m_sz < _sz){
		//拡大する場合
		memset(&m_pt[m_sz], Get_Sign() >> 7, _sz - m_sz);
	}

	m_sz = _sz;
}

//==============================================================
//			10進数 数値表示
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//==============================================================
void	BigInt::Print_Dec()
{
	string	_str;
	Get_Ascii_Dec(&_str);
	cout << _str.c_str() << endl;
}

//==============================================================
//			16進数 数値表示
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//==============================================================
void	BigInt::Print_Hex()
{
	unsigned	char*	cData	= (unsigned char*)Get_Ptr();
				int		n		= Get_Byte_Length();
				int		i		= 0;

	cout	<<	setfill('0')	<<	hex;
	while(i<n){
		cout	<<	setw(2)	<<	(int)cData[i];
		if((i % 4)==3){
			cout	<<	" ";
		}
		i++;
	}
	cout	<<	dec	<<	endl;
}

//==============================================================
//			ポインタを取得する。
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			unsigned char*	ポインタ
//==============================================================
char*	BigInt::Get_Ptr()						//
{
	return(m_pt);
}

//==============================================================
//			バイト長を得る。
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			unsigned	int	バイト長
//==============================================================
unsigned	int	BigInt::Get_Byte_Length()
{
	resize();
	return(m_sz);
}

//==============================================================
//			ビット長を得る。
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			unsigned	long	ビット長
//==============================================================
unsigned	long	BigInt::Get_Bit_Length()
{
	unsigned	long	ln;
	unsigned	int		i = 0x80;
	unsigned	char	d;
				char	s;

	resize();

	d	= Get_Sign();
	s	= d >> 7;
	ln	= m_sz;
	ln	*= 8;

	while(i > 0){
		if((d & i) != (s & i)){
			break;
		} else {
			ln--;
		}
		i >>= 1;
	}

	return(ln);
}

//==============================================================
//			10進数の文字列を作る
//--------------------------------------------------------------
//	●引数
//			string* _str	文字列を入れるオブジェクトのポインタ
//	●返値
//			無し
//==============================================================
void	BigInt::Get_Ascii_Dec(string* _asc)
{
	_asc->clear();

	if(*this == 0){

		_asc->append(1, '0');

	} else {

		BigInt			_temp;
		BigInt			_mod;
		BigInt			_div	= 10;

		string			_str;

		unsigned	int	i;
		char			_sign	= Get_Sign();

		if(_sign & 0x80){
			_temp	= -*this;
		} else {
			_temp	= *this;
		}

		while(_temp != 0){
			BigInt_Div(&_temp, &_mod, _temp, _div);
			_str.append(1, (char)_mod.m_pt[0] + 0x30);
		}

		if(_sign & 0x80){
			_asc->append(1, '-');
		}

		i	= _str.size();
		while(i > 0){
			i--;
			_asc->append(1, _str[i]);
		}
		_asc->append(1, (char)0);
	}

}

//==============================================================
//			符号（最上位Byte）を得る
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			unsigned	int	バイト長
//==============================================================
char	BigInt::Get_Sign()
{
	return(m_pt[m_sz - 1]);
}

//==============================================================
//			代入(MOV)
//--------------------------------------------------------------
//	●引数
//			char*			_ptr	整数実体のポインタ
//			unsigned int	_sz		整数のサイズ
//	●返値
//			BigInt&					オブジェクトの参照
//	●処理
//			多長倍整数を、指定の数値に設定する。
//			以下のメンバー変数を更新する。
//				m_sz	バッファのサイズ
//				m_pt	バッファのポインタ
//==============================================================
BigInt&	BigInt::Set(char* _ptr, unsigned int _sz)
{
	//バッファサイズが小さかったら、新たに確保
	if(Buff_Size < _sz){
		Buff_resize(_sz);
	}

	m_sz		= _sz;
	memcpy(m_pt, _ptr, m_sz);

	return(*this);
}

/****************************************************************/
/*			演算子のオーバーロード								*/
/****************************************************************/
//==============================================================
//			代入(MOV)
//--------------------------------------------------------------
//	●処理
//			多長倍整数を、指定のストリングスに設定する。
//			以下のメンバー変数を更新する。
//				m_sz	バッファのサイズ
//				m_pt	バッファのポインタ
//==============================================================
BigInt&	BigInt::operator=(BigInt	_Src)
{
	//バッファサイズが小さかったら、新たに確保
	if(Buff_Size < _Src.Buff_Size){
		Buff_resize(_Src.Buff_Size);
	}

	m_sz		= _Src.m_sz;
	memcpy(m_pt, _Src.m_pt, m_sz);

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator=(int		_Src)
{
	int* _pt;

	m_sz = sizeof(int);

	//バッファサイズが小さかったら、新たに確保
	if(Buff_Size < m_sz){
		Buff_resize(m_sz);
	}

	_pt = (int *)m_pt;
	_pt[0] = _Src;

	return(*this);
}

//==============================================================
//			加算(ADD)
//==============================================================
BigInt&	BigInt::operator+=(BigInt	_Src)
{
//	resize();
//	_Src.resize();

	if(m_sz <= _Src.m_sz){
		//ソースのサイズと同等orそれ以下の場合
		//ソースのサイズより、1Byte大きくする。
		resize(_Src.m_sz + 1);
	} else {
		//それ以外の場合は、現行より1Byte大きくする。
		resize(m_sz + 1);
	}

	BigInt_Add(this, &_Src);
	resize();

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator+=(int		_Src)
{
	BigInt	_temp	= _Src;
	*this += _temp;
	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator++(void			)
{
	*this += 1;
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator++(int			)
{
	BigInt	_temp	= *this;
	*this += 1;
	return(_temp);
}

//--------------------------------------------------------------
BigInt	BigInt::operator+(BigInt&	_Src)
{
	BigInt	_result;

//	resize();
//	_Src.resize();

	//1Byte大きくする。
	_result.resize(((m_sz < _Src.m_sz)? _Src.m_sz : m_sz) + 1);

	BigInt_Copy(&_result, this);
	BigInt_Add(&_result, &_Src);
	_result.resize();

	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator+(int		_Src)
{
	BigInt	_temp	= _Src;
	return((*this) + _temp);
}

//==============================================================
//			減算(SUB)
//==============================================================
BigInt&	BigInt::operator-=(BigInt	_Src)
{
//	resize();
//	_Src.resize();

	if(m_sz <= _Src.m_sz){
		//ソースのサイズと同等orそれ以下の場合
		//ソースのサイズより、1Byte大きくする。
		resize(_Src.m_sz + 1);
	} else {
		//それ以外の場合は、現行より1Byte大きくする。
		resize(m_sz + 1);
	}

	BigInt_Sub(this, &_Src);
	resize();

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator-=(int		_Src)
{
	BigInt	_temp	= _Src;
	*this -= _temp;
	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator--(void			)
{
	*this -= 1;
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator--(int			)
{
	BigInt	_temp	= *this;
	*this -= 1;
	return(_temp);
}

//--------------------------------------------------------------
BigInt	BigInt::operator-(BigInt&	_Src)
{
	BigInt	_result;

//	resize();
//	_Src.resize();

	//1Byte大きくする。
	_result.resize(((m_sz < _Src.m_sz)? _Src.m_sz : m_sz) + 1);

	BigInt_Copy(&_result, this);
	BigInt_Sub(&_result, &_Src);
	_result.resize();

	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator-(int		_Src)
{
	BigInt	_temp	= _Src;
	return((*this) - _temp);
}

//==============================================================
//			乗算(MUL)
//==============================================================
BigInt&	BigInt::operator*=(BigInt&	_Src)
{
	BigInt_Mul(this, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator*=(int		_Src)
{
	BigInt_Mul(this, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator*(BigInt&	_Src)
{
	BigInt	_result;
	BigInt_Mul(&_result, *this, _Src);
	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator*(int		_Src)
{
	BigInt	_result;
	BigInt_Mul(&_result, *this, _Src);
	return(_result);
}

//==============================================================
//			除算(DIV)
//==============================================================
BigInt&	BigInt::operator/=(BigInt&	_Src)
{
	BigInt	_temp;
	BigInt_Div(this, &_temp, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator/=(int		_Src)
{
	BigInt	_temp;
	BigInt_Div(this, &_temp, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator/(BigInt&	_Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_result, &_temp, *this, _Src);
	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator/(int		_Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_result, &_temp, *this, _Src);
	return(_result);
}

//==============================================================
//			余剰(MOD)
//==============================================================
BigInt&	BigInt::operator%=(BigInt&	_Src)
{
	BigInt	_temp;
	BigInt_Div(&_temp, this, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator%=(int		_Src)
{
	BigInt	_temp;
	BigInt_Div(&_temp, this, *this, _Src);
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator%(BigInt&	_Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_temp, &_result, *this, _Src);
	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator%(int		_Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_temp, &_result, *this, _Src);
	return(_result);
}

//==============================================================
//			マイナス(NEG)
//==============================================================
BigInt	BigInt::operator-(void			)
{
	BigInt	_result;

//	resize();

	if(Chk_Zero()){
		_result = *this;
	} else {
		_result.resize(m_sz);
		BigInt_Not(&_result, this);
		_result++;
	}

	return(_result);
}

//==============================================================
//			ビット反転(NOT)
//==============================================================
BigInt	BigInt::operator~(void			)
{
	BigInt	_result;

//	resize();

	_result.resize(m_sz);
	BigInt_Not(&_result, this);
	return(_result);
}

//==============================================================
//			論理積(AND)
//==============================================================
BigInt&	BigInt::operator&=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		resize(_Src.m_sz);
	} else {
		_Src.resize(m_sz);
	}

	BigInt_And(this, &_Src);
	resize();

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator&=(int		_Src)
{
	BigInt	_temp	= _Src;
	*this &= _temp;
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator&(BigInt&	_Src)
{
	BigInt	_result;

//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		_result.resize(_Src.m_sz);
		BigInt_Copy(&_result, this);
		BigInt_And(&_result, &_Src);
	} else {
		_result.resize(m_sz);
		BigInt_Copy(&_result, &_Src);
		BigInt_And(&_result, this);
	}

	_result.resize();

	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator&(int		_Src)
{
	BigInt	_temp	= _Src;
	return((*this) & _temp);
}

//==============================================================
//			論理和(OR)
//==============================================================
BigInt&	BigInt::operator|=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		resize(_Src.m_sz);
	} else {
		_Src.resize(m_sz);
	}

	BigInt_Or(this, &_Src);
	resize();

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator|=(int		_Src)
{
	BigInt	_temp	= _Src;
	*this |= _temp;
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator|(BigInt&	_Src)
{
	BigInt	_result;

//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		_result.resize(_Src.m_sz);
		BigInt_Copy(&_result, this);
		BigInt_Or(&_result, &_Src);
	} else {
		_result.resize(m_sz);
		BigInt_Copy(&_result, &_Src);
		BigInt_Or(&_result, this);
	}

	_result.resize();

	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator|(int		_Src)
{
	BigInt	_temp	= _Src;
	return((*this) | _temp);
}

//==============================================================
//			排他的論理和(XOR)
//==============================================================
BigInt&	BigInt::operator^=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		resize(_Src.m_sz);
	} else {
		_Src.resize(m_sz);
	}

	BigInt_Xor(this, &_Src);
	resize();

	return(*this);
}

//--------------------------------------------------------------
BigInt&	BigInt::operator^=(int		_Src)
{
	BigInt	_temp	= _Src;
	*this ^= _temp;
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator^(BigInt&	_Src)
{
	BigInt	_result;

//	resize();
//	_Src.resize();

	//サイズを同じにして、論理演算実行。
	if(m_sz <= _Src.m_sz){
		_result.resize(_Src.m_sz);
		BigInt_Copy(&_result, this);
		BigInt_Xor(&_result, &_Src);
	} else {
		_result.resize(m_sz);
		BigInt_Copy(&_result, &_Src);
		BigInt_Xor(&_result, this);
	}

	_result.resize();

	return(_result);
}

//--------------------------------------------------------------
BigInt	BigInt::operator^(int		_Src)
{
	BigInt	_temp	= _Src;
	return((*this) ^ _temp);
}

//==============================================================
//			シフト(SHL)
//==============================================================
BigInt&	BigInt::operator<<=(unsigned int	_Cnt)
{
	resize(m_sz+1+(_Cnt>>3));
	BigInt_Shl(this, _Cnt);
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator<<(unsigned int	_Cnt)
{
	BigInt	_result;

//	resize();

	_result.resize(m_sz+1+(_Cnt>>3));
	BigInt_Copy(&_result, this);
	BigInt_Shl(&_result, _Cnt);
	return(_result);
}

//==============================================================
//			シフト(SHR)
//==============================================================
BigInt&	BigInt::operator>>=(unsigned int	_Cnt)
{
	BigInt_Shr(this, _Cnt);
	resize();
	return(*this);
}

//--------------------------------------------------------------
BigInt	BigInt::operator>>(unsigned int	_Cnt)
{
	BigInt	_result = *this;		//拡張はしないので、そのまま代入する。
	BigInt_Shr(&_result, _Cnt);
	_result.resize();
	return(_result);
}

//==============================================================
//			比較演算（eq & neq）
//==============================================================
bool	BigInt::operator==(BigInt&	_Src)
{
	BigInt	_temp = *this - _Src;
	return(_temp.Chk_Zero());
}

//--------------------------------------------------------------
bool	BigInt::operator==(int		_Src)
{
	if(_Src == 0){
		return(Chk_Zero());		//_Srcが0だったら、いちいち引き算しない。
	} else {
		BigInt _temp = *this - _Src;
		return(_temp.Chk_Zero());
	}
}

//--------------------------------------------------------------
bool	BigInt::operator!=(BigInt&	_Src)
{
	return(!(*this == _Src));
}

//--------------------------------------------------------------
bool	BigInt::operator!=(int		_Src)
{
	return(!(*this == _Src));
}

//==============================================================
//			比較演算（不等号系）
//==============================================================
bool	BigInt::operator<(BigInt&	_Src)
{
	BigInt	_temp = *this - _Src;
	return(_temp.Chk_Sign() & 0x80);
}

//--------------------------------------------------------------
bool	BigInt::operator<(int		_Src)
{
	BigInt	_temp = _Src;
	return(*this < _temp);
}

//--------------------------------------------------------------
bool	BigInt::operator<=(BigInt&	_Src)
{
	BigInt	_temp = *this - _Src;
	return((_temp.Chk_Zero()) || (_temp.Chk_Sign() & 0x80));
}

//--------------------------------------------------------------
bool	BigInt::operator<=(int		_Src)
{
	BigInt	_temp = _Src;
	return(*this <= _temp);
}

//--------------------------------------------------------------
bool	BigInt::operator>(BigInt&	_Src)
{
	BigInt	_temp = *this - _Src;
	return(!((_temp.Chk_Zero()) || (_temp.Chk_Sign() & 0x80)));
}

//--------------------------------------------------------------
bool	BigInt::operator>(int		_Src)
{
	BigInt	_temp = _Src;
	return(*this > _temp);
}

//--------------------------------------------------------------
bool	BigInt::operator>=(BigInt&	_Src)
{
	BigInt	_temp = *this - _Src;
	return(!(_temp.Chk_Sign() & 0x80));
}

//--------------------------------------------------------------
bool	BigInt::operator>=(int		_Src)
{
	BigInt	_temp = _Src;
	return(*this >= _temp);
}



//==============================================================
//			累乗（Power）
//==============================================================
BigInt	BigInt::Power(BigInt&	_Src)
{
	BigInt	_result;
	BigInt_Power(&_result, *this, _Src);
	return(_result);
}
//--------------------------------------------------------------
BigInt	BigInt::Power(int	_Src)
{
	BigInt	_result;
	BigInt_Power(&_result, *this, _Src);
	return(_result);
}



/****************************************************************/
/*			グローバル演算子のオーバーロード					*/
/****************************************************************/
//==============================================================
//			加算(ADD)
//==============================================================
BigInt	operator+(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp += _Src;
	return(_temp);
}

//==============================================================
//			減算(SUB)
//==============================================================
BigInt	operator-(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp -= _Src;
	return(_temp);
}

//==============================================================
//			乗算(MUL)
//==============================================================
BigInt	operator*(int _Dst, BigInt& _Src)
{
	BigInt	_result;
	BigInt_Mul(&_result, _Src, _Dst);
	return(_result);
}

//==============================================================
//			除算(Div)
//==============================================================
BigInt	operator/(int _Dst, BigInt& _Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_result, &_temp, _Dst, _Src);
	return(_result);
}

//==============================================================
//			剰余(Mod)
//==============================================================
BigInt	operator%(int _Dst, BigInt& _Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_temp,&_result, _Dst, _Src);
	return(_result);
}

//==============================================================
//			論理積(AND)
//==============================================================
BigInt	operator&(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp &= _Src;
	return(_temp);
}

//==============================================================
//			論理和(OR)
//==============================================================
BigInt	operator|(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp |= _Src;
	return(_temp);
}

//==============================================================
//			排他的論理和(XOR)
//==============================================================
BigInt	operator^(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp ^= _Src;
	return(_temp);
}

//==============================================================
//			比較演算
//==============================================================
bool	operator==(int _Dst, BigInt& _Src)
{
	return(_Src == _Dst);
}

bool	operator!=(int _Dst, BigInt& _Src)
{
	return(_Src != _Dst);
}

bool	operator<( int _Dst, BigInt& _Src)
{
	return(_Src > _Dst);
}

bool	operator<=(int _Dst, BigInt& _Src)
{
	return(_Src >= _Dst);
}

bool	operator>( int _Dst, BigInt& _Src)
{
	return(_Src < _Dst);
}

bool	operator>=(int _Dst, BigInt& _Src)
{
	return(_Src <= _Dst);
}

