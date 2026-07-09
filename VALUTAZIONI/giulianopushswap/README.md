*This project has been created as part of the 42 curriculum by caguiari and gipaglie.*

# Description

This project implements a sorting program inspired by the 42 `push_swap` challenge.  
The goal is to sort a list of integers using only a limited set of stack operations on two linked lists, `a` and `b`.

The program:
- parses and validates command-line arguments,
- detects duplicates and invalid inputs,
- assigns indexes to values,
- chooses a sorting strategy,
- prints the performed operations on `stdout`,
- can optionally print benchmark statistics on `stderr`.

# Instructions
- Build:
  - Run: make
- Usage:
  - Include the library headers in your program and call 
    ```Bash
    ./push_swap $(shuf -i 0-500 -n 100 | tr "\n" " ")

    ```
- The program supports custom flags:
  - --simple
  - --medium
  - --complex
  - --adaptive
  - --bench

# Algorithms
- The project uses multiple algorithms depending on input size and disorder level.
- For small lists (2 to 5 values) the program uses direct optimized cases:
  - swap
  - rotate
  - reverse rotate
  - push/pull between stacks
- O(n²) strategy: used when the list is almost sorted and the disorder rate is low.
  - push every element except the minimum to stack b,
  - rebuild sorted order from b,
  - finish by rotating a to the correct position.
- O(n√n) strategy: used for medium disorder.
  - index values,
  - split the list into chunks of size about √n,
  - move elements to b chunk by chunk,
  - bring the best candidate to the top with the shortest rotation path,
  - push them back in sorted order.
- O(n log n) strategy: used for highly disordered lists.
  - assign sorted indexes to values,
  - process bits of the indexes with radix sort,
  - move elements according to the current bit,
  - restore them back to a after each pass.
- Adaptive mode: computes the disorder ratio and chooses one of the already described algorithms.

# Resources
- References
  - 42 push_swap subject
  - Linked list data structures
  - Radix sort documentation
  - Insertion-style sorting strategies
  - Stack manipulation and algorithm optimization tutorials

# Contributions
- As both learners were everyday in the cluster, they decided to use the software development technique called "pair programming" in which two programmers work together at one workstation. One, the driver, writes code while the other, the observer or navigator, reviews each line of code as it is typed in. While reviewing, the observer also considers the "strategic" direction of the work, coming up with ideas for improvements and likely future problems to address. This is intended to free the driver to focus all of their attention on the "tactical" aspects of completing the current task, using the observer as a safety net and guide. 

## AI usage

- AI was used to:
  - help structure this README,
  - improve the wording of the project description,
  - organize the algorithm explanations,
  - summarize the sorting strategies and their justification.
- AI was not used to replace the implementation or the project logic.
