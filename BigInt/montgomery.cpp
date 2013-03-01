#include "StdAfx.h"
#include "montgomery.h"

//==============================================================
//			�R���X�g���N�^
//==============================================================
montgomery::montgomery(void)
{
}

//==============================================================
//			�f�X�g���N�^
//==============================================================
montgomery::~montgomery(void)
{
}

//==============================================================
//			Set Modulo
//--------------------------------------------------------------
//	������
//		BigInt	N		�@�iModulo�j
//	���Ԓl
//						����
//	������
//		�����S�������_�N�V�����ׂ̈ɁA�\��N', R, R2���v�Z����B
//		Calculate N' R, R2 for the montgomery reduction.
//			m	<=	Int( log2( N ) )		�c N�̃r�b�g��
//			R	<=	2^(m+1)					�c 2�̗ݏ抎��N�ȏ�̒l
//			R2	<=	R * R mod N
//			NN'	��	-1 mod R	�ƂȂ�N'���Z�o����B
//	������
//		N�͑f���ł��鎖�B	�iGCD( N , R )==1�𖞂������j
//		�i���̏����𖞂����Ȃ���R�Ƒf�ɂȂ炸�A�����S�������_�N�V�����ł��Ȃ��j
//	�s ���w�I��� �t
//		R�͑f�����������Ă��A2�݂̂ł���̂ŁA
//		N�͑f���������������ɁA2���܂܂Ȃ��l�A�܂�f���ł���Ηǂ��B
//==============================================================
void	montgomery::Set_Modulo(BigInt* N)
{
				BigInt	t		=	0;	//N'�v�Z�p	(t mod R)��R-1��
				BigInt	i		=	1;	//N'�v�Z�p	
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

	//NN'	��	 -1 mod R	�́A
	//NN'	��	R-1 mod R	�ł��邩��A���̏����𖞂���N'��T���B

	//R��2^m�Ȃ̂ŁA�����-1����Ɖ��ʃr�b�g�͑S��"1"�ɂȂ�B
	//�ȉ��́A��Z NN'��R-1 �ɂ����āA�Q�i���̕M�Z�� N' �����߂�R�[�h�B
	//���ʂ���1bit���t�Z���A���ʃr�b�g���S��"1"�ɂȂ�悤�ɁA
	//while()�\���ɂāAN*2^n��i�K�I�ɉ��Z����B
	i.resize(szN);
	m_invN.resize(szN);
	t.resize(szN+1);
	ptT = t.Get_Ptr();				//
	while(m>0){						//R-1�̃r�b�g�����J��Ԃ��i�܂�N�̃r�b�g���j�B
		if((ptT[0] & 0x01) == 0){	//�ŉ��ʃr�b�g��"0"��������A
			BigInt_Add(&t, N);		//111...1<2>�ɂ��邽�߂ɁAN�����Z�i�{����N<<���ځj
			BigInt_Or(&m_invN, &i);	//���Z�����̂ŁAN'�̂��̌���1�ɂ���B
		}
		BigInt_Shr1(&t);			//�{���̓V�t�g���Ȃ����ǁA�v�Z�̌������̂��߁B
		BigInt_Shl1(&i);			//���݂̌�
		m--;
	}
}

//==============================================================
//		Montgomery reduction	�i�����S�����E���_�N�V�����j
//--------------------------------------------------------------
//	������
//		BigInt&	result	���ʂ�����I�u�W�F�N�g�ւ̃|�C���^
//		BigInt	T		�����S�������_�N�V�������鐮���̃I�u�W�F�N�g
//	���Ԓl
//						����
//	������
//		t	<=	( T + ( TN' mod R ) N ) / R
//		if t>=N then t-= N
//	������
//		0 �� T �� 2N-1	�ł��鎖�B
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
//			��Z�iMUL�j
//--------------------------------------------------------------
//	������
//		BigInt&	_Dst	���ʂ̐��l������I�u�W�F�N�g�̃|�C���^
//		BigInt	_Src1	�������鐔�̃I�u�W�F�N�g�i�Q�Ɓj
//		int		_Src2	�����鐔�̃I�u�W�F�N�g�i�Q�Ɓj
//	���Ԓl
//						����
//	������
//		_Dst �� _Src1 �~ _Src2 mod N
//	������
//		�ȉ��̏����𖞂�����
//		0 �� src1 �� N-1
//		0 �� src2 �� N-1
//==============================================================
void	montgomery::Mul(BigInt* dst, BigInt& src1, BigInt& src2)
{
	BigInt_Mul(dst, src1, src2);
	MR(dst, *dst);
	BigInt_Mul(dst, *dst, m_R2);
	MR(dst, *dst);
}

//==============================================================
//			�ݏ�i�o�C�i���[�@�F���ʌ�����v�Z�j
//--------------------------------------------------------------
//	������
//		BigInt*	_Result	���ʂ̐��l������I�u�W�F�N�g�̃|�C���^
//		BigInt	_Src1	�ݏ悳��鐔�̃I�u�W�F�N�g�i�Q�Ɓj
//		BigInt&	_Src2	�ݏ悷�鐔�̃I�u�W�F�N�g�i�Q�Ɓj
//	���Ԓl
//						����
//	������
//		_Result �� _Src1 sup _Src2 mod N
//	������
//		�ȉ��̏����𖞂�����
//		0 �� _Src1 �� N-1
//		0 �� _Src2
//==============================================================
void	montgomery::Power(BigInt* _Result, BigInt& _Src1, BigInt& _Src2)
{
	//�搔�����̏ꍇ
	BigInt	temp	= _Src2;
	BigInt	dst		= _Src1;

	//----------
	//�����S�����\���ɕϊ�
	dst	*=	m_R2;
	MR(&dst, dst);
	MR(_Result, m_R2);

	//----------
	//�����S�����̈�Ōv�Z
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
	//�����S�����\������t�ϊ�
	MR(_Result, *_Result);
}
