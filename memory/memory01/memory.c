#include <stdlib.h>
#include "memory.h"

/* 初始化创建内存池 */
void create_memory_pool(struct MemoryPool *mpool, u16 nUnitSize,
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

struct MemoryBlock *_mem_malloc(struct MemoryPool *mpool)
{
	/* 从堆中申请空间 */
	struct MemoryBlock *pBlock =
	(struct MemoryBlock *)malloc(sizeof(struct MemoryBlock) + mpool->nGrowSize * mpool->nUnitSize);
	if (!pBlock) {
		return NULL;
	}

	/* 初始化pBlock */
	pBlock->nSize = mpool->nUnitSize * mpool->nGrowSize;
	pBlock->nFree = mpool->nGrowSize - 1; /* 要立即返回1个内存单元 */
	pBlock->nFirst = 1;
	pBlock->pNext = NULL;

	/* 初始化每个内存单元的编号 */
	char *pData = pBlock->aData;
	u16 i = 0;
	for (; i < mpool->nGrowSize; i++) {
		*((u16 *)pData) = i;
		pData += mpool->nUnitSize;
	}

	return pBlock;
}

/* 申请内存 */
void *mem_malloc(struct MemoryPool *mpool)
{
	struct MemoryBlock *pBlock = mpool->pBlock;

 	/* 无可用内存单元,查看下一个内存块 */
	while (pBlock && !pBlock->nFree) {
		pBlock = pBlock->pNext;
	}

	/* 有可用内存单元 */
	if (pBlock) {
		/* 计算出可用内存单元的首地址 */
		char *pFree = pBlock->aData + (pBlock->nFirst * mpool->nUnitSize);
		/* 取出下一个可用内存单元编号 */
		pBlock->nFirst = *((u16 *)pFree);

		/* 可用单元个数减一 */
		pBlock->nFree--;
		/* 返回申请的内存的首内置 */
		return (void *)pFree;
	}
	/* 无可用内存单元 */
	else {
		/* 不允许扩展? */
		if (!mpool->nGrowSize) {
			return NULL;
		}
		
		/* 从堆中,给内存块申请空间 */
		pBlock = _mem_malloc(mpool);
		if (!pBlock) {
			return NULL;
		}

		/* 将申请好的内存块放到链表头 */
		pBlock->pNext = mpool->pBlock;
		mpool->pBlock = pBlock;

		return (void *)(pBlock->aData);
	}
}
