#include "StdAfx.h"
#include "BigInt.h"

//==============================================================
//			�R���X�g���N�^
//==============================================================
BigInt::BigInt(void)
{
	//�����������ꍇ�́A�o�b�t�@���m�ۂ��Ȃ��B
	Buff_Size	= 0;
	m_sz		= 0;
	m_pt		= NULL;		//�k���|�C���^�i�u�ʂ�ہv�̌ꌹ�I�H�j

//	m_pt		= new char[Buff_Size];
//	m_pt[0]		= 0;
}

//==============================================================
//			�R���X�g���N�^
//==============================================================
BigInt::BigInt(BigInt&	_Src)
{
	Buff_Size	= _Src.Buff_Size;
	m_sz		= _Src.m_sz;
	m_pt		= new char [Buff_Size];
	memcpy(m_pt, _Src.m_pt, m_sz);
}

//==============================================================
//			�R���X�g���N�^
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
//			�C�Ӓ��̐��l�ŏ�����
//==============================================================
BigInt::BigInt(char* _ptr, unsigned int _sz)
{
	Buff_Size	= (_sz + 16) & 0xFFFFFFF0;
	m_sz		= _sz;
	m_pt		= new char[Buff_Size];

	memcpy(m_pt, _ptr, m_sz);
}

//==============================================================
//			�f�X�g���N�^
//==============================================================
BigInt::~BigInt(void)
{
	delete [] m_pt;
}

//==============================================================
//			�o�b�t�@���T�C�Y�i�ŏ��T�C�Y�j
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//	������
//			�o�b�t�@���A�ŏ��T�C�Y�Ƀ��T�C�Y����B
//				m_pt	�o�b�t�@�̃|�C���^
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
//			�o�b�t�@���T�C�Y�i�w��T�C�Y�j
//--------------------------------------------------------------
//	������
//			unsigned int _sz	�o�C�g��
//	���Ԓl
//			����
//	������
//			�o�b�t�@���A�w��T�C�Y�Ƀ��T�C�Y����B
//			�A���A16Byte�P�ʂɂ���B
//				m_pt	�o�b�t�@�̃|�C���^
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
//			���T�C�Y�i�ŏ��T�C�Y�j
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//	������
//			�����{�����̃o�b�t�@���A�ŏ��T�C�Y�Ƀ��T�C�Y����B
//			�ȉ��̃����o�[�ϐ����X�V����B
//				m_sz	�o�b�t�@�̃T�C�Y
//==============================================================
void	BigInt::resize()
{
	int		pt	= m_sz;						//���[�v�p�|�C���^
	char	d = m_pt[--pt];					//�ǂݍ��ݗp
	char	i = d >> 7;						//�����̕����i0x00:���^0xFF:���j

	//��ʃo�C�g���A�ǂ��܂ŗ]���ɂȂ��Ă��邩�`�F�b�N�B
	while((d == i)&&(pt > 0)){
		d = m_pt[--pt];
	}

	//�����������Ȃ��悤�ɁA���T�C�Y����B
	m_sz = pt + 1 + (((d^i) & 0x80)?1:0);

//�|�C���^��ς��Ă��܂��̂ŁA���T�C�Y���Ȃ��B
//#ifdef	_DEBUG
//	Buff_resize();
//#endif

}

//==============================================================
//			���T�C�Y�i�w��T�C�Y�j
//--------------------------------------------------------------
//	������
//			unsigned int _sz	�o�C�g��
//	���Ԓl
//			����
//	������
//			�����{�����̃o�b�t�@���A�w��T�C�Y�Ƀ��T�C�Y����B
//			�ȉ��̃����o�[�ϐ����X�V����B
//				m_sz	�o�b�t�@�̃T�C�Y
//==============================================================
void	BigInt::resize(unsigned int _sz)
{
	if(Buff_Size < _sz){
		Buff_resize(_sz);
	}

	if(m_sz < _sz){
		//�g�傷��ꍇ
		memset(&m_pt[m_sz], Get_Sign() >> 7, _sz - m_sz);
	}

	m_sz = _sz;
}

//==============================================================
//			10�i�� ���l�\��
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//==============================================================
void	BigInt::Print_Dec()
{
	string	_str;
	Get_Ascii_Dec(&_str);
	cout << _str.c_str() << endl;
}

//==============================================================
//			16�i�� ���l�\��
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
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
//			�|�C���^���擾����B
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			unsigned char*	�|�C���^
//==============================================================
char*	BigInt::Get_Ptr()						//
{
	return(m_pt);
}

//==============================================================
//			�o�C�g���𓾂�B
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			unsigned	int	�o�C�g��
//==============================================================
unsigned	int	BigInt::Get_Byte_Length()
{
	resize();
	return(m_sz);
}

//==============================================================
//			�r�b�g���𓾂�B
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			unsigned	long	�r�b�g��
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
//			10�i���̕���������
//--------------------------------------------------------------
//	������
//			string* _str	�����������I�u�W�F�N�g�̃|�C���^
//	���Ԓl
//			����
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
//			�����i�ŏ��Byte�j�𓾂�
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			unsigned	int	�o�C�g��
//==============================================================
char	BigInt::Get_Sign()
{
	return(m_pt[m_sz - 1]);
}

//==============================================================
//			���(MOV)
//--------------------------------------------------------------
//	������
//			char*			_ptr	�������̂̃|�C���^
//			unsigned int	_sz		�����̃T�C�Y
//	���Ԓl
//			BigInt&					�I�u�W�F�N�g�̎Q��
//	������
//			�����{�������A�w��̐��l�ɐݒ肷��B
//			�ȉ��̃����o�[�ϐ����X�V����B
//				m_sz	�o�b�t�@�̃T�C�Y
//				m_pt	�o�b�t�@�̃|�C���^
//==============================================================
BigInt&	BigInt::Set(char* _ptr, unsigned int _sz)
{
	//�o�b�t�@�T�C�Y��������������A�V���Ɋm��
	if(Buff_Size < _sz){
		Buff_resize(_sz);
	}

	m_sz		= _sz;
	memcpy(m_pt, _ptr, m_sz);

	return(*this);
}

/****************************************************************/
/*			���Z�q�̃I�[�o�[���[�h								*/
/****************************************************************/
//==============================================================
//			���(MOV)
//--------------------------------------------------------------
//	������
//			�����{�������A�w��̃X�g�����O�X�ɐݒ肷��B
//			�ȉ��̃����o�[�ϐ����X�V����B
//				m_sz	�o�b�t�@�̃T�C�Y
//				m_pt	�o�b�t�@�̃|�C���^
//==============================================================
BigInt&	BigInt::operator=(BigInt	_Src)
{
	//�o�b�t�@�T�C�Y��������������A�V���Ɋm��
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

	//�o�b�t�@�T�C�Y��������������A�V���Ɋm��
	if(Buff_Size < m_sz){
		Buff_resize(m_sz);
	}

	_pt = (int *)m_pt;
	_pt[0] = _Src;

	return(*this);
}

//==============================================================
//			���Z(ADD)
//==============================================================
BigInt&	BigInt::operator+=(BigInt	_Src)
{
//	resize();
//	_Src.resize();

	if(m_sz <= _Src.m_sz){
		//�\�[�X�̃T�C�Y�Ɠ���or����ȉ��̏ꍇ
		//�\�[�X�̃T�C�Y���A1Byte�傫������B
		resize(_Src.m_sz + 1);
	} else {
		//����ȊO�̏ꍇ�́A���s���1Byte�傫������B
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

	//1Byte�傫������B
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
//			���Z(SUB)
//==============================================================
BigInt&	BigInt::operator-=(BigInt	_Src)
{
//	resize();
//	_Src.resize();

	if(m_sz <= _Src.m_sz){
		//�\�[�X�̃T�C�Y�Ɠ���or����ȉ��̏ꍇ
		//�\�[�X�̃T�C�Y���A1Byte�傫������B
		resize(_Src.m_sz + 1);
	} else {
		//����ȊO�̏ꍇ�́A���s���1Byte�傫������B
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

	//1Byte�傫������B
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
//			��Z(MUL)
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
//			���Z(DIV)
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
//			�]��(MOD)
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
//			�}�C�i�X(NEG)
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
//			�r�b�g���](NOT)
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
//			�_����(AND)
//==============================================================
BigInt&	BigInt::operator&=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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
//			�_���a(OR)
//==============================================================
BigInt&	BigInt::operator|=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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
//			�r���I�_���a(XOR)
//==============================================================
BigInt&	BigInt::operator^=(BigInt&	_Src)
{
//	resize();
//	_Src.resize();

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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

	//�T�C�Y�𓯂��ɂ��āA�_�����Z���s�B
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
//			�V�t�g(SHL)
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
//			�V�t�g(SHR)
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
	BigInt	_result = *this;		//�g���͂��Ȃ��̂ŁA���̂܂ܑ������B
	BigInt_Shr(&_result, _Cnt);
	_result.resize();
	return(_result);
}

//==============================================================
//			��r���Z�ieq & neq�j
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
		return(Chk_Zero());		//_Src��0��������A�������������Z���Ȃ��B
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
//			��r���Z�i�s�����n�j
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
//			�ݏ�iPower�j
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
/*			�O���[�o�����Z�q�̃I�[�o�[���[�h					*/
/****************************************************************/
//==============================================================
//			���Z(ADD)
//==============================================================
BigInt	operator+(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp += _Src;
	return(_temp);
}

//==============================================================
//			���Z(SUB)
//==============================================================
BigInt	operator-(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp -= _Src;
	return(_temp);
}

//==============================================================
//			��Z(MUL)
//==============================================================
BigInt	operator*(int _Dst, BigInt& _Src)
{
	BigInt	_result;
	BigInt_Mul(&_result, _Src, _Dst);
	return(_result);
}

//==============================================================
//			���Z(Div)
//==============================================================
BigInt	operator/(int _Dst, BigInt& _Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_result, &_temp, _Dst, _Src);
	return(_result);
}

//==============================================================
//			��](Mod)
//==============================================================
BigInt	operator%(int _Dst, BigInt& _Src)
{
	BigInt	_temp;
	BigInt	_result;
	BigInt_Div(&_temp,&_result, _Dst, _Src);
	return(_result);
}

//==============================================================
//			�_����(AND)
//==============================================================
BigInt	operator&(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp &= _Src;
	return(_temp);
}

//==============================================================
//			�_���a(OR)
//==============================================================
BigInt	operator|(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp |= _Src;
	return(_temp);
}

//==============================================================
//			�r���I�_���a(XOR)
//==============================================================
BigInt	operator^(int _Dst, BigInt& _Src)
{
	BigInt	_temp	= _Dst;
	_temp ^= _Src;
	return(_temp);
}

//==============================================================
//			��r���Z
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

