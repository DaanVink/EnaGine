from subprocess import call
import os
import sys

cwd = os.getcwd()
cwd += "/"
cwd = cwd.replace("\\", "/")
if (sys.argv[1] == "c"):
	with open("fileList.txt") as file:
		for line in file:
			os.system('mingw32-gcc -g -c "' + cwd + line[:-1] + '" -o "' + cwd + "obj/" + line[:-2] + 'o"')
elif (sys.argv[1] == "o"):
	comp = "g++ -o"
	with open("fileList.txt") as file:
		for line in file:
			comp += " obj/" + line[:-2] + "o"
	comp += " -lws2_32"
	os.system(comp)