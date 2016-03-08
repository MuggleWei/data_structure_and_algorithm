#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include "base.h"
#include <string>
#include "TList.h"

NS_MUGGLE_BEGIN

struct PolynomialNode
{
	int coefficient;
	int exponent;

	bool operator==(const PolynomialNode& ref)
	{
		return coefficient == ref.coefficient && exponent == ref.exponent;
	}
};

class Polynomial
{
public:
	/*
	 *  parse string to polynomial, in exponent descending order
	 */
	void LoadFromString(const std::string& str_ref);
	/*
	 *  print this polynomial
	 */
	void Print();

	/*
	 *  to add/multiply two polynomial, polynomial in exponent descending order 
	 */
	Polynomial operator+(Polynomial& poly);
	Polynomial operator*(Polynomial& poly);

protected:
	/*
	 *  parse string to a polynomial node
	 */
	bool ParseNode(char* ptr_ch, PolynomialNode* ptr_node);

	static bool DescendingOrder(const PolynomialNode& node1, const PolynomialNode& node2);
	static bool CoefficientEqual(const PolynomialNode& node1, const PolynomialNode& node2);

protected:
	TList<PolynomialNode> data_;
};

NS_MUGGLE_END

#endif