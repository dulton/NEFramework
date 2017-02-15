/*********************************************************************
*  ����Ϊduilib�����Ķ�̬��ɫ��ؼ�����ȫģ�¿ṷ��QQ�ĵ�ɫ�幦��
*  ʹ����HSB����ģʽ��������Լ��о�����һ���㷨���ƽ���
*  �����ɫ����Դ���ģ�³��ṷ��ɫ�����ʽ�����ǻ��в��
*  �������λ����֪�����õ��㷨���������ƵĴ��룬���ߴ˴�����bug������ϵ��
*  By:Redrain  QQ��491646717   2014.8.19
*  ��������Ƶ����󣬿����޸������
*  thumbimage����ָ��ѡ����ɫ�Ĺ���زĵ�λ�ã��ز��Ҹ��ӵ���ѹ�����У����Լ��޸�
*  sample:<ColorPalette name="Pallet" width="506" height="220" palletheight="200" barheight="14" padding="8,5,0,0" bkcolor="#FFFFFFFF" thumbimage="UI\skin\cursor.png" />
*
*  ��ģ��������ԭ���Ĵ����������ѡ���֮�������±�д�ģ��޸����㷨����������˶�̬��ɫ������ܣ�֮�����ֽ�����һ��bug���ٴθ�л��֮����Ĵ���
*********************************************************************/


#ifndef UI_PALLET_H
#define UI_PALLET_H

#pragma once

namespace ui {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API ColorPalette : public Control
	{
		DECLARE_DUICONTROL(ColorPalette)
	public:
		ColorPalette();
		virtual ~ColorPalette();

		void AttachColorChanged(const EventCallback & cb);

		//��ȡ���ձ�ѡ�����ɫ������ֱ����������duilib����ɫ
		DWORD GetSelectColor();
		void SetSelectColor(DWORD dwColor);

		virtual std::wstring GetClass() const;
		virtual LPVOID GetInterface(std::wstring pstrName);
		virtual void SetAttribute(std::wstring pstrName, std::wstring pstrValue);

		//����/��ȡ Pallet����ɫ�������棩�ĸ߶�
		void SetPalletHeight(int nHeight);
		int	GetPalletHeight() const;

		//����/��ȡ �·�Bar������ѡ�������ĸ߶�
		void SetBarHeight(int nHeight);
		int GetBarHeight() const;
		//����/��ȡ ѡ��ͼ���·��
		void SetThumbImage(std::wstring pszImage);
		std::wstring GetThumbImage() const;

		virtual void SetPos(RECT rc);
		virtual void DoInit();
		virtual void DoEvent(EventArgs& event);
		virtual void PaintBkColor(HDC hDC);
		virtual void PaintPallet(HDC hDC);

	protected:
		//��������
		void UpdatePalletData();
		void UpdateBarData();

	private:
		HDC			mem_dc_;
		HBITMAP		mem_bitmap_;
		BITMAP		bitmap_info_;
		BYTE		*bits_;
		UINT		btn_state_;
		bool		in_bar_;
		bool		in_pallet_;
		int			cur_h_;
		int			cur_s_;
		int			cur_b_;

		int			pallet_height_;
		int			bar_height_;
		CDuiPoint		last_pallet_mouse_;
		CDuiPoint		last_bar_mouse_;
		std::wstring  thumb_image_;
	};
}

#endif // UI_PALLET_H