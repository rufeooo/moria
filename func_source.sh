#!/bin/bash
ADDR=`nm moria | grep "\<$1\>" | cut -f 1 -d ' '`
[ -z "$ADDR" ] && ADDR=`nm moria | grep $1 | cut -f 1 -d ' '`
echo $ADDR
IFS=":" read -a INF <<< `addr2line -e moria 0x$ADDR`

echo ${INF[@]}
if [ ${#INF[@]} -gt 0 ]; then
  vim -R ${INF[0]} +${INF[1]}
fi
