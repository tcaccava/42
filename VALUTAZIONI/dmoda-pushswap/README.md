*This project has been created as part of the 42 curriculum by dmoda and grocca.*

# push_swap

## Description: 
The goal of this project is to make you discover algorithmic complexity in a very
concrete way. Sorting numbers is easy; sorting them fast with only two stacks and 
a handful of allowed moves is another story. Sorting a fully random list and 
sorting an almost sorted list are also two extremely different things.
Having been given two stacks - the first with random positive and/or negative integers
with a maximum capaciy of 500, and the second one completely empty - the goal is to
sort them in ascending order using the least amount of moves possible. 
The only possible moves are push (from stack a to stack b and viceversa), swap (the 
first two elements of stack a, of stack b, or both), reverse (in stack a, in stack b 
or both) and reverse rotate (in stack a, in stack b, or both).
To do so you have to implement three algorithms: a simple algorithm (O(n²)), a medium 
algorithm (O(n rad n)) and a complex algorithm (O(n log n)). At the end you will need 
to implement a custom algorithm that chooses from the three algorithms above.
Before doing anything is crucial to control if the stack is made from only numbers, if
there are no duplicates and if the disorder is higher than 00.00 (no disorder) with  a
maximum of 1. The output has to be the list of all the movements you've made to sort
the stack.

It follows a complete tree of functions with their usage explained.

> _1) Input reading and controlling_ -------------------------------------------------------------------------------

`ft_is_number`											| It checks if the given string has only numbers in it
				| `ft_is_overflow`						| > explained in the same section
`ft_no_duplicate`										| It checks if there are duplicates in the string
				|`ft_is_number`							| > explained in the same section
				|`ft_atoi`								| It transorms the string in an integer
`ft_check_flag`											| It ensure that there is a correct flag/flag combination
				|`ft_strcmp`							| It compares two strings
`ft_is_overflow`										| It checks if the string is made by integers
				|`ft_strlen`							| It gives you the lenght of a string
				|`ft_strcmp`							| > explained in the same section
`ft_disorder`											| It calculates the string disorder 

> _2) Creating the stack_ ------------------------------------------------------------------------------------------

`ft_create_list`										| It creates a doubled concatenated list
				|`ft_no_duplicate`						| > explained in the section before
				|`ft_is_number`							| > explained in the section before
				|`ft_add_node`							| It allows you to add a node in the list
				|`ft_new_node`							| It allows you to create a node
				|`ft_atoi`								| > explained in the section before
				|`ft_check_flag`						| > explained in the section before
`ft_new_head`											| It creates a pointer to the head of the list
`ft_free_list`											| It frees the list

> _3) Choosing the best algorithm_ --------------------------------------------------------------------------------

`ft_choose_algorithm`									| It chooses which algorithm to use for the best sorting
				|`ft_stack_len`							| It gives you the lenght of the stack
				|`ft_disorder`							| > explained in the first section
				|`ft_print_bench`						| It allows you to print details from your sorting
				|				|`ft_is_bench`          | It tells you if --bench is a used flag
                |               |`ft_strcmp`            | > explained in the first section
                |				|`ft_printf`            					| It prints "%d", "%%", "%f" (two decimals)
				|				|			|`ft_function`					| thanks to
				|				|			|			|`ft_putnbr_fd`		| these two
				|				|			|			| 			|`n_len`| It gives you the number lenght		
                | 				|`ft_check_flag`       						| > explained in the first section
                |				|`ft_bench_sp`          					| The number of push and swap operation
                |               			|`ft_printf`            		| > explained in the same section
                | 				|`ft_bench_r`   							| The number of rotate and reverse rotate op.
				|				|			|`ft_printf`					| > explained in the same section
				|`ft_strcmp`												| > explained in the first section
				|`ft_bubble`												| > explained in the next section
				|`ft_block`													| > explained in the next section
				|`ft_sqrt`													| > explained in the next section
				|`ft_quick`													| > explained in the next section

> _4) Sorting algorithms_ ----------------------------------------------------------------------------------------------

`ft_bubble`												| Simple algorithm (O(n²)): bubble sort
				|`ft_disorder`							| > explained in the first section
				|`ft_stack_len`							| > explained in the section before
				|`ft_push`								| It pushes a stack-a element in the b stack or viceversa
				|`ft_swap`								| It swaps the first two elements of stack a, b or both
`ft_block`												| Medium algorithm (O(n rad n)): block-based sort
				|`ft_stack_len`							| > explained in the section before
				|`ft_bubble_two`						| It executes a bubble sort on two stacks at the same time
				|				|`ft_disorder`			| > explained in the first section
				|				|`ft_max`				| It calculates the higher number in a couple
				|				|`ft_next_mv`			| It executes the next movements of bubble two
				|				|			|`ft_swap`	| > explained in the same section
				|				|			|`ft_rotate`| It rotates the first and last elements of stack a, b or both
				|				|`ft_rotate`			| > explained in the same section
				|`ft_push`								| > explained in the same section
				|`ft_sort` 								| It sorts the semi-sorted stack
				|				|`ft_push`				| > explained in the same section
				|				|`ft_condition`			| Simply a too long condition to fit in 25 lines
				|				|`ft_rotcalc`			| It shows if itś best to move clock-wise or anti-clock-wise
				|				|`ft_rotate`			| > explained in the same section
				|`ft_block`								| > explained in the same section
				|				|`ft_bubble_two`		| > explained in the same section
				|				|`ft_push`				| > explained in the same section
				|				|`ft_block`				| > explained in the same section
				|				|`ft_sort`				| > explained in the same section
`ft_quick`												| Complex algorithm (O(n log n)): quick-sort
				|`ft_swap`								| > explained in the same section
				|`pivot`								| It calculates the pivot 
				|`ft_divide`							| It moves the higher or lower from the pivot in the other stack
				|				|`ft_push`				| > explained in the same section
				|				|`ft_rotate`			| > explained in the same section
				|`ft_quick`								| It executes the quick sort on the stacks
				|				|`ft_swap`				| > explained in the same section
				|				|`ft_divide`			| > explained in the same section
				|				|`ft_quick`				| > explained in the same section
				|				|`ft_rev_quick`			| It executes the quick sort on reverse
								|			|`ft_push`				| > explained in the same section
								|			|`ft_swap`				| > explained in the same section
								|			|`pivot`				| > explained in the same section
								|			|`ft_rev_divide`		| It works like a reverse `ft_divide`
								|			|`ft_quick`				| > explained in the same section
								|			|`ft_rev_quick`			| > explained in the same section

> _5) Moves and output_ ----------------------------------------------------------------------------------------

`ft_push`												| > explained in the section before
				|`ft_add_node`							| > explained in the second section
				|`ft_bench_sp`							| > explained in the third section
				|				|`ft_printf`			| > explained in the third section
`ft_swap`												| > explained in the section before
				|`ft_swap`								| > explained in the section before
				|`ft_bench_sp`							| > explained in the third section
				|				|`ft_printf`			| > explained in the third section
`ft_rotate`												| > explained in the section before
				|`ft_bench_r`							| > explained in the third section
				|				|`ft_printf`			| > explained in the third section
`ft_print_bench`										| > explained in the third section

> 6)_Int_main_ ---------------------------------------------------------------------------------------------------

`ft_new_head`											| > explained in the second section
`ft_create_list`										| > explained in the second section
`ft_choose_algorithm`									| > explained in the third section
`ft_free_list`											| > explained in the second section
------------------------------------------------------------------------------------------------------------------

# Instructions:
For the compilation, clone the repository and run `make`
(warning: we do not have a githup profile, this is written only for readme requirements)

git clone https://github.com/<dmoda>/push_swap.git
		  https://github.com/<grocca>/push_swap.git

cd `push_swap`
make

Compile with gcc and -Wall -Wextra -Werror flags

----------------------------------------------------------------------------
Remember to include the "push_swap.h" header.

# Resources
For this project we've searched for help in github old 42 students profiles and 
to Claude AI, for better understanding the algorithmic logic.
In some cases we used Claude AI for making sure everything was correct, especially 
when the logic nor our peers couldn't assist us anymore.

# License
Feel free to use this library to better understand the project, but don't copy!
Or you'll never learn!!!

Best regards, dmoda and grocca <3
