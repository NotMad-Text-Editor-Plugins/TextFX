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

EXTERNC unsigned strchrstrans(TCHAR **dest, size_t *destsz, size_t *destlen, TCHAR **stopeol, const TCHAR *chrs, unsigned chrsl, const TCHAR *repls);
EXTERNC unsigned strchrstransA(CHAR **dest, size_t *destsz, size_t *destlen, CHAR **stopeol, const CHAR *chrs, unsigned chrsl, const CHAR *repls) ;

EXTERNC unsigned encodeURIcomponent(TCHAR **dest, size_t *destsz, size_t *destlen);
EXTERNC unsigned encodeURIcomponentA(CHAR **dest, size_t *destsz, size_t *destlen);

EXTERNC void convertall(char cmd, unsigned flags, const TCHAR *s1, const TCHAR *s2, const TCHAR *s3, const TCHAR *s4) ;

namespace Tests
{
	TEST_CLASS(ConvertTest)
	{

	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In ConvertTest Class Initialize>>>");
			::gScintillaMessageSender = &mockSendScintillaMessage;
			resetScintillaSimulator();
			
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

				int len=128;
				MultiByteToWideChar (CP_ACP, 0, txUCS2+3, strlen (txUCS2+3) + 1, bufferA, len) ;  

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

				wsprintf(bufferW,TEXT("%s"), txUCS2+3);

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
		TEST_METHOD(MEMCQSPNSTART_SIMPLE_AW_1)
		{
			Logger::WriteMessage("MEMCQSPNSTART AW ---");
			
			TCHAR bufferA[64]={0};
			{
				unsigned quick[256/(sizeof(unsigned)*8)]={0};

				const CHAR g_ENCODEURISTR1[] = ("&<>\"");

				memcqspnstartA(g_ENCODEURISTR1,strlen(g_ENCODEURISTR1),quick);

				wsprintf(bufferA,TEXT("%d %d %d %d %d %d %d %d"), quick[0], quick[1], quick[2], quick[3], quick[4], quick[5], quick[6], quick[7]);
				Logger::WriteMessage(bufferA);
			}
			TCHAR bufferW[64]={0};
			{
				unsigned quick[256/(sizeof(unsigned)*8)]={0};

				const TCHAR g_ENCODEURISTR[] = TEXT("&<>\"");

				memcqspnstart(g_ENCODEURISTR,lstrlen(g_ENCODEURISTR),quick);



				wsprintf(bufferW,TEXT("%d %d %d %d %d %d %d %d"), quick[0], quick[1], quick[2], quick[3], quick[4], quick[5], quick[6], quick[7]);
				Logger::WriteMessage(bufferW);
			}

			Assert::AreEqual(0, lstrcmp(bufferA, bufferW));

		}
		
		TEST_METHOD(EncodeHTMLTEST_AW)
		{
			Logger::WriteMessage("In Encode HTML TEST AW ==>");
			resetScintillaSimulator();
			const CHAR testTextNoChanges[] = ("AAA This is a <>&Encode test.;;;;;;");
			auto s1=TEXT("&<>\"");
			auto s2=TEXT(",&&amp;,<&lt;,>&gt;,\"&quot;,");
		
			TCHAR* buffer_simple_W = new TCHAR[1024]{0};
			{
				size_t allocatedTextBufferSize=1024;
				//MultiByteToWideChar(CP_ACP, 0, testTextNoChanges, strlen (testTextNoChanges) + 1, buffer_simple_W, allocatedTextBufferSize) ;  
				lstrcpy(buffer_simple_W, TEXT("WWW This is a <>&Encode test.;;;;;;"));
				size_t textBufferLength = strlen(testTextNoChanges);
				strchrstrans(&buffer_simple_W, &allocatedTextBufferSize, &textBufferLength, NULL, s1, lstrlen(s1), s2);
				Logger::WriteMessage(buffer_simple_W);
			}
			CHAR* buffer_simple_A = new CHAR[1024]{0};
			{
				auto s1=("&<>\"");
				auto s2=(",&&amp;,<&lt;,>&gt;,\"&quot;,");
				size_t allocatedTextBufferSize=1024;
				size_t textBufferLength = strlen(testTextNoChanges);
				strcpy(buffer_simple_A, ("AAA This is a <>&Encode test.;;;;;;"));
				strchrstransA(&buffer_simple_A, &allocatedTextBufferSize, &textBufferLength, NULL, s1, strlen(s1), s2);
				Logger::WriteMessage(buffer_simple_A);
			}

			Assert::AreEqual((int)strlen(buffer_simple_A), (int)lstrlen(buffer_simple_W));

			TCHAR* buffer_simul = new TCHAR[1024]{0};
			{
				setTextA(testTextNoChanges);
				mockSendScintillaMessage(0, SCI_SETSEL, 0, (LPARAM)strlen(testTextNoChanges));
				//size_t textBufferLength = mockSendScintillaMessage(0, SCI_GETSELTEXT, 0, (LPARAM)testTextNoChanges) - 1;

				//::pfencodeHTML();

				//convertall(CONVERTALL_CMD_strchrstrans, CAFLAG_DENYBLOCK|CAFLAG_CHARISNREADOFTCHAR, (TCHAR*)"&<>\"", (TCHAR*)",&&amp;,<&lt;,>&gt;,\"&quot;,", NULL, NULL);
				convertall(CONVERTALL_CMD_strchrstrans, CAFLAG_DENYBLOCK, s1, s2, NULL, NULL);

				int len=1024;
				MultiByteToWideChar(CP_ACP, 0, textBuffer, strlen (textBuffer) + 1, buffer_simul, len) ;
				
				Logger::WriteMessage(buffer_simul);
			}

			//::pfencodeURIcomponent();
		
			//Assert::AreEqual(testTextNoChanges, convertTextbuffer());

		}
		


		TEST_METHOD(Data_Test)
		{
			std::string dataX="happy";
			std::string dataX_SUB=dataX.substr(2);

			TCHAR buffer[100]={0};

			{
				wsprintf(buffer,TEXT("happy=%d=%d"), dataX.data(), dataX_SUB.data());
				Logger::WriteMessage(buffer);

				CHAR* Xdata = new CHAR[64];

				std::string dataXX=Xdata;

				wsprintf(buffer,TEXT("sad=%d=%d"), Xdata, dataXX.data());
				Logger::WriteMessage(buffer);

				wsprintf(buffer,TEXT("strspn #1 = %d"), strspn("http", "http://test/"));
				Logger::WriteMessage(buffer);

				wsprintf(buffer,TEXT("strspn #2 = %d"), strspn("http://tests/123", "http://tests/"));
				Logger::WriteMessage(buffer);

				wsprintf(buffer,TEXT("strspn #3 = %d"), strspn("http://tessss123", "http://test/"));
				Logger::WriteMessage(buffer);
			}

			Logger::WriteMessage("In Data_Test ==>");
			TCHAR* p1=new TCHAR[64]{0};
			p1[63]='\0';
			TCHAR* p2=p1+3;
			Assert::AreEqual(3, (int)(p2-p1));
			Assert::AreEqual(4, (int)(1+p2-p1));
			wsprintf(buffer,TEXT("%d=%d=%d"), lstrlen(p1),(int) sizeof(p1), sizeof(TCHAR));
			Logger::WriteMessage(buffer);
			//Assert::AreEqual();

			Logger::WriteMessage(p1);
			//memset(p1, 'a', sizeof(TCHAR)*63);
			auto data = TEXT("HAPPY");
			memcpy(p1, data, sizeof(TCHAR)*lstrlen(data));
			Logger::WriteMessage(p1);

			{
				CHAR printBuf[64];
				//CHAR* buffer = "123456789"; 
				CHAR* buffer = new CHAR[64]; strcpy(buffer, "123456789");
				sprintf(printBuf,("123 :: %d"), strcspn(buffer, "789"));
				Logger::WriteMessage(printBuf);

				buffer[2]='\0';
				sprintf(printBuf,("456 :: %d"), strcspn(buffer, "789"));
				Logger::WriteMessage(printBuf);
			}
		}

	};
}