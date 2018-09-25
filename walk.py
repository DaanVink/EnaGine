import os
files = []
for ret in os.walk(os.getcwd() + "\\Ena"):
	#print(ret)
	for x in range(0,len(ret[2])):
		pass
		files.append(ret[0].replace("\\", "/") + ret[2][x])
print(files)