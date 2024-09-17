#!/bin/bash

# file command you can put in the command in this file
COMMANDS_FILE="command.txt"

# temp file to store the result
MINISHELL_OUTPUT="minishell_output.txt"
BASH_OUTPUT="bash_output.txt"

# init the result of the minishell and bash
> $MINISHELL_OUTPUT
> $BASH_OUTPUT

# excute
while IFS= read -r command; do
    echo "Running: $command"

    # minishell excute and store
    echo "$command" | ./minishell >> $MINISHELL_OUTPUT 2>&1

    # bash excute and store
    echo "$command" | bash >> $BASH_OUTPUT 2>&1
done < "$COMMANDS_FILE"

# compare the result
diff -u $MINISHELL_OUTPUT $BASH_OUTPUT

# if diff --> print the difference
if [ $? -eq 0 ]; then
	echo -e "\e[32mAll outputs match!\e[0m"
else
	echo -e "\e[31mOutputs differ!\e[0m"
fi
