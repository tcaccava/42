*This project has been created as part of the 42 curriculum by slorefic, kcorasan.*

## Description

Push_swap is a sorting algorithm project. The goal is to sort a stack of integers in ascending order using two stacks (a and b) and a limited set of operations, with the minimum number of operations possible.

The program receives integers as arguments, builds stack a, and prints the sequence of operations needed to sort it. It automatically selects the best algorithm based on how disordered the input is.

## Instructions

### Compilation
```bash
make
make clean
make fclean
make re
```

### Usage
```bash
./push_swap [numbers]
./push_swap --simple [numbers]
./push_swap --medium [numbers]
./push_swap --complex [numbers]
./push_swap --adaptive [numbers]
./push_swap --bench [numbers]
```

### Examples
```bash
./push_swap 3 2 1
./push_swap "3 2 1"
./push_swap --bench 5 4 3 2 1
ARG="4 67 3 87 23"; ./push_swap --adaptive $ARG | wc -l
```

### Error handling
The program prints `Error` on stderr for:
- Non-integer arguments
- Integers out of range
- Duplicate values

## Available Operations

| Operation | Description |
|-----------|-------------|
| sa | Swap first two elements of stack a |
| sb | Swap first two elements of stack b |
| ss | sa and sb at the same time |
| pa | Push top of stack b to stack a |
| pb | Push top of stack a to stack b |
| ra | Rotate stack a (first becomes last) |
| rb | Rotate stack b (first becomes last) |
| rr | ra and rb at the same time |
| rra | Reverse rotate stack a (last becomes first) |
| rrb | Reverse rotate stack b (last becomes first) |
| rrr | rra and rrb at the same time |

## Algorithms

### Simple Sort - O(n²)
Selection sort adaptation. Finds the minimum, rotates it to the top of stack a, pushes it to stack b. Repeats until a is empty, then pushes everything back. Used when disorder < 0.2.

### Medium Sort - O(n√n)
Chunk-based sorting. Divides numbers into groups and pushes them to stack b in chunks. Used when disorder is between 0.2 and 0.5.

### Complex Sort - O(n log n)
Radix sort adaptation. Sorts numbers bit by bit using both stacks. Used when disorder ≥ 0.5.

### Adaptive Sort
Measures the disorder of the initial stack and selects the most appropriate algorithm automatically:
- disorder < 0.2 → Simple O(n²)
- 0.2 ≤ disorder < 0.5 → Medium O(n√n)
- disorder ≥ 0.5 → Complex O(n log n)

## Algorithm Justification

### Why Simple Sort for low disorder (< 0.2)?
When the stack is almost sorted, most elements are already in the right position. Simple sort (selection sort adaptation) works well here because it only needs a few rotations to find the minimum and place it correctly. With few mistakes to fix, even an O(n²) algorithm is efficient.

### Why Adaptive Sort?
The adaptive strategy avoids wasting operations. Instead of always using the same algorithm regardless of the input, it measures the disorder first and picks the most appropriate strategy. This way, an almost-sorted input uses a simple and fast approach, while a completely random input uses a more powerful algorithm.

### Why these thresholds (0.2 and 0.5)?
- Below 0.2: less than 20% of pairs are out of order, so the input is close to sorted. Simple sort handles this efficiently.
- Between 0.2 and 0.5: moderate disorder, medium sort balances speed and simplicity.
- Above 0.5: more than half the pairs are out of order, requiring the most powerful algorithm.


## Performance Benchmark

| Input size | Minimum | Good | Excellent |
|------------|---------|------|-----------|
| 100 numbers | < 2000 ops | < 1500 ops | < 700 ops |
| 500 numbers | < 12000 ops | < 8000 ops | < 5500 ops |

## Resources

- [Push_swap visualizer](https://github.com/o-reo/push_swap_visualizer)
- [Sorting algorithms](https://en.wikipedia.org/wiki/Sorting_algorithm)
- [Linked lists in C](https://en.wikipedia.org/wiki/Linked_list)

### AI Usage
- Help understand how sorting algorithms work and why some are faster than others
- Explain concepts like linked lists, stack operations, and how disorder is measured
- Help structure and write this README
