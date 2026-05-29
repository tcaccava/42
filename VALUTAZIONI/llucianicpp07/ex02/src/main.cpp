/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:09:27 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:09:27 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Array.hpp"


int main()
{
   
   
    Array<std::string>ar(5);
    int j = 0;
    std::string arrayy[5] = {"hi", "bye", "hello", "world", "hola"};

    //empty array test

    try
    {
        Array<size_t>emptyArray;
        if(emptyArray.size() == 0)
            std::cout << "Empty array was created" << std::endl;
        else
            std::cout <<  " Impossible created an empty array" << std::endl;

        //test out of range
        //std::cout << emptyArray[1] << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    try
    {
        for(size_t i = 0; i < 5; i++)
        {
            ar[i] = arrayy[j++];
        }

        for(size_t i = 0; i < 5; i++)
            std::cout << "value is: " << ar[i] << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }


    std::cout << "\nTest Out of Range" << std::endl;
    try
    {
        Array<std::string>arry(5);
        arry = ar;

        //direct access test
        std::cout << arry[8] << std::endl;

        /*for(size_t i = 0; i < 7; i++)
            std::cout << arry[i] << std::endl;*/
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}