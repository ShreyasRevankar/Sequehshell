# main: main.c prompt.c hostname.c commands.c pwd.c cd.c echo.c
# 	gcc -g main.c prompt.c hostname.c commands.c pwd.c cd.c echo.c -o sequenshell

# main: main.c drivers/prompt.c drivers/hostname.c drivers/commands.c drivers/init.c drivers/background.c drivers/signals.c drivers/raw.c programs/pwd.c programs/cd.c programs/echo.c programs/ls.c programs/history.c programs/exit.c programs/pinfo.c programs/discover.c 
# 	gcc -g main.c drivers/prompt.c drivers/hostname.c drivers/commands.c drivers/init.c drivers/background.c drivers/signals.c drivers/raw.c programs/pwd.c programs/cd.c programs/echo.c programs/ls.c programs/history.c programs/exit.c programs/pinfo.c programs/discover.c -o sequenshell

main: main.c drivers/*.c programs/*.c
	gcc -g main.c drivers/*.c programs/*.c -o sequenshell