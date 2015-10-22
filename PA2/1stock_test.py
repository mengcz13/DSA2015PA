import random

f=open("input.txt","w")
N=100000
maxbreak=(2**31-1)/(2*N)

daybreak=random.randint(1,maxbreak)
stock=random.randint(0,999)
f.write("%d %d\n"%(daybreak,stock))
onmarket=1

for i in range(1,N):
    daybreak=random.randint(1,maxbreak)
    if (onmarket==0):
        flag=1
    else:
        flag=random.randint(0,1)

    if (flag):
        stock=random.randint(0,999)
        f.write("%d %d\n"%(daybreak,stock))
        onmarket=onmarket+1
    else:
        f.write("%d\n"%(daybreak))
        onmarket=onmarket-1

for i in range(0,onmarket):
    daybreak=random.randint(1,maxbreak)
    f.write("%d\n"%(daybreak))

f.write("%d"%((N+onmarket)/2))

f.close()