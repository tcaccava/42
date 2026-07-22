*This project has been created as part of the 42 curriculum by mde-matt, dfelicia.*

┌──────────────────────────────────────────────────────┐
|							                           |
|       push_swap         		    		           |
|							                           |
└──────────────────────────────────────────────────────┘

┌─────────────────────────────┐
│ Description                 │
└─────────────────────────────┘

# What is push_swap?

    "push_swap" is an algorithmic project from the 42 curriculum that must be
completed by exactly two students working together.
The goal is to write a C program that sorts a list of integers using as few
operations as possible, by manipulating two stacks with a very limited set of
instructions.

┌─────────────────────────────┐
│ Structure and subdivision   │
└─────────────────────────────┘

# How to basic

┌───┐ ┌───┐     *Explaination:*
│ 4 │ │ x │     *You have two stacks, a and b.*
│ 2 │ │ x │     *At the start 'a' holds a random set of integers passed as arguments*
│ 1 │ │ x │     *(positive and/or negative, no duplicates), while b is empty.*
│ 3 │ │ x │     *The goal is to sort a in ascending order*
└───┘ └───┘     *by moving values between the two stacks.*
  a     b

The only allowed operations are:
    ◦ **swap** *— sa, sb, ss;*
    ◦ **push** *— pa, pb;*
    ◦ **rotate** *— ra, rb, rr;*
    ◦ **reverse rotate** *— rra, rrb, rrr.*
    
The program must print the resulting sequence of operations to stdout,
one per line, aiming for the smallest possible number of moves.

# Mandatory parts

It requires implementing four distinct sorting strategies, selectable at runtime:
    ◦ **Simple** *– O(n²) (e.g., an adapted insertion, selection, or bubble sort)*
    ◦ **Medium** *– O(n√n) (e.g., chunk- or bucket-based sorting)*
    ◦ **Complex** *– O(n log n) (e.g., radix sort, or a stack-adapted merge/quick sort)*
    ◦ **Adaptive** *– a custom strategy that picks its method based*
    *on a "disorder" metric, respecting fixed complexity thresholds depending*
    *on how disordered the input is.*

The program must support flags such as --simple, --medium, --complex, and --adaptive,
along with a --bench mode that prints statistics to stderr
(disorder level, strategy used, and operation counts by type).

# Structure

**main functions**
├── main.c
│   ├── flagcheck               -> ft_strncmp, ft_printf
│   ├── solverpick
│   ├── printstack
│   └── main                    -> ft_memset
│
├── ft_parser.c
│   ├── dupchecker              -> ft_atoi
│   ├── overcheck               -> ft_strlen, ft_strncmp
│   ├── numchecker
│   └── stack_parser            -> ft_printf, ft_atoi, ft_lstadd_back
│
└── benchmark.c
    ├── disordermeter
    ├── strategyused            -> ft_strlen
    ├── totalops
    ├── opsprinter
    └── benchmaker
    
*main functions*
    Manages the program startup: reads the command-line arguments, interprets
    any strategy flags (--simple/--medium/--complex/--adaptive),
    parses the numbers into a linked list, and verifies their validity
    (no duplicates, no overflow, only digits).
    If the stack is already sorted, it terminates immediately;
    otherwise, it dispatches the workload to the correct solver (solverpick)
    or to hard-coded resolutions for 2–3 elements.
    Finally, it prints the sorted stack.
    
    benchmark.c provides supporting metrics: disordermeter measures how "disordered"
    the stack is (used by adaptivesolver to choose the strategy), while strategyused
    logs to stderr which algorithm was selected.
    
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
**operators**
├── operators.c
│   ├── sa / sb                 -> ft_printf
│   ├── ss                      -> ft_printf
│   ├── pb                      -> ft_lstadd_front, ft_printf
│   └── pa                      -> ft_lstadd_front, ft_printf
│
├── add_operators.c
│   ├── ra / rb                 -> ft_lstadd_back, ft_printf
│   ├── rr                      -> ft_printf
│   └── rra / rrb               -> ft_lstadd_front, ft_printf
│
└── last_operators.c
    ├── rrr                     -> ft_printf
    ├── twostack
    ├── threestack
    ├── sortchecker
    └── find_min
    
*operators*
    Contains all the movement required by the project:
    *- swaps (sa, sb, ss);*
    *- pushes between stacks (pa, pb);*
    *- shift-up rotations (ra, rb, rr);*
    *- reverse shift-down rotations (rra, rrb, rrr).*
    Each function prints its own name via ft_printf and updates the t_ops counter.
    
    last_operators.c includes:
    - twostack/threestack, two hard-coded resolutions for 2- or 3-element stacks;
    - sortchecker (which checks if the stack is already sorted);
    - find_min (which finds the position of the minimum element).

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

**simplesolver**
└── simplesolver.c
    ├── minraiser
    └── simplesolver
    
*simplesolver*
    The simplest sorting algorithm in the project, based on selection-sort
    ($O(n^2)$), designed for small stacks.
    minraiser brings the minimum element to the top of the stack by choosing
    the cheapest rotation (ra or rra depending on its position), while
    simplesolver repeats this process for each element, pushing it onto stack_b,
    and finally pushes everything back orderly onto stack_a.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

**mediumsolver**
├── mediumsolver.c
│   ├── stackfree
│   ├── stackdup                -> ft_lstadd_back
│   ├── groupfinder
│   └── mediumsolver            -> ft_lstsize
│
├── mediumsolver_utils.c
│   ├── groupdivider
│   ├── find_max
│   ├── min_value
│   ├── max_value
│   └── groupassigner           -> ft_lstsize
│
└── mediumsolver_add_utils.c
    ├── count_bigger
    ├── chunk_pb
    └── group_population

*mediumsolver*
    A bucket/chunk-based sorting algorithm (roughly $O(n\sqrt{n})$),
    used for intermediate-sized stacks. mediumsolver.c orchestrates the process:
    it duplicates the original stack (stackdup), divides the values into chunks,
    and for each chunk, it brings the belonging elements to the top (groupfinder)
    and pushes them onto stack_b.
    
    mediumsolver_utils.c provides statistical functions
    (number of chunks, min/max, chunk assignment for a value).
    
    mediumsolver_add_utils.c handles the ordered insertion into stack_b using
    targeted rotations (chunk_pb, count_bigger), avoiding the need to traverse
    the entire stack.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

**complexsolver**
└── complexsolver.c
    └── complexsolver           -> ft_lstsize

*complexsolver*
    A base-2 radix sort algorithm ($O(n \log n)$), designed for large stacks.
    It normalizes the values relative to the minimum, then, for each bit
    (from least significant to most significant), it distributes the elements
    between stack_a and stack_b based on the value of that bit, repiling
    everything at each pass.
    It is the only solver in the group that does not use external auxiliary
    functions; the entire logic is self-contained within a single function.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

**adaptivesolver**
└── adaptivesolver.c
    └── adaptivesolver          -> ft_lstsize

*adaptivesolver*
    A meta-algorithm that dynamically chooses which solver to use based on the
    size of the stack and a disorder index (disordermeter, from benchmark.c).
    It uses simplesolver for small stacks and complexsolver for large stacks,
    while for intermediate cases, it decides between simplesolver, mediumsolver
    and complexsolver depending on how "shuffled" the stack is.
    **It is the default solver if no command-line flag is specified.**
    
# Benchmark
	
**benchmark.c**
	├── disordermeter
    ├── strategyused            -> ft_strlen
    ├── totalops
    ├── opsprinter
    └── benchmaker
    
*benchmark*
	This function implements a benchmarking module for a push_swap-style sorting project.
disordermeter measures how "unsorted" the stack is by comparing every pair of
elements and calculating a disorder percentage (scaled to 10000 for two decimal
places of precision). strategyused prints a human-readable label describing which
sorting algorithm was chosen based on a check code (simple, medium, complex,
adaptive, or already sorted). totalops sums every operation counter
(sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr) and writes the total operation
count to stderr. opsprinter writes each individual operation count out for
detailed inspection. Finally, benchmaker ties everything together,
printing the disorder percentage, the chosen strategy, the total operations,
and the per-operation breakdown, all prefixed with *[bench]* for easy filtering
in the terminal output.
	
# Bonus
		
**checker_bonus.c**

*checker_bonus*
	This code implements a "checker" companion program for push_swap:
stackfree releases stack nodes to prevent memory leaks, while mimic parses a
single instruction line (checking longer names like "rrr" before shorter ones
like "rr" to avoid mismatches) and dispatches it to the matching stack operation.
Main builds the initial stack_a from the arguments, then reads instructions
line-by-line via get_next_line until EOF, applying each one through mimic.
Once all operations are processed, it checks whether stack_a is sorted and stack_b
is empty, printing "OK" or "KO" accordingly.
		
┌─────────────────────────────┐
│ Instructions                │
└─────────────────────────────┘

# Instructions

    This project expects the standard structure with libft/ and printf/
    as subdirectories (included via ft_push_swap.h).
Once built the program via Makefile, you can run it adding flags and the integers 
you want to stack.
    **If no command-line flag is specified,**
    **the program start automatically with the adaptivesolver.**
    
    Examples:
./push_swap --simple 3 1 2 5 4
./push_swap --medium 3 1 2 5 4
./push_swap --complex 3 1 2 5 4
./push_swap --adaptive 3 1 2 5 4

# Benchmark

	For benchmark you can add "--bench" as a flag, and type "2>/tmp/bench.txt"
	as an argument, and then type "cat /tmp/bench.txt" for the output.
	
	Example:
./push_swap --bench --complex 3 1 2 5 4 2>/tmp/bench.txt
cat /tmp/bench.txt

# Bonus

	For bonus purposes you can add " | ./checker" followed by the same numbers
	given to the original program.
	
	**note: for the bonus files you need to compile them typing "make bonus".**
	
	Example:
./push_swap --adaptive 3 1 2 5 4 | ./checker 3 1 2 5 4

┌─────────────────────────────┐
│ Resources                   │
└─────────────────────────────┘

# Resources

 	for this project, we used this sites along the peer learning and the ai:
 	
 	cppreference.com
 	- used this site's examples for the bit manipulation used into the complexsolver
 		(https://en.cppreference.com/c/numeric/bit_manip)
 	esadecimale.it
 	 
 	
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
# AI Usage
 
 	In this project, we used Claude ai by Anthropic for double-checking, logic
 	explainations and bug-fixing assistance. 
 		Since this project was already started with another peer, dfelicia used
 	Claude also to learn e figure it out how the whole project was structured.
 	He asked to Claude to describe three different output given and the program's
 	flow chart to see the entire logic behind the code.
 	*It was also used to help us writing this README.*
