import matplotlib.pyplot as plt
import os

files=os.listdir("timing")
for i in files:
    print(i)
    n=[]
    time=[]
    with open("timing/"+i, "r") as f:
        input='0'
        while 1:
             input=f.readline()
             if input=='':
                 break
             n.append(int(input.split(" ")[0]))
             time.append(float(input.split(" ")[1][:-1:]))
    print(n)
    print(time)
    print('\n')
    plt.plot(n, time)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('nodes')
    plt.ylabel('time')
    plt.title(i.replace('.txt', ''))
    plt.savefig("plots/"+i.replace('.txt', '')+'.png')
    plt.clf()
