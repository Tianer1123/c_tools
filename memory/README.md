# 内存池

## 定义
应用程序可以通过系统的内存分配调用预先一次性申请适当大小的内存作为一个内存池，之后应用程序自己对内存的分配和释放则可以通过这个内存池来完成。只有当内存池大小需要动态扩展时，才需要再调用系统的内存分配函数，其他时间对内存的一切操作都在应用程序的掌控之中。

## 分类
1. 按线程安全
    内存池可以分为单线程内存池和多线程内存池。
2. 按可分配的内存大小
    可以分为固定内存池和可变内存池。  

## 原理
内存池，内存块，内存单元。

在内存池初次生成时，只向系统申请了一个内存块，返回的指针作为整个内存池的头指针。

内存池判断需要动态扩大时，才再次向系统申请新的内存块，并把所有这些内存块通过指针链接起来。

## 实例
这是一个应用于单线程环境且分配单元大小固定的内存池，一般用来为执行时会动态频繁地创建且可能会被多次创建的类对象或者结构体分配内存。

### MemoryPool:
```
class MemoryPool
{
	private:
		MemoryBlock*   pBlock;
		USHORT          nUnitSize;
		USHORT          nInitSize;
		USHORT          nGrowSize;

	public:
		MemoryPool( USHORT nUnitSize,
				USHORT nInitSize = 1024,
				USHORT nGrowSize = 256 );
		~MemoryPool();

		void*           Alloc();
		void            Free( void* p );
};
```

### MemoryBlock:
```
struct MemoryBlock
{
	USHORT          nSize;
	USHORT          nFree;
	USHORT          nFirst;
	USHORT          nDummyAlign1;
	MemoryBlock*  pNext;
	char            aData[1];

	static void* operator new(size_t, USHORT nTypes, USHORT nUnitSize)
	{
		return ::operator new(sizeof(MemoryBlock) + nTypes * nUnitSize);
	}
	static void  operator delete(void *p, size_t)
	{
		::operator delete (p);
	}

	MemoryBlock (USHORT nTypes = 1, USHORT nUnitSize = 0);
	~MemoryBlock() {}
};
```

### 内部数据结构：


![memp1](https://github.com/Tianer1123/c_tools/blob/master/memory/img/mempool1.png)


## 机制

### 内存块

`MemoryPool`内存池是从堆中申请的较大连续内存区域.包含:
* `MemoryBlock`内存块:组成链表,`MemoryPool`的`pBlock`是内存块的链表头.
* 内存单元

### 内存单元
* 大小固定(`MemoryPool->nUnitSize`).
* 有两个重要成员:`nFree`和`nFirst`.
	1. `nFree`: 内存块还有多少自由分配单元.
	2. `nFirst`: 记录下一个可供分配的单元的编号(头两个字节记录下一个内存单元的编号，由此链在一起).
* 内存单元编号：每一个内存连续的编号(比如3块内存单元,编号分别为0,1,2)

### 内存请求,有可用内存块

1. 遍历`MemoryPool`中的`MemoryBlock`链表.
2. 找到有可用内存单元的`MemoryBlock`.(`MemoryBlock->nFree > 0`)
3. 获取`MemoryBlock->nFirst`值,定位到申请内存的其实地址.
4. 返回这个地址前,将这个位置开始两个字节赋值给`nFirst`.
5. 同时`nFree - 1`.
6. 返回这个地址.

### 无可用内存单元

1. `MemoryPool`从堆中申请一个内存块.
2. 初始化内存块.
	1. `nSize`为所有内存单元的大小.
	2. `nFree`为`n-1`.(假设内存单元个数为n)因为马上要分配一个.
	3. `nFirst`为1.
	4. 将编号为0的分配单元之后的内存单元链接起来.(根据编号和单元大小乘积定位)
3. `aData[1]`是内存单元的开始.

### delete

1. `delete`不反回堆，返回`MemoryPool`.
2. 返回时，`MemoryPool`需要知道该单元的起始地址.
3. 遍历内存块,检测该地址是否在内存块地址内.
	1. 如果不在，不属于该`MemoryPool`.
	2. 如果在,1)将该内存单元所在的内存块移到链表头.2)`nFree+1`.
4. 继续判断：
	1. 如果所在内存块都是自由的,内存块返回给堆.
	2. 如果有非自由的,移动该内存块到链表头部.(减少遍历`MemoryPool`遍历次数)

## 代码
目录[memory01/](https://github.com/Tianer1123/c_tools/blob/master/memory/memory01/)中为c语言版本的源码.

## 参考文档
[C++ 应用程序性能优化，第 6 章：内存池](http://www.ibm.com/developerworks/cn/linux/l-cn-ppp/index6.html#icomments)
