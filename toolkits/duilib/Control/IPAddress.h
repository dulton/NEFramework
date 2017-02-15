#ifndef __UIIPADDRESS_H__
#define __UIIPADDRESS_H__

#pragma once

//���ÿؼ����һ������dtstyle

namespace ui
{
	class IPAddressWnd;

	/// ʱ��ѡ��ؼ�
	class UILIB_API IPAddress : public Label
	{
		DECLARE_DUICONTROL(IPAddress)

		friend class IPAddressWnd;
	public:
		IPAddress();
		std::wstring GetClass() const;
		LPVOID GetInterface(std::wstring pstrName);

		DWORD GetIP();
		void SetIP(DWORD dwIP);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void DoEvent(EventArgs& event);

		void SetAttribute(std::wstring pstrName, std::wstring pstrValue);

	protected:
		DWORD	ip_;
		bool    read_only_;
		int		update_flag_;

		IPAddressWnd* wnd_;
	};
}
#endif // __UIIPADDRESS_H__