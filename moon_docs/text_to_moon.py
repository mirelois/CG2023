import re
import sys
import random as rn

moon_file = open(sys.argv[1], "r")
xml_file = open(sys.argv[2], "w")
xml_string = str()
res = list()


for line in moon_file:
    # print(line)
    res += (re.findall(
        r"(.+?)(?:\(currently lost\))?- distance: (\d+(?:\.\d+)?) million km- diameter: (\d+(?:.\d+)?) km", line))

for name, distance, diameter in res:

    angle_x = rn.randint(0, 360)
    angle_y = rn.randint(0, 360)

    xml_file.write(
        f"""<group>
            <!-- {name}-->
            <transform>
                <rotate angle=\"{angle_x}\" x=\"1\" y=\"0\" z=\"0\"/>
                <rotate angle=\"{angle_y}\" x=\"0\" y=\"1\" z=\"0\"/>
                <translate x=\"0\" y=\"{float(distance)}\" z=\"0\" />
                <scale x=\"0.15\" y=\"0.15\" z=\"0.15\" />
            </transform>
            <models>
                <model file=\"../../models/sphere.3d\" />
            </models>
         </group>
         """
    )
