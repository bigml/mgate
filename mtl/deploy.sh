#!/bin/bash

PATH=/usr/local/bin:/usr/local/sbin:/bin:/usr/bin:/usr/sbin

SITE_NAME=miad
SITE_PATH=/usr/local/miad/xy/

useage()
{
   echo "useage: $0 -i ipsfile -n -c -b -x"
   echo "-i ipsfile: deplopy machines ip list, default is ip.list"
   echo "-n: for fresh system, will deploy system library"
   echo "-c: deploy config file"
   echo "-b: deploy binary file"
   echo "-x: restart binary"
   echo "example: $0 -bx"
   exit -1
}

IPFILE=ip.list
FRESH=0
CONFIG=0
BINARY=0
RESTART=0

FILE_CFG=${SITE_PATH}/config.hdf
FILE_CFG_MEVENT=${SITE_PATH}/xport/client.hdf

# process parameter
while getopts 'i:ncbx' OPT; do
    case $OPT in
        i)
            IPFILE="$OPTARG";;
        n)
            FRESH=1;;
        c)
            CONFIG=1;;
        b)
            BINARY=1;;
        x)
            RESTART=1;;
        ?)
            useage
    esac
done

for i in `cat $IPFILE`;
do
    echo "deploy to " $i "..."


    if [ $FRESH -eq 1 ]; then
        echo "system library ..."
        rsync -rl /usr/local/lib root@$i:/usr/local/
        ssh root@$i > /dev/null 2>&1 <<EOF
mkdir -p ${SITE_PATH}/tpl
mkdir -p ${SITE_PATH}/run
mkdir -p ${SITE_PATH}/fly
mkdir -p /etc/mevent
mkdir -p /var/www && cd /var/www && ln -s ${SITE_PATH} ${SITE_NAME}
if ! grep '/usr/local/lib' /etc/ld.so.conf > /dev/null 2>&1
then
    echo "/usr/local/lib" >> /etc/ld.so.conf
    ldconfig
fi
EOF
    fi

    if [ $CONFIG -eq 1 ]; then
        echo "config file ..."
        rsync ${FILE_CFG} root@$i:${FILE_CFG}
        rsync ${FILE_CFG_MEVENT} root@$i:/etc/mevent/client.hdf
    fi

    if [ $BINARY -eq 1 ]; then
        echo "binary ..."
        rsync -rl ${SITE_PATH}/tpl root@$i:${SITE_PATH}/
        rsync -rl ${SITE_PATH}/fly root@$i:${SITE_PATH}/
        rsync ${SITE_PATH}/run/viki root@$i:${SITE_PATH}/run/viki
    fi

    if [ $RESTART -eq 1 ]; then
        echo "restart ..."
        ssh root@$i > /dev/null 2>&1 <<EOF
killall viki
spawn-fcgi -F 1 -d /var/www/${SITE_NAME}/fly/run/ -f /var/www/${SITE_NAME}/fly/run/viki -s /var/run/viki.socket  -u www >> /tmp/vikistart 2>&1
EOF
    fi

    echo "done"
done
