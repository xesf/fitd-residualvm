/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 
 <TODO: Add in GPLv2-notice, need to make sure if we are v2-only, or v2-or-later,
 we are atleast v2>
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FITD_FITD_H
#define FITD_FITD_H

#include <cstdio>
#include <cstdlib>
#include "cvars.h"
#include "common.h"

namespace Fitd {
	
enum gameType {
	GType_AITD1 = 1,
	GType_JACK = 2,
	GType_AITD2 = 3,
	GType_AITD3 = 4,
	GType_TIMEGATE = 5,
};

int fileExists(char* name);
void allocTextes(void);
void sysInitSub1(char* var0, char* var1);
	
class FitdEngine {
	gameType _gameType;
	enumCVars* _currentCVarTable;
	int _numCVars;
public:
	void sysInit(void)
	{
		int i;
		
#ifndef PCLIKE
		//  unsigned long int ltime;
#else
		//  time_t ltime;
#endif
		FILE* fHandle;
		
		setupScreen();
		//setupInterrupt();
		//setupInterrupt2();
		//setupInterrupt3();
		
		//setupVideoMode();
		
		// time( &ltime );
		
		srand(time(NULL));
		
		if(!initMusicDriver())
		{
			musicConfigured = 0;
			musicEnabled = 0;
		}
		
		// TODO: reverse sound init code
		
		
		aux = (char*)malloc(65068);
		if(!aux)
		{
			theEnd(1,"Aux");
		}
		
		aux2 = (char*)malloc(64000);
		if(!aux2)
		{
			theEnd(1,"Aux2");
		}
		
		sysInitSub1(aux2,screen);
		/*  sysInitSub2(aux2);
		 sysInitSub3(aux2); */
		
		bufferAnim = (char*)malloc(4960);
		if(!bufferAnim)
		{
			theEnd(1,"BufferAnim");
		}
		
		CVars = (short int*)malloc(getNumCVars() * sizeof(short int));
		
		switch(getGameType())
		{
			case GType_JACK:
			case GType_AITD2:
			case GType_AITD3:
			case GType_TIMEGATE:
			{
				fontData = loadPakSafe("ITD_RESS",1);
				break;
			}
			case GType_AITD1:
			{
				fontData = loadPakSafe("ITD_RESS",5);
				break;
			}
		}
		
		initFont(fontData, 14);
		
		if(getGameType() == GType_AITD1)
		{
			initFont2(2,0);
		}
		else
		{
			initFont2(2,1);
		}
		
		switch(getGameType())
		{
			case GType_JACK:
			case GType_AITD2:
			case GType_AITD3:
			{
				aitdBoxGfx = loadPakSafe("ITD_RESS",0);
				break;
			}
			case GType_AITD1:
			{
				aitdBoxGfx = loadPakSafe("ITD_RESS",4);
				break;
			}
		}
		
		priority = loadFromItd("PRIORITY.ITD");
		
		fHandle = fopen("DEFINES.ITD","rb");
		if(!fHandle)
		{
			theEnd(0,"DEFINES.ITD");
		}
		
		///////////////////////////////////////////////
		{
			fread(CVars,getNumCVars(),2,fHandle);
			fclose(fHandle);
			
			for(i=0;i<getNumCVars();i++)
			{
				CVars[i] = ((CVars[i]&0xFF)<<8) | ((CVars[i]&0xFF00)>>8);
			}
		}
		//////////////////////////////////////////////
		
		allocTextes();
		
		//  if(musicConfigured)
		{
			listMus = HQR_InitRessource("ListMus",110000,40);
		}
		
		listSamp = HQR_InitRessource("ListSamp",64000,30);
		
		hqrUnk = HQR_Init(10000,50);
	}
	void detectGame(void) {
		if(fileExists("LISTBOD2.PAK")) {
			_gameType = GType_AITD1;
			_numCVars = 45;
			_currentCVarTable = AITD1KnownCVars;
			
			printf("Detected Alone in the Dark 1\n");
			return;
		}
		if(fileExists("PERE.PAK")) {
			_gameType = GType_JACK;
			_numCVars = 70;
			_currentCVarTable = AITD2KnownCVars;
			
			printf("Detected Jack in the Dark\n");
			return;
		}
		if(fileExists("MER.PAK")) {
			_gameType = GType_AITD2;
			_numCVars = 70;
			_currentCVarTable = AITD2KnownCVars;
			
			printf("Detected Alone in the Dark 2\n");
			return;
		}
		if(fileExists("AN1.PAK")) {
			_gameType = GType_AITD3;
			_numCVars = 70;
			_currentCVarTable = AITD2KnownCVars;
			
			printf("Detected Alone in the Dark 3\n");
			return;
		}
		if(fileExists("PURSUIT.PAK")) {
			_gameType = GType_TIMEGATE;
			_numCVars = 70; // TODO: figure this
			_currentCVarTable = AITD2KnownCVars; // TODO: figure this
			
			printf("Detected Time Gate\n");
			return;
		}
		
		printf("FATAL: Game detection failed...\n");
		exit(1);
	}
public:
	FitdEngine() {
		detectGame();
	}
	int getGameType() {
		return _gameType;
	}
	int getNumCVars() {
		return _numCVars;
	}
};

extern FitdEngine *g_fitd;

} // end of namespace Fitd

#endif
