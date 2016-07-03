/*
 * =====================================================================================
 *
 *       Filename:  sort_array.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/03/2016 10:47:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define   N_ELEM(a)	(sizeof (a) / sizeof (a[0]))
/*数组下标转元素地址*/
#define  __NtoPTR(n, base, width)	\
		((char *)base + (n) * (width))		
#define  NtoPTR(n)	__NtoPTR(n, base, width)

int cmp_int(const void *a, const void *b)
{
	int *pa = (int *)a;
	int *pb = (int *)b;

	return *pa - *pb;
}

void swap(void *a, void *b, size_t size)
{
	char *c = (char *)malloc(size);
	static int count = 0;
	++count;
#ifdef _DEBUG
	printf("count = %d\n", count);
#endif
	memcpy(c, a, size);
	memcpy(a, b, size);
	memcpy(b, c, size);
	free(c);
}

void bubble_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	size_t start, end;

	for ( end = nel - 1; end > 0; --end) {
		for (start = 0;  start < end; ++start) {
			if (cmp(NtoPTR(start), NtoPTR(start + 1)) > 0) {
				swap(NtoPTR(start), NtoPTR(start + 1), width);
			}
		}
	}
}


void select_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	size_t i,j;
	size_t min;
	for (i = 0; i < nel - 1; ++i) {
		min = i;
		for (j = i + 1; j < nel; ++j) {
			if (cmp(NtoPTR(j), NtoPTR(min)) < 0) {
				min = j;
			}
		}
		if (min != i) {
			swap(NtoPTR(i), NtoPTR(min), width);
		}
	}
}


void insert_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	size_t i;
	int j;
	char *buf = (char *)malloc(width);
	
	for (i = 1; i < nel; ++i) {
		j = i - 1;
		if (cmp(NtoPTR(i), NtoPTR(j)) >= 0) {
			continue;
		}
		memcpy(buf, NtoPTR(i), width);
		for (; j >= 0; --j) {
			if (cmp(NtoPTR(j), buf) > 0) {
				memcpy(NtoPTR(j + 1),
					NtoPTR(j), width);
			} else {
				// j <= buf 
				break;
			}
		}

		memcpy(NtoPTR(j + 1), buf, width);
	}

	free(buf);
}

void shell_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	int  step[] = {3, 2, 1}; //增量序列
	
	int i,j,k;//i为step[]下标， 控制 排序次数 
			//j 为待排序数。 j=offset+step[i]
			//k为 j上一元素 k=j-step[i]
	int offset;//k每次排序开始的位置 

	char *pbuf = (char *)malloc(width);

	for (i = 0; i < sizeof step/ sizeof step[0]; ++i) {
		for (offset = 0; offset < step[i]; ++offset) {
#ifdef _DEBUG
			for (j = offset; j < nel; j += step[i]) {
			printf("%d ", *(int *)NtoPTR(j));
	}
#endif
		/*sort.*/
		for (j = offset + step[i]; j < nel; j += step[i]) {
		k = j - step[i];
		if (cmp(NtoPTR(j), NtoPTR(k)) >= 0) {
			continue;
		} 

		memcpy(pbuf, NtoPTR(j), width);
		for (; k >= offset; k -= step[i]) {
			if (cmp(NtoPTR(k), pbuf) > 0) {
				memcpy(NtoPTR(k + step[i]),
				NtoPTR(k), width);
			} else {
				break;
			}
		}
	
		memcpy(NtoPTR(k + step[i]), pbuf, width);
}


			printf("\n--------\n");
#ifdef _DEBUG
			for (j = offset; j < nel; j += step[i]) {
			printf("%d ", *(int *)NtoPTR(j));
	}
			printf("\n");
#endif
		}
	}
	free(pbuf);
}


void __meger_sort(void *base, size_t nel, size_t width, 
     char *buf, int (*cmp)(const void *a, const void *b))
{

	//1. 分
	if (nel < 2) {
		return ;
	}//nel拆不下去，返回到调用它的函数上去。
	
	__meger_sort(base, nel / 2, width, buf, cmp);
	__meger_sort(NtoPTR(nel / 2), nel - nel / 2, width, buf, cmp);

	//2. 治：排序

	int l, r;
	size_t index = 0;
	l = 0;
	r = nel / 2;
//while ((l<nel/2)&&(r<nel))
	for (; (l < nel / 2) && (r < nel); ) {
		if (cmp(NtoPTR(l), NtoPTR(r)) <= 0) {
			memcpy(__NtoPTR(index, buf, width), NtoPTR(l), width);
				//
			++l;
		} else {
			memcpy(__NtoPTR(index, buf, width), NtoPTR(r), width);
			++r;
		}
		++index;
	}

	if (l < nel / 2) {
		//左半部分没有拷完
		memcpy(__NtoPTR(index, buf, width), NtoPTR(l), (nel / 2 - l) * width);
	} else {
		//右半部分没有考完
		memcpy(__NtoPTR(index, buf, width), NtoPTR(r), (nel - r) * width);
	}	

	//3. 合
	memcpy(base, buf, nel * width);
}

void meger_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	char *pbuf = (char *)calloc(nel, width);
	assert(pbuf != NULL);

	__meger_sort(base, nel, width, pbuf, cmp);

	free(pbuf);
}

void __quick_sort(void *base, size_t nel, size_t width, 
    char *pbuf, int (*cmp)(const void *a, const void *b))
{
	if (nel < 2) {
		return ;
	}

	int pivot = 0; //FIXME 轴值的选择会加入随机
	
	size_t l, r;
	
	memcpy(pbuf, NtoPTR(pivot), width);
	l = 0;
	r = nel - 1;	

	for (; l < r;) {
		/*自右向左找，找出小于轴值，拷贝到左边l的位置*/
		while (l < r) {
			if (cmp(NtoPTR(r), pbuf) < 0) {
				memcpy(NtoPTR(l), NtoPTR(r), width);
				++l;
				break;
			} else {
				--r;
			}
		}
		/*自左向右找，找出大于轴值，拷贝到右边r的位置*/
		while (l < r) {
			if (cmp(NtoPTR(l), pbuf) > 0) {
				memcpy(NtoPTR(r), NtoPTR(l), width);
				--r;
				break;
			} else {
				++l;
			}
		}
	}
	
	memcpy(NtoPTR(l), pbuf, width);

	//尾递归,以轴值为中心，分成两段序列，递归排序
	__quick_sort(base, l, width, pbuf, cmp);
	__quick_sort(NtoPTR(l + 1), nel - l - 1, width, pbuf, cmp);
	
}

void quick_sort(void *base, size_t nel, size_t width, 
		int (*cmp)(const void *a, const void *b))
{
	char *buf = (char *)malloc(width);

	__quick_sort(base, nel, width, buf, cmp);

	free(buf);	
}

int main()
{
	int  a[] = { 3, 2, 5, 7, 4, 6, 1, 0, 9, 8};

	int i;
	for (i = 0; i < N_ELEM(a); ++i) {
		printf("%d ", a[i]);
	}
	printf("\n----sorting---\n");
	/*元素为函数指针的数组*/
	void (*ap_sort[])(void *, size_t, size_t, int (*)(const void *, const void *)) = {
		qsort,
		bubble_sort,
		select_sort,
		insert_sort,
		shell_sort,
		meger_sort,
		quick_sort,
	};

	ap_sort[6](a, N_ELEM(a), sizeof(a[0]), cmp_int);
	//swap(&a[0], &a[5], sizeof(a[0]));
	for (i = 0; i < N_ELEM(a); ++i) {
		printf("%d ", a[i]);
	}
	printf("\n");
	printf("NtoPTR(2) = %d\n", *(int *)__NtoPTR(2, a, sizeof(int)));


	return 0;
}
