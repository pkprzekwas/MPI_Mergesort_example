if [ $# -ne 2 ]; then
    echo "You need to specify process count as the first argument"
    echo "and count of list elements as the second."
    exit 1
fi

PROCCOUNT=$1
LISTCOUNT=$2

mpicc main.c -o out \
    && mpirun -np $PROCCOUNT out $LISTCOUNT \
    && rm out
