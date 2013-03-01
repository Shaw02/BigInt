#include "StdAfx.h"
#include "montgomery.h"

//==============================================================
//			コンストラクタ
//==============================================================
montgomery::montgomery(void)
{
}

//==============================================================
//			デストラクタ
//==============================================================
montgomery::~montgomery(void)
{
}

//==============================================================
//			Set Modulo
//--------------------------------------------------------------
//	●引数
//		BigInt	N		法（Modulo）
//	●返値
//						無し
//	●処理
//		モンゴメリリダクションの為に、予めN', R, R2を計算する。
//		Calculate N' R, R2 for the montgomery reduction.
//			m	<=	Int( log2( N ) )		… Nのビット長
//			R	<=	2^(m+1)					… 2の累乗且つN以上の値
//			R2	<=	R * R mod N
//			NN'	≡	-1 mod R	となるN'を算出する。
//	●注意
//		Nは素数である事。	（GCD( N , R )==1を満たす事）
//		（この条件を満たさないとRと素にならず、モンゴメリリダクションできない）
//	《 数学的解説 》
//		Rは素因数分解しても、2のみであるので、
//		Nは素因数分解した時に、2を含まない値、つまり素数であれば良い。
//==============================================================
void	montgomery::Set_Modulo(BigInt* N)
{
				BigInt	t		=	0;	//N'計算用	(t mod R)がR-1に
				BigInt	i		=	1;	//N'計算用	
				char*	ptT;
	unsigned	int		szN		=	N->Get_Byte_Length();
	unsigned	int		m		=	N->Get_Bit_Length();

						m_m		=	m;

						m_R_1	=	1;
						m_R_1	<<=	m;
						m_R_1	-=	1;

						m_R2	=	1;
						m_R2	<<= m*2;
						m_R2	%=	*N;

						m_N		=	*N;
						m_invN	=	0;

	//NN'	≡	 -1 mod R	は、
	//NN'	≡	R-1 mod R	であるから、この条件を満たすN'を探す。

	//Rは2^mなので、これに-1すると下位ビットは全て"1"になる。
	//以下は、乗算 NN'＝R-1 において、２進数の筆算で N' を求めるコード。
	//下位から1bitずつ逆算し、下位ビットが全て"1"になるように、
	//while()構文にて、N*2^nを段階的に加算する。
	i.resize(szN);
	m_invN.resize(szN);
	t.resize(szN+1);
	ptT = t.Get_Ptr();				//
	while(m>0){						//R-1のビット数分繰り返す（つまりNのビット数）。
		if((ptT[0] & 0x01) == 0){	//最下位ビットが"0"だったら、
			BigInt_Add(&t, N);		//111...1<2>にするために、Nを加算（本来はN<<桁目）
			BigInt_Or(&m_invN, &i);	//加算したので、N'のその桁を1にする。
		}
		BigInt_Shr1(&t);			//本来はシフトしないけど、計算の効率化のため。
		BigInt_Shl1(&i);			//現在の桁
		m--;
	}
}

//==============================================================
//		Montgomery reduction	（モンゴメリ・リダクション）
//--------------------------------------------------------------
//	●引数
//		BigInt&	result	結果を入れるオブジェクトへのポインタ
//		BigInt	T		モンゴメリリダクションする整数のオブジェクト
//	●返値
//						無し
//	●処理
//		t	<=	( T + ( TN' mod R ) N ) / R
//		if t>=N then t-= N
//	●注意
//		0 ≦ T ≦ 2N-1	である事。
//==============================================================
void	montgomery::MR(BigInt* result, BigInt T)
{
//	*result = (T + (T * m_invN & m_R_1) * m_N) >> m_m;

	BigInt_Mul(result, T, m_invN);
	*result	&=	m_R_1;
	*result	*=	m_N;
	*result	+=	T;
	*result	>>=	m_m;

	if(*result >= m_N){
		*result	-=	m_N;
	}
}

//==============================================================
//			乗算（MUL）
//--------------------------------------------------------------
//	●引数
//		BigInt&	_Dst	結果の数値を入れるオブジェクトのポインタ
//		BigInt	_Src1	かけられる数のオブジェクト（参照）
//		int		_Src2	かける数のオブジェクト（参照）
//	●返値
//						無し
//	●処理
//		_Dst ＝ _Src1 × _Src2 mod N
//	●注意
//		以下の条件を満たす事
//		0 ≦ src1 ≦ N-1
//		0 ≦ src2 ≦ N-1
//==============================================================
void	montgomery::Mul(BigInt* dst, BigInt& src1, BigInt& src2)
{
	BigInt_Mul(dst, src1, src2);
	MR(dst, *dst);
	BigInt_Mul(dst, *dst, m_R2);
	MR(dst, *dst);
}

//==============================================================
//			累乗（バイナリー法：下位桁から計算）
//--------------------------------------------------------------
//	●引数
//		BigInt*	_Result	結果の数値を入れるオブジェクトのポインタ
//		BigInt	_Src1	累乗される数のオブジェクト（参照）
//		BigInt&	_Src2	累乗する数のオブジェクト（参照）
//	●返値
//						無し
//	●処理
//		_Result ＝ _Src1 sup _Src2 mod N
//	●注意
//		以下の条件を満たす事
//		0 ≦ _Src1 ≦ N-1
//		0 ≦ _Src2
//==============================================================
void	montgomery::Power(BigInt* _Result, BigInt& _Src1, BigInt& _Src2)
{
	//乗数が正の場合
	BigInt	temp	= _Src2;
	BigInt	dst		= _Src1;

	//----------
	//モンゴメリ表現に変換
	dst	*=	m_R2;
	MR(&dst, dst);
	MR(_Result, m_R2);

	//----------
	//モンゴメリ領域で計算
	while (temp.Chk_Zero() == false){
		if(BigInt_Shr1(&temp) != 0){
			*_Result *= dst;
			MR(_Result, *_Result);
			break;
		}
		dst *= dst;
		MR(&dst, dst);
	};

	while (temp.Chk_Zero() == false){
		dst *= dst;
		MR(&dst, dst);
		if(BigInt_Shr1(&temp) != 0){
			*_Result *= dst;
			MR(_Result, *_Result);
		}
	};

	//----------
	//モンゴメリ表現から逆変換
	MR(_Result, *_Result);
}
