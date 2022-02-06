import getopt
import sys
import os.path
import glob
import random
import subprocess
from datetime import datetime

md5output=""

def Md5FileCreation(x,directoryName,outputmd5file):
    global md5output
    size = random.randint(10000, 99999)
    fileName=str(x)+".txt"
    cmd = "head -c " + str(size) + " </dev/urandom > " + directoryName + fileName
    os.system(cmd)
    cmd2 = "md5sum " +  directoryName + fileName
    md5sumoutput=subprocess.check_output(cmd2, shell=True)
    splitword = md5sumoutput.split()
    md5output += splitword[1] + ": " + splitword[0] + "\n"
    return md5output

# Get the arguments from the command-line except the filename
start=datetime.now()
    
n = len(sys.argv)
print("Total arguments passed:", n)

if n != 3 :
    print("wrong number of arguments it must contain directory and number of files to be created")
    exit()
for i in range(1, n):
    print(sys.argv[i])

directoryName=sys.argv[1]
try:
    numberofFiles=int(sys.argv[2])
except:
    print("number of files should be of type int")
    exit()

# Check if Directory is present or not
isdir = os.path.isdir(directoryName) 
print(isdir)

#Create the directory if not present
if isdir == False :
    os.mkdir(directoryName)

# Remove all the files in the directory if present
files = glob.glob(directoryName + "*")
for f in files:
    os.remove(f)
outputmd5file = directoryName + "expected.md5"
# iterate and create n files
temp = [ Md5FileCreation(x,directoryName,outputmd5file) for x in range(1,numberofFiles+ 1)]

file1 = open(outputmd5file, "a")
file1.write(md5output);
file1.close()

print datetime.now()-start
