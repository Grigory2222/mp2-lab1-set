#include <iostream>
#include <bitset>

using namespace std;
int main() {
	typedef unsigned int TELEM;

	class TBitField
	{
	private:
		int  BitLen; // ����� �������� ���� - ����. �-�� �����
		TELEM* pMem; // ������ ��� ������������� �������� ����
		int  MemLen; // �-�� ��-��� ��� ��� ������������� ���.����

		// ������ ����������

		// ������ � p��� ��� ���� n       (#�2)
		int   GetMemIndex(const int n) const {
			return n / (sizeof(TELEM) * 8);
			
		}
		
		// ������� ����� ��� ���� n       (#�3)
		TELEM GetMemMask(const int n) const {
			return TELEM(1) << (n % (sizeof(TELEM) * 8));
		}
	public:
		TBitField(int len) : BitLen(len) {
			if (len < 0) {
				throw std::invalid_argument("Bit length cannot be negative");
			}

			// ��������� ����������� ���������� ��������� ������
			MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
			pMem = new TELEM[MemLen];

			// �������������� ������ ������
			for (int i = 0; i < MemLen; i++) {
				pMem[i] = TELEM(0);
			}
		}                
		TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) {
			pMem = new TELEM[MemLen];
			for (int i = 0; i < MemLen; i++) {
				pMem[i] = bf.pMem[i];
			}
		}
		~TBitField();                      //                                    (#�)

		// ������ � �����
		// 
		// 
		// �������� ����� (�-�� �����)
		int GetLength(void) const {
			return BitLen;
		}    

		// ���������� ��� 
		void SetBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}
			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] |= mask;

		}

		// �������� ��� 
		void ClrBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}


			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] &= ~mask;
		}


		// �������� �������� ����
			int  GetBit(const int n)const {
				int memIndex = GetMemIndex(n);
				TELEM mask = GetMemMask(n);
				return (pMem[memIndex] & mask) != 0;


			}





		// ���� ���������, ��� TBitField ����� ���� ������ ����� � int-��
		// TBitField bf1(100) - 4 int, bf2(15) - 1 int
		 // �������� ������������
			TBitField& operator=(const TBitField& bf) {
				if (this != &bf) {
					delete[] pMem;

					BitLen = bf.BitLen;
					MemLen = bf.MemLen;
					pMem = new TELEM[MemLen];

					for (int i = 0; i < MemLen; i++) {
						pMem[i] = bf.pMem[i];
					}
				}
				return *this;
			}            
			// �������� "���" (#�6)
			TBitField operator|(const TBitField& bf) {
				// �������� ������� �����
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// �������� ���� �� �������� �������
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = pMem[i];
				}

				// ��������� �������� OR � ������ ��������
				for (int i = 0; i < bf.MemLen; i++) {
					result.pMem[i] |= bf.pMem[i];
				}

				return result;
			}


			// �������� "�" (#�2)
			TBitField operator&(const TBitField& bf) {
				// �������� ������� �����
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// ��������� �������� AND ��� ����� ������
				int min_mem_len = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
				for (int i = 0; i < min_mem_len; i++) {
					result.pMem[i] = pMem[i] & bf.pMem[i];
				}

				// ��� ���������� ����� (���� ���� ������ �����)
				// ���� �� longer ���� AND 0 = 0, ������� ������ ��������� ����
				// ������� ��� ���� � result ����� �������������

				return result;
			}


			// ��������� (#�5)
			int operator==(const TBitField& bf) const {
				if (BitLen != bf.BitLen) return 0;

				for (int i = 0; i < MemLen; i++) {
					if (pMem[i] != bf.pMem[i]) return 0;
				}
				return 1;
			}



			// ��������� (�� �����)
			int operator!=(const TBitField& bf) const {
				return !(*this == bf);
			}



			// ���������
			TBitField operator~(void) {
				TBitField result(BitLen);

				// ����������� ��� ����
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = ~pMem[i];
				}

				// �������� ������ ���� � ��������� ��������
				int extra_bits = BitLen % (sizeof(TELEM) * 8);
				if (extra_bits != 0) {
					TELEM mask = (TELEM(1) << extra_bits) - 1;
					result.pMem[MemLen - 1] &= mask;
				}

				return result;
			}

		friend istream& operator>>(istream& istr, TBitField& bf);       //      (#�7)
		friend ostream& operator<<(ostream& ostr, const TBitField& bf); //      (#�4)
	};

}