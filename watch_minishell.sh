#!/bin/bash

while true; do
    pid=$(pgrep -x minishell)
    if [[ -n "$pid" ]]; then
        lsof -p "$pid"
    else
        echo "minishell not running"
    fi
    sleep 1
    clear
done
