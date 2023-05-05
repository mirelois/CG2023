import sys
import math

xml_file = open(sys.argv[1], "w")
xml_string = str()
res = list()

b = 4.8*150000000/(1392700*25)
a = 17.8*150000000/(1392700*25)

xml_file.write(
    f"""<group>
    <transform>
        <translate x = "{a-3}" y = "0" z = "0"/>
    </transform>
    <group>
    <transform>
        <translate time=\"{60*12*24*365}\" align="true" draw="true">
            <point x = "{-a}" y = "0" z = "0" />
    """
)

print(a,b)

print(math.sqrt(abs(a**2*b**2 - a**2 * b**2)/a**2))

l = []

for i in range(0,int(a), int(a/10)):
    x = i
    z = math.sqrt(abs(a**2*b**2 - x**2 * b**2)/a**2)
    l.append((x,z))
    

for x,z in reversed(l):
    print((x,z))
    xml_file.write(
    f"""        <point x = "{-x}" y = "0" z = "{z}" />
    """
)


xml_file.write(
    f"""        <point x = "0" y = "0" z = "{b}" />
    """
)

for x,z in l:
    print((-x,z))
    xml_file.write(
    f"""        <point x = "{x}" y = "0" z = "{-z}" />
    """
)

xml_file.write(
    f"""        <point x = "{a}" y = "0" z = "0" />
    """
)

for x,z in reversed(l):
    print((-x,-z))
    xml_file.write(
    f"""        <point x = "{-x}" y = "0" z = "{-z}" />
    """
)

xml_file.write(
    f"""        <point x = "0" y = "0" z = "{-b}" />
    """
)

for x,z in l:
    print((x,-z))
    xml_file.write(
    f"""        <point x = "{-x}" y = "0" z = "{z}" />
    """
)

xml_file.write(
    f"""        </translate>
        </transform>
        <models>
            <model file="./models/sphere.3d" />
        </models>
    </group>
</group>
    """
)