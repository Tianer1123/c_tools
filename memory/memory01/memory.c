#include "memory.h"

/* 初始化创建内存池 */
void *create_memory_pool(struct MemoryPool *mpool, u16 nUnitSize,
				u16 nInitSize, u16 nGrowSize)
{
	mpool->pBlock    = NULL;
	mpool->nInitSize = nInitSize;
	mpool->nGrowSize = nGrowSize;
	
	if (nUnitSize > 4) {
		/* 补齐到MEMPOOL_ALIGNMENT的倍数 */
		mpool->nUnitSize =
		(nUnitSize + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT - 1);
	}
	else if (nUnitSize <= 2) {
		/* 最少2个字节存内存单元编号 */
		mpool->nUnitSize = 2;
	}
	else {
		mpool->nUnitSize = 4;
	}
		
}
