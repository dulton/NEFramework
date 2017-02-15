// WndShadow.h : header file
//
// Version 0.1
//
// Copyright (c) 2006 Perry Zhu, All Rights Reserved.
//
// mailto:perry@live.com
//
//
// This source file may be redistributed unmodified by any means PROVIDING 
// it is NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the author's name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright (c) 2006 Perry Zhu" must be included in the "Startup 
// Banner", "About Box" or "Printed Documentation". This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
//****************************************************************************

/********************************************************************
	created:	2015/01/09
	filename: 	UIShadow.h
	author:		Redrain
	
	purpose:	DuiLib��Ӱ�࣬��ԭWndShadow��Ļ����ϣ�������ͨ��PNGͼƬ������Ӱ�Ĺ��ܣ����ҰѴ�����DuiLib�ں�
*********************************************************************/

#ifndef __UISHADOW_H__
#define __UISHADOW_H__

#pragma once

#include <map>

namespace ui
{

class UILIB_API ShadowUI
{
public:
	friend class PaintManager;

	ShadowUI(void);
	virtual ~ShadowUI(void);

public:
	// bShowΪ��ʱ�Żᴴ����Ӱ
	void ShowShadow(bool bShow);	
	bool IsShowShadow() const;

	void DisableShadow(bool bDisable);
	bool IsDisableShadow() const;

	// �㷨��Ӱ�ĺ���
	bool SetSize(int NewSize = 0);
	bool SetSharpness(unsigned int NewSharpness = 5);
	bool SetDarkness(unsigned int NewDarkness = 200);
	bool SetPosition(int NewXOffset = 5, int NewYOffset = 5);
	bool SetColor(COLORREF NewColor = 0);

	// ͼƬ��Ӱ�ĺ���
	bool SetImage(std::wstring szImage);
	bool SetShadowCorner(RECT rcCorner);	// �Ź���ʽ������Ӱ
	
	// ���Լ�����Ӱ��ʽ���Ƶ��������
	bool CopyShadow(ShadowUI* pShadow);

	//	������Ӱ���壬��CPaintManagerUI�Զ�����,�����Լ�Ҫ����������Ӱ
	void Create(PaintManager* pPaintManager);
protected:

	//	��ʼ����ע����Ӱ��
	static bool Initialize(HINSTANCE hInstance);

	// �����Ѿ����ӵĴ������������������Ӱ��,������ParentProc()������ͨ������õ���Ӱ��
	static std::map<HWND, ShadowUI *>& GetShadowMap();

	//	���໯������
	static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ������ı��С���ƶ������������ػ���Ӱʱ����
	void Update(HWND hParent);

	// ͨ���㷨������Ӱ
	void MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent);

	// ����alphaԤ��ֵ
	inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
	{
		return (GetRValue(cl) * (DWORD)nAlpha / 255) |
			(GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
			(GetBValue(cl) * (DWORD)nAlpha / 255) << 16 ;
	}

protected:
	enum ShadowStatus
	{
		SS_ENABLED = 1,				// Shadow is enabled, if not, the following one is always false
		SS_VISABLE = 1 << 1,		// Shadow window is visible
		SS_PARENTVISIBLE = 1<< 2	// Parent window is visible, if not, the above one is always false
	};

	
	static bool s_bHasInit;

	PaintManager	*pm_;		// �������CPaintManagerUI��������ȡ�ز���Դ�͸�������
	HWND			 wnd_;			// ��Ӱ����ľ��
	LONG_PTR		 parent_proc_;	// ���໯������
	BYTE			 status_;
	bool			 is_image_mode_;	// �Ƿ�ΪͼƬ��Ӱģʽ
	bool			 is_show_shadow_;	// �Ƿ�Ҫ��ʾ��Ӱ
	bool			is_disable_show_;
	// �㷨��Ӱ��Ա����
	unsigned char darkness_;	// Darkness, transparency of blurred area
	unsigned char sharp_ness_;	// Sharpness, width of blurred border of shadow window
	signed char size_;	// Shadow window size, relative to parent window size

	// The X and Y offsets of shadow window,
	// relative to the parent window, at center of both windows (not top-left corner), signed
	signed char x_offset_;
	signed char y_offset_;

	// Restore last parent window size, used to determine the update strategy when parent window is resized
	LPARAM wnd_size_;

	// Set this to true if the shadow should not be update until next WM_PAINT is received
	bool update_;

	COLORREF color_;	// Color of shadow

	// ͼƬ��Ӱ��Ա����
	std::wstring	shadow_image_;
	RECT		show_corner_;
};

}

#endif //__UISHADOW_H__