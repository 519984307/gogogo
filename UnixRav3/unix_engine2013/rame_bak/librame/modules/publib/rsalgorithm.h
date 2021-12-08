#ifndef RS_ALGORITHM_H_
#define RS_ALGORITHM_H_


namespace rsalgorithm
{

template <class T>
T * bsearch(T arr[], int count, const T &k)
{
	int low = 0;
	int high = count;
	int mid;

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (k < arr[mid])
		{
			high = mid - 1;
		}
		else if (arr[mid] < k)
		{
			low = mid + 1;
		}
		else // *pm == k
		{
			return arr + mid;
		}
	}

	return 0;
}

template <class T>
void swap(T &t1, T &t2)
{
	T t= t1;
	t1 = t2;
	t2 = t;
}

template <class T>
void shortsort(T arr[], int lo, int hi)
{
	int p, max;
	while (hi > lo) {
		max = lo;

		for (p = lo + 1; p <= hi; p++) {
			if (arr[max] < arr[p]) {
				max = p;
			}
		}

		if ( max!=hi)
			swap<T>(arr[max], arr[hi]);

		hi--;
	}
}


static const int STKSIZE(8 * sizeof(int));
static const int CUTOFF(8);

template <class T>
void qsort(T arr[], int count)
{
	assert(0 != arr && 1 < count );

	register int lo = 0, hi = count - 1;
	register int mid;
	register int size;
	register int loguy, higuy;

	int lostk[STKSIZE] = {0}, histk[STKSIZE] = {0};
	int stkptr = 0;

recurse:

	size = (hi - lo) + 1;

	if (size <= CUTOFF){
		shortsort<T>(arr, lo, hi);
	}
	else{
		
		mid = lo + size / 2;

		if (arr[mid] < arr[lo]){
			swap<T>(arr[lo], arr[mid]); 
		}
		if (arr[hi] < arr[lo]){
			swap<T>(arr[hi], arr[lo]);
		}
		if (arr[hi] < arr[mid]){
			swap<T>(arr[hi], arr[mid]);
		}

		loguy = lo;
		higuy = hi;
		
		for ( ; ; ) {
			if (mid > loguy) {
				do {
					loguy++;
				} while (loguy < mid && arr[loguy] <= arr[mid]);
			}
			if (mid <= loguy) {
				do {
					loguy++;
				} while (loguy <= hi && arr[loguy] <= arr[mid]);
			}

			do {
				higuy--;
			} while (higuy > mid && arr[mid] < arr[higuy]);

			if (higuy < loguy)
				break;

			swap<T>(arr[loguy], arr[higuy]);
			
			if (mid == higuy)
				mid = loguy;
		}

		higuy++;
		if (mid < higuy){
			do {
				higuy--;
			}while(mid < higuy && arr[higuy] == arr[mid]);
		}
		if (mid >= higuy) {
			do {
				higuy--;
			}while (higuy > lo && arr[higuy] == arr[mid]);
		}

		if (higuy - lo >= hi - loguy) {
			if (lo < higuy) {
				lostk[stkptr] = lo;
				histk[stkptr] = higuy;
				++stkptr;
			}

			if (loguy < hi){
				lo = loguy;
				goto recurse;
			}
		}
		else {
			if (loguy < hi) {
				lostk[stkptr] = loguy;
				histk[stkptr] = hi;
				++stkptr;
			}

			if (lo < higuy) {
				hi = higuy;
				goto recurse;
			}
		}
	}

	--stkptr;
	if (stkptr >= 0) {
		lo = lostk[stkptr];
		hi = histk[stkptr];
		goto recurse;
	}
	else {
		return;
	}
}

}

#endif //RS_ALGORITHM_H_

