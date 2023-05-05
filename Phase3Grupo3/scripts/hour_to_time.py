import sys

file = open(sys.argv[1], "r")

for size in file:
    l = size.split(" ")
    #t = float(l[1][:-1])/1392700.0
    if len(l) >= 2:
        #print((float(l[1][:-1])*300)/(5832))
        print(l[0])
        print((float(l[1][:-1])*300*24)/(5832))