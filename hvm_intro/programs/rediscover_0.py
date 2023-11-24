def show(arr):
    print(str(arr))

nums = [0, 1, 2, 3, 4]
func = lambda x: x * 2

if func is not None:
    for i in range(len(nums)):
        nums[i] = func(nums[i])

show(nums) # [0, 2, 4, 6, 8]
