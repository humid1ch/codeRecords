#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <stack>

using std::cout;
using std::endl;

void printVector(const std::vector<int>& nums) {
	for (auto elem : nums) {
		cout << elem << " ";
	}
	cout << endl;
}

void printVector(const std::vector<int>& nums, int begin, int end) {
    for (int i = begin; i <= end; i++) {
        cout << nums[i] << " ";
    }
}

/*
 * 1. 插入排序
 *
 *  将数组看作两部分: [0, end] 已排序, [end+1, n]未排序
 *  插入排序要做的就是, 循环将 end+1 位置上的数据, 放到 已排序的数据中, 并且保证 [0, end+1] 依旧已排序, 让已排序的部分增长1 也就是说, end是不断增长的,
 *  直到end增长到数组末尾, 就说明整个数组已排序
 *
 *  那么, 如何将 end+1位置上的数据, 放在[0, end]的合适位置,
 *  就是插入排序的主要实现
 *
 *  [0,end]的位置, 是已经排好序的, 如果要将end+1位置的数据放在合适的位置
 *  那么就需要 从end向前遍历, 记录当前对比位置为 now 如果 nums[now] 比 nums[end+1] 大, 那么就继续向前比
 *  直到比nums[end+1]小 当nums[now] < nums[end+1] 时, 那么nums[end+1]的位置就应该是 now+1 
 *  但是不能直接覆盖, 因为要先移动数据, 要将原[now+1, end]的数据, 都向后挪动一位
 *
 *  所以还需要遍历一遍[now+1, end]的数据吗?
 *  答案是不需要, 可以在寻找now的同时, 移动比nums[end+1]大的数据, 就将数据放在now+1位置上
 *  为什么呢? 分析一下, now是从end开始的,
 *  如果此时now==end, nums[end+1] < nums[now], 那么nums[now]就要放在end+1位置
 *  因为, nums[end+1] < nums[now], 而此时now==end, 那么为了保证前面有序, 就必须让nums[now] 在nums[end+1]后面 所以nums[now]需要向后一位, 即now+1位置
 *  在now==end时 也就是此时的 end+1 位置上
 *
 *  然后需要拿 nums[end+1] 继续向前对比, 那么now就需要--
 *  此次 继续按照上面的思路进行对比 和 移动数据, 拿nums[end+1] 与现在的
 *  nums[now]对比 如果 nums[now] > nums[end+1], 那么 nums[now]就需要后移一位, 保证nums[now]在nums[end+1]之后, 也就是给 nums[end+1]腾位置 如此循环往复
 *
 *  直到 nums[now] < nums[end+1], 那么此时, 直接将nums[end+1] 放在 nums[now+1]就可以了 然后本趟插入排序完成
 */
void insertSort(std::vector<int>& nums) {
	for (int i = 0; i < nums.size() - 1;i++) { // i 最后一次应该在 size-1处, 此次是将 size 位置的数据 向前插入, 即最后一趟插入
		int end = i;			 // end 与循环条件i同步, 作为已排序部分的结尾
		int cur = nums[end + 1]; // cur 为未排序部分的第一个元素
		int now = end;
		while (now >= 0) {
			if (nums[now] > cur) {
				nums[now + 1] = nums[now]; // now位置数据, 后移一位
				now--;
			}
			else {
				break; // 找到了比 nums[end+1]小的数, 退出循环, 找位置结束
			}
		}

		nums[now + 1] = cur; // 一趟插入数据结束
	}
    // 可以看到, 变量end 在查找now位置时, 从头到尾没有用过, 所以其实可以用end代替now--
    // 不过 像这样写可以更好的理解
}

//void shellSort(std::vector<int>& nums) {
 //   cout << "希尔排序开始" << endl;
 //   int gap = nums.size();

 //   int count = 0;

 //   while (gap > 1) {               // gap > 1 就继续预排序
 //       gap = gap / 3 + 1;          // 预排序 gap的取值方法
 //       cout << "以 " << gap << " 作为间隔分组" << endl;
 //       for (int i = 0; i < gap; i++) {     // 外层循环次数 其实就是分出来的的组数
 //           for (int j = i; j < nums.size() - gap; j += gap) {
 //               int end = j;
 //               int cur = nums[end + gap];
 //               while (end >= 0) {
 //                   if (cur < nums[end]) {
 //                       nums[end + gap] = nums[end];
 //                       end -= gap;
 //                   }
 //                   else {
 //                       break;
 //                   }
 //                   count++;
 //               }
 //               nums[end + gap] = cur;
 //               printVector(nums);
 //           }
 //       }
 //   }

 //   cout << endl << "希尔排序结束, " << "共做 " << count << " 次数据对比" << endl;
////	cout << endl << "希尔排序结束" << endl;
//}

void shellSort(std::vector<int>& nums) {
	int gap = nums.size();
    
    while (gap > 1) {
        gap = gap / 3 + 1;
        for (int i = 0; i < nums.size() - gap; i++) {
			int end = i;
            int cur = nums[end + gap];
            while (end >= 0) {
                if (cur < nums[end]) {
					nums[end + gap] = nums[end];
                    end -= gap;
                }
                else {
                    break;
                }
            }
            nums[end + gap] = cur;
        }
	}
}

//void bubbleSort(std::vector<int>& nums) {
//	cout << "冒泡排序开始" << endl << endl;
//	for(int i = 0; i < nums.size(); i++) {
//		cout << "第" << i+1 << "趟" << endl;
//        for(int j = 0; j < nums.size() - i - 1; j++) {
//            if (nums[j] > nums[j+1]) {
//                std::swap(nums[j], nums[j+1]);
//            }
//        }
//		printVector(nums);
//    }
//	cout << "冒泡排序结束" << endl << endl;
//}


void bubbleSort(std::vector<int>& nums) {
	for(int i = 0; i < nums.size(); i++) {
        int isExchange = 0;				// 用于记录本趟冒泡是否发生数据交换
        for(int j = 0; j < nums.size() - i - 1; j++) {
            if (nums[j] > nums[j+1]) {
				std::swap(nums[j], nums[j+1]);
                isExchange = 1;			// 发生数据交换, 置1
            }
        }
        // 一趟冒泡结束
        if (!isExchange) 
            break;				// 如果一趟冒泡结束了, 还没有发生数据交换,说明整个数组已经有序了, 可以退出外部循环
    }
}

//void selectSort(std::vector<int>& nums) {
//	cout << "选择排序开始" << endl << endl;
//	for (int i = 0; i < nums.size(); i++) {
//        int maxi = 0;
//        int left =  nums.size() - i;
//		cout << "第" << i+1 << "趟" << endl;
//        for (int j = 0; j <= left; j++) {
//			if (nums[j] > nums[maxi]) {
//				maxi = j;					// 记录最大值的坐标
//            }
//        }
//        std::swap(nums[maxi], nums[left]);
//		printVector(nums);
//    }
//	cout << "选择排序结束" << endl << endl;
//}

void selectSort(std::vector<int>& nums) {
	cout << "选择排序开始" << endl << endl;
    int left = 0;
    int right = nums.size() - 1;
    
    // 循环停止的条件就是两指针相遇
    while(left < right) {
        int mini = left;
        int maxi = left;
        for (int i = left; i <= right; i++) {
            if (nums[i] > nums[maxi])
                maxi = i;				// 记录最大值的位置
            if (nums[i] < nums[mini])
                mini = i;				// 记录最小值的位置
        }
        
        // 找完极值, 就可以交换数据
        std::swap(nums[maxi], nums[right]);
        // 将最大值放入有效范围的末尾之后
        // 不能再直接放最小值
        // 因为 mini 原本可能就在有效范围的末尾, 即right
        // 如果是这样, 原来的nums[mini]就被交换走了
        // 此时, nums[mini]其实就是刚放入的 nums[maxi]
        // 所以, 要先判断一下 mini 是否 就是right
        if (mini == right) {
            mini = maxi;
        }
        std::swap(nums[mini], nums[left]);
 
        // 缩小有效范围
        left++;
        right--;
    }
}

void numSwap(int& num1, int& num2) {
	int tmp = num1;
    num1 = num2;
    num2 = tmp;
}

// Hoare版
int hoareSortAPart(std::vector<int>& nums, int left, int right) {
	int keyi = left;
    int key = nums[keyi];
    while (left < right) {
		while (nums[right] >= key && left < right)
            right--;
        
        while (nums[left] <= key && left < right)
            left++;
        
        numSwap(nums[left], nums[right]);
    }
    numSwap(nums[keyi], nums[left]);
    
    return left;	// 返回 left, 即两只指针相遇地点, 也就是划分数组界限的位置
}

// pit
int pitSortAPart(std::vector<int>& nums, int left, int right) {
    int key = nums[left];
    int pit = left;
    while (left < right) {
		while (nums[right] >= key && left < right)
            right--;
        nums[pit] = nums[right];
        pit = right;
        
        while (nums[left] <= key && left < right)
            left++;
        nums[pit] = nums[left];
        pit = left;
    }
	nums[left] = key;		// 相遇位置是最后的坑
    
    return left;	// 返回 left, 即两只指针相遇地点, 也就是划分数组界限的位置
}

// 前后指针版
int pointerSortAPart(std::vector<int>& nums, int left, int right) {
    int key = nums[left];
    int fast = left + 1;
    int last = left;
    while (fast <= right) {
		if (nums[fast] < key && fast != ++last)
            numSwap(nums[fast], nums[last]);
        
        fast++;
    }
    numSwap(nums[left], nums[last]);
    
    return last;	// 返回 last, 划分数组界限的位置
}

// 三数取中
int qSortGetMid(std::vector<int>& nums, int begin, int end) {
    // 假如 [12, 20], 那么 mid 为 16
    int mid = begin + (end - begin) / 2;		// begin 不一定为0
    if (nums[begin] > nums[mid]) {
        if (nums[mid] > nums[end]) 				// begin > mid > end
            return mid;
        else if (nums[end] > nums[begin])		// end > begin > mid
            return begin;
        else 
            return end;
    }
    else { // mid > begin
        if (nums[end] > nums[mid]) 				// end > mid > begin
            return mid;
        else if (nums[begin] > nums[end])		// mid > begin > end
            return begin;
        else 
            return end;
    }
}

void _quickSort(std::vector<int>& nums, int begin, int end) {
    if (begin >= end) {
        return;			// 两指针不在维护一个有效数组, 返回
    }
    
    int midi = qSortGetMid(nums, begin, end);
    numSwap(nums[midi], nums[begin]);

//    int keyi = hoareSortAPart(nums, begin, end);
//    int keyi = pitSortAPart(nums, begin, end);
    int keyi = pointerSortAPart(nums, begin, end);
    _quickSort(nums, begin, keyi - 1);
    _quickSort(nums, keyi + 1, end);
}

void quickSort(std::vector<int>& nums) {
    _quickSort(nums, 0, nums.size()-1);
}

void nonRecursiveQuickSort(std::vector<int>& nums) {
    std::stack<int> qSortIntervalSt;	// 快排区间栈, 用于存储数组区间
    qSortIntervalSt.push(0);
    qSortIntervalSt.push(nums.size() - 1);
    
    while (!qSortIntervalSt.empty()) {
        int end = qSortIntervalSt.top();
        qSortIntervalSt.pop();
        int begin = qSortIntervalSt.top();
        qSortIntervalSt.pop();
        
        int midi = qSortGetMid(nums, begin, end);
    	numSwap(nums[midi], nums[begin]);				// 将取到的中值, 与数组头元素交换位置, 后边的处理中, 就会用中值做key
        
        int keyi = pointerSortAPart(nums, begin, end);	// 处理给定范围的数组
        if (begin < keyi - 1) {			// 保证入栈为有效数组区间
            qSortIntervalSt.push(begin);
            qSortIntervalSt.push(keyi - 1);
        }
        if (end > keyi + 1) {
            qSortIntervalSt.push(keyi + 1);
            qSortIntervalSt.push(end);
        }
    }
}

std::vector<int> nums = {456, 12, 76, 7, 62, 3, 65,
					6345, 3756, 34523, 6456, 23534, 64576, 3653,
					324, 768, 2, 43657};

void initNums(std::vector<int>& nums, int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        int num = rand() % 10000;
        nums.push_back(num);
    }
}


void insertSortTest() {
//	printVector(nums);
	insertSort(nums);
//	printVector(nums);
}

void shellSortTest() {
//    printVector(nums);
    shellSort(nums);
//    printVector(nums);
}

void bubbleSortTest() {
//	printVector(nums);
    bubbleSort(nums);
//	printVector(nums);
}

void selectSortTest() {
//	printVector(nums);
    selectSort(nums);
//	printVector(nums);
}

void quickSortTest() {
//    printVector(nums);
    quickSort(nums);
//    printVector(nums);
}

void nonRecursiveQuickSortTest() {
//    printVector(nums);
    nonRecursiveQuickSort(nums);
//    printVector(nums);
}

int main() {
    clock_t start = 0, finish = 0;
//    initNums(nums, 10000000);

//    start = clock();
//    insertSortTest();
//    bubbleSortTest();
//    selectSortTest();
//    shellSortTest();
//    quickSortTest();
    nonRecursiveQuickSortTest();

    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}
