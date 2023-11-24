def show(arr):
    print(str(arr))

nums = [0, 1, 2, 3, 4]
func = lambda x: x * 2

nums[0] = func(nums[0])
nums[1] = func(nums[1])
nums[2] = func(nums[2])
nums[3] = func(nums[3])
nums[4] = func(nums[4])

show(nums) # [0, 2, 4, 6, 8]
