# Snake
Es el juego Snake, pero jugable desde una terminal Linux. Cuenta con una IA super básica en el archivo *inteligenciaSnake.h* que puedes activar presionando la tecla X cuando estés en el juego. Para mover a la serpiente, usa las teclas WASD.

![image](https://github.com/user-attachments/assets/e5f3e3d1-133a-4fb4-a2bf-ac0c6d7fc979)

## Compilación y ejecución
Si deseas compilar tú mismo el programa, ten en cuenta que se está haciendo uso de la librería *curses*. En Debian o Ubuntu puedes instalarla mediante el siguiente comando:
```sh
sudo apt-get install libncurses5-dev
```
Y para compilar el programa:
```sh
gcc snake.c -o snake -lncurses
```
En caso de utilizar Termux (Linux en Android), utiliza los siguientes comandos para compilarlo:
```sh
cc ./snake.c -o snake -lncurses
termux-elf-cleaner ./snake
```
