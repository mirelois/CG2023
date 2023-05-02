import sys

file = open(sys.argv[1], "r")

for size in file:
    l = size.split(" ")
    #t = float(l[1][:-1])/1392700.0
    if len(l) >= 2:
        print(l[0])
        t = (float(l[1][:-1])*2)/(1392700)
        print(f'<scale x=\"{t:.20f}\" y=\"{t:.20f}\" z=\"{t:.20f}\" />')
        #print(f'<translate x=\"{t:.20f}\" y=\"0\" z=\"0\" />')
