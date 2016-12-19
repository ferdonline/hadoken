/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*
*/


#include <iostream>

//
// Simple and stupid version tool for hadoken
//

std::string hadoken_help();

int main(){
	std::cout << "\n\t\t"<< "Hadoken: C++ Utility library, modular, header-only, in the STL style " 
	<< "\n\n" << "hadoken_version: " << HADOKEN_VERSION_MAJOR << "." << HADOKEN_VERSION_MINOR
	<< "\n" << "hadoken_features: " << "range spinlock geometry parallelism threefry abstract_random string_utils ublas format small_vector"
	<< "\n\n\n\n"
	<< hadoken_help() << "\n";
	return 0;
}






















std::string hadoken_help(){
	return "                                      :/-\n                                  `/ydddddo+`\n                             `:::/smNhhydmdmy`     ``               ``` ``\n                           `/+//:-:ohy+/hdmNd`   `/soo`       `---/:ooo/+++:-\n                          `o:oy+:--/+ho/+s::`.`. :hyys`     `.`--/:-`oo+syo::+-\n                          .o-h/:::-:/:-/+ssoo/:/ohs/s+.     .-+o/:.`.s//+/..../-\n                          .o-h/:://o/::--/s+/++smmmo+:`    :+o+/-.`-:hy:/:..``.o`\n                          .s:sy/oyhysoooo+/:---odd+-`  `-/+//:-.``.:odNs/-.````+`\n                          .o/:+yys/:/o/+yo++//+hNdso-`    -:/+:-..:/mm+./-.```-+`\n                          `/.::-.---`.::+/`::::/+h/sy-    `://+:-``./h++oo:--:+`\n                          `++---...```-/+o      `dyyh-       -:/+:-..++osss/+:`\n                        `:-/dy/:----.-//o:      `sysy`          .-.//:/-:/-.\n                       /os.//oddhyooydo:\n                      :s/+.-/++ommmNyo//:..\n                    `oo:-o:...:/mmhN+++.```-.-.`\n                   .y+:-`.:+++osNmhN+:---...``.//\n                  -y+-.````.-:ohNmmN+o/:-:-.-..:s\n               `+oho-````.:oso:hNdhNs+oss/--:.-++\n             `/++/+/.``.:os:   `hyod``:yy+-/-`:o`\n            .o::-./:..-+s:       ..`   :/:/:../s`\n        `.`:+:-..`-:+ss:               s+/-..-/:\n       .s:+y+-.``-/os:                 o/-.-+--.\n       .s::ys-.://+:                   .syoso`\n       +o--//:---:                     +o:/+`\n       s/--/:.----....```             +o-----:- `\n `..---oos+y//------------------------:+/++/++/ss`\n `..---------.........---------------------------.\n    ```````           `````````````````````````\n";
}

