/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   README.md                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <mdecarli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:04:36 by mdecarli          #+#    #+#             */
/*   Updated: 2026/07/01 17:33:58 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

*This project has been created as part of the 42 curriculum by mdecarli*

## Description
Get Next Line is a function that returns one line at a time from a file descriptor.
This project teaches two key concepts: file descriptors (open, read, close) and static variables in C.

## Instructions
Compile with: make

To clean object files: make fclean

To compile with a custom buffer size:
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c

## Resources
- Man pages: open, read, close, malloc, free
- Claude.ai (claude.ai): used as a learning tool to understand new concepts 
  such as static variables, file descriptors, and buffer management. 
  No code was generated directly by AI.
  
## Algorithm
GNL reads the file BUFFER_SIZE bytes at a time using read(), accumulating 
the data in a static variable (storage) that persists between calls.

1. read_to_storage: reads from the file and appends to storage until a \n is found or the file ends
2. extract_line: extracts the first line from storage (up to and including \n)
3. clean_storage: saves everything after \n in storage for the next call
4. get_next_line: calls the three functions in order and returns the line