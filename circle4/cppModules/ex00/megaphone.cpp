/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:08:42 by donghank          #+#    #+#             */
/*   Updated: 2024/10/30 22:14:31 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main(int argc, char **argv)
{
	if (argc == 1)
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
	else
	{
		for (int i = 1; i < argc; i++) // argument value
		{
			std::string str(argv[i]); // declare the value
			for (int j = 0; argv[i][j]; j++) // element of the argument value
				std::cout << (char)std::toupper(str[j]);
		}
		std::cout << std::endl;
	}
	return (0);
}
