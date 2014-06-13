#ifndef SYB_IPQUEUE_HPP
#define SYB_IPQUEUE_HPP

#include <vector>


namespace syb
{
	template<typename T>
	class PQ
	{
	public:
		PQ(std::vector<T> &keys, int max_size) :
			m_Keys(keys),
			m_MaxSize(max_size),
			m_Size(0)
		{
			m_Heap.assign(max_size + 1, 0);
			m_InvHeap.assign(max_size + 1, 0);
		}

		bool Empty() const 
		{ 
			return (m_Size == 0); 
		}

		void Insert(const int index)
		{
			++m_Size;
			m_Heap[m_Size] = index;
			m_InvHeap[index] = m_Size;
			ReorderUpwards(m_Size);
		}

		int Pop()
		{
			Swap(1, m_Size);
			ReorderDownwards(1, m_Size - 1);
			return m_Heap[m_Size--];
		}

		void ChangePriority(const int index)
		{
			ReorderUpwards(m_InvHeap[index]);
		}
	private:
		std::vector<T> &m_Keys;
		std::vector<int> m_Heap;
		std::vector<int> m_invHeap;
		int m_Size;
		int m_MaxSize;

		void Swap(int a, int b)
		{
			int temp = m_Heap[a]; 
			m_Heap[a] = m_Heap[b]; 
			m_Heap[b] = temp;

			m_invHeap[m_Heap[a]] = a; 
			m_invHeap[m_Heap[b]] = b;
		}

		void ReorderUpwards(int nd)
		{
			while ((nd>1) && (m_vecKeys[m_Heap[nd / 2]] > m_vecKeys[m_Heap[nd]]))
			{
				Swap(nd / 2, nd);
				nd /= 2;
			}
		}

		void ReorderDownwards(int nd, int HeapSize)
		{
			while (2 * nd <= HeapSize)
			{
				int child = 2 * nd;

				if ((child < HeapSize) && (m_vecKeys[m_Heap[child]] > m_vecKeys[m_Heap[child + 1]]))
					++child;

				if (m_vecKeys[m_Heap[nd]] > m_vecKeys[m_Heap[child]])
				{
					Swap(child, nd);
					nd = child;
				}
				else
				{
					break;
				}
			}
		}
	};
} // namespace syb

#endif // SYB_IPQUEUE_HPP