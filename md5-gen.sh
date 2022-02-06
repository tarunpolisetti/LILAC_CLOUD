#!/bin/sh

DirectoryName=$1
NoOfFiles=$2
md5file="expected.md5"
newline="\n"
colon=": "
FLOOR=10000;
CEILING=99999;
RANGE=$(($CEILING-$FLOOR+1));
echo $# arguments

# throw the error if we do not receive expected parameters
if [ "$#" -ne 2 ]; then
   echo "expected only 2 parameters but given $#"
   exit
fi

# check for proper Input for number of files
if [  "$NoOfFiles" -eq 0  -o   "$NoOfFiles" -lt 0  ]; then
   echo "Invalid Input  number of files must be greater than 0"
   exit
fi


echo "Directory Name is: $DirectoryName"
echo "Number of files to be created is: $NoOfFiles"

#check if the directory is present or not, if not create the directory
if [ -d "$DirectoryName" ]; then
   echo "Directory Allready exists"
else
   mkdir $DirectoryName
fi


# remove all the files in the directory if present
rm -rf $DirectoryName/*

# iterate and create n files

for i in `seq $NoOfFiles`
do
  size=$RANDOM
  size=$(($size+$FLOOR));
  head -c $size  </dev/urandom > $DirectoryName/$i.txt
  md5sumoutput=`md5sum  $DirectoryName$i.txt`
  splitwords=($md5sumoutput)
  md5fileoutput="$DirectoryName$i.txt$colon$splitwords" 
  echo $md5fileoutput >> $DirectoryName/$md5file
done

