#include "polynomial.h"
#include "string_convert.h"

NS_MUGGLE_BEGIN

void Polynomial::LoadFromString(const std::string& ref_str)
{
	// clear old data
	data_.MakeEmpty();

	// get the polynomial characters
	char *ptr_ch_begin = (char*)malloc(ref_str.size() + 1);
	memcpy(ptr_ch_begin, ref_str.c_str(), ref_str.size());
	ptr_ch_begin[ref_str.size()] = '\0';

	// parse
	char *p, *q;
	p = q = ptr_ch_begin;
	while (*p != '\0')
	{
		if (*p == ' ' || *p == '+')
		{
			*p++ = '\0';

			if (p - q > 1)
			{
				PolynomialNode node;
				bool parse_ret = ParseNode(q, &node);
				MASSERT_MSG(parse_ret, "Failed in parse string");
				if (parse_ret)
				{
					data_.Insert(node, [](const PolynomialNode& node1, const PolynomialNode& node2)->
						bool{ return node1.exponent > node2.exponent; });
				}
			}

			q = p;
		}
		else
		{
			++p;
		}
	}
	if (p - q > 1)
	{
		PolynomialNode node;
		bool parse_ret = ParseNode(q, &node);
		MASSERT_MSG(parse_ret, "Failed in parse string");
		if (parse_ret)
		{
			data_.Insert(node, Polynomial::DescendingOrder);
		}
	}

	MASSERT_MSG(data_.IsOrdered(Polynomial::DescendingOrder), "polynomial is not ordered");

	free(ptr_ch_begin);
}
void Polynomial::Print()
{
	for (auto node = data_.First(); node != nullptr; node = node->Next())
	{
		MLOG("%d^%d", node->Get()->coefficient, node->Get()->exponent);
		if (node->Next())
		{
			MLOG(" + ");
		}
	}
	MLOG("\n");
}

Polynomial Polynomial::operator+(Polynomial& poly)
{
	MASSERT_MSG(data_.IsOrdered(Polynomial::DescendingOrder), "polynomial is not ordered");
	MASSERT_MSG(poly.data_.IsOrdered(Polynomial::DescendingOrder), "polynomial is not ordered");

	Polynomial new_poly;
	TListNode<PolynomialNode> *p, *q;
	p = data_.First();
	q = poly.data_.First();

	TListNode<PolynomialNode>* prev = nullptr;
	while (p != nullptr && q != nullptr)
	{
		if (Polynomial::DescendingOrder(*p->Get(), *q->Get()))
		{
			if (prev)
			{
				prev = new_poly.data_.Add(*p->Get(), prev);
			}
			else
			{
				prev = new_poly.data_.Insert(*p->Get());
			}
			p = p->Next();
		}
		else
		{
			if ((*p->Get()).exponent == (*q->Get()).exponent)
			{
				PolynomialNode new_node = { (*p->Get()).coefficient + (*q->Get()).coefficient, (*p->Get()).exponent };
				if (new_node.coefficient != 0)
				{
					prev = new_poly.data_.Add(new_node, prev);
				}
				p = p->Next();
				q = q->Next();
			}
			else
			{
				if (prev)
				{
					prev = new_poly.data_.Add(*q->Get(), prev);
				}
				else
				{
					prev = new_poly.data_.Insert(*q->Get());
				}
				q = q->Next();
			}			
		}
	}

	if (p == nullptr)
	{
		for (; q; q = q->Next())
		{
			prev = new_poly.data_.Add(*q->Get(), prev);
		}
	}
	else
	{
		for (; p; p = p->Next())
		{
			prev = new_poly.data_.Add(*p->Get(), prev);
		}
	}

	return new_poly;
}
Polynomial Polynomial::operator*(Polynomial& poly)
{
	MASSERT_MSG(data_.IsOrdered(Polynomial::DescendingOrder), "polynomial is not ordered");
	MASSERT_MSG(poly.data_.IsOrdered(Polynomial::DescendingOrder), "polynomial is not ordered");

	Polynomial new_poly;

	// multiply
	for (auto node1 = data_.First(); node1 != nullptr; node1 = node1->Next())
	{
		for (auto node2 = poly.data_.First(); node2 != nullptr; node2 = node2->Next())
		{
			PolynomialNode new_node;
			new_node.coefficient = node1->Get()->coefficient * node2->Get()->coefficient;
			new_node.exponent = node1->Get()->exponent + node2->Get()->exponent;
			new_poly.data_.Insert(new_node, Polynomial::DescendingOrder);
		}
	}

	// merge similar item
	for (auto node = new_poly.data_.First(); node != nullptr; )
	{
		if (node->Next() && node->Get()->exponent == node->Next()->Get()->exponent)
		{
			node->Get()->coefficient += node->Next()->Get()->coefficient;
			data_.RemoveNext(node);
			continue;
		}
		else
		{
			node = node->Next();
		}
	}

	// clear node with zero coefficient
	PolynomialNode zero_data = { 0, 0 };
	while (data_.FindAndRemove(zero_data, Polynomial::CoefficientEqual))
	{}

	return new_poly;
}

bool Polynomial::ParseNode(char* ptr_ch, PolynomialNode* ptr_node)
{
	char *p = ptr_ch;
	while (*p != '\0')
	{
		if (*p == '^')
		{
			*p++ = '\0';

			return StringConvert_atoi(ptr_ch, &ptr_node->coefficient) && 
				StringConvert_atoi(p, &ptr_node->exponent);
		}
		else
		{
			++p;
		}
	}

	return true;
}

bool Polynomial::DescendingOrder(const PolynomialNode& node1, const PolynomialNode& node2)
{
	return node1.exponent > node2.exponent;
}
bool Polynomial::CoefficientEqual(const PolynomialNode& node1, const PolynomialNode& node2)
{
	return node1.coefficient == node2.coefficient;
}

NS_MUGGLE_END