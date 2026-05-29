*This project has been created as part of the 42 curriculum by fionni.*

# so_long

## Description

**so_long** is a small 2D game developed in C as part of the 42 curriculum.
The goal of the project is to familiarize students with basic graphics programming,
event handling, memory management, and parsing, using the **MiniLibX** graphical library.

In the game, the player moves on a 2D map viewed from above, collects all collectibles,
and reaches the exit using the minimum number of moves possible.
The map is provided as a `.ber` file and must respect strict formatting rules.

---

## Instructions

### Requirements
- Linux environment (42 workstations)
- `gcc`
- `make`
- X11 libraries (already available on 42 machines)

### Compilation

From the root of the repository, run:

```bash
make
```

This command will:
- compile `libft`
- compile `MiniLibX`
- build the `so_long` executable

### Execution

Run the program with a valid map as argument:

```bash
./so_long maps/level1.ber
```

### Controls
- `W` / `A` / `S` / `D` → move the player
- `ESC` or window close button → quit the game

The number of moves is displayed in the shell and updated at each movement.

---

## Resources

### Documentation & References
- MiniLibX documentation (42)
- X11 documentation
- GNU C documentation
- 42 `so_long` subject PDF

---

## Use of AI

AI tools were used during the development of this project to:
- clarify subject requirements
- review Makefile structure and build logic
- debug compilation and runtime errors
- reason about memory management and Valgrind output

All design decisions, implementation, and final code were written and validated by the author.
