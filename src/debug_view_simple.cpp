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
#include "debug_view_simple.h"

namespace mips_tools
{
	void DebugTreeNode_Simple::addChild(DebugTreeNode_Simple* dtns)
	{
		// Add it in the list
		children.push_back(dtns);

		// Then add it in the lookup
		this->nameLookup.insert(std::pair<std::string, DebugTreeNode_Simple*>(dtns->getName(), dtns));
	}

	DebugTreeNode_Simple* DebugTreeNode_Simple::findChild(const std::string& name)
	{
		std::string search_name(name);
		return this->nameLookup.at(search_name);
	}

	DebugTreeNode_Simple::~DebugTreeNode_Simple()
	{
		if(!this->children.empty())
		{
			for(std::list<DebugTreeNode_Simple*>::iterator ita = this->children.begin(); ita != this->children.end(); ita++)
			{
				delete *ita;
			}
		}
	}

	DebugTree_Simple* DebugTree_Simple_List::newTree(std::string rootName, std::string rootValue)
	{
		DebugTree_Simple* addTree = new DebugTree_Simple;
		addTree->rootNode().setName(rootName);
		addTree->rootNode().setValue(rootValue);
		this->trees.push_back(addTree);
		return addTree;
	}

	DebugTree_Simple_List::~DebugTree_Simple_List()
	{
		for(size_t ind = 0; ind < trees.size(); ind++)
		{
			delete trees[ind];
		}
	}
}