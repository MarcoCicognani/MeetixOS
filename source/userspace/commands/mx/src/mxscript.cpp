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

#include "mxscript.hpp"
#include <sstream>

using namespace std;

/**
 *
 */
MXScriptParser::MXScriptParser(FILE *fileToParse)
{
	file = fileToParse;
	step();
}

/**
 *
 */
void MXScriptParser::step()
{
	c = fgetc(file);
}

/**
 *
 */
LsDocument *MXScriptParser::document()
{
	LsDocument *doc = new LsDocument();

	while (true)
	{
		LsStatement *stat = statement();

		if (stat == 0) break;

		doc->statements.push_back(stat);
	}

	return doc;
}

/**
 *
 */
LsStatement *MXScriptParser::statement()
{

	LsStatement *stat = new LsStatement();

	// skip line end & spaces
	while (c == '\n' || isspace(c) || c == '#')
	{
		if (c == '#')
		{
			while (c != '\n')
			{
				step();
			}
			step();
		}

		else step();
	}

	// read pairs
	while (true)
	{
		LsPair *p = pair();

		if (p == 0) break;

		stat->pairs.push_back(p);
	}

	// no pairs? script finished
	if (stat->pairs.size() == 0)
	{
		delete stat;
		return 0;
	}

	return stat;
}

/**
 *
 */
LsPair *MXScriptParser::pair()
{

	LsPair *p = new LsPair();

	// key
	string k = key();
	if (k.empty())
	{
		delete p;
		return 0;
	}

	// ':'
	if (c == ':') step();

	// value
	string v = value();
	if (v.empty())
	{
		delete p;
		return 0;
	}

	p->key = k;
	p->value = v;

	return p;
}

/**
 *
 */
string MXScriptParser::key()
{
	while (c != '\n' && isspace(c)) step();

	stringstream ks;
	while (c != ':' && c != '\n' && c != EOF)
	{
		ks << c;
		step();
	}
	return ks.str();
}

/**
 *
 */
string MXScriptParser::value()
{
	while (c != '\n' && isspace(c)) step();

	bool instr = false;
	bool esc = false;

	stringstream ks;
	while (c != EOF)
	{
		if (c == '"' && !esc)
		{
			instr = !instr;
			esc = false;
		}

		else if (c == '\\' && !esc) esc = true;

		else if (isspace(c) && !instr && !esc) break;

		else
		{
			ks << c;
			esc = false;
		}
		step();
	}
	return ks.str();
}
