#ifndef GifAnimUIEX_h__
#define GifAnimUIEX_h__
#pragma once
/* write by wangji 2016.03.16
** ������gif�ؼ���gdi+������ռ��CPU���ߵ����⣬�������ximage���
** ע�⣺ʹ�õ�ʱ����Ԥ����ͷ�ļ��а���UIlib.hǰ�ȶ����USE_XIMAGE_EFFECT
** #define USE_XIMAGE_EFFECT
** #include "UIlib.h"
*/
#ifdef USE_XIMAGE_EFFECT
namespace ui
{
	class Label;

	class UILIB_API GifAnimEx : public Label
	{
		DECLARE_DUICONTROL(GifAnimEx)
	public:
		GifAnimEx(void);
		~GifAnimEx(void);
	public:
		virtual std::wstring	GetClass() const;
		virtual LPVOID	GetInterface(std::wstring pstrName);
		virtual void Init();
		virtual void SetAttribute(std::wstring pstrName, std::wstring pstrValue);
		virtual void SetVisible(bool bVisible = true);
		virtual void SetInternVisible(bool bVisible = true);
		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
		virtual void DoEvent(EventArgs& event);

	public:
		void StartAnim();
		void StopAnim();

	protected:
		struct Imp;
		Imp* imp_;
	};
}
#endif //USE_XIMAGE_EFFECT
#endif // GifAnimUIEx_h__
