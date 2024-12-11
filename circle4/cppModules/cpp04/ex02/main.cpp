/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:19 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:52:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongDog.hpp"
#include "WrongCat.hpp"
#include "Brain.hpp"

#define NUM_OF_ANIMAL 8

/*
	deep copy vs shallow copy

	deep copy --> Copies the value of the object **and all the associated data**.
		- Includes pointers and dynamically allocated memory.
		- Original object's address != Copied object's address (new memory is allocated for the copy).
		- The copied object is **independent** from the original.

	shallow copy --> Copies only the value of the object (not its associated data).
		- Original object's address == Copied object's address (both share the same memory).
		- The copied object is **dependent** on the original (e.g., they share pointers to the same data).

	Differences:
	1. Memory allocation:
		- Deep copy allocates new memory for the copied data.
		- Shallow copy does not allocate new memory; it simply shares the same memory as the original.

	2. Use case:
		- Use deep copy when objects must be independent of each other.
		- Use shallow copy when data sharing between objects is intentional or when no dynamic memory is involved.

	3. Safety:
		- Deep copy is safer for dynamic memory, as it avoids issues like dangling pointers or unintended side effects.
		- Shallow copy is faster but can lead to problems if not managed properly.
*/
// Aim of the ex02 --> know about of the abstract class which contain pure virutal function
int main()
{
	//Part of Animal
	Animal	*animal[NUM_OF_ANIMAL];
	Brain	*brain;

	std::cout << "\033[1;31mCreating Animals\033[0m" << std::endl;
	for (int i = 0; i < NUM_OF_ANIMAL; i++) {
		if (i % 2 == 0)
			animal[i] = new Dog();
		else
			animal[i] = new Cat();
		std::cout << std::endl; // print the new-line
	}
	std::cout << "Adding some ideas in animal[7]" << std::endl;
	brain = animal[7]->getBrain();
	brain->addIdeas("Hello I want to play with ball.");
	brain->addIdeas("Hungry");
	brain->addIdeas("Balllllll");
	brain->addIdeas("Zombiessssss");

	std::cout << "The first idea of animal[7]: " << brain->getIdeas(0) << std::endl;
	std::cout << "Assigning ideas animal[7] to animal[5]" << std::endl;
	*(animal[5]) = *(animal[7]);
	std::cout << "\nThe first idea of animal[5]: " << animal[5]->getBrain()->getIdeas(0) << std::endl;

	std::cout << "\n---- CHECK MEMORY ASSIGNEMENT ----" << std::endl;
	std::cout << "Brain pointer of animal[5]: " << animal[5]->getBrain() << std::endl;
	std::cout << "Brain pointer of animal[7]: " << animal[7]->getBrain() << std::endl;
	std::cout << "If Memory is different, the copy action is successfully done" << std::endl;
	std::cout << "------ END AND CONTINUE ------" << std::endl;

	std::cout << "\n-------- MODE CHECK----------" << std::endl;
	std::cout << "animal[5] idx 0: " << animal[5]->getBrain()->getIdeas(0) << std::endl;
	std::cout << "animal[5] idx 1: " << animal[5]->getBrain()->getIdeas(1) << std::endl;
	std::cout << "animal[5] idx 2: " << animal[5]->getBrain()->getIdeas(2) << std::endl;
	std::cout << "animal[5] idx 3: " << animal[5]->getBrain()->getIdeas(3) << std::endl;
	std::cout << "--------- End MODE CHECK ----------" << std::endl;

	std::cout << "\nAdd new ideas to animal[5] and animal[7] (doing deep copy) " << std::endl;
	std::cout << "\n--------- getBrain CHECK -----------" << std::endl;
	std::cout << "getBrain idx 0: " << animal[5]->getBrain()->getIdeas(0) << std::endl;
	std::cout << "getBrain idx 1: " << animal[5]->getBrain()->getIdeas(1) << std::endl;
	std::cout << "getBrain idx 2: " << animal[5]->getBrain()->getIdeas(2) << std::endl;
	std::cout << "getBrain idx 3: " << animal[5]->getBrain()->getIdeas(3) << std::endl;
	std::cout << "--------- END getBrain CHECK ----------" << std::endl;

	animal[5]->getBrain()->addIdeas("Last idea of the animal[5]");
	std::cout << "Last idea of the animal[5]: " << animal[5]->getBrain()->getIdeas(4) << std::endl;
	std::cout << "\n----------- MODE DEBUG ------------" << std::endl;
	std::cout << "Last idea of the animal[5] idx 0: " << animal[5]->getBrain()->getIdeas(0) << std::endl;
	std::cout << "Last idea of the animal[5] idx 1:" << animal[5]->getBrain()->getIdeas(1) << std::endl;
	std::cout << "Last idea of the animal[5] idx 2: " << animal[5]->getBrain()->getIdeas(2) << std::endl;
	std::cout << "Last idea of the animal[5] idx 3: " << animal[5]->getBrain()->getIdeas(3) << std::endl;
	std::cout << "Last idea of the animal[5] idx 4: " << animal[5]->getBrain()->getIdeas(4) << std::endl;
	std::cout << "--------- END MODE DEBUG ----------" << std::endl;

	animal[7]->getBrain()->addIdeas("Last idea of the animal[7]");
	std::cout << "\nLast idea of the animal[7]: " << animal[7]->getBrain()->getIdeas(4) << std::endl;

	std::cout << "\nDestructing animals" << std::endl;
	for (int i = 0; i < NUM_OF_ANIMAL; i++)
		delete animal[i];
	return 0;
}
