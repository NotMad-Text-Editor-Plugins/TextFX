#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"

#include <windows.h>
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define EXTERNC extern "C"

namespace Tests
{
#define TEST_BUFFER_CHARS 8192
	EXTERNC char textBuffer[TEST_BUFFER_CHARS];
	EXTERNC unsigned anchorPosition;
	EXTERNC unsigned currentPosition;

	LRESULT mockSendScintillaMessage(BOOL which, UINT Msg, WPARAM wParam, LPARAM lParam);
	void setText(const TCHAR* textIn);
	void setTextA(const CHAR* textIn);
	TCHAR* convertTextbuffer(const char* input = NULL);
	void resetScintillaSimulator();
}



// convertall is here to do all the hard work getting text from and back to Scintilla which
// is big now but it all started with this simple goal:
// 1) Extract the selected text from Scintilla into a c-string buffer with spare space
// 2) Call a text transform on the buffer. The transform will halt if spare space runs out.
// 3) Put the buffer back into the document if the transform indicates that the user will want the result.
// 4) Reselect the entire text, or the failed text if a failure occured
// Rectangular selections are handled without any special programming in the transforms.
// Transforms do not need to handle restrictions. Transforms only work with a simple c-string.
#define CAFLAG_DENYBLOCK					    1 /* We typically deny rectangular selection if the tranform has no use for partial lines or transformed text would fit poorly in the rectangular area. */
#define CAFLAG_REQUIREBLOCK					    2 /* Setting both DENY and REQUIRE block would be a bad idea */
/* CAFLAG_EXTENDTOLINES is defined above */
#define CAFLAG_REQUIREMULTILINES			    8 /* Two or more lines must be selected (doesn't always calculate right) */
#define CAFLAG_GETCLIPBOARDTEXT				 0x10 /* Get and submit the clipboard but it is not required */
#define CAFLAG_REQUIRECLIPBOARDTEXT			 0x20 /* Get ... and the clipboard cannot be empty */
#define CAFLAG_REQUIRECLIPBOARDTEXT1CHAR	 0x40 /* Get ... and the clipboard must contain exactly 1 character */
#define CAFLAG_GETALLWHENNOSELECTION		 0x80 /* Get the entire document if the user hasn't selected anything */
//#define CAFLAG_GETCURLINEWHENNOSELECTION 0x100 /* Get the current line the user hasn't selected anything */
#define CAFLAG_DENYBINARY					0x200 /* User is not permitted to select any text with \0 in it */
#define CAFLAG_USEUNICODE					0x400 /* Use the UNICODE version of this function if in the right mode */
#define CAFLAG_UNICODENTONLY				0x800 /* Unicode is automatically blocked if not in NT */
#define CAFLAG_CHARISNREADOFTCHAR			0x1000 /* Unicode is automatically blocked if not in NT */
#define IsScintillaUnicode(currentEdit) (SENDMSGTOCED(currentEdit, SCI_GETCODEPAGE, 0, 0)==SC_CP_UTF8)

#define CONVERTALL_CMD_memlowercase			'1'
#define CONVERTALL_CMD_memuppercase			'R'
#define CONVERTALL_CMD_mempropercase		'P'
#define CONVERTALL_CMD_meminvertcase		'p'
#define CONVERTALL_CMD_memsentencecase		's'
#define CONVERTALL_CMD_memchrtran1			'c'
#define CONVERTALL_CMD_asciiEBCDIC			'e'
#define CONVERTALL_CMD_zapchar				'z'
#define CONVERTALL_CMD_addup				'+'
#define CONVERTALL_CMD_wordcount			'-'
#define CONVERTALL_CMD_memstrtran			'S'
#define CONVERTALL_CMD_strchrstrans			'Z'
#define CONVERTALL_CMD_prepostpendlines		'&'
#define CONVERTALL_CMD_lineup				','
#define CONVERTALL_CMD_space2tabs			'\t'
#define CONVERTALL_CMD_insertclipboardcolumn '|'
#define CONVERTALL_CMD_rewraptexttest		'Ww'
#define CONVERTALL_CMD_stripHTMLtags		'h'
#define CONVERTALL_CMD_indentlines			'i'
#define CONVERTALL_CMD_indentlinessurround	'I'
#define CONVERTALL_CMD_strqsortlines		'q'
#define CONVERTALL_CMD_killwhitenonqt		' '
#define CONVERTALL_CMD_findqtstrings		'"'
#define CONVERTALL_CMD_trimtrailingspace	't'
#define CONVERTALL_CMD_deleteblanklines		'd'
#define CONVERTALL_CMD_reindentcode			'A'
#define CONVERTALL_CMD_submitW3C			'3'
#define CONVERTALL_CMD_numberconvert		'n'
#define CONVERTALL_CMD_encodeURIcomponent	'H'
#define CONVERTALL_CMD_filldown				'f'
#define CONVERTALL_CMD_memchrtran			'C'
#define CONVERTALL_CMD_tohex				'X'
#define CONVERTALL_CMD_fromhex				'x'
#define CONVERTALL_CMD_insertlinenumbers	'l'
#define CONVERTALL_CMD_deletefirstword		'L'
#define CONVERTALL_CMD_extendblockspaces	'E'
#define CONVERTALL_CMD_cleanemailquoting	'Q'
#define CONVERTALL_CMD_uudecode				'u'
#define CONVERTALL_CMD_base64decode			'U'
#define CONVERTALL_CMD_hexbyterunstolittlendian '4'
#define CONVERTALL_CMD_littlendiantohexbyteruns	'5'
#define CONVERTALL_CMD_converteol			'O'
#define CONVERTALL_CMD_splitlinesatch		'~'