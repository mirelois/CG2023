import sys

file = open(sys.argv[1], "r")

for size in file:
    l = size.split(" ")
    t = float(l[1][:-1])/1392700.0
    if len(l) >= 2:
        print(f'<scale x=\"{t:.20f}\" y=\"{t:.20f}\" z=\"{t:.20f}\" />')
