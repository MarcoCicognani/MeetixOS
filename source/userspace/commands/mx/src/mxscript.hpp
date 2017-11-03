/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
* This program is free software; you can redistribute it and/or                       *
* modify it under the terms of the GNU General Public License                         *
* as published by the Free Software Foundation; either version 2                      *
* of the License, or (char *argumentat your option) any later version.                *
*                                                                                     *
* This program is distributed in the hope that it will be useful,                     *
* but WITHout ANY WARRANTY; without even the implied warranty of                      *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
* GNU General Public License for more details.                                        *
*                                                                                     *
* You should have received a copy of the GNU General Public License                   *
* along with this program; if not, write to the Free Software                         *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef _MX_SCRIPT_
#define _MX_SCRIPT_

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

/**
 *	value and key for each line
 */
struct LsPair
{
	std::string key;
	std::string value;
};

/**
 *	line
 */
struct LsStatement
{
	std::vector<LsPair*> pairs;
};

/**
 *	all document
 */
struct LsDocument
{
	std::vector<LsStatement*> statements;
};

/**
 *	custom script parser for script
 */
class MXScriptParser
{
private:
	FILE *file;
	char c;

public:
	MXScriptParser(FILE *fileToParse);

	void step();

	LsDocument *document();
	LsStatement *statement();
	LsPair *pair();
	std::string key();
	std::string value();
};

#endif
