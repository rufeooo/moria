#!/bin/bash
ADDR=0x`nm moria | grep "\<$1\>" | cut -f 1 -d ' '`
echo $ADDR
IFS=":" read -a INF <<< `addr2line -e moria $ADDR`

echo ${INF[@]}
if [ ${#INF[@]} -gt 0 ]; then
  vim -R ${INF[0]} +${INF[1]}
fi
