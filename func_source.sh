#!/bin/bash
IFS=":" read -a INF <<< `~/spontaneous/bin/callgraph -a $1 < fullobj | addr2line -e ./moria`

echo ${INF[@]}
if [ ${#INF[@]} -gt 0 ]; then
  vim -R ${INF[0]} +${INF[1]}
fi
