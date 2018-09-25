from subprocess import call
import os
from filecmp import cmp
import argparse

parser = argparse.ArgumentParser()

parser.add_argument("-compile_all", action = "store_true")
parser.add_argument("-dest", dest = "dest", default = "build.exe")

args = parser.parse_args()


cwd = os.getcwd()
cwd += "/"
cwd = cwd.replace("\\", "/")
cwdLen = len(cwd)

compiler_c = "gcc -g " # This is the compiler that compiles the c code to assembly
compiler_assembly = "g++ -o " + args.dest # This is the compiler that compiles the assembly code to an executable


files = []
changedFiles = []

# Here we walk through the entire /Ena/ directory, and find all ".c" files
# We then check to see if they have changed since the last time we compiled
# If they have, we add them to the list of files to recompile
# Otherwise we move on to the next file
# However, if the "-compile_all" flag is set, we instead add all files to the list
# os.walk() returns a tuple in the following format:
# ("directory name", ["all subdirectories in this directory"], ["all files in this directory"])
for ret in os.walk(os.getcwd() + "/Ena"): # Main loop for walking the /Ena/ directory and all subdirectories
	for x in range(0,len(ret[2])): # Loop over every file in the directory
		if ret[2][x][-2:] == ".c": # Check if the filename ends on ".c"
			fileName = ret[2][x] # Make the filename more easily accesible
			dirName = (ret[0].replace("\\", "/"))[cwdLen:] + "/" # Format the directory name with forward slashes (windows only)
			fullPath = cwd + dirName + fileName # Assemble the cwd, current subdirectory and filename into a complete path for the compiler
			if not (cmp(fullPath, cwd + ".temp/" + dirName + fileName)) | args.compile_all: # Check if the file has changed since the last compilation
				changedFiles.append(fullPath) # Add the file to the list for compilation
			files.append(fullPath)
				
print(files)
print(changedFiles)

# compile to assembly
for file in changedFiles:
	#pass
	print(compiler_c +  "-c" + cwd + file + '" -o "' + cwd + "obj/" + file[:-1] + 'o"')
	
	compiler_assembly += " obj/" + line[:-1] + "o"
compiler_assembly += " -lws2_32"
os.system(compiler_assembly)
