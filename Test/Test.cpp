// BigInteger.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

//==============================================================
//			16進数 数値表示
//--------------------------------------------------------------
//	●引数
//			int		n		表示Byte数
//			void	*Data	表示する配列[Byte単位]
//	●返値
//			無し
//==============================================================
void	dataPrint(int n, void *Data)
{
	unsigned	char*	cData	= (unsigned char*)Data;
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


int	__cdecl	_tmain(int argc, _TCHAR* argv[])
{
	static	BigInt	N[] = {	-100000000,
							-10000000,
							-1000000,
							-100000,
							-10000,
							-1000,
							-100,
							-10,
							-1,
							0,
							1,
							10,
							100,
							1000,
							10000,
							100000,
							1000000,
							10000000,
							100000000};		//0～18

	BigInt	n1,n2,n3,n4;

	unsigned	int	i, x,y;
				int	n;

	string	s1, s2, s3, s4;

	//シフト・テスト
	cout	<< "=== Bit Right Shift Test ===" << endl;

	n1 = 0xFEDCBA98;
	n1 <<= 32;
	n1 |= 0x76543210;
	n1.Print_Hex();

	for(i=0;i<128;i+=4){
		n2 = n1;
		x = BigInt_Shr(&n2, i);
		printf("%08x : ", x);
		n2.Print_Hex();
	}

	n1 = 1;
	n1 <<= 128;
	n1.Print_Hex();
	i = 0;

	while(i < 180){
		n2 = n1;
		x = BigInt_Shr(&n2, i);
		printf("%08x : ", x);
		n2.Print_Hex();
		i++;
	}

	//符号拡張テスト
	n1 = -1;
	i=0;
	while(i < 64){
		n2 = n1;
		x = BigInt_Shr(&n2, i);
		printf("%08x : ", x);
		n2.Print_Hex();
		i++;
	}

	//足し算テスト
	cout	<< "=== Add Test ===" << endl;
	x = 19;
	while(x > 0){
		x--;
		y = 19;
		while(y > 0){
			y--;
			n1 = N[x] + N[y];
			N[x].Get_Ascii_Dec(&s1);
			N[y].Get_Ascii_Dec(&s2);
			n1.Get_Ascii_Dec(&s3);
			cout << s1 << "+ " << s2 << "= " << s3 << endl;
		}
	}

	//引き算テスト
	cout	<< "=== Sub Test ===" << endl;
	x = 19;
	while(x > 0){
		x--;
		y = 19;
		while(y > 0){
			y--;
			n1 = N[x] - N[y];
			N[x].Get_Ascii_Dec(&s1);
			N[y].Get_Ascii_Dec(&s2);
			n1.Get_Ascii_Dec(&s3);
			cout << s1 << "- " << s2 << "= " << s3 << endl;
		}
	}

	//掛け算テスト（BigInt = int * int）
	cout	<< "=== Mul Test ===" << endl;
	x = 19;
	while(x > 0){
		x--;
		y = 19;
		while(y > 0){
			y--;
			n1 = N[x] * N[y];
			N[x].Get_Ascii_Dec(&s1);
			N[y].Get_Ascii_Dec(&s2);
			n1.Get_Ascii_Dec(&s3);
			cout << s1 << "* " << s2 << "= " << s3 << endl;
		}
	}

	//掛け算テスト（BigInt = int * int）
	BigInt_Mul(&n1, 10000, 10000);
	n1.Print_Dec();
	BigInt_Mul(&n1, 10000, -10000);
	n1.Print_Dec();
	BigInt_Mul(&n1, -10000, 10000);
	n1.Print_Dec();
	BigInt_Mul(&n1, -10000, -10000);
	n1.Print_Dec();

	//割り算テスト
	cout	<< "=== Div Test ===" << endl;
	x = 19;
	while(x > 0){
		x--;
		y = 19;
		while(y > 0){
			bool	f;
			y--;
			f = BigInt_Div(&n1, &n2, N[x], N[y]);
	//		f = true;
	//		n1 = N[x] / N[y];
	//		n2 = N[x] % N[y];
			N[x].Get_Ascii_Dec(&s1);
			N[y].Get_Ascii_Dec(&s2);
			n1.Get_Ascii_Dec(&s3);
			n2.Get_Ascii_Dec(&s4);
			if(f == true){
				cout << s1 << "/ " << s2 << "= " << s3 << "% " << s4 << endl;
			} else {
				cout << s1 << "/ " << s2 << ": ０で除算しました。 " << endl;
			}
		}
	}

	//累乗テスト（BigInt）
	//掛け算テスト（BigInt = int * int）
	cout	<< "=== Power Test ===" << endl;
	x = 14;
	while(x > 7){
		x--;
		y = 13;
		while(y > 7){
			y--;
			n1 = N[x].Power(N[y]);
			N[x].Get_Ascii_Dec(&s1);
			N[y].Get_Ascii_Dec(&s2);
			n1.Get_Ascii_Dec(&s3);
			cout << "Power(" << s1 << ", " << s2 << ")= " << s3 << endl;
		}
	}


	n1 = -16;
	n2 = 100;
	while(n1 <= 16){
		n3 = n2.Power(n1);
		n1.Get_Ascii_Dec(&s1);
		n2.Get_Ascii_Dec(&s2);
		n3.Get_Ascii_Dec(&s3);
		cout << "Power(" << s2 << "," << s1 << ")= " << s3 << endl;
		n1++;		
	}

	//累乗テスト（Int）
	n = -16;
	n2 = 100;
	while(n <= 16){
		n3 = n2.Power(n);
		n2.Get_Ascii_Dec(&s2);
		n3.Get_Ascii_Dec(&s3);
		cout << "Power(" << s2 << "," << n << ")= " << s3 << endl;
		n++;		
	}

	//余剰計算
	cout	<< "=== Montgomery Test ===" << endl;
	montgomery*	mMul	=	new	montgomery();

	n4	=	123456789;
	n4.Get_Ascii_Dec(&s4);
	mMul->Set_Modulo(&n4);

	x = 19;
	while(x > 9){
		x--;
		y = 19;
		while(y > 9){
			y--;
			mMul->Power(&n1, N[x], N[y]);
			n1.Get_Ascii_Dec(&s1);
			N[x].Get_Ascii_Dec(&s2);
			N[y].Get_Ascii_Dec(&s3);
			cout	<<	"Power(" << s2 << "," << s3 << ") mod " << s4 << " ≡ " << s1 << endl;
		}
	}

	//条件分岐テスト
	n1=0x40;
	while(0 != n1){
		if(0x20 == n1){
			n1.Print_Dec();
			printf("n1 == 0x20 \n");
		}else if(0x30 <= n1){
			n1.Print_Dec();
			printf("n1 >= 0x30 \n");
		}else if(0x28 < n1){
			n1.Print_Dec();
			printf("n1 > 0x28 \n");
		}else if(0x08 >= n1){
			n1.Print_Dec();
			printf("n1 <= 0x08 \n");
		}else if(0x10 > n1){
			n1.Print_Dec();
			printf("n1 < 0x10 \n");
		}
		n1--;
	}

	n1 = 1;
	n1 <<= 256;
	n1.Print_Dec();

	n2 = 1;
	n2 <<= 128;
	n2.Print_Dec();

	n1 /= n2;

	n1.Print_Dec();
	n1.Print_Hex();

	printf("%d \n", n1.Get_Bit_Length());

	char c[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	n2 = n1.Set(c, sizeof(c));

	BigInt	n_ = BigInt(c, sizeof(c));

	n1.Print_Hex();
	n1.Print_Dec();
	n2.Print_Dec();
	n_.Print_Dec();


	printf("Byte Length = %d : Bit Length = %d \n", n1.Get_Byte_Length(), n1.Get_Bit_Length() );

	n1 *= n1;
	n1.Print_Hex();
	n1.Print_Dec();
	printf("Byte Length = %d : Bit Length = %d \n", n1.Get_Byte_Length(), n1.Get_Bit_Length() );

	return 0;
}

