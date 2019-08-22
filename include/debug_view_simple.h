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

#ifndef __DEBUG_VIEW_SIMPLE_H__
#define __DEBUG_VIEW_SIMPLE_H__
#include <map>
#include <list>
#include "debug_view.h"
#include "priscas_global.h"

namespace priscas
{

	/* Simple DebugTreeNode
	 * A tree node that has a name and a string value
	 */
	class DebugTreeNode_Simple
	{
		public:
			const std::string& getName() { return this->name; }
			const std::string& getValue() { return this->value; }
			void setName(const std::string& inName) { this->name = inName; }
			void setValue(const std::string& inValue) { this->value = inValue; }
			
			// Child related functions
			void addChild(DebugTreeNode_Simple * dtns); // Child is managed by the tree node itself
			DebugTreeNode_Simple* findChild(const std::string& name);
			std::list<DebugTreeNode_Simple*> getAllChildren() { return this->children; }

			DebugTreeNode_Simple() {}

			DebugTreeNode_Simple(const std::string& inName, const std::string& inValue) :
				name(inName),
				value(inValue)
			{}

			~DebugTreeNode_Simple();

		private:
			DebugTreeNode_Simple(DebugTreeNode_Simple &);
			DebugTreeNode_Simple operator=(DebugTreeNode_Simple &);
			std::string name;
			std::string value;
			std::map<std::string, DebugTreeNode_Simple*> nameLookup;
			std::list<DebugTreeNode_Simple*> children;
	};

	/* Simple DebugTree
	 * -
	 * A very simple tree the has one root node and an arbritrary amount of children
	 * Each node has a string name and string value
	 */
	class DebugTree_Simple
	{
		public:
			DebugTreeNode_Simple& rootNode() { return root; }
			DebugTreeNode_Simple* findChild(std::string name) { return root.findChild(name); } // Find a NODE given a name
			void addChild(std::string name, std::string value) { root.addChild(new DebugTreeNode_Simple(name, value)); }	// Add a new child node to this tree.
		private:
			DebugTreeNode_Simple root;
	};

	/* Simple Debug Tree List
	 * Holds several debug trees.
	 *
	 */
	class DebugTree_Simple_List : public DebugView
	{
		public:
			std::string listName;
			const std::vector<DebugTree_Simple*>& get_DebugTrees(){ return trees; }
			DebugTree_Simple* newTree(std::string rootName, std::string rootValue);
			DBGType getDBGType() { return DebugView::SIMPLE_TREE_LIST; }
			void setName(const std::string& name) { this->listName = name; }
			const std::string& getName() { return this->listName; }
			
			~DebugTree_Simple_List();
		
		private:
			std::vector<DebugTree_Simple*> trees;
	};

	typedef priscas::DebugTableSingleType<std::string> DebugTableStringValue;
	typedef priscas::TablePoint<std::string> TablePointStringV;
}

#endif