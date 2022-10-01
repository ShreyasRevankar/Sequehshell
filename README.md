# Sequenshell

## Run instructions

To compile the shell run `make`

Execute using `./sequenshell`

## File distribution

1. main.c contains the driver code for the shell
2. headers.h contains all the needed global variables, macros and libraries
3. the drivers directory contains all the files which do the grunt work of the shell i.e. functions which the main function directly calls
4. the programs directory contains all the files for the execution of each command that is needed
5. .history is a hidden file maintained to save the history of the shell

## Assumptions

1. The maximum command length can be 1023 charecters long
3. When a background process is called its pid is printed after another prompt
4. When a background process finishes and its exit status is printed, we do not get a new prompt statement. However, the user may type the prompt there itself if they feel like it without any functionality change
5. symlinks have not been handled
6. For normal expected behaviour of pinfo it must be provided with pids of such processes which the user has permissions of
7. If no previous working directory is set, `cd -` will lead to going into the home directory
8. .history should not be a file used by the user
9. Deleting .history and then running the shell would delete previous histories