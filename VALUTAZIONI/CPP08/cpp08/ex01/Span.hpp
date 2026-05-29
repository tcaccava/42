/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcalabre <pcalabre@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:33:15 by pcalabre          #+#    #+#             */
/*   Updated: 2025/03/04 18:33:36 by pcalabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPAN_HPP
#define SPAN_HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

class Span {
  private:
    std::vector<int> _data;
    int _len;

    Span();

  public:
    Span(int n);
    Span(const Span &s);
    ~Span();
    Span &operator=(const Span &s);

    void addNumber(int n);
    unsigned int shortestSpan();
    unsigned int longestSpan();

    class OutOfRange : public std::exception {
        virtual const char *what() const throw() {
            return "Span: OutOfRange Exception: index out of contructor len";
        }
    };

    class SizeTooSmall : public std::exception {
        virtual const char *what() const throw() {
            return "Span: OutOfRange Exception: index out of range";
        }
    };
};

#endif
