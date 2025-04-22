#pragma once

# include <stack>

/* 
std::map: 		Sorted associative container with unique keys.												used: x		suggestion: ex00: map{{date, value}}, find value using date and multiply by amount
std::vector:	Dynamic array with random access.															used: x		suggestion: ex01: Only need is to iterate an array
std::list: 		Doubly-linked list with efficient insertions and deletions.									used: x		suggestion: ex02: Insertions and deletion are key to do this exercice
std::deque: 	Double-ended queue with efficient insertions and deletions at both ends and random access	used: x		suggestion: ex02: Insertions and deletion are key to do this exercice


9 8 4 1 7 20 13 2 3

9,8  4,1  7,20  13,2			Divir parejas en listas
8,9  1,4  7,20  2,13			Ordenar parejas a1 < a2

{8,9 1,4} {7,20 2,13}			Dividir en parejas de parejas, lista con 2 listas
{1,4 8,9} {2,13 7,20}			Ordenar parejas de parejas segun a1 < b1

{{1,4 8,9} {2,13 7,20}}			Dividir en parejas de parejas de parejas, lista con 2 listas con 2 listas cada una
{{1,4 8,9} {2,13 7,20}}			Ordenar again
  b,a b,a   b,a  b,a

{1, 4, 9, 13, 20}				Crear nueva secuencia con el b1 y los a
{8, 2, 7, 3}					Crear nueva secuencia con los b restantes y el numero impar si hay

{1, 2, 3, 4, 7, 8, 9, 13, 20}	Ordenar meidante insercion binaria y otras cosas: https://dev.to/emuminov/human-explanation-and-step-by-step-visualisation-of-the-ford-johnson-algorithm-5g91
*/