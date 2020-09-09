
n=$1

if [ "$1" == "" ] ; then

    n=10000
fi


for (( i=1; i<=$n; i++ ))
do

    echo { starting test $i }

    outfile=$2
    
    if [ "$2" == "" ] ; then
    
        outfile="/dev/null"
    fi
    
    ./exec > $outfile
    
    echo { test $i finished }
    
done
