from subprocess import call
from os import walk, getcwd, system, path, makedirs
from filecmp import cmp
import argparse
from shutil import rmtree
from hashlib import md5
import json
from pprint import pprint


def gethash(fname):
	with open(fname, 'rb') as afile:
		buf = afile.read()
	return md5(buf).hexdigest()

	
parser = argparse.ArgumentParser()

parser.add_argument("-obj", dest = "obj", default = "obj")
parser.add_argument("-exe", dest = "exe", default = "build.exe")
parser.add_argument("-compile_all", action = "store_true")
parser.add_argument("-ghost", action = "store_true")

args = parser.parse_args()


if args.obj[-1] != "/":
	args.obj += "/"
if args.exe[-1] != " ":
	args.exe += " "


cwd = getcwd()
cwd += "/"
cwd = cwd.replace("\\", "/")
cwdLen = len(cwd)


compiler_c = "gcc -w -g " # This is the compiler that compiles the c code to assembly
compiler_assembly = "g++ -o " + args.exe # This is the compiler that compiles the assembly code to an executable


files = []
changedFiles = []
prev_dir = ""

try:
	with open("prev.json") as f:
		jsonFiles = json.load(f)
		f.close()
except (IOError, json.decoder.JSONDecodeError):
	jsonFiles = json.loads("{}")
	f = open("prev.json", "w+")
	f.close()
	print("Unable to locate prev.json! Generating new one")

# Here we walk through the entire /Ena/ directory, and find all ".c" files
# We then check to see if they have changed since the last time we compiled
# If they have, we add them to the list of files to recompile
# Otherwise we move on to the next file
# However, if the "-compile_all" flag is set, we instead add all files to the list
# os.walk() returns a tuple in the following format:
# ("directory name", ["all subdirectories in this directory"], ["all files in this directory"])
for ret in walk(cwd + "Ena"): # Main loop for walking the /Ena/ directory and all subdirectories
	for x in range(0,len(ret[2])): # Loop over every file in the directory
		if prev_dir != ret[0]: # Check if we've entered a new directory
			if not path.exists((cwd + args.obj + ret[0][cwdLen:]).replace("\\", "/")): # Check if the directory already exists
				makedirs((cwd + args.obj + ret[0][cwdLen:]).replace("\\", "/")) # If not, create the directory
		if ret[2][x][-2:] == ".c": # Check if the filename ends on ".c"
			fileName = ret[2][x] # Make the filename more easily accesible
			dirName = (ret[0].replace("\\", "/"))[cwdLen:] + "/" # Format the directory name with forward slashes (windows only)
			fullPath = (cwd + dirName + fileName) # Assemble the cwd, current subdirectory and filename into a complete path for the compiler
			fileHash = gethash(fullPath)
			try: 
				hasChanged = (fileHash != jsonFiles[str(dirName + fileName)]) | args.compile_all # Check if the file has changed since the last compilation
			except KeyError:
				hasChanged = True # The filepath was not found in prev.json
			jsonFiles[str(dirName + fileName)] = fileHash
			if hasChanged:
				changedFiles.append(fullPath) # Add the file to the list for compilation
			files.append(fullPath)
		prev_dir = ret[0]
	

# compile to assembly
for file in changedFiles:
	if args.ghost:
		print(compiler_c +  '-c "' + file + '" -o "' + file[:cwdLen] + args.obj + file[cwdLen:-1] + 'o"')
	else:
		system(compiler_c +  '-c "' + file + '" -o "' + file[:cwdLen] + args.obj + file[cwdLen:-1] + 'o"')
		
for file in files:
	compiler_assembly += cwd + args.obj + file[cwdLen:-1] + "o "

compiler_assembly += " -lws2_32"
if args.ghost:
	print(compiler_assembly)
else:
	system(compiler_assembly)
	
with open("prev.json", "w") as out:
	json.dump(jsonFiles, out)