#pragma once
#include "Vectorset.h"

namespace BWAPI
{
	template<class _T>
	class Typeset : public Vectorset<_T>
	{
	public:
		// Constructors
		Typeset() : Vectorset() {};
		// copy ctor
		Typeset(const Typeset &other) : Vectorset(other) {};
		// type ctor
		Typeset(const _T &val) : Vectorset() { this->push_back(val); };

		// Operators (adding elements)
		Typeset operator +(const _T &other)
		{
			Typeset newset(*this);
			newset.insert(other);
			return newset;
		};
		Typeset &operator +=(const _T &other)
		{
			this->insert(other);
			return *this;
		};
		Typeset operator |(const _T &other)
		{
			Typeset newset(*this);
			newset.insert(other);
			return newset;
		};
		Typeset &operator |=(const _T &other)
		{
			this->insert(other);
			return *this;
		};


	};


	template<class _T>
	class Type
	{
	private:
		int _id;
	public:
		// Constructor
		Type(int id) : _id(id) {}
		
		// Types
		typedef Typeset<_T> set;

		// Operators
		operator int() const { return _id; }
		set operator +(const _T &other)
		{
			set rset(this->_id);
			rset.insert(other.getID());
			return rset;
		}
		set operator |(const _T &other)
		{
			set rset(this->_id);
			rset.insert(other.getID());
			return rset;
		}
		
		// Members
		int getID() const { return _id; }
	};
}
