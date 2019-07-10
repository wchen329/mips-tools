#ifndef __DEBUGVIEW_H__
#define __DEBUGVIEW_H__
#include <cstdio>
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include "primitives.h"

/* Special debugging views
 * wchen329@wisc.edu
 */

namespace mips_tools
{
	class DebugView
	{
		public:
	
			enum DBGType
			{
				TREE,
				TABLE
			};

			virtual DBGType getDBGType() = 0;
			virtual operator std::string() const = 0;
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

			operator std::string() const
			{
				// todo
				return "";
			}


		private:
			std::shared_ptr<DebugTreeNode> root;
	};

	/* 2D_Point 64*2 bits, both use (0,0) as an absolute reference
	 * Used for table below.
	 */
	class TablePoint
	{
		public:
			unsigned long long x;
			unsigned long long y;

		TablePoint() : x(0), y(0) {}
		TablePoint(unsigned long long x_in, unsigned long long y_in) : x(x_in), y(y_in) {}
		bool operator==(TablePoint& tp) { return (this->x == tp.x) && (this->y == tp.y); }
		TablePoint operator-(TablePoint& tp) { return TablePoint(this->x - tp.x, this->y - tp.y); }
		TablePoint operator+(TablePoint& tp) { return TablePoint(this->x + tp.x, this->y + tp.y); }
		TablePoint operator-=(TablePoint& tp) { this->x = this->x - tp.x; this->y = this->y - tp.y; }
		TablePoint operator+=(TablePoint& tp) { this->x = this->x + tp.x; this->y = this->y + tp.y; }
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
	template <class DType> class DebugTableSingleType : public DebugType
	{
		public:
			HighLevelType getTableDataType() { return getTypeGeneric<DType>(); }
			DType& getData(unsigned long x, unsigned long y)
			{
				TablePoint pt(x,y);

				if(ptDataMapping.count(pt) > 0)
				{
					return ptDataMapping[pt];
				}

				else
				{
					return nullDType;
				}
			}

			void setData(unsigned long x, unsigned long y, DType data)
			{
				TablePoint pt(x,y);

				// Redefinition, just set it in to map only.
				if(ptDataMapping.count(pt) > 0)
				{
					ptDataMapping[pt] = data;
				}

				// New record definition
				else
				{
					pts.push_back(TablePoint(x,y));
					ptDataMapping[pt] = data;
				}
			}

			const std::vector& getDefinedPtList() { return this->pts;}

			operator std::string() const
			{
				return "";// todo
			}

		private:
			DType nullDType;
			std::vector<TablePoint> pts;
			std::map<TablePoint, DType> ptDataMapping;
	};


}

#endif