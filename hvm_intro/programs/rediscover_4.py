def show(num):
    print(str(num(lambda x: x+1)(0)))

num0 = lambda s: lambda z: z
num1 = lambda s: lambda z: s(z)
num2 = lambda s: lambda z: s(s(z))
num3 = lambda s: lambda z: s(s(s(z)))
num4 = lambda s: lambda z: s(s(s(s(z))))
func = lambda x: lambda s: lambda z: x(lambda x: s(s(x)))(z)

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
