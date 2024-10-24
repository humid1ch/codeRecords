//给定一个大小为 n 的数组，找到其中的多数元素。多数元素是指在数组中出现次数 大于 ⌊ n/2 ⌋ 的元素。
//你可以假设数组是非空的，并且给定的数组总是存在多数元素。

//来源：力扣（LeetCode）
//链接：https://leetcode-cn.com/problems/majority-element
//著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

int cmp_int(const void* e1, const void* e2)
{
    return (*(int*)e1 - *(int*)e2);
}

int majorityElement(int* nums, int numsSize)
{
    qsort(nums, numsSize,sizeof(nums[0]),cmp_int);

    return *(nums + numsSize/2);
}