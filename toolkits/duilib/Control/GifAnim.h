#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace ui
{
	class UILIB_API GifAnim : public Control
	{
		enum
		{ 
			EVENT_TIEM_ID = 100,
		};
		DECLARE_DUICONTROL(GifAnim)
	public:
		GifAnim(void);
		~GifAnim(void);

		std::wstring	GetClass() const;
		LPVOID	GetInterface(std::wstring pstrName);
		void	DoInit() override;
		void	DoPaint(HDC hDC, const RECT& rcPaint) override;
		void	DoEvent(EventArgs& event) override;
		void	SetVisible(bool bVisible = true ) override;
		void	SetAttribute(std::wstring pstrName, std::wstring pstrValue) override;
		void	SetBkImage(std::wstring pStrImage);
		std::wstring GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	PlayGif();
		void	PauseGif();
		void	StopGif();

	private:
		void	InitGifImage();
		void	DeleteGif();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );		// ����GIFÿ֡

	private:
		Gdiplus::Image	*gif_image_;
		UINT			frame_count_;				// gifͼƬ��֡��
		UINT			frame_position_;			// ��ǰ�ŵ��ڼ�֡
		Gdiplus::PropertyItem*	property_item_;	// ֡��֮֡����ʱ��

		std::wstring	bk_image_;
		bool			is_auto_play_;				// �Ƿ��Զ�����gif
		bool			is_auto_size_;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			playing_;
	};
}

#endif // GifAnimUI_h__
