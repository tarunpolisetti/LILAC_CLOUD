#!/bin/sh

# directly input path
DIR=/home/spolisetti/LILAC/TEST/

# expected  MD5 output file
expectedfile=$DIR/expected.md5

# actual MD5 output file
outputfile=$DIR/out.md5

############################## TEST CASE 1 START (CREATE 10 files) ####################
#number of files 
numfiles=10
result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

result= `taskset -c 0-1 ./md5-compute $DIR`
echo " result of c is $result"

result=`diff $expectedfile $outputfile`

echo "result of the diff is $result"

if [ -z "$line" ] ; then
    echo "TEST CASE PASSED"
else
    echo "TEST CASE FAILED"    
fi
############################## TEST CASE 1 END ######################


############################## TEST CASE 2 START (CREATE 100 files) ####################
#number of files 
numfiles=100
result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

result= `taskset -c 0-1 ./md5-compute $DIR`
echo " result of c is $result"

result=`diff $expectedfile $outputfile`

echo "result of the diff is $result"

if [ -z "$result" ] ; then
    echo "TEST CASE PASSED"
else
    echo "TEST CASE FAILED"    
fi
############################## TEST CASE 2 END ######################

############################## TEST CASE 3 START (CREATE 500 files) ####################
#number of files 
numfiles=500
result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

result= `taskset -c 0-1 ./md5-compute $DIR`
echo " result of c is $result"

result=`diff $expectedfile $outputfile`

echo "result of the diff is $result"

if [ -z "$result" ] ; then
    echo "TEST CASE PASSED"
else
    echo "TEST CASE FAILED"    
fi
############################## TEST CASE 3 END ######################

############################## TEST CASE 4 START (CREATE 1000 files) ####################
#number of files 
numfiles=1000
result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

result= `taskset -c 0-1 ./md5-compute $DIR`
echo " result of c is $result"

result=`diff $expectedfile $outputfile`

echo "result of the diff is $result"

if [ -z "$result" ] ; then
    echo "TEST CASE PASSED"
else
    echo "TEST CASE FAILED"    
fi
############################## TEST CASE 4 END ######################

############################## TEST CASE 4 START ( Negative Case  0 files) ####################
#number of files 
numfiles=0
result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

if [ -z "$result" ] ; then
    echo "TEST CASE FAILED"
else
    echo "TEST CASE PASSED"    
fi
############################## TEST CASE 4 END ######################

############################## TEST CASE 5 START ( Negative Case  Giving Incorrect directory in C program) ####################
#number of files 
numfiles=10
DIR2=/home/spolisetti/LILAC/TEST2/

result=`sh md5-gen.sh $DIR $numfiles`
echo " result of shell is $result"

result= `taskset -c 0-1 ./md5-compute $DIR2`
echo " result of c is $result"

result=`diff $expectedfile $outputfile`

echo "result of the diff is $result"

# check for the absence of out.md5 file
if [ -f $outputfile ] ; then
    echo "TEST CASE FAILED"
else
    echo "TEST CASE PASSED"
fi
############################## TEST CASE 5 END ######################





