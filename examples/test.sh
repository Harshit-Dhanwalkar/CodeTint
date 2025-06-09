#!/bin/bash
# test.sh

# Print a greeting
echo "Hello, Bash from CodeTint!"

# Define a variable
NAME="User"

# Use a conditional statement
if [ "$NAME" == "User" ]; then
  echo "Welcome, $NAME!"
else
  echo "Who are you?"
fi

# Loop from 1 to 3
for i in 1 2 3; do
  echo "Loop iteration: $i"
done

# Define a function
my_function() {
  echo "This is a function."
}

# Call the function
my_function
