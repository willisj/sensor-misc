if [ $# -eq 2 ]; then
    echo "Error: specify interface"
    exit 1
fi

set -x 
ifconfig $1 down
iwconfig $1 mode monitor
ifconfig $1 up
