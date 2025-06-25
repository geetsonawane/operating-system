#!/bin/bash

# Simulate getpid in the parent process
echo "Parent PID: $$"

# Start a background process to simulate the child
sleep 1 &

# Get the PID of the last background process (child)
child_pid=$!

# Simulate getpid in the child process
echo "Child PID:" $child_pid

# Wait for the background child process to finish
wait

#Output:-
#bhupesh-suryawanshi@bhupesh-suryawanshi:~/OS$ sh sample3.sh
#Parent PID: 6308
#Child PID: 6309
