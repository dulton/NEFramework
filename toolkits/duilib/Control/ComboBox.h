#ifndef __UICOMBOBOX_H__
#define __UICOMBOBOX_H__

#pragma once

namespace ui
{
	/// ��չ�����б��
	/// ����arrowimage����,һ��ͼƬƽ���ֳ�5��,Normal/Hot/Pushed/Focused/Disabled(������source����)
	/// <Default name="ComboBox" value="arrowimage=&quot;file='sys_combo_btn.png' source='0,0,16,16'&quot; "/>
	class UILIB_API ComboBox : public Combo
	{
		DECLARE_DUICONTROL(ComboBox)
	public:
		ComboBox();
		std::wstring GetClass() const;

		void SetAttribute(std::wstring pstrName, std::wstring pstrValue);

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

	protected:
		std::wstring arrow_image_;
		int        arrow_width_;
	};
}

#endif // __UICOMBOBOX_H__
