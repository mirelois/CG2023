import re
import sys
import random as rn

moon_file = open(sys.argv[1], "r")
xml_file = open(sys.argv[2], "w")
xml_string = str()
res = list()

mother_planet_size = 49244

# (\w+) (\d+) km (\d+(?:.\d+)?) km

lista = list()

for line in moon_file:

    lista = re.findall(
        r"(\w+) (\d+(?:.\d+)?) million km (\d+(?:.\d+)?) km", line)

    if lista:
        res += lista

    lista = re.findall(r"(\w+) (\d+(?:.\d+)?) km (\d+(?:.\d+)?) km", line)
    res += lista

for name, distance, diameter in res:

    angle = rn.randint(0, 360)
    rotate_on_x = rn.randint(0, 1)
    rotate_on_y = rn.randint(0, 1)
    rotate_on_z = rn.randint(0, 1)

    dist = ((10000000*float(distance)/(mother_planet_size))/9) + 2
    diam = float(diameter)/(mother_planet_size/10)

    xml_file.write(
        f"""<group>
            <!-- {name}-->
            <transform>
                <rotate angle=\"{angle}\" x=\"{rotate_on_x}\" y=\"{rotate_on_y}\" z=\"{rotate_on_z}\"/>
                <translate x=\"0\" y=\"{float(dist):.20f}\" z=\"0\" />
                <scale x=\"{(float(diam)):.20f}\" y=\"{(float(diam)):.20f}\" z=\"{(float(diam)):.20f}\" />
            </transform>
            <models>
                <model file=\"../../models/sphere.3d\" />
            </models>
         </group>
         """
    )
