from math import sin, pi

entries = 128 # How many entries in the sine lookup table
precision = 8
lut = []

for i in range(entries):
    theta = (i / entries) * pi / 2
    sintheta = sin(theta)
    fixedpoint = int(round(sintheta * (2 ** precision)))
    print(f"sin({theta})={sintheta}[{hex(fixedpoint)}]")
    lut.insert(i, fixedpoint.to_bytes(2, "little"))

with open("data/sin_lut.bin", mode="wb") as file:
    for sine in lut:
        file.write(sine)

print("--------------------------------------------------------------------------------\ndone!")
