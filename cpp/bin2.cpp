<typename T>
T * bin2(T *first, unsigned integer count, T target)
{
	if (first == NULL) return NULL;
	if (count == 0) return NULL;
	if (*first == target) return first;

	unsigned integer mid = floor(count/2);

	T *val = first + mid;

	if (*val == target) return *val;
	else if (target > *val)
		bin2(val + 1, count - (mid + 1), target);
	else
		bin2(first, mid, target);
}
