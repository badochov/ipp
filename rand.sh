if [[ $# != 2 ]]
then
	echo "$0 usage <AMOUNT> <RANGE>" >&2
	exit 1
elif [[ $2<1 ]]
then
	echo "Range must be grater than 0" >&2
	exit 1 
fi

AMOUNT=$1
RANGE=$2

for ((i = 0; i < $AMOUNT; i++))
do
	echo $((`od -vAn -N4 -tu4 < /dev/random` % RANGE))	
done

