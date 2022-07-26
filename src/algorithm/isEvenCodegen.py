#!/usr/bin/python3

import sys

border = 1000
if(len(sys.argv) >= 2):
	border = int(sys.argv[1])

f = open("codegenIsEven.h", "w")

f.write("namespace isEven {\n")
f.write("bool codegenIsEven(unsigned int number) {\n")
f.write("\tswitch(number) {\n") 

for i in range(0, border, 1):
	f.write("\t\tcase " + str(i) + ": return ")
	if(i % 2) == 0:
		f.write("true;\n")
	else:
		f.write("false;\n")


f.write("\t}\n")
f.write("\treturn false;")
f.write("}}\n")
f.close()
