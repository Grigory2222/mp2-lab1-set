#include <iostream>
#include <bitset>

using namespace std;
int main() {
	typedef unsigned int TELEM;

	class TBitField
	{
	private:
		int  BitLen; // äëčíŕ áčňîâîăî ďîë˙ - ěŕęń. ę-âî áčňîâ
		TELEM* pMem; // ďŕě˙ňü äë˙ ďđĺäńňŕâëĺíč˙ áčňîâîăî ďîë˙
		int  MemLen; // ę-âî ýë-ňîâ Ěĺě äë˙ ďđĺäńňŕâëĺíč˙ áčň.ďîë˙

		// ěĺňîäű đĺŕëčçŕöčč

		// číäĺęń â pĚĺě äë˙ áčňŕ n       (#Î2)
		int   GetMemIndex(const int n) const {
			return n / (sizeof(TELEM) * 8);
			
		}
		
		// áčňîâŕ˙ ěŕńęŕ äë˙ áčňŕ n       (#Î3)
		TELEM GetMemMask(const int n) const {
			return TELEM(1) << (n % (sizeof(TELEM) * 8));
		}
	public:
		TBitField(int len) : BitLen(len) {
			if (len < 0) {
				throw std::invalid_argument("Bit length cannot be negative");
			}

			// Âű÷čńë˙ĺě íĺîáőîäčěîĺ ęîëč÷ĺńňâî ýëĺěĺíňîâ ďŕě˙ňč
			MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
			pMem = new TELEM[MemLen];

			// Číčöčŕëčçčđóĺě ďŕě˙ňü íóë˙ěč
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
		~TBitField();                      //                                    (#Ń)

		// äîńňóď ę áčňŕě
		// 
		// 
		// ďîëó÷čňü äëčíó (ę-âî áčňîâ)
		int GetLength(void) const {
			return BitLen;
		}    

		// óńňŕíîâčňü áčň 
		void SetBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}
			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] |= mask;

		}

		// î÷čńňčňü áčň 
		void ClrBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}


			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] &= ~mask;
		}


		// ďîëó÷čňü çíŕ÷ĺíčĺ áčňŕ
			int  GetBit(const int n)const {
				int memIndex = GetMemIndex(n);
				TELEM mask = GetMemMask(n);
				return (pMem[memIndex] & mask) != 0;


			}





		// íŕäî ó÷čňűâŕňü, ÷ňî TBitField ěîćĺň áűňü đŕçíîé äëčíű â int-ŕő
		// TBitField bf1(100) - 4 int, bf2(15) - 1 int
		 // Îďĺđŕňîđ ďđčńâŕčâŕíč˙
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
			// Îďĺđŕöč˙ "čëč" (#Î6)
			TBitField operator|(const TBitField& bf) {
				// Âűáčđŕĺě áîëüřóţ äëčíó
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// Ęîďčđóĺě áčňű čç ňĺęóůĺăî îáúĺęňŕ
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = pMem[i];
				}

				// Ďđčěĺí˙ĺě îďĺđŕöčţ OR ń âňîđűě îáúĺęňîě
				for (int i = 0; i < bf.MemLen; i++) {
					result.pMem[i] |= bf.pMem[i];
				}

				return result;
			}


			// Îďĺđŕöč˙ "č" (#Ë2)
			TBitField operator&(const TBitField& bf) {
				// Âűáčđŕĺě áîëüřóţ äëčíó
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// Ďđčěĺí˙ĺě îďĺđŕöčţ AND äë˙ îáůčő ÷ŕńňĺé
				int min_mem_len = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
				for (int i = 0; i < min_mem_len; i++) {
					result.pMem[i] = pMem[i] & bf.pMem[i];
				}

				// Äë˙ îńňŕâřĺéń˙ ÷ŕńňč (ĺńëč ďîë˙ đŕçíîé äëčíű)
				// Áčňű čç longer ďîë˙ AND 0 = 0, ďîýňîěó ďđîńňî îńňŕâë˙ĺě íóëč
				// ęîňîđűĺ óćĺ ĺńňü â result ďîńëĺ číčöčŕëčçŕöčč

				return result;
			}


			// Ńđŕâíĺíčĺ (#Î5)
			int operator==(const TBitField& bf) const {
				if (BitLen != bf.BitLen) return 0;

				for (int i = 0; i < MemLen; i++) {
					if (pMem[i] != bf.pMem[i]) return 0;
				}
				return 1;
			}



			// Ńđŕâíĺíčĺ (íĺ đŕâíî)
			int operator!=(const TBitField& bf) const {
				return !(*this == bf);
			}



			// Îňđčöŕíčĺ
			TBitField operator~(void) {
				TBitField result(BitLen);

				// Číâĺđňčđóĺě âńĺ áčňű
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = ~pMem[i];
				}

				// Îáíóë˙ĺě ëčříčĺ áčňű â ďîńëĺäíĺě ýëĺěĺíňĺ
				int extra_bits = BitLen % (sizeof(TELEM) * 8);
				if (extra_bits != 0) {
					TELEM mask = (TELEM(1) << extra_bits) - 1;
					result.pMem[MemLen - 1] &= mask;
				}

				return result;
			}

		friend istream& operator>>(istream& istr, TBitField& bf);       //      (#Î7)
		friend ostream& operator<<(ostream& ostr, const TBitField& bf); //      (#Ď4)
	};

}
