import re

s = ""

#escrever os modelos do windows para o final
with open(r"D:\Trabalho\Code\Universidade\3.2\CG\CG2023\xml_files\windows\solar_system_moons_windows.xml", "r") as f:
    for line in f:
        print(re.sub(r'<model file="(?:.|\n)*\\models\\((?:.|\n)*)">', r'<model file="./models/\1">', line))
        s += re.sub(r'<model file="(?:.|\n)*\\models\\((?:.|\n)*)">', r'<model file="./models/\1">', line)

#escrever as texturas do final para o final
with open(r"D:\Trabalho\Code\Universidade\3.2\CG\CG2023\xml_files\solar_system_moons.xml", "r") as g:
    for line in g:
        print(re.sub(r'<texture file="(?:.|\n)*\\textures\\((?:.|\n)*)"/>', r'<texture file="./textures/\1"/>', line) == line)
        s += re.sub(r'<texture file="(?:.|\n)*\\textures\\((?:.|\n)*)"/>', r'<texture file="./textures/\1"/>', line)

with open(r"D:\Trabalho\Code\Universidade\3.2\CG\CG2023\xml_files\solar_system_moons.xml", "w") as h:
    h.write(s)