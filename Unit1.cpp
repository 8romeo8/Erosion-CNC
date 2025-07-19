//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

#include "DxfParser.h"   // Парсер dxf
#include "DrawOrderer.h" //Сортировка по стыкам
#include "GraphicsBuilder.h"

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

//Обьвяим глобальные данные
std::vector<DxfPos> ToolDrawConvertDXF;  //Суда положу данные взятые из загруженного файла DXF
DrawOrderer *orderer = nullptr;      //Класс выполняющий стыковку обьектов
GraphicsBuilder* builder = nullptr; // Данные отрисовки
std::vector<DrawOrderer::OrderStruct> orderedPath ;

bool isInitialized = false;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
	DxfParser parser;

	OpenDialog1->FileName = "";
	OpenDialog1->Execute();

	AnsiString ansiPath = OpenDialog1->FileName;
	parser.parse(ansiPath.c_str(), &ToolDrawConvertDXF);

	// Упорядочиваем элементы
	orderer = new DrawOrderer(ToolDrawConvertDXF);
	orderedPath = orderer->createOrderedPath();

	// Удаляем старый builder, если был
	delete builder;
    builder = new GraphicsBuilder(orderedPath);
	isInitialized = false; // Сбрасываем флаг инициализации

  	ToolPathDraw->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToolPathDrawPaint(TObject *Sender)
{

	TRect drawArea = ToolPathDraw->ClientRect;
	TCanvas* canvas = ToolPathDraw->Canvas;

  if (builder != nullptr) {
		// Первичная настройка (вызывается один раз)
		if (!isInitialized) {
			builder->fitToView(drawArea);
			isInitialized = true;
		}

		builder->paint(canvas, drawArea);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        delete builder;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolPathDrawMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (builder != nullptr) {
             builder->mouseDown(X, Y);
			 if (Button == mbLeft) {
				// Поиск элементов под курсором
				auto elements = builder->findElementsAt(X, Y);

				if (!elements.empty()) {
					// Выделяем первый найденный элемент
					builder->clearSelection();
					builder->selectElement(elements[0]);
					ToolPathDraw->Invalidate(); // Перерисовываем
				}
			}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolPathDrawMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if (builder != nullptr) {
			builder->mouseMove(X, Y);
			ToolPathDraw->Invalidate(); // Перерисовка
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolPathDrawMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (builder != nullptr) {
		builder->mouseUp();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled)
{
	if (builder != nullptr) {
		builder->mouseWheel(WheelDelta, MousePos.x, MousePos.y);
		ToolPathDraw->Invalidate();
		Handled = true;
    }
}
//---------------------------------------------------------------------------

