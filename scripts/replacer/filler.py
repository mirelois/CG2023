import re

s = ""

with open(r"D:\Trabalho\Code\Universidade\3.2\CG\CG2023\xml_files\windows\solar_system_moons_windows.xml", "r") as f:
    for line in f:
        print(re.sub(r'<model file="(.*)"></model>', r'<model file="\1"><color><diffuse R="131" G="127" B="133"/><ambient R="128" G="128" B="128"/><specular R="0" G="0" B="0"/><emissive R="0" G="0" B="0"/><shininess value="0"/></color></model>', line) == line)
        s += re.sub(r'<model file="(.*)"></model>', r'<model file="\1"><color><diffuse R="131" G="127" B="133"/><ambient R="128" G="128" B="128"/><specular R="0" G="0" B="0"/><emissive R="0" G="0" B="0"/><shininess value="0"/></color></model>', line)

with open(r"D:\Trabalho\Code\Universidade\3.2\CG\CG2023\xml_files\windows\solar_system_moons_windows.xml", "w") as g:
    g.write(s)