/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:17:14 by donghank          #+#    #+#             */
/*   Updated: 2025/01/14 11:25:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MutantStack.hpp"

#define RED "\033[31m"
#define END "\033[0m"

int main()
{
	std::cout << RED << "========== Test start ==========" << END << std::endl;
	std::cout << RED << "[info]" << END << "The result of mstack and list must be same" << std::endl;
	{
		std::cout << RED << "========== MutantStack part ==========" << END << std::endl;
		MutantStack<int> mstack;
		mstack.push(5);
		mstack.push(17);
		std::cout << "top: " << mstack.top() << std::endl;
		mstack.pop();
		std::cout << "size:" << mstack.size() << std::endl;
		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		mstack.push(0);
		MutantStack<int>::iterator it = mstack.begin();
		MutantStack<int>::iterator ite = mstack.end();
		++it;
		--it;
		while (it != ite) {
			std::cout << *it << std::endl;
			++it;
		}
		std::stack<int> s(mstack);
	}
	
	{
		std::cout << RED << "========== list part ==========" << END << std::endl;
		std::list<int> list;
		list.push_back(5);
		list.push_back(17);
		std::cout << "top: " << list.back() << std::endl;
		list.pop_back();
		std::cout << "size: " << list.size() << std::endl;
		list.push_back(3);
		list.push_back(5);
		list.push_back(737);
		list.push_back(0);
		std::list<int>::iterator it = list.begin();
		std::list<int>::iterator ite = list.end();
		++it;
		--it;
		while (it != ite) {
			std::cout << *it << std::endl;
			++it;
		}
		std::list<int> l(list);
	}

	{
		std::cout << RED << "========== MutantStack const part ==========" << END << std::endl;
		MutantStack<int> mstack;
		mstack.push(5);
		mstack.push(17);
		std::cout << "top: " << mstack.top() << std::endl;
		mstack.pop();
		std::cout << "size: " << mstack.size() << std::endl;
		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		mstack.push(0);
		MutantStack<int>::const_iterator it = mstack.begin();
		MutantStack<int>::const_iterator ite = mstack.end();
		++it;
		--it;
		while (it != ite) {
			std::cout << *it << std::endl;
			++it;
		}
		std::stack<int> s(mstack);
	}

	{
		std::cout << RED << "========== MutantStack reverse part ==========" << END << std::endl;
		MutantStack<int> mstack;
		mstack.push(5);
		mstack.push(17);
		std::cout << "top: " <<mstack.top() << std::endl;
		mstack.pop();
		std::cout << "size: " <<mstack.size() << std::endl;
		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		mstack.push(0);
		MutantStack<int>::reverse_iterator it = mstack.rbegin();
		MutantStack<int>::reverse_iterator ite = mstack.rend();
		++it;
		--it;
		while (it != ite) {
			std::cout << *it << std::endl;
			++it;
		}
		std::stack<int> s(mstack);
	}

	{
		std::cout << RED << "========== MutantStack const reverse part ==========" << END << std::endl;
		MutantStack<int> mstack;
		mstack.push(5);
		mstack.push(17);
		std::cout << "top: " <<mstack.top() << std::endl;
		mstack.pop();
		std::cout << "size: " << mstack.size() << std::endl;
		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		mstack.push(0);
		MutantStack<int>::const_reverse_iterator it = mstack.rbegin();
		MutantStack<int>::const_reverse_iterator ite = mstack.rend();
		++it;
		--it;
		while (it != ite) {
			std::cout << *it << std::endl;
			++it;
		}
		std::stack<int> s(mstack);
		std::cout << RED << "=========  Test end ==========" << END << std::endl;
	}
	return 0;
}
