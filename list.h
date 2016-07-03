/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/03/2016 14:32:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define   INIT_LIST_HEAD(head) \
	struct  list_head head = { &(head), &(head) }
/*
inline: 内联用来申明函数使用
特点： inline 是GCC中的一个关键词，用来[建议]编译器将某段代码优化，提高程序的执行效率。

1. 使用内联减少了寻址,传参,返回等过程，提高程序执行的效率，
2. 内联的功能是由编译器完成，在编译过程中，在调用内联函数的位置，将其函数体代码拷贝到当前位置，减少了传参返回等开销
3. 内联仅仅是编译器中一个建议关键词，具体是否能够采纳还需要遵循一些规则
比如： 
	- 函数体要简洁(10行以内)
	- 函数体中不允许循环、递归
	- 使用简易的控制语句(if, 三目运算符)
	- 不能使用函数指针指向该类型的函数
	- 不能使用递归函数嵌套调用该函数
	- 程序的体积会有一定增加
	- 从执行效率提高角度来看，还是利大于弊的
4. 内联函数作用域在当前文件有效，为了扩大作用域，通常会定义头文件中，只要include 该头文件都可以使用内联函数。
为了防止多个c文件中函数名的冲突, inline 通常与 static 一起使用
*/
static inline void  list_head_init(struct list_head *phead)
{
	phead->next = phead;
	phead->prev = phead;
}

static inline  void  __list_add(struct list_head *pnode
		, struct list_head *Prev, 
		  struct list_head *Next)
{
	pnode->next = Next;
	pnode->prev = Prev;

	Prev->next = pnode;
	Next->prev = pnode;
}

static inline  void  list_add(struct list_head *pnode,
			struct list_head *phead)
{
	__list_add(pnode, phead, phead->next);
}

static inline  void  list_add_tail(struct list_head *pnode,
			struct list_head *phead)
{
	__list_add(pnode, phead->prev, phead);
}

static  inline  int list_is_empty(struct list_head *phead)
{
	return (phead->next == phead) && (phead->prev == phead);
}

static inline void  list_del(struct list_head *pnode)
{
	pnode->next->prev = pnode->prev;
	pnode->prev->next = pnode->next;
}

static inline void list_del_init(struct list_head *pnode)
{
	list_del(pnode);
	list_head_init(pnode);	
}

#define  list_for_each(cur, head) \
	for (cur = (head)->next; cur != head; cur = cur->next)

#define  list_for_each_continue(cur, head) \
	for (cur = (cur)->next; cur != head; cur = cur->next)
#define  list_for_each_from(cur, head) \
	for ( ; cur != head; cur = cur->next)
/*
取小结构体与大结构体之间的偏移量(以字节为单位)
@type: 大结构体的类型
@member: 小结构体在大结构体中的成员名
注意： (->)箭头运算符后面的参数不能加'()',否则会改变执行顺序，发生语法错误
*/
#define  offset_of(type, member) \
	((size_t)&(((type *)0)->member))

#define  container_of(ptr, type, member)	\
	((type *)((char *)ptr - offset_of(type, member)))

/*反向遍历*/
#define  list_for_each_reverse(cur, head) \
	for (cur = (head)->prev; cur != head; cur = (cur)->prev)	

/*安全遍历*/
#define  list_for_each_safe(cur, Next, head) \
	for (cur = (head)->next; (Next = (cur)->next) && (cur != head); cur = (Next))

#define  list_for_each_safe_continue(cur, Next, head) \
	for (cur = (cur)->next; (Next = (cur)->next) && (cur != head); cur = (Next))

#define  list_for_each_safe_from(cur, Next, head) \
	for ( ; (Next = (cur)->next) && (cur != head); cur = (Next))
