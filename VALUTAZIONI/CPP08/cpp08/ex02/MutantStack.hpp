/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcalabre <pcalabre@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:33:46 by pcalabre          #+#    #+#             */
/*   Updated: 2025/03/04 18:33:48 by pcalabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

#include <deque>
#include <iostream>
#include <stack>
#include <vector>

template <typename T> class MutantStack : public std::stack<T> {
  public:
    typedef typename std::stack<T>::container_type::iterator iterator;

    MutantStack() {}
    MutantStack(const MutantStack &m) { *this = m; }
    ~MutantStack() { this->c.clear(); }

    MutantStack &operator=(const MutantStack &m) {
        if (this == &m)
            return *this;
        this->c = m.c;
        return *this;
    }

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
};

#endif
