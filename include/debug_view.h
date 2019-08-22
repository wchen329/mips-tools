//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEBUGVIEW_H__
#define __DEBUGVIEW_H__
#include <cstdio>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include "primitives.h"
#include "mt_exception.h"

/* Special debugging views
 * wchen329@wisc.edu
 */

namespace priscas
{
	class DebugView
	{
		public:
	
			enum DBGType
			{
				SIMPLE_TREE_LIST,
				SIMPLE_TABLE,
				TREE,
				TABLE
			};

			virtual DBGType getDBGType() = 0;
	};

	/* A tree node
	 * Has arbritrary amount of children
	 */
	class DebugTreeNode
	{
		public:
			// Hierarchy related funcs.
			int getLevel() { return this->level; }
			void addChild(std::shared_ptr<DebugTreeNode> c)
			{
				// Check: c must have no children and have level 0. Otherwise, throw an exception

				c->setLevel(this->level + 1);
				childList.push_back(c);
			}

			std::shared_ptr<DebugTreeNode> getParent();
			const std::vector<std::shared_ptr<DebugTreeNode>> Children() { return this->childList; }
			virtual HighLevelType getNType() { return T_NONE;}
			DebugTreeNode() : level(0) {}
		private:
			std::vector<std::shared_ptr<DebugTreeNode>> childList;
			int level; // level i.e. count of nodes to shortest path to root
			void setLevel(int newLevel) { this->level = level; }
	};

	/* A generic DebugTreeNode that can hold data members
	 * Of various key, pair combinations
	 *
	 * WARNING: using non primitive types to construct these classes (except std::string) is highly discourages
	 *          in fact, it will the data enclosed could be potentially useless then!
	 */
	template<class NClass, class VClass> class DebugTreeNodeD : public DebugTreeNode
	{
		public:

			HighLevelType getNType() { return getTypeGeneric<NClass>(); }

			HighLevelType getVType() { return getTypeGeneric<VClass>(); }

			bool hasData() { return true; }
			NClass getNameData() { return this-> nData; }
			VClass getValueData() { return this-> vData; }
			NClass setNameData(NClass n) { this->nData = n; }
			VClass setValueData(VClass v) { this->vData = v; }

		private:
			NClass nData;
			VClass vData;
	};

	/* A tree with each node having a 
	 * list of key-value pairs
	 *
	 */
	class DebugTree : public DebugView
	{
		public:
			DBGType getDBGType() { return DebugView::TREE; }
			std::shared_ptr<DebugTreeNode> getRootNode() { return this->root; }
			void setRootNode(std::shared_ptr<DebugTreeNode> n) { this->root = n; }
			DebugTree() : root(nullptr) { }

		private:
			std::shared_ptr<DebugTreeNode> root;
	};

	/* 2D_Point 64*2 bits, both use (0,0) as an absolute reference
	 * Used for table below.
	 */
	template <class TPDType> class TablePoint
	{
		private:
			unsigned long long x;
			unsigned long long y;
			TPDType data;
		public:
			unsigned long long getX() { return x;}
			unsigned long long getY() { return y;}
			void setData(const TPDType dataIn) { this->data = dataIn;}
			TPDType getData() { return data; }
			void setX(unsigned long long xin) { this->x = xin; }
			void setY(unsigned long long yin) { this->y = yin; }
			TablePoint() : x(0), y(0) {}
			TablePoint(unsigned long long x_in, unsigned long long y_in) : x(x_in), y(y_in) {}
			bool operator==(const TablePoint tp) { return (this->x == tp.x) && (this->y == tp.y); }
			TablePoint operator-(TablePoint& tp) { return TablePoint(this->x - tp.x, this->y - tp.y); }
			TablePoint operator+(TablePoint& tp) { return TablePoint(this->x + tp.x, this->y + tp.y); }
			void operator-=(TablePoint& tp) { this->x = this->x - tp.x; this->y = this->y - tp.y; }
			void operator+=(TablePoint& tp) { this->x = this->x + tp.x; this->y = this->y + tp.y; }
	};

	class DebugTable : public DebugView
	{
		DBGType getDBGType() { return DebugTable::TABLE; }
		virtual HighLevelType getDType() = 0;
	};

	/* A table; similar to a spreadsheet table, given X, Y coordinates on the table'
	 * in simUI,
	 * (0,0) is the upper-left most entry
	 * i.e.
	 * -------------------------
	 * | (0,0) | (1,0) | (2,0) |
	 * -------------------------
	 * | (0,1) | (1,1) | (2,1) |
	 * -------------------------
	 *
	 * -
	 * The typing of a debug table is completely uniform throughout.
	 * That is, the data indexed must be of a single type
	 * 
	 * -
	 * The specific DType MUST be listed under "HighLevelType" enum to be any useful.
	 */
	template <class DType> class DebugTableSingleType : public DebugView
	{
		public:
			HighLevelType getTableDataType() { return getTypeGeneric<DType>(); }
			DebugView::DBGType getDBGType() { return DebugView::TABLE; }

			void setData(unsigned long x, unsigned long y, DType data)
			{
				TablePoint<DType> pt(x,y);

				if(x >= limit || y >= limit)
				{
					return;
				}

				// New record definition, not checked for duplicates
				pt.setData(data);
				pts.push_back(pt);
				
				if(x > max_X)
				{
					max_X = x;
				}

				if(y > max_Y)
				{
					max_Y = y;
				}
			}
			
			unsigned long long getMaxDefX()
			{
				return this->max_X;
			}

			unsigned long long getMaxDefY()
			{
				return this->max_Y;
			}

			void registerPC(int sig, unsigned long pc)
			{
				if(this->sigToPC.count(sig) > 0 || sig >= limit)
					return;
				this->sigToPC.insert(std::pair<int, unsigned long>(sig, pc));
			}

			bool hasSig(int sig)
			{
				return this->sigToPC.count(sig) > 0 ? true : false;
			}

			unsigned long getPC(int sig)
			{
				if(this->sigToPC.count(sig) > 0)
				{
					return sigToPC[sig];
				}
				else
				{
					return 0; // todo throw exception
				}
			}

			const std::vector<TablePoint<DType>>& getDefinedPtList() { return this->pts;}
			DebugTableSingleType<DType>()
				: max_X(0), max_Y(0), limit(100)
			{
			}

		private:
			DType nullDType;
			std::vector<TablePoint<DType>> pts;
			std::map<int, size_t> sigToPC;
			unsigned long long max_X;
			unsigned long long max_Y;
			unsigned long long limit;
	};


}

#endif
