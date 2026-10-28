AC_DEFUN([AC_LIST_SORT_CMP_CONST_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/list.h>
			#include <linux/list_sort.h>

			static int cmp(void *priv, const struct list_head *a,
				       const struct list_head *b)
			{
				return a != b;
			}
		], [
			LIST_HEAD(head);
			list_sort(NULL, &head, cmp);
		], [], [
			AC_DEFINE(BPM_LIST_SORT_CMP_CONST_NOT_PRESENT, 1,
				  [list_sort() does not accept a comparator with const list entries])
		])
	])
])
