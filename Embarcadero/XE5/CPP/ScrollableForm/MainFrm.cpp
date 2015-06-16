//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CalcContentBoundsProc(TObject *Sender, TRectF &ContentBounds)
{
	if ((FNeedOffset) && (FKBBounds.Top > 0))
	{
		ContentBounds.Bottom = Max(ContentBounds.Bottom,
								2 * ClientHeight - FKBBounds.Top);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::RestorePosition()
{
	VertScrollBox1->ViewportPosition = PointF(VertScrollBox1->ViewportPosition.X, 0);
	MainLayout1->Align = TAlignLayout::alClient;
	VertScrollBox1->RealignContent();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UpdateKBBounds()
{
	FNeedOffset = false;
	if (Focused != NULL) {
		TControl * LFocused = dynamic_cast<TControl*>(Focused->GetObject());
		TRectF LFocusRect = LFocused->AbsoluteRect;
		LFocusRect.Offset(VertScrollBox1->ViewportPosition.X, VertScrollBox1->ViewportPosition.Y);
		if (LFocusRect.IntersectsWith(FKBBounds) && LFocusRect.Bottom > FKBBounds.Top) {
			FNeedOffset = true;
			MainLayout1->Align = TAlignLayout::alHorizontal;
			VertScrollBox1->RealignContent();
			Application->ProcessMessages();
			VertScrollBox1->ViewportPosition = PointF(VertScrollBox1->ViewportPosition.X,
				LFocusRect.Bottom - FKBBounds.Top);
		}
	}
	if (!FNeedOffset) {
		RestorePosition();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	VertScrollBox1->OnCalcContentBounds = CalcContentBoundsProc;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
	FKBBounds = TRectF(0, 0, 0, 0);
	FNeedOffset = false;
	RestorePosition();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
	FKBBounds = TRectF(Bounds);
	FKBBounds = TRectF(ScreenToClient(FKBBounds.TopLeft()),ScreenToClient(FKBBounds.BottomRight()));
	UpdateKBBounds();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormFocusChanged(TObject *Sender)
{
	UpdateKBBounds();
}
//---------------------------------------------------------------------------
