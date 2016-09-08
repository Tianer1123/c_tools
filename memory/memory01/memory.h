#ifndef __MEMORY_H__
#define __MEMORY_H__

typedef unsigned short u16;
typedef unsigned long  UL;

#define MEMPOOL_ALIGNMENT 4

struct MemoryBlock
{
	u16                 nSize;          //所有内存单元的大小
	u16                 nFree;          //内存块还有多少自由的内存单元
	u16                 nFirst;         //记录下一个可供分配的单元的编号
	struct MemoryBlock *pPrev;          //前一内存块
	struct MemoryBlock *pNext;          //内存块链表
	char                aData[1];       //是内存单元的开始
};

struct MemoryPool
{
	struct MemoryBlock *pBlock;          //内存块
	u16                 nUnitSize;       //内存单元大小
	u16                 nInitSize;       //初始化大小
	u16                 nGrowSize;       //新申请内存单元增长大小
};

/* 创建内存池 */
void create_memory_pool(struct MemoryPool *mpool, u16 nUnitSize, 
				u16 nInitSize, u16 nGrowSize);

void *mem_malloc(struct MemoryPool *mpool);

void mem_free(struct MemoryPool *mpool, void *pFree);

#endif //__MEMORY_H__
