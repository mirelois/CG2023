import re


file = open("/home/utilizador/Documents/3_ano/2_semestre/computacao_grafica/CG2023/xml_files/solar_system_moons.xml", "r")

for line in file:
    tup = re.findall(
        r'<scale x="(\d.\d*)" y="(\d.\d*)" z="(\d.\d*)" \/>', line)
    x, y, z = 0, 0, 0
    if tup:
        x, y, z = tup[0]
        x = float(x)*100
        y = float(y)*100
        z = float(z)*100
    print(re.sub(r'<scale x="(\d.\d*)" y="(\d.\d*)" z="(\d.\d*)" \/>',
          f'<scale x="{x}" y="{y}" z="{z}" />', line))
