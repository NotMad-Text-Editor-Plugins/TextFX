#include "stdafx.h"
#include "CppUnitTest.h"

#include <windows.h>
#include <wchar.h>
#include <tchar.h>

#include "..\SRC\Scintilla.h"

#define EXTERNC extern "C"
#include "..\Src\NPPTextFX.h"
#include "scintilla_simu.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(ConvertTest)
	{

	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In ConvertTest Class Initialize>>>");
			
			/*
			27 functions:
			"C:Encode URI Component"),pfencodeURIcomponent
			"C:Encode HTML (&&<>")"),pfencodeHTML
			"C:Strip HTML tags table tabs"),pfstripHTMLtags
			"C:Strip HTML tags table nontabs"),pfstripHTMLnotabs
			"C:Submit to W3C HTML Validator"),pfsubmitHTML
			"C:Submit to W3C CSS Validator"),pfsubmitCSS
			"C:Convert text to code command("text=\\"value\\"");"),pfprepostpendlines
			"C:Convert Decimal Number to Binary"),pfdecimal2binary
			"C:Convert Decimal Number to Octal"),pfdecimal2octal
			"C:Convert Decimal Number to Hex"),pfdecimal2hex
			"C:Convert Hex Number to Decimal"),pfhex2decimal
			"C:Convert Octal Number to Decimal"),pfoctal2decimal
			"C:Convert Binary Number to Decimal"),pfbinary2decimal
			"C:Convert C-style Number to Decimal"),pfcnum2decimal
			"C:Convert text to Hex-16"),pftohex16
			"C:Convert text to Hex-32"),pftohex32
			"C:Convert text to Hex-64"),pftohex64
			"C:Convert text to Hex-128"),pftohex128
			"C:Convert hex byte runs into LE-WORDS"),pfhexbyterunstolittlendian2
			"C:Convert hex byte runs into LE-DWORDS"),pfhexbyterunstolittlendian4
			"C:Convert LE-words to hex byte runs"),pflittlendiantohexbyteruns
			"C:Convert Hex to text"),pffromhex
			"C:ROT13 Text"),pfrot13
			"C:Convert EBCDIC to ASCII"),pfEBCDIC2ascii
			"C:Convert ASCII to EBCDIC"),pfascii2EBCDIC
			"C:Convert KOI8_R to CP1251"),pfKOI8_Rtocp1251
			"C:Convert CP1251 to KOI8_R"),pfcp1251toKOI8_R
			*/
		}
		TEST_METHOD(EncodeHTMLTEST_SIMPLE_AW)
		{
			TCHAR bufferA[128];
			{
				Logger::WriteMessage("In Encode HTML TEST AW ---");

				size_t allocatedTextBufferSize = 512;

				char * txUCS2 = (char*)mallocsafe(allocatedTextBufferSize, _T("convertall-allocatedTextBufferSize"));
				strcpy(txUCS2, "AAA This is a test text.;;;;;;");

				size_t textBufferLength = strlen(txUCS2);

				encodeURIcomponentA(&txUCS2, &allocatedTextBufferSize, &textBufferLength);

				Logger::WriteMessage(txUCS2);

				wsprintf(bufferA,TEXT("s"), txUCS2+3);

				freesafe(txUCS2, 0);
			}
			TCHAR bufferW[128];
			{
				Logger::WriteMessage("In Encode HTML TEST W ---");

				size_t allocatedTextBufferSize = 512;

				TCHAR * txUCS2 = (TCHAR*)mallocsafe(allocatedTextBufferSize, _T("convertall-allocatedTextBufferSize"));
				lstrcpy(txUCS2, _T("WWW This is a test text.;;;;;;"));

				size_t textBufferLength = lstrlen(txUCS2);

				encodeURIcomponent(&txUCS2, &allocatedTextBufferSize, &textBufferLength);

				Logger::WriteMessage(txUCS2);

				wsprintf(bufferW,TEXT("s"), txUCS2+3);

				freesafe(txUCS2, 0);
			}
			Assert::AreEqual(0, lstrcmp(bufferA, bufferW));
		}
		TEST_METHOD(MEMSPN_SIMPLE_AW)
		{
			int Wres;
			{
				Logger::WriteMessage("In MEMSPN TEST W ---");

				size_t allocatedTextBufferSize = 512;

				TCHAR text[]=TEXT("MEMSPN--;;\r\n 123123");


				auto end = memcspn(text,text+lstrlen(text),L"\r\n",2);

				TCHAR buffer[100]={0};
				wsprintf(buffer,TEXT("WWW MEMSPN=%d"), Wres=end-text);

				//Logger::WriteMessage(buffer);
			}
			int Ares;
			{
				Logger::WriteMessage("In MEMSPN TEST A ---");

				size_t allocatedTextBufferSize = 512;

				CHAR text[]=("MEMSPN--;;\r\n 123123");

				auto end = memcspnA(text,text+strlen(text),"\r\n",2);

				TCHAR buffer[100]={0};
				wsprintf(buffer,TEXT("AAA MEMSPN=%d"), Ares=end-text);

				//Logger::WriteMessage(buffer);
			}

			Assert::AreEqual(Wres, Ares);
		}
		TEST_METHOD(MEMCQSPNSTART_SIMPLE_AW)
		{
			Logger::WriteMessage("MEMCQSPNSTART AW ---");
			
			TCHAR bufferA[64]={0};
			{
				unsigned quick[256/(sizeof(unsigned)*8)]={0};

				const CHAR g_ENCODEURISTR1[] = ("!'()*-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~");

				memcqspnstartA(g_ENCODEURISTR1,strlen(g_ENCODEURISTR1),quick);

				wsprintf(bufferA,TEXT("%d %d %d %d %d %d %d %d"), quick[0], quick[1], quick[2], quick[3], quick[4], quick[5], quick[6], quick[7]);
				Logger::WriteMessage(bufferA);
			}
			TCHAR bufferW[64]={0};
			{
				unsigned quick[256/(sizeof(unsigned)*8)]={0};

				const TCHAR g_ENCODEURISTR[] = TEXT("!'()*-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~");

				memcqspnstart(g_ENCODEURISTR,lstrlen(g_ENCODEURISTR),quick);

				wsprintf(bufferW,TEXT("%d %d %d %d %d %d %d %d"), quick[0], quick[1], quick[2], quick[3], quick[4], quick[5], quick[6], quick[7]);
				Logger::WriteMessage(bufferW);
			}

			Assert::AreEqual(0, lstrcmp(bufferA, bufferW));

		}
		
		//TEST_METHOD(EncodeHTMLTESTA)
		//{
		//	Logger::WriteMessage("In Encode HTML TEST A ==>");
		//	resetScintillaSimulator();
		//	const CHAR testTextNoChanges[] = ("This is a test text.;;;;;;");
		//	const TCHAR testTextWithQuotesIn[] = _T("This' is a test text.");
		//	const TCHAR testTextWithQuotesOut[] = _T("This\" is a test text.");
		//	currentPosition = 7;
		//
		//	// No-op test:
		//	setTextA(testTextNoChanges);
		//
		//	//mockSendScintillaMessage(0, SCI_SETSEL, 0, (LPARAM)strlen(testTextNoChanges));
		//
		//	//::pfencodeHTML();
		//
		//	::pfencodeURIcomponent();
		//
		//	Logger::WriteMessage(textBuffer);
		//
		//	//Assert::AreEqual(testTextNoChanges, convertTextbuffer());
		//}
		


	};
}