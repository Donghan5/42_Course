#!/bin/bash
# Simple basic test semi automatic tester

GREEN='\033[0;32m'
RED='\033[0;31m'
BLANK='\033[0m'

# Check if exactly one parameter is provided
if [ $# -ne 1 ]; then
	echo "Please provide exactly one parameter."
	echo "Example: bash basic_test.sh 'cd ..'"
	exit 1
fi

# Assign the parameter to a variable
command="$1"

# Execute the command using bash
bash_result=$(echo "$command" | bash)
bash_exit=$?

# Execute the command using minishell
minishell_result=$(echo "$command" | ./minishell)
minishell_exit=$?

# Compare the results and display the output
if [ "$bash_result" == "$minishell_result" ]; then
	printf "$GREEN OK$BLANK\n"
else
	printf "$RED ERROR$BLANK\n"
fi

printf "=========bash_result=========\n$bash_result\n"
printf "=========minishell_result=========\n$minishell_result\n"
