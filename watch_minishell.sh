#!/bin/bash

tput civis  # Hide cursor for a cleaner display
trap "tput cnorm; exit" SIGINT SIGTERM  # Restore cursor on exit

while true; do
    pid=$(pgrep -x minishell)
    tput cup 0 0  # Move cursor to top-left
    if [[ -n "$pid" ]]; then
        lsof -p "$pid"
    else
        echo "minishell not running"
    fi
    sleep 1
done
