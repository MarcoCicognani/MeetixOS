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

#include "login.hpp"

#include "CandyShell.hpp"
#include "HeadlessScreen.hpp"
#include "screen.hpp"

#include <Api.h>
#include <fstream>
#include <io/keyboard.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Utils/Environment.hh>
#include <Utils/PropertyFileParser.hh>
#include <Utils/Utils.hh>

/**
 *	constructor
 */
Login::Login() {
    rootUser.clear();
    rootPassword.clear();

    user.clear();
    passwd.clear();

    createAmbient();
}

/**
 *	main of login
 */
void Login::LoginEntry() {
    Login* acc = new Login();

    s_task_register_id("Login");
    acc->loginLoop();
}

/**
 *	called when in text shell type "lock"
 */
void Login::lockLogin() {
    CandyShell* lockLogin = new CandyShell();

    while ( true ) {
        createAmbient();
        ShellInputStatus st;

        user = Utils::Environment::getLoggedUser();

        out->write(' ' + user, RGB(255, 255, 255));
        out->updateCursor();

        passwd.clear();

        out->write("\n PassWord: ", RGB(255, 255, 255));
        out->updateCursor();
        passwd = lockLogin->readInput(passwd, out, &st, 0, false);

        if ( researchCredential(user, passwd) )
            break;

        else {
            out->write("\n Wrong PassWord", RGB(255, 255, 255));
            out->updateCursor();
            s_sleep(750);
        }
    }

    delete lockLogin;
}

/**
 *	login loop
 */
void Login::loginLoop() {
    CandyShell* loginShell = new CandyShell();

    while ( true ) {
        createAmbient();
        ShellInputStatus status;

        user.clear();

        out->write(" UserName: ", RGB(255, 255, 255));
        out->updateCursor();
        user = loginShell->readInput(user, out, &status, 0);

        passwd.clear();

        out->write("\n PassWord: ", RGB(255, 255, 255));
        out->updateCursor();
        passwd = loginShell->readInput(passwd, out, &status, 0, false);

        if ( researchCredential(user, passwd) )
            break;

        else {
            out->write("\n Wrong UserName or PassWord", RGB(255, 0, 0));
            out->updateCursor();
            s_sleep(750);
        }
    }

    delete loginShell;
}

/**
 *	create ambient
 */
void Login::createAmbient() {
    out = new HeadlessScreen();

    out->clean();
    out->activate();

    loadLogo(logoPath);
}

/**
 *	load logo
 */
void Login::loadLogo(const char* path) {
    ifstream logoptfile(path);
    if ( logoptfile.good() ) {
        string loginlogo((istreambuf_iterator<char>(logoptfile)), istreambuf_iterator<char>());
        out->write(loginlogo, RGB(0, 200, 0));

        out->write("\n", RGB(255, 255, 255));
        logoptfile.close();
    }
}

/**
 *	research imitted credential
 */
bool Login::researchCredential(const string& username, const string& password) {
    // open password file
    ifstream cred("/usr/crd");

    // parse it
    Utils::PropertyFileParser  parser(cred);
    map<string, string> properties = parser.getProperties();

    // find
    for ( pair<string, string> entry : properties ) {
        if ( entry.first == "rootUsername" )
            rootUser = entry.second;
        else if ( entry.first == "rootPassword" )
            rootPassword = entry.second;
        else if ( entry.first == "userName" )
            user = entry.second;
        else if ( entry.first == "passWord" )
            passwd = entry.second;
    }

    // closing
    cred.close();

    // test
    if ( username == rootUser ) {
        if ( password == rootPassword ) {
            logged(username);
            return true;
        }

        else
            return false;
    }

    else if ( username == user ) {
        if ( password == passwd ) {
            logged(username);
            return true;
        }

        else
            return false;
    }

    else
        return false;
}

/**
 *	write environment variable
 */
void Login::logged(const string& loggedUser) {
    Utils::Environment::set("USER", loggedUser);
}
