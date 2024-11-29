/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:16:07 by donghank          #+#    #+#             */
/*   Updated: 2024/11/29 13:52:12 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

/*
	Concept: Verifying if a point lies inside a triangle.
			Using Cross Product
	Approach:
	- Divide the triangle (ABC) into three smaller triangles using the new point (P): ABP, ACP, and BCP.
	- Check if the total area of the original triangle (ABC) equals the sum of the areas of the three smaller triangles (ABP + ACP + BCP).
	- If the areas match, the point (P) lies inside the triangle.

	Example:
	- Triangle ABC:
		A(0, 0), B(5, 0), C(0, 5)
	- Point P:
		P(1, 1)

	Calculation:
	- Area of ABC: Total area of triangle with vertices A, B, C
	- Area of ABP: Triangle formed by A, B, and P
	- Area of ACP: Triangle formed by A, C, and P
	- Area of BCP: Triangle formed by B, C, and P
	- Verify: If (Area of ABP + Area of ACP + Area of BCP == Area of ABC), then P is inside the triangle.
	- resume : |x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2)| / 2
	Visual Representation:
		  C
		  |\
		  | \
		  |  \
		  |   \
		  |    \
		  A------B
		    (P)
*/

/*
	Implement ABS
	@param
		Fixed x: want to convert absolute number
	@return
		x: Fixed value converted to absolute number
*/
static Fixed	myAbs(Fixed x) {
	if (x < Fixed(0))
		return x * Fixed(-1);
	return (x);
}

/*
	to doing calculation Cross product
	@param
		a: first point of triangle
		b: second point of triangle
		c: third point of triangle
		area: area of the triangle
	@return
		area: area of the whole triangle
*/
static Fixed	verifyArea(Point const a, Point const b, Point const c) {
	float	area;

	area = ((a.getX().toFloat() * (b.getY().toFloat() - c.getY().toFloat())) \
			+ (b.getX().toFloat() * (c.getY().toFloat() - a.getY().toFloat())) \
			+ (c.getX().toFloat() * (a.getY().toFloat() - b.getY().toFloat()))) / 2;
	return (area);
}

/*
	note: have to add the 0 cases the areas
	calculation of bsp
	@param
		a: first point of triangle
		b: second point of triangle
		c: third point of triangle
		point: unknown point (possible outside or inside of triangle)
	@return
		success: area of triangle abc
		fail: not sams as area of triangle abc
*/
bool	bsp(Point const a, Point const b, Point const c, Point const point) {
	Fixed	abc = myAbs(verifyArea(a, b, c));
	Fixed	abp = myAbs(verifyArea(a, b, point));
	Fixed	acp = myAbs(verifyArea(a, c, point));
	Fixed	bcp = myAbs(verifyArea(b, c, point));

	return (abc == (abp + acp + bcp));
}
