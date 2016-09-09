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
	int size = sizeof(struct MemoryBlock) + mpool->nGrowSize * mpool->nUnitSize;
	struct MemoryBlock *pBlock = (struct MemoryBlock *)malloc(size);
	if (!pBlock) {
		return NULL;
	}

	/* 初始化pBlock */
	pBlock->nSize = mpool->nUnitSize * mpool->nGrowSize;
	pBlock->nFree = mpool->nGrowSize - 1; /* 要立即返回1个内存单元 */
	pBlock->nFirst = 1;
	pBlock->pPrev = NULL;
	pBlock->pNext = NULL;

	/* 初始化每个内存单元的编号，初始化时有顺序，实际情况可能乱序,靠编号链接 */
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
		if (!mpool->pBlock) {
			mpool->pBlock = pBlock;
			pBlock->pNext = NULL;
			pBlock->pPrev = NULL;
		}
		else {
			pBlock->pNext = mpool->pBlock;
			pBlock->pNext->pPrev = pBlock;
			mpool->pBlock = pBlock;
		}

		return (void *)(pBlock->aData);
	}
}

/* 释放内存 */
void mem_free(struct MemoryPool *mpool, void *pFree)
{
	/* 当前内存块 */
	struct MemoryBlock *pBlock = mpool->pBlock;

	/* 比较每一个内存块,判断pFree是否在其中 */
	while(pBlock) {
		if (((UL)pFree < (UL)pBlock->aData) ||
		((UL)pFree >= ((UL)pBlock->aData + pBlock->nSize))) {
			pBlock = pBlock->pNext;
		}
		else {
			break;
		}
	}

	/* 如果pFree不属于内存池，直接free返回给堆 */
	if (!pBlock) {
		free(pFree);
		pFree = NULL;
		return;
	}

	/* 如果属于内存池 */
	pBlock->nFree++;
	/* 
	 * 将内存块中的内存单元串起来.
	 * 原来的第一个自由内存单元作为刚释放的内存单元的下一个自由内存单元.
	 * 刚释放的内存单元作为第一个自由的内存单元.
	 * 只是编号变，内存地址不变.
	 * */
	*((u16 *)pFree) = pBlock->nFirst;
	pBlock->nFirst = (u16)(((UL)pFree - (UL)pBlock->aData) / mpool->nUnitSize);

	/* 如果该内存块全部是自由的内存单元,返回内存块给堆 */
	if (pBlock->nFree * mpool->nUnitSize == pBlock->nSize) {
		/* 如果是第一个内存块 */
		if (pBlock == mpool->pBlock) {
			mpool->pBlock = pBlock->pNext;
			if (pBlock->pNext)
				mpool->pBlock->pPrev = NULL;
		}
		else {
			pBlock->pPrev->pNext = pBlock->pNext;
			if (pBlock->pNext)
				pBlock->pNext->pPrev = pBlock->pPrev;
		}
		free(pBlock);
		pBlock = NULL;
	}
	/* 否则移动到链表头 */
	else {
		/* 如果是第一个内存块 */
		if (pBlock == mpool->pBlock) {
			return;
		}

		/* 从链表中移出 */
		pBlock->pPrev->pNext = pBlock->pNext;
		if (pBlock->pNext)
			pBlock->pNext->pPrev = pBlock->pPrev;
			
		/* 插到链表头 */
		pBlock->pNext = mpool->pBlock;
		pBlock->pPrev = NULL;
		mpool->pBlock = pBlock;
	}
}

/* 销毁内存池 */
void destory_memory_pool(struct MemoryPool *mpool)
{
	struct MemoryBlock *pBlock = mpool->pBlock;
	struct MemoryBlock *ptmp;
	while (pBlock) {
		ptmp = pBlock;
		pBlock = pBlock->pNext;

		free(ptmp);
		ptmp = NULL;
	}
}
