#include <iostream>
#include <bitset>

using namespace std;
int main() {
	typedef unsigned int TELEM;

	class TBitField
	{
	private:
		int  BitLen; // длина битового поля - макс. к-во битов
		TELEM* pMem; // память для представления битового поля
		int  MemLen; // к-во эл-тов Мем для представления бит.поля

		// методы реализации

		// индекс в pМем для бита n       (#О2)
		int   GetMemIndex(const int n) const {
			return n / (sizeof(TELEM) * 8);
			
		}
		
		// битовая маска для бита n       (#О3)
		TELEM GetMemMask(const int n) const {
			return TELEM(1) << (n % (sizeof(TELEM) * 8));
		}
	public:
		TBitField(int len) : BitLen(len) {
			if (len < 0) {
				throw std::invalid_argument("Bit length cannot be negative");
			}

			// Вычисляем необходимое количество элементов памяти
			MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
			pMem = new TELEM[MemLen];

			// Инициализируем память нулями
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
		~TBitField();                      //                                    (#С)

		// доступ к битам
		// 
		// 
		// получить длину (к-во битов)
		int GetLength(void) const {
			return BitLen;
		}    

		// установить бит 
		void SetBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}
			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] |= mask;

		}

		// очистить бит 
		void ClrBit(const int n) {
			if (n < 0 || n >= BitLen) {
				throw std::out_of_range("Bit index out of range");
			}


			TELEM mask = GetMemMask(n);
			pMem[GetMemIndex(n)] &= ~mask;
		}


		// получить значение бита
			int  GetBit(const int n)const {
				int memIndex = GetMemIndex(n);
				TELEM mask = GetMemMask(n);
				return (pMem[memIndex] & mask) != 0;


			}





		// надо учитывать, что TBitField может быть разной длины в int-ах
		// TBitField bf1(100) - 4 int, bf2(15) - 1 int
		 // Оператор присваивания
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
			// Операция "или" (#О6)
			TBitField operator|(const TBitField& bf) {
				// Выбираем большую длину
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// Копируем биты из текущего объекта
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = pMem[i];
				}

				// Применяем операцию OR с вторым объектом
				for (int i = 0; i < bf.MemLen; i++) {
					result.pMem[i] |= bf.pMem[i];
				}

				return result;
			}


			// Операция "и" (#Л2)
			TBitField operator&(const TBitField& bf) {
				// Выбираем большую длину
				int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
				TBitField result(max_len);

				// Применяем операцию AND для общих частей
				int min_mem_len = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
				for (int i = 0; i < min_mem_len; i++) {
					result.pMem[i] = pMem[i] & bf.pMem[i];
				}

				// Для оставшейся части (если поля разной длины)
				// Биты из longer поля AND 0 = 0, поэтому просто оставляем нули
				// которые уже есть в result после инициализации

				return result;
			}


			// Сравнение (#О5)
			int operator==(const TBitField& bf) const {
				if (BitLen != bf.BitLen) return 0;

				for (int i = 0; i < MemLen; i++) {
					if (pMem[i] != bf.pMem[i]) return 0;
				}
				return 1;
			}



			// Сравнение (не равно)
			int operator!=(const TBitField& bf) const {
				return !(*this == bf);
			}



			// Отрицание
			TBitField operator~(void) {
				TBitField result(BitLen);

				// Инвертируем все биты
				for (int i = 0; i < MemLen; i++) {
					result.pMem[i] = ~pMem[i];
				}

				// Обнуляем лишние биты в последнем элементе
				int extra_bits = BitLen % (sizeof(TELEM) * 8);
				if (extra_bits != 0) {
					TELEM mask = (TELEM(1) << extra_bits) - 1;
					result.pMem[MemLen - 1] &= mask;
				}

				return result;
			}

		friend istream& operator>>(istream& istr, TBitField& bf);       //      (#О7)
		friend ostream& operator<<(ostream& ostr, const TBitField& bf); //      (#П4)
	};

}