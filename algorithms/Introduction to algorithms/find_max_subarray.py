def find_max_crossing_subarray(A, low, mid, high):
    left_sum = -9999999999
    sum = 0
    max_left = mid
    for i in range(mid, low, -1):
        sum += A[i]
        if sum > left_sum:
            left_sum = sum
            max_left = i

    right_sum = -9999999999
    sum = 0
    max_right = mid + 1

    for j in range(mid + 1, high):
        sum += A[j]
        if sum > right_sum:
            right_sum = sum
            max_right = j

    return max_left, max_right, left_sum + right_sum

def find_max_subarray1(A, low, high):
    if high == low:
        return low, high, A[low] #base case: only one element
    else:
        mid = (low + high) // 2
        left_low, left_high, left_sum = find_max_subarray1(A, low, mid)
        right_low, right_high, right_sum = find_max_subarray1(A, mid + 1, high)
        cross_low, cross_high, cross_sum =find_max_crossing_subarray(A, low, mid, high)

    if left_sum >= right_sum and left_sum >= cross_sum:
        return left_low, left_high, left_sum
    elif right_sum >= left_sum and right_sum >= cross_sum:
        return right_low, right_high, right_sum
    else:
        return cross_low, cross_high, cross_sum


def find_max_subarray2(array):
    maxSum = 0
    sumvar = 0
    start = 0
    end = 0
    s = 0
    e = 0
    
    for i in range(len(array)):
        sumvar += array[i]
        if sumvar > maxSum:
            maxSum = sumvar
            start = s
            e = i
            end = e

        if sumvar < 0:
            s = i + 1
            e = s 
            sumvar = 0
    
    print(f"Max subarray found at start: {start} end: {end} maxSum: {maxSum}")

a = [13,-3,-25,20 ,-3 ,-16,-23,18,20,-7,12,-5,-22,15,-4,7]

start, end, max_sum = find_max_subarray1(a, 0, len(a)-1)
print(f"find_max_subarray1 Max subarray found at start: {start} end: {end} max_sum: {max_sum}")
find_max_subarray2(a)
