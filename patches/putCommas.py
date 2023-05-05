f = open("rawAsteroid.txt", "r")
f2 = open("asteroid.patch", "w")

n_patches = int(f.readline())
f2.write(str(n_patches)+"\n")
while n_patches>=0:
    x = f.readline()
    points = list(map(int, x.split()))
    points_with_Strings = ", ".join(map(str, points))
    f2.write(points_with_Strings+"\n")
    n_patches -= 1

n_points = int(f.readline())
f2.write(str(n_points)+"\n")
while n_points>=0:
    x = f.readline()
    points = list(map(float, x.split()))
    points_with_Strings = ", ".join(map(str, points))
    f2.write(" "+points_with_Strings+"\n")
    n_points -= 1