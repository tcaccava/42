/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:32:03 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:27:18 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <string>

class Brain {
private:
    std::string ideas[100];
public:
    Brain();
    ~Brain();
    void setIdea(int index, const std::string &idea);
    std::string getIdea(int index) const;
};

#endif
