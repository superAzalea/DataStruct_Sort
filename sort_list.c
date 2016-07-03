/*
 * =====================================================================================
 *
 *       Filename:  sort_list.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/03/2016 15:31:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>

struct data_info {
	int data;
	struct list_head list;
};

int cmp(const struct list_head *a, const struct list_head *b)
{
	struct data_info *pa = container_of(a, struct data_info, list);
	struct data_info *pb = container_of(b, struct data_info, list);
	
	return pa->data - pb->data;
}

void swap(struct list_head *a, struct list_head *b)
{
	struct list_head c;
	__list_add(&c, a, a->next);
	list_del(a);
	__list_add(a, b, b->next);
	list_del(b);
	__list_add(b, &c, c.next);
	list_del(&c);
}


void bubble_sort(struct list_head *phead, 
			int (*cmp)(const struct list_head *a,
				const struct list_head *b))
{
	struct  list_head *start, *end;
	for (end = phead->prev; end != phead; end = end->prev){
		for (start = phead->next; (start != end) && (start != phead); start = start->next) {
			if (cmp(start, start->next) > 0) {
				swap(start, start->next);
				//start 因为交换所以需要归位
				start = start->prev;
				//end 发生交换就需要归位，避免跳跃过多个节点	
				if (start == end) {
					end = end->next;
				}
			}
		}
	}
}

/* 宏、内联、函数
marco: 预处理过程中的一种表达式
	可以把一个固定或者冗余和繁琐的操作，使用一个宏表达式，去代表，将来在宏表达式会被（预处理器），完成宏与填充表达式的一种替换
	因为它的实现是一种简单的查找替换，所以使用过程中需要特别注意一些细节：
	1） 参数和表达式容易混淆或者改变运算优先级，所以参数和整个填充表达式，需用使用'()'
	#define  PI   (3.14)
	#define  AREA(r)	(PI * (r) * (r))
	2） 宏名和参数()部分之间没有空格 
	  FOO(a, b)
	3) 填充语句后面无需加';'
	4) 不能使用'++''--'运算符的表达式作为参数传递给宏
	FOO(a++, b) 
	5) 填充语句写不下，需要用'\' 完成多行的连接
	...

内联：（inline）
	内联主要用来修饰函数，可以提高函数的执行效率，减去了寻址，传参，返回的一系列的开销，主要适用于被频繁调用的（简洁）函数。
    其只是建议，在通常情况还需要结合场景和编译器优化算法，决定 内联是否真正起作用
    内联功能的实现，是由编译器

普通函数：
	FOO(1, 2);
	foo(1, 2);
	函数是一组代码块的罗列，主要是完成某一种功能，它具备易读性，易维护，可调试等特点，这些是宏所不具备
*/

void select_sort(struct list_head *phead, 
		int (*cmp)(const struct list_head *a, 
				const struct list_head *b))
{
	struct  list_head *i, *j, *min;
	
	for (i = phead->next; i != phead->prev; i = i->next) {
		min = i;
		for (j = i->next; j != phead; j = j->next) {
			if (cmp(j, min) < 0) {
				min = j;
			}
		}

		if (min != i) {
			swap(i, min);
			//i 需要归位
			i = min;
		}
	}
}



void insert_sort(struct list_head *phead, 
		int (*cmp)(const struct list_head *a, 
				const struct list_head *b))
{
	struct list_head *i, *j;
	struct list_head *Next;
	
	for (i = phead->next->next; i != phead; i = Next) {
		Next = i->next;
		j = i->prev;
		if (cmp(i, j) >= 0) {
			continue;
		}
		list_del_init(i);
		for (; j != phead; j = j->prev) {
			if (cmp(j, i) < 0) {
				break;
			}
		}
		__list_add(i, j, j->next);
	}
}


int main()
{
	struct  data_info s[] = {
		{3}, {7}, {4}, {1}, {0}, {5}, {2}, {6}, {8},
	};

	struct list_head *phead = (struct list_head *)
			malloc(sizeof(*phead));
	assert(phead != NULL);

	list_head_init(phead);
	
	int i;
	for (i = 0; i < sizeof s/ sizeof s[0]; ++i) {
		list_add_tail(&s[i].list, phead);
	}

	struct list_head *cur;
	list_for_each(cur, phead) {
		struct data_info *pa = 
			container_of(cur, struct data_info, list);
		printf("%d ", pa->data);
	}
	printf("\n");
	printf("-----sorting----\n");
	
	//bubble_sort(phead, cmp);
	//select_sort(phead, cmp);
	insert_sort(phead, cmp);
	//swap(&s[0].list, &s[8].list);

	list_for_each(cur, phead) {
		struct data_info *pa = 
			container_of(cur, struct data_info, list);
		printf("%d ", pa->data);
	}
	printf("\n");

	



	
	free(phead);
	return 0;
}
