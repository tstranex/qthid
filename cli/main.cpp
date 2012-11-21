// Command-line interface for the FCD.

/***************************************************************************
 *  This file is part of Qthid.
 *
 *  Copyright (C) 2012  Timothy Stranex <tstranex@carpcomm.com>
 *
 *  Qthid is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Qthid is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Qthid.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "fcd.h"
#include "fcdhidcmd.h"
#include <iostream>
#include <string>
#include <cstdlib>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

static string modeName(FCD_MODE_ENUM mode) {
  switch(mode) {
  case FCD_MODE_NONE: return "no FCD detected";
  case FCD_MODE_BL: return "bootloader";
  case FCD_MODE_APP: return "application";
  }
  return "unknown";
}

static string fcdFirmwareVersion() {
  char fwVerStr[6];
  fcdGetFwVerStr(fwVerStr);
  return string(fwVerStr);
}

static int fcdGetFreqHz() {
  unsigned int freq_hz;
  fcdAppGetFreq(&freq_hz);
  return freq_hz;
}

static int printInfo()
{
  FCD_MODE_ENUM mode = fcdGetMode();
  if (mode == FCD_MODE_NONE) {
	cerr << "No FCD detected!" << endl;
	return 1;
  }

  cout << "Mode: " << modeName(mode) << endl;
  cout << "Firmware version: " << fcdFirmwareVersion() << endl;
  cout << "Frequency: " << fcdGetFreqHz() << " Hz" << endl;
  // TODO: Output the stage gains, etc.

  return 0;
}

static int setFreqHz(const char* freq_hz_str) {
  long long freq_hz = atoll(freq_hz_str);
  unsigned int actual_freq_hz;
  FCD_MODE_ENUM mode = fcdAppSetFreq(freq_hz, &actual_freq_hz);
  if (mode != FCD_MODE_APP) {
	cerr << "Error setting frequency." << endl;
	return 1;
  }
  return 0;
}

static void printHelp() {
  cout << "qthid-cli: Command-line FUNcube Dongle controller" << endl;
  cout << "Examples:" << endl;
  cout << "$ qthid-cli" << endl;
  cout << "    Prints FCD status info." << endl;
  cout << "$ qthid-cli --set_freq_hz 437505000" << endl;
  cout << "    Tunes to the given frequency." << endl;
}

int main(int argc, char* argv[])
{
  for (int i = 1; i < argc; i++) {
	string arg = argv[i];

	if (arg == "--help") {
	  printHelp();
	  return 0;

	} else if (arg == "--cli") {
	  // Ignore.

	} else if (arg == "--set_freq_hz") {
	  if (i + 1 < argc) {
		return setFreqHz(argv[i+1]);
	  } else {
		cerr << "Missing argument to --set_freq_hz." << endl;
		return 1;
	  }

	} else {
	  cerr << "Unrecognised option '" << arg << "'." << endl;
	  return 1;
	}
  }

  return printInfo();
}
