//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

#include "DxfParser.h"   // ѕарсер dxf
#include "DrawOrderer.h" //—ортировка по стыкам

#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

#define _USE_MATH_DEFINES
#include <cmath>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//ќбьв€им глобальные данные
std::vector<DxfPos> ToolDrawConvertDXF;  //—уда положу данные вз€тые из загруженного файла DXF
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

///////////////////////////////////  END DXF FILE  ///////////////////////////

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
	DxfParser parser;

	OpenDialog1->FileName = "";
	OpenDialog1->Execute();



	Memo1->Clear(); // ќчищаем memo перед выводом новых данных
	AnsiString ansiPath = OpenDialog1->FileName;
	parser.parse(ansiPath.c_str(), &ToolDrawConvertDXF);

    // ”пор€дочиваем элементы
	DrawOrderer orderer(ToolDrawConvertDXF);
	auto orderedPath = orderer.createOrderedPath();

		int entityCount = static_cast<int>(orderedPath.size());

		Memo1->Lines->Add("Parsed " + IntToStr(entityCount) + " entities:");
		Memo1->Lines->Add("--------------------------------------");
		for (int i = 0; i < orderedPath.size(); i++) {
			 AnsiString str = " Line = " + String(orderedPath[i].type);
			 str += "\r\n";
			 str += "X=" + String().FormatFloat("0.###",orderedPath[i].x1);
			 str += "  Y=" + String().FormatFloat("0.###",orderedPath[i].y1);
			 str += "  X2=" + String().FormatFloat("0.###",orderedPath[i].x2);
			 str += "  Y2=" + String().FormatFloat("0.###",orderedPath[i].y2);
			 str += "  I=" + String().FormatFloat("0.###",orderedPath[i].i);
			 str += "  J=" + String().FormatFloat("0.###",orderedPath[i].j);
             str += "\r\n";
			  Memo1->Lines->Add(str);
		}

}
//---------------------------------------------------------------------------
