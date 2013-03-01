#pragma once

/****************************************************************/
/*			�萔��`											*/
/****************************************************************/
//0�ŏ��Z���ɁA�O���Z���荞�݁i��O�j�𔭐�������ꍇ�A��`���ĉ������B
//#define	Zero_Div_Err			

/****************************************************************/
/*			�v���g�^�C�v�錾									*/
/****************************************************************/
class	BigInt;

//�A�Z���u������ŏ����ꂽ�֐�
extern "C"{
	void			__fastcall	BigInt_Copy(BigInt* _dst,	BigInt* _src);	//�����g���t���R�s�[
	void			__fastcall	BigInt_Add(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Sub(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Not(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_And(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Or(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Xor(	BigInt* _dst,	BigInt* _src);
	void			__fastcall	BigInt_Shl(	BigInt* _src,	unsigned int _Cnt);
	unsigned	int	__fastcall	BigInt_Shr(	BigInt* _src,	unsigned int _Cnt);

	void			__fastcall	BigInt_Shl1(BigInt* _src);	//����1bit�V�t�g
	unsigned	int	__fastcall	BigInt_Shr1(BigInt* _src);	//����1bit�V�t�g
}


/****************************************************************/
/*			�N���X��`											*/
/****************************************************************/
class BigInt
{
private:
	//--------------
	//�����o�[�ϐ�
	unsigned	int		m_sz;			//�v�Z�p�̃T�C�Y
				char*	m_pt;			//�o�b�t�@�̃|�C���^
	unsigned	int		Buff_Size;		//���ۂɊm�ۂ����������T�C�Y

public:
	//--------------
	//�֐�
	BigInt(void);									// 0 �ŏ�����
	BigInt(BigInt&	_Src);							//�w�肳�ꂽ���{�������ŏ�����
	BigInt(int		_Src);							//�w�肳�ꂽ���l�ŏ�����
	BigInt(char* _ptr, unsigned int _sz);			//�C�Ӓ��̐��l�ŏ�����
	~BigInt(void);									//

	BigInt&			Set(char* _ptr, unsigned int _sz);	//���

private:
	void			Buff_resize();					//�������E�o�b�t�@���ŏ����Ɋm�ۂ��Ȃ����B
	void			Buff_resize(unsigned int _sz);	//�������E�o�b�t�@���w��l�Ɋm�ۂ��Ȃ����B

public:
	void			resize();						//�v�Z�T�C�Y���ŏ����ɐݒ肷��B
	void			resize(unsigned int _sz);		//�v�Z�T�C�Y���w��l�ɐݒ肷��B

	void			Print_Dec();					//10�i���\����W���o�͂ɏo��
	void			Print_Hex();					//16�i���\����W���o�͂ɏo��

	char*			Get_Ptr();						//�����̃|�C���^���擾����B
	unsigned int	Get_Byte_Length();				//�����̃o�C�g�����擾����
	unsigned long	Get_Bit_Length();				//�����̃r�b�g�����擾����
	char			Get_Sign();						//�����̕������擾����
	void			Get_Ascii_Dec(string* _asc);	//10�i��������擾����

//--------------
//operator ���Z�q
public:
	//���
	BigInt&	operator=(BigInt	_Src);	//���
	BigInt&	operator=(int		_Src);

	//�l�����Z
	BigInt&	operator+=(BigInt	_Src);	//���Z�i�P���j
	BigInt&	operator+=(int		_Src);
	BigInt&	operator++(				);
	BigInt	operator++(int			);	//�i�����̂�Ԃ��j
	BigInt	operator+(BigInt&	_Src);	//���Z�i�Q���j
	BigInt	operator+(int		_Src);

	BigInt&	operator-=(BigInt	_Src);	//���Z�i�P���j
	BigInt&	operator-=(int		_Src);
	BigInt&	operator--(				);
	BigInt	operator--(int			);	//�i�����̂�Ԃ��j
	BigInt	operator-(BigInt&	_Src);	//���Z�i�Q���j
	BigInt	operator-(int		_Src);

	BigInt&	operator*=(BigInt&	_Src);	//��Z
	BigInt&	operator*=(int		_Src);
	BigInt	operator*(BigInt&	_Src);	//
	BigInt	operator*(int		_Src);

	BigInt&	operator/=(BigInt&	_Src);	//���Z
	BigInt&	operator/=(int		_Src);
	BigInt	operator/(BigInt&	_Src);	//
	BigInt	operator/(int		_Src);

	BigInt&	operator%=(BigInt&	_Src);	//Mod
	BigInt&	operator%=(int		_Src);
	BigInt	operator%(BigInt&	_Src);	//
	BigInt	operator%(int		_Src);

	//�␔
	BigInt	operator-(void);			//NEG
	BigInt	operator~(void);			//NOT

	//�_�����Z
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

	//�V�t�g
//	BigInt&	operator<<=(BigInt&	_Cnt);
//	BigInt	operator<<(BigInt&	_Cnt);
//	BigInt&	operator>>=(BigInt&	_Cnt);
//	BigInt	operator>>(BigInt&	_Cnt);
	BigInt&	operator<<=(unsigned int	_Cnt);
	BigInt	operator<<( unsigned int	_Cnt);
	BigInt&	operator>>=(unsigned int	_Cnt);
	BigInt	operator>>( unsigned int	_Cnt);

	//��r
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
	BigInt	Power(BigInt&	_Src);	//�ݏ�
	BigInt	Power(int		_Src);

	//�O���[�o���ȉ��Z�q
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
