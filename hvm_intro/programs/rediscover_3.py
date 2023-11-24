def show(num):
    print(str(num))

num0 = 0
num1 = 1
num2 = 2
num3 = 3
num4 = 4
func = lambda x: x * 2

num0 = func(num0)
num1 = func(num1)
num2 = func(num2)
num3 = func(num3)
num4 = func(num4)

show(num0) # 0
show(num1) # 2
show(num2) # 4
show(num3) # 6
show(num4) # 8
