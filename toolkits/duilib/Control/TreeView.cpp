#include "StdAfx.h"
#include "TreeView.h"

#pragma warning( disable: 4251 )
namespace ui
{
	IMPLEMENT_DUICONTROL(TreeNode)

	//************************************
	// ��������: CTreeNodeUI
	// ��������: 
	// ������Ϣ: CTreeNodeUI * _ParentNode
	// ����˵��: 
	//************************************
	TreeNode::TreeNode( TreeNode* _ParentNode /*= NULL*/ )
	{
		text_color_ = 0x00000000;
		tect_hot_color_ = 0;
		selected_text_color_ = 0;
		selected_text_hot_color_	= 0;

		tree_view_ = NULL;
		level_ = 0;
		visible_ = TRUE;
		is_checkbox_ = FALSE;
		parent_	= NULL;

		hbox_ = new HBox();
		folder_button_ = new CheckBox();
		dotted_line_ = new Label();
		checkbox_ = new CheckBox();
		option_ = new Option();

		this->SetFixedHeight(18);
		this->SetFixedWidth(250);
		folder_button_->SetFixedWidth(GetFixedHeight());
		//dotted_line_->SetFixedWidth(2);
		dotted_line_->SetPadding({ 2,0,0,0});
		checkbox_->SetFixedWidth(GetFixedHeight());
		option_->SetAttribute(_T("align"),_T("left"));
		dotted_line_->SetVisible(FALSE);
		checkbox_->SetVisible(FALSE);
		option_->SetMouseEnabled(FALSE);

		if(_ParentNode) {
			// TODO : luyang
			//if (_tcsicmp(_ParentNode->GetClass().c_str(), _T("TreeNode")) != 0) return;
			dotted_line_->SetVisible(_ParentNode->IsVisible());
			this->SetParentNode(_ParentNode);
		}
		hbox_->SetChildVAlign(DT_VCENTER);
		hbox_->Add(dotted_line_);
		hbox_->Add(folder_button_);
		hbox_->Add(checkbox_);
		hbox_->Add(option_);
		Add(hbox_);
	}
	
	//************************************
	// ��������: ~CTreeNodeUI
	// ��������: 
	// ������Ϣ: void
	// ����˵��: 
	//************************************
	TreeNode::~TreeNode( void )
	{

	}

	//************************************
	// ��������: GetClass
	// ��������: std::wstring
	// ����˵��: 
	//************************************
	std::wstring TreeNode::GetClass() const
	{
		return _T("TreeNode");
	}

	//************************************
	// ��������: GetInterface
	// ��������: LPVOID
	// ������Ϣ: std::wstring pstrName
	// ����˵��: 
	//************************************
	LPVOID TreeNode::GetInterface( std::wstring pstrName )
	{
		if( _tcsicmp(pstrName.c_str(), _T("TreeNode")) == 0 )
			return static_cast<TreeNode*>(this);
		return ListContainerElement::GetInterface(pstrName);
	}

	void TreeNode::SetText(std::wstring txt)
	{
		option_->SetText(txt);
	}

	void TreeNode::AttachDoubleClick(const EventCallback & cb)
	{
		on_event_[kEventMouseDoubleClick] += cb;
	}
	
	//************************************
	// ��������: DoEvent
	// ��������: void
	// ������Ϣ: TEventUI & event
	// ����˵��:
	//************************************
	void TreeNode::DoEvent( EventArgs& event )
	{
		event.sender_ = this;

		if( !IsMouseEnabled() && event.type_ > kEventMouseBegin && event.type_ < kEventMouseEnd ) {
			if( owner_ != NULL ) owner_->DoEvent(event);
			else Box::DoEvent(event);
			return;
		}
		ListContainerElement::DoEvent(event);
		if( event.type_ == kEventMouseDoubleClick ) {
			if( IsEnabled() ) {
				Notify(event);
				Invalidate();
			}
			return;
		}
		if( event.type_ == kEventMouseEnter ) {
			if( IsEnabled()) {
				if(selected_ && GetSelItemHotTextColor())
					option_->SetTextColor(GetSelItemHotTextColor());
				else
					option_->SetTextColor(GetItemHotTextColor());
			}
			else 
				option_->SetTextColor(option_->GetDisabledTextColor());

			return;
		}
		if( event.type_ == kEventMouseLeave ) {
			if( IsEnabled()) {
				if(selected_ && GetSelItemTextColor())
					option_->SetTextColor(GetSelItemTextColor());
				else if(!selected_)
					option_->SetTextColor(GetItemTextColor());
			}
			else 
				option_->SetTextColor(option_->GetDisabledTextColor());

			return;
		}
	}

	//************************************
	// ��������: Invalidate
	// ��������: void
	// ����˵��: 
	//************************************
	void TreeNode::Invalidate()
	{
		if( !IsVisible() )
			return;

		if( GetParent() ) {
			Box* pParentContainer = static_cast<Box*>(GetParent()->GetInterface(_T("Container")));
			if( pParentContainer ) {
				RECT rc = pParentContainer->GetPos();
				RECT rcInset = pParentContainer->GetInset();
				rc.left += rcInset.left;
				rc.top += rcInset.top;
				rc.right -= rcInset.right;
				rc.bottom -= rcInset.bottom;
				ScrollBar* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
				if( pVerticalScrollBar && pVerticalScrollBar->IsVisible() ) rc.right -= pVerticalScrollBar->GetFixedWidth();
				ScrollBar* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
				if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rc.bottom -= pHorizontalScrollBar->GetFixedHeight();

				RECT invalidateRc = rect_;
				if( !::IntersectRect(&invalidateRc, &rect_, &rc) ) 
					return;

				Control* pParent = GetParent();
				RECT rcTemp;
				RECT rcParent;
				while( pParent = pParent->GetParent() ) {
					rcTemp = invalidateRc;
					rcParent = pParent->GetPos();
					if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
						return;
				}

				if( pm_ != NULL ) pm_->Invalidate(invalidateRc);
			}
			else {
				Box::Invalidate();
			}
		}
		else {
			Box::Invalidate();
		}
	}
	
	//************************************
	// ��������: Select
	// ��������: bool
	// ������Ϣ: bool bSelect
	// ����˵��: 
	//************************************
	bool TreeNode::Select( bool bSelect /*= true*/ , bool send_notify)
	{
		bool nRet = ListContainerElement::Select(bSelect, send_notify);
		if(selected_)
			option_->SetTextColor(GetSelItemTextColor());
		else 
			option_->SetTextColor(GetItemTextColor());

		return nRet;
	}

	bool TreeNode::SelectMulti(bool bSelect)
	{
		bool nRet = ListContainerElement::SelectMulti(bSelect);
		if(selected_)
			option_->SetTextColor(GetSelItemTextColor());
		else 
			option_->SetTextColor(GetItemTextColor());

		return nRet;
	}
	//************************************
	// ��������: Add
	// ��������: bool
	// ������Ϣ: CControlUI * _pTreeNodeUI
	// ����˵��: ͨ���ڵ������ӽڵ�
	//************************************
	bool TreeNode::Add( Control* _pTreeNodeUI )
	{
		if (_tcsicmp(_pTreeNodeUI->GetClass().c_str(), _T("TreeNode")) == 0)
			return AddChildNode((TreeNode*)_pTreeNodeUI);
		
		return ListContainerElement::Add(_pTreeNodeUI);
	}

	//************************************
	// ��������: AddAt
	// ��������: bool
	// ������Ϣ: CControlUI * pControl
	// ������Ϣ: int iIndex				�ò�������Ե�ǰ�ڵ��µ��ֵ������������б���ͼ����
	// ����˵��: 
	//************************************
	bool TreeNode::AddAt( Control* pControl, int iIndex )
	{
		if(NULL == static_cast<TreeNode*>(pControl->GetInterface(_T("TreeNode"))))
			return FALSE;

		TreeNode* pIndexNode = static_cast<TreeNode*>(nodes_.GetAt(iIndex));
		if(!pIndexNode){
			if(!nodes_.Add(pControl))
				return FALSE;
		}
		else if(pIndexNode && !nodes_.InsertAt(iIndex,pControl))
			return FALSE;

		if(!pIndexNode && tree_view_ && tree_view_->GetItemAt(GetTreeIndex()+1))
			pIndexNode = static_cast<TreeNode*>(tree_view_->GetItemAt(GetTreeIndex()+1)->GetInterface(_T("TreeNode")));

		pControl = CalLocation((TreeNode*)pControl);

		if(tree_view_ && pIndexNode)
			return tree_view_->AddAt((TreeNode*)pControl,pIndexNode);
		else 
			return tree_view_->Add((TreeNode*)pControl);

		return TRUE;
	}

	//************************************
	// ��������: Remove
	// ��������: bool
	// ������Ϣ: CControlUI * pControl
	// ����˵��: 
	//************************************
	bool TreeNode::Remove( Control* pControl )
	{
		return RemoveAt((TreeNode*)pControl);
	}

	//************************************
	// ��������: SetVisibleTag
	// ��������: void
	// ������Ϣ: bool _IsVisible
	// ����˵��: 
	//************************************
	void TreeNode::SetVisibleTag( bool _IsVisible )
	{
		visible_ = _IsVisible;
	}

	//************************************
	// ��������: GetVisibleTag
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeNode::GetVisibleTag()
	{
		return visible_;
	}

	//************************************
	// ��������: SetItemText
	// ��������: void
	// ������Ϣ: std::wstring pstrValue
	// ����˵��: 
	//************************************
	void TreeNode::SetItemText( std::wstring pstrValue )
	{
		option_->SetText(pstrValue);
	}

	//************************************
	// ��������: GetItemText
	// ��������: DuiLib::std::wstring
	// ����˵��: 
	//************************************
	std::wstring TreeNode::GetItemText()
	{
		return option_->GetText();
	}

	//************************************
	// ��������: CheckBoxSelected
	// ��������: void
	// ������Ϣ: bool _Selected
	// ����˵��: 
	//************************************
	void TreeNode::CheckBoxSelected( bool _Selected )
	{
		checkbox_->Selected(_Selected);
	}

	//************************************
	// ��������: IsCheckBoxSelected
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeNode::IsCheckBoxSelected() const
	{
		return checkbox_->IsSelected();
	}

	//************************************
	// ��������: IsHasChild
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeNode::IsHasChild() const
	{
		return !nodes_.IsEmpty();
	}
	
	//************************************
	// ��������: AddChildNode
	// ��������: bool
	// ������Ϣ: CTreeNodeUI * _pTreeNodeUI
	// ����˵��: 
	//************************************
	bool TreeNode::AddChildNode( TreeNode* _pTreeNodeUI )
	{
		if (!_pTreeNodeUI)
			return FALSE;

		if (_tcsicmp(_pTreeNodeUI->GetClass().c_str(), _T("TreeNode")) != 0)
			return FALSE;

		_pTreeNodeUI = CalLocation(_pTreeNodeUI);

		bool nRet = TRUE;

		if(tree_view_){
			TreeNode* pNode = static_cast<TreeNode*>(nodes_.GetAt(nodes_.GetSize()-1));
			if(!pNode || !pNode->GetLastNode())
				nRet = tree_view_->AddAt(_pTreeNodeUI,GetTreeIndex()+1) >= 0;
			else nRet = tree_view_->AddAt(_pTreeNodeUI,pNode->GetLastNode()->GetTreeIndex()+1) >= 0;
		}

		if(nRet)
			nodes_.Add(_pTreeNodeUI);

		return nRet;
	}

	//************************************
	// ��������: RemoveAt
	// ��������: bool
	// ������Ϣ: CTreeNodeUI * _pTreeNodeUI
	// ����˵��: 
	//************************************
	bool TreeNode::RemoveAt( TreeNode* _pTreeNodeUI )
	{
		int nIndex = nodes_.Find(_pTreeNodeUI);
		TreeNode* pNode = static_cast<TreeNode*>(nodes_.GetAt(nIndex));
		if(pNode && pNode == _pTreeNodeUI)
		{
			while(pNode->IsHasChild())
				pNode->RemoveAt(static_cast<TreeNode*>(pNode->nodes_.GetAt(0)));

			nodes_.Remove(nIndex);

			if(tree_view_)
				tree_view_->Remove(_pTreeNodeUI);

			return TRUE;
		}
		return FALSE;
	}

	//************************************
	// ��������: SetParentNode
	// ��������: void
	// ������Ϣ: CTreeNodeUI * _pParentTreeNode
	// ����˵��: 
	//************************************
	void TreeNode::SetParentNode( TreeNode* _pParentTreeNode )
	{
		parent_ = _pParentTreeNode;
	}

	//************************************
	// ��������: GetParentNode
	// ��������: CTreeNodeUI*
	// ����˵��: 
	//************************************
	TreeNode* TreeNode::GetParentNode()
	{
		return parent_;
	}

	//************************************
	// ��������: GetCountChild
	// ��������: long
	// ����˵��: 
	//************************************
	long TreeNode::GetCountChild()
	{
		return nodes_.GetSize();
	}

	//************************************
	// ��������: SetTreeView
	// ��������: void
	// ������Ϣ: CTreeViewUI * _CTreeViewUI
	// ����˵��: 
	//************************************
	void TreeNode::SetTreeView( TreeView* _CTreeViewUI )
	{
		tree_view_ = _CTreeViewUI;
	}

	//************************************
	// ��������: GetTreeView
	// ��������: CTreeViewUI*
	// ����˵��: 
	//************************************
	TreeView* TreeNode::GetTreeView()
	{
		return tree_view_;
	}

	//************************************
	// ��������: SetAttribute
	// ��������: void
	// ������Ϣ: std::wstring pstrName
	// ������Ϣ: std::wstring pstrValue
	// ����˵��: 
	//************************************
	void TreeNode::SetAttribute( std::wstring strName, std::wstring strValue )
	{
		auto pstrName = strName.c_str();
		auto pstrValue = strValue.c_str();

		std::wstring cls;
		if(_tcsicmp(pstrName, _T("text")) == 0 )
			option_->SetText(pstrValue);
		else if (_tcsicmp(pstrName, _T("horizattr")) == 0) {
			cls = GlobalManager::GetClass(pstrValue);
			hbox_->ApplyAttributeList(cls);
		}
		else if (_tcsicmp(pstrName, _T("dotlineattr")) == 0) {
			cls = GlobalManager::GetClass(pstrValue);
			dotted_line_->ApplyAttributeList(cls);
		}
		else if (_tcsicmp(pstrName, _T("folderattr")) == 0) {
			cls = GlobalManager::GetClass(pstrValue);
			folder_button_->ApplyAttributeList(cls);
		}
		else if (_tcsicmp(pstrName, _T("checkboxattr")) == 0) {
			cls = GlobalManager::GetClass(pstrValue);
			checkbox_->ApplyAttributeList(cls);
		}
		else if (_tcsicmp(pstrName, _T("itemattr")) == 0) {
			cls = GlobalManager::GetClass(pstrValue);
			option_->ApplyAttributeList(cls);
		}
		else if(_tcsicmp(pstrName, _T("itemtextcolor")) == 0 ){
			SetItemTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0 ){
			SetItemHotTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("selitemtextcolor")) == 0 ){
			SetSelItemTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("selitemhottextcolor")) == 0 ){
			SetSelItemHotTextColor(GlobalManager::GetColor(strValue));
		}
		else ListContainerElement::SetAttribute(strName, strValue);
	}

	//************************************
	// ��������: GetTreeNodes
	// ��������: DuiLib::CStdPtrArray
	// ����˵��: 
	//************************************
	CStdPtrArray TreeNode::GetTreeNodes()
	{
		return nodes_;
	}

	//************************************
	// ��������: GetChildNode
	// ��������: CTreeNodeUI*
	// ������Ϣ: int _nIndex
	// ����˵��: 
	//************************************
	TreeNode* TreeNode::GetChildNode( int _nIndex )
	{
		return static_cast<TreeNode*>(nodes_.GetAt(_nIndex));
	}

	//************************************
	// ��������: SetVisibleFolderBtn
	// ��������: void
	// ������Ϣ: bool _IsVisibled
	// ����˵��: 
	//************************************
	void TreeNode::SetVisibleFolderBtn( bool _IsVisibled )
	{
		folder_button_->SetVisible(_IsVisibled);
	}

	//************************************
	// ��������: GetVisibleFolderBtn
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeNode::GetVisibleFolderBtn()
	{
		return folder_button_->IsVisible();
	}

	//************************************
	// ��������: SetVisibleCheckBtn
	// ��������: void
	// ������Ϣ: bool _IsVisibled
	// ����˵��: 
	//************************************
	void TreeNode::SetVisibleCheckBtn( bool _IsVisibled )
	{
		checkbox_->SetVisible(_IsVisibled);
	}

	//************************************
	// ��������: GetVisibleCheckBtn
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeNode::GetVisibleCheckBtn()
	{
		return checkbox_->IsVisible();
	}
	
	//************************************
	// ��������: GetNodeIndex
	// ��������: int
	// ����˵��: ȡ��ȫ������ͼ������
	//************************************
	int TreeNode::GetTreeIndex()
	{
		if(!tree_view_)
			return -1;

		for(int nIndex = 0;nIndex < tree_view_->GetCount();nIndex++){
			if(this == tree_view_->GetItemAt(nIndex))
				return nIndex;
		}

		return -1;
	}
	
	//************************************
	// ��������: GetNodeIndex
	// ��������: int
	// ����˵��: ȡ��������ֵܽڵ�ĵ�ǰ����
	//************************************
	int TreeNode::GetNodeIndex()
	{
		if(!GetParentNode() && !tree_view_)
			return -1;

		if(!GetParentNode() && tree_view_)
			return GetTreeIndex();

		return GetParentNode()->GetTreeNodes().Find(this);
	}

	void TreeNode::SetIndentation(int indentation, int max_indentation)
	{
		int fixed_width = dotted_line_->GetFixedWidth();
		if (fixed_width <= 0) {
			int width = level_ > 0 ? (fixed_width + indentation * (level_)) : fixed_width;
			width = width > (max_indentation + fixed_width) ? (max_indentation == -1 ? width : max_indentation) : width;
			dotted_line_->SetFixedWidth(width);
		}
	}

	//************************************
	// ��������: GetLastNode
	// ��������: CTreeNodeUI*
	// ����˵��:
	//************************************
	TreeNode* TreeNode::GetLastNode( )
	{
		if(!IsHasChild()) return this;

		TreeNode* nRetNode = NULL;
		for(int nIndex = 0;nIndex < GetTreeNodes().GetSize();nIndex++){
			TreeNode* pNode = static_cast<TreeNode*>(GetTreeNodes().GetAt(nIndex));
			if(!pNode) continue;
			if(pNode->IsHasChild())
				nRetNode = pNode->GetLastNode();
			else 
				nRetNode = pNode;
		}
		
		return nRetNode;
	}
	
	//************************************
	// ��������: CalLocation
	// ��������: CTreeNodeUI*
	// ������Ϣ: CTreeNodeUI * _pTreeNodeUI
	// ����˵��: ��������
	//************************************
	TreeNode* TreeNode::CalLocation( TreeNode* _pTreeNodeUI )
	{
		_pTreeNodeUI->GetDottedLine()->SetVisible(TRUE);
		_pTreeNodeUI->SetParentNode(this);
		_pTreeNodeUI->GetItemButton()->SetGroup(option_->GetGroup());
		_pTreeNodeUI->SetTreeView(tree_view_);

	/*	int indentation = GetTreeView()->GetItemIndentation();
		int max_indentaiton = GetTreeView()->GetMaxItemIndentation();
		int width = dotted_line_->GetFixedWidth() + indentation * (_pTreeNodeUI->GetTreeLevel() - 1);
		width = width > max_indentaiton ? (max_indentaiton == -1 ? width : max_indentaiton) : width;
		_pTreeNodeUI->GetDottedLine()->SetFixedWidth(width);*/

		//_pTreeNodeUI->SetIndentation();

		//SetIndentation(GetTreeView()->GetItemIndentation(), GetTreeView()->GetMaxItemIndentation());

		return _pTreeNodeUI;
	}

	//************************************
	// ��������: SetTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwTextColor
	// ����˵��: 
	//************************************
	void TreeNode::SetItemTextColor( DWORD _dwItemTextColor )
	{
		text_color_	= _dwItemTextColor;
		option_->SetTextColor(text_color_);
	}

	//************************************
	// ��������: GetTextColor
	// ��������: DWORD
	// ����˵��: 
	//************************************
	DWORD TreeNode::GetItemTextColor() const
	{
		return text_color_;
	}

	//************************************
	// ��������: SetTextHotColor
	// ��������: void
	// ������Ϣ: DWORD _dwTextHotColor
	// ����˵��: 
	//************************************
	void TreeNode::SetItemHotTextColor( DWORD _dwItemHotTextColor )
	{
		tect_hot_color_ = _dwItemHotTextColor;
		Invalidate();
	}

	//************************************
	// ��������: GetTextHotColor
	// ��������: DWORD
	// ����˵��: 
	//************************************
	DWORD TreeNode::GetItemHotTextColor() const
	{
		return tect_hot_color_;
	}

	//************************************
	// ��������: SetSelItemTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwSelItemTextColor
	// ����˵��: 
	//************************************
	void TreeNode::SetSelItemTextColor( DWORD _dwSelItemTextColor )
	{
		selected_text_color_ = _dwSelItemTextColor;
		Invalidate();
	}

	//************************************
	// ��������: GetSelItemTextColor
	// ��������: DWORD
	// ����˵��: 
	//************************************
	DWORD TreeNode::GetSelItemTextColor() const
	{
		return selected_text_color_;
	}

	//************************************
	// ��������: SetSelHotItemTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwSelHotItemTextColor
	// ����˵��: 
	//************************************
	void TreeNode::SetSelItemHotTextColor( DWORD _dwSelHotItemTextColor )
	{
		selected_text_hot_color_ = _dwSelHotItemTextColor;
		Invalidate();
	}

	//************************************
	// ��������: GetSelHotItemTextColor
	// ��������: DWORD
	// ����˵��: 
	//************************************
	DWORD TreeNode::GetSelItemHotTextColor() const
	{
		return selected_text_hot_color_;
	}

	/*****************************************************************************/
	/*****************************************************************************/
	/*****************************************************************************/
	IMPLEMENT_DUICONTROL(TreeView)
	
	//************************************
	// ��������: CTreeViewUI
	// ��������: 
	// ������Ϣ: void
	// ����˵��: 
	//************************************
	TreeView::TreeView( void ) : folder_btn_visible_(TRUE),check_btn_visible_(FALSE),item_min_width_(0)
	{
		this->GetHeader()->SetVisible(FALSE);
	}
	
	//************************************
	// ��������: ~CTreeViewUI
	// ��������: 
	// ������Ϣ: void
	// ����˵��: 
	//************************************
	TreeView::~TreeView( void )
	{
		
	}

	//************************************
	// ��������: GetClass
	// ��������: std::wstring
	// ����˵��: 
	//************************************
	std::wstring TreeView::GetClass() const
	{
		return _T("TreeView");
	}


	UINT TreeView::GetListType()
	{
		return kListTree;
	}

	//************************************
	// ��������: GetInterface
	// ��������: LPVOID
	// ������Ϣ: std::wstring pstrName
	// ����˵��: 
	//************************************
	LPVOID TreeView::GetInterface( std::wstring pstrName )
	{
		if( _tcsicmp(pstrName.c_str(), _T("TreeView")) == 0 ) return static_cast<TreeView*>(this);
		return List::GetInterface(pstrName);
	}

	void TreeView::DoInit()
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			TreeNode *pNode = static_cast<TreeNode*>(GetItemAt(i)->GetInterface(L"TreeNode"));
			if (pNode) {
				pNode->SetIndentation(item_indentation_, max_item_indentation_);
			}
		}
	}

	//************************************
	// ��������: Add
	// ��������: bool
	// ������Ϣ: CTreeNodeUI * pControl
	// ����˵��: 
	//************************************
	bool TreeView::Add( TreeNode* pControl )
	{
		if (!pControl) return false;
		if (_tcsicmp(pControl->GetClass().c_str(), _T("TreeNode")) != 0) return false;

		pControl->AttachDoubleClick(std::bind(&TreeView::OnDBClickItem, this, std::placeholders::_1));
		pControl->GetFolderButton()->AttachSelectedChanged(std::bind(&TreeView::OnFolderChanged, this, std::placeholders::_1));
		pControl->GetCheckBox()->AttachSelectedChanged(std::bind(&TreeView::OnCheckBoxChanged, this, std::placeholders::_1));

		pControl->SetVisibleFolderBtn(folder_btn_visible_);
		pControl->SetVisibleCheckBtn(check_btn_visible_);
		if(item_min_width_ > 0)
			pControl->SetMinWidth(item_min_width_);
		//pControl->SetIndentation(item_indentation_, max_item_indentation_);

		List::Add(pControl);

		if(pControl->GetCountChild() > 0) {
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++) {
				TreeNode* pNode = pControl->GetChildNode(nIndex);
				if(pNode) Add(pNode);
			}
		}

		pControl->SetTreeView(this);
		return true;
	}

	//************************************
	// ��������: AddAt
	// ��������: long
	// ������Ϣ: CTreeNodeUI * pControl
	// ������Ϣ: int iIndex
	// ����˵��: �÷������Ὣ������Ľڵ������λ�������������Ľڵ�Ϊ�Ǹ��ڵ㣬��ʹ��AddAt(CTreeNodeUI* pControl,CTreeNodeUI* _IndexNode) ����
	//************************************
	long TreeView::AddAt( TreeNode* pControl, int iIndex )
	{
		if (!pControl) return -1;
		if (_tcsicmp(pControl->GetClass().c_str(), _T("TreeNode")) != 0) return -1;
		pControl->AttachDoubleClick(std::bind(&TreeView::OnDBClickItem, this, std::placeholders::_1));
		pControl->GetFolderButton()->AttachSelectedChanged(std::bind(&TreeView::OnFolderChanged, this, std::placeholders::_1));
		pControl->GetCheckBox()->AttachSelectedChanged(std::bind(&TreeView::OnCheckBoxChanged, this, std::placeholders::_1));

		pControl->SetVisibleFolderBtn(folder_btn_visible_);
		pControl->SetVisibleCheckBtn(check_btn_visible_);

		if(item_min_width_ > 0) {
			pControl->SetMinWidth(item_min_width_);
		}
		List::AddAt(pControl, iIndex);
		if(pControl->GetCountChild() > 0) {
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0; nIndex < nCount; nIndex++) {
				TreeNode* pNode = pControl->GetChildNode(nIndex);
				if(pNode)
					return AddAt(pNode,iIndex+1);
			}
		}
		else {
			return iIndex + 1;
		}

		return -1;
	}

	//************************************
	// ��������: AddAt
	// ��������: bool
	// ������Ϣ: CTreeNodeUI * pControl
	// ������Ϣ: CTreeNodeUI * _IndexNode
	// ����˵��:
	//************************************
	bool TreeView::AddAt( TreeNode* pControl, TreeNode* _IndexNode )
	{
		if(!_IndexNode && !pControl)
			return FALSE;

		int nItemIndex = -1;
		for(int nIndex = 0;nIndex < GetCount();nIndex++) {
			if(_IndexNode == GetItemAt(nIndex)) {
				nItemIndex = nIndex;
				break;
			}
		}

		if(nItemIndex == -1)
			return FALSE;

		return AddAt(pControl,nItemIndex) >= 0;
	}

	//************************************
	// ��������: Remove
	// ��������: bool
	// ������Ϣ: CTreeNodeUI * pControl
	// ����˵��: pControl �����Լ��µ����нڵ㽫��һ���Ƴ�
	//************************************
	bool TreeView::Remove( TreeNode* pControl )
	{
		if(pControl->GetCountChild() > 0) {
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++) {
				TreeNode* pNode = pControl->GetChildNode(nIndex);
				if(pNode){
					pControl->Remove(pNode);
				}
			}
		}
		List::Remove(pControl);
		return TRUE;
	}

	//************************************
	// ��������: RemoveAt
	// ��������: bool
	// ������Ϣ: int iIndex
	// ����˵��: iIndex �����Լ��µ����нڵ㽫��һ���Ƴ�
	//************************************
	bool TreeView::RemoveAt( int iIndex )
	{
		TreeNode* pItem = (TreeNode*)GetItemAt(iIndex);
		if(pItem->GetCountChild())
			Remove(pItem);
		return TRUE;
	}

	void TreeView::RemoveAll()
	{
		List::RemoveAll();
	}

	//************************************
	// ��������: OnCheckBoxChanged
	// ��������: bool
	// ������Ϣ: void * param
	// ����˵��: 
	//************************************
	bool TreeView::OnCheckBoxChanged(const EventArgs& arg)
	{
		CheckBox* pCheckBox = (CheckBox*)arg.sender_;
		TreeNode* pItem = (TreeNode*)pCheckBox->GetParent()->GetParent();
		SetItemCheckBox(pCheckBox->GetCheck(), pItem);
		return true;
	}
	
	//************************************
	// ��������: OnFolderChanged
	// ��������: bool
	// ������Ϣ: void * param
	// ����˵��: 
	//************************************
	bool TreeView::OnFolderChanged(const EventArgs& arg)
	{
		CheckBox* pFolder = (CheckBox*)arg.sender_;
		TreeNode* pItem = (TreeNode*)pFolder->GetParent()->GetParent();
		bool is_checked = pFolder->GetCheck();
		pItem->SetVisibleTag(is_checked);
		SetItemExpand(is_checked,pItem);
		return true;
	}
	
	//************************************
	// ��������: OnDBClickItem
	// ��������: bool
	// ������Ϣ: void * param
	// ����˵��:
	//************************************
	bool TreeView::OnDBClickItem( const EventArgs &arg )
	{
		TreeNode* pItem		= static_cast<TreeNode*>(arg.sender_);
		CheckBox* pFolder	= pItem->GetFolderButton();
		bool is_checked = pFolder->GetCheck();
		pFolder->Selected(!is_checked, false);
		pItem->SetVisibleTag(!is_checked);
		SetItemExpand(!is_checked, pItem);

		return true;
	}

	//************************************
	// ��������: SetItemCheckBox
	// ��������: bool
	// ������Ϣ: bool _Selected
	// ������Ϣ: CTreeNodeUI * _TreeNode
	// ����˵��: 
	//************************************
	bool TreeView::SetItemCheckBox( bool _Selected,TreeNode* _TreeNode /*= NULL*/)
	{
		if(_TreeNode) {
			if(_TreeNode->GetCountChild() > 0) {
				int nCount = _TreeNode->GetCountChild();
				for(int nIndex = 0;nIndex < nCount;nIndex++) {
					TreeNode* pItem = _TreeNode->GetChildNode(nIndex);
					pItem->GetCheckBox()->Selected(_Selected);
					if(pItem->GetCountChild())
						SetItemCheckBox(_Selected,pItem);
				}
			}
			return TRUE;
		}
		else {
			int nIndex = 0;
			int nCount = GetCount();
			while(nIndex < nCount) {
				TreeNode* pItem = (TreeNode*)GetItemAt(nIndex);
				pItem->GetCheckBox()->Selected(_Selected);
				if(pItem->GetCountChild())
					SetItemCheckBox(_Selected,pItem);

				nIndex++;
			}
			return TRUE;
		}
		return FALSE;
	}

	//************************************
	// ��������: SetItemExpand
	// ��������: void
	// ������Ϣ: bool _Expanded
	// ������Ϣ: CTreeNodeUI * _TreeNode
	// ����˵��: 
	//************************************
	void TreeView::SetItemExpand( bool _Expanded,TreeNode* _TreeNode /*= NULL*/, bool force)
	{
		if(_TreeNode) {
			if(_TreeNode->GetCountChild() > 0) {
				int nCount = _TreeNode->GetCountChild();
				for(int nIndex = 0;nIndex < nCount;nIndex++) {
					TreeNode* pItem = _TreeNode->GetChildNode(nIndex);
					pItem->SetVisible(_Expanded);
					if((pItem->GetCountChild() && pItem->GetFolderButton()->IsSelected()) || force) {
						SetItemExpand(_Expanded, pItem, force);
					}
				}
			}
		}
		else {
			int nIndex = 0;
			int nCount = GetCount();
			while(nIndex < nCount) {
				TreeNode* pItem = (TreeNode*)GetItemAt(nIndex);
				pItem->SetVisible(_Expanded);
				if(pItem->GetCountChild() && pItem->GetFolderButton()->IsSelected()) {
					SetItemExpand(_Expanded,pItem);
				}
				nIndex++;
			}
		}
	}

	void TreeView::ExpandItemReverse(TreeNode * node, bool expand)
	{
		TreeNode *parent = node->GetParentNode();
		while (parent && parent->GetTreeLevel() != 0)
		{
			CheckBox* pFolder = parent->GetFolderButton();
			pFolder->Selected(true, false);
			parent->SetVisibleTag(true);
			parent->SetVisible(true);
			
			if (parent->IsHasChild()) {
				int count = parent->GetCountChild();
				for (int i = 0; i < count; ++i)
				{
					TreeNode* pItem = (TreeNode*)parent->GetChildNode(i);
					CheckBox* pFolder = pItem->GetFolderButton();
					bool is_checked = pFolder->GetCheck();
					pFolder->Selected(is_checked, false);
					pItem->SetVisible(true);
					pItem->SetVisibleTag(is_checked);
					
				}
			}
			
			parent = parent->GetParentNode();
		}
	}



	//************************************
	// ��������: SetVisibleFolderBtn
	// ��������: void
	// ������Ϣ: bool _IsVisibled
	// ����˵��: 
	//************************************
	void TreeView::SetVisibleFolderBtn( bool _IsVisibled )
	{
		folder_btn_visible_ = _IsVisibled;
		int nCount = this->GetCount();
		for(int nIndex = 0; nIndex < nCount; nIndex++) {
			TreeNode* pItem = static_cast<TreeNode*>(this->GetItemAt(nIndex));
			pItem->GetFolderButton()->SetVisible(folder_btn_visible_);
		}
	}

	//************************************
	// ��������: GetVisibleFolderBtn
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeView::GetVisibleFolderBtn()
	{
		return folder_btn_visible_;
	}

	//************************************
	// ��������: SetVisibleCheckBtn
	// ��������: void
	// ������Ϣ: bool _IsVisibled
	// ����˵��: 
	//************************************
	void TreeView::SetVisibleCheckBtn( bool _IsVisibled )
	{
		check_btn_visible_ = _IsVisibled;
		int nCount = this->GetCount();
		for(int nIndex = 0; nIndex < nCount; nIndex++) {
			TreeNode* pItem = static_cast<TreeNode*>(this->GetItemAt(nIndex));
			pItem->GetCheckBox()->SetVisible(check_btn_visible_);
		}
	}

	//************************************
	// ��������: GetVisibleCheckBtn
	// ��������: bool
	// ����˵��: 
	//************************************
	bool TreeView::GetVisibleCheckBtn()
	{
		return check_btn_visible_;
	}

	//************************************
	// ��������: SetItemMinWidth
	// ��������: void
	// ������Ϣ: UINT _ItemMinWidth
	// ����˵��: 
	//************************************
	void TreeView::SetItemMinWidth( UINT _ItemMinWidth )
	{
		item_min_width_ = _ItemMinWidth;

		for(int nIndex = 0;nIndex < GetCount();nIndex++){
			TreeNode* pTreeNode = static_cast<TreeNode*>(GetItemAt(nIndex));
			if(pTreeNode) {
				pTreeNode->SetMinWidth(GetItemMinWidth());
			}
		}
		Invalidate();
	}

	//************************************
	// ��������: GetItemMinWidth
	// ��������: UINT
	// ����˵��: 
	//************************************
	UINT TreeView::GetItemMinWidth()
	{
		return item_min_width_;
	}
	
	//************************************
	// ��������: SetItemTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwItemTextColor
	// ����˵��: 
	//************************************
	void TreeView::SetItemTextColor( DWORD _dwItemTextColor )
	{
		for(int nIndex = 0;nIndex < GetCount();nIndex++){
			TreeNode* pTreeNode = static_cast<TreeNode*>(GetItemAt(nIndex));
			if(pTreeNode) {
				pTreeNode->SetItemTextColor(_dwItemTextColor);
			}
		}
	}

	//************************************
	// ��������: SetItemHotTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwItemHotTextColor
	// ����˵��: 
	//************************************
	void TreeView::SetItemHotTextColor( DWORD _dwItemHotTextColor )
	{
		for(int nIndex = 0;nIndex < GetCount();nIndex++){
			TreeNode* pTreeNode = static_cast<TreeNode*>(GetItemAt(nIndex));
			if(pTreeNode) {
				pTreeNode->SetItemHotTextColor(_dwItemHotTextColor);
			}
		}
	}

	//************************************
	// ��������: SetSelItemTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwSelItemTextColor
	// ����˵��: 
	//************************************
	void TreeView::SetSelItemTextColor( DWORD _dwSelItemTextColor )
	{
		for(int nIndex = 0;nIndex < GetCount();nIndex++){
			TreeNode* pTreeNode = static_cast<TreeNode*>(GetItemAt(nIndex));
			if(pTreeNode) {
				pTreeNode->SetSelItemTextColor(_dwSelItemTextColor);
			}
		}
	}
		
	//************************************
	// ��������: SetSelItemHotTextColor
	// ��������: void
	// ������Ϣ: DWORD _dwSelHotItemTextColor
	// ����˵��: 
	//************************************
	void TreeView::SetSelItemHotTextColor( DWORD _dwSelHotItemTextColor )
	{
		for(int nIndex = 0;nIndex < GetCount();nIndex++){
			TreeNode* pTreeNode = static_cast<TreeNode*>(GetItemAt(nIndex));
			if(pTreeNode) {
				pTreeNode->SetSelItemHotTextColor(_dwSelHotItemTextColor);
			}
		}
	}

	//************************************
	// ��������: SetAttribute
	// ��������: void
	// ������Ϣ: std::wstring pstrName
	// ������Ϣ: std::wstring pstrValue
	// ����˵��: 
	//************************************
	void TreeView::SetAttribute( std::wstring strName, std::wstring strValue )
	{
		auto pstrName = strName.c_str();
		auto pstrValue = strValue.c_str();

		if(_tcsicmp(pstrName,_T("visiblefolderbtn")) == 0)
			SetVisibleFolderBtn(_tcsicmp(pstrValue,_T("TRUE")) == 0);
		else if(_tcsicmp(pstrName,_T("visiblecheckbtn")) == 0)
			SetVisibleCheckBtn(_tcsicmp(pstrValue,_T("TRUE")) == 0);
		else if(_tcsicmp(pstrName,_T("itemminwidth")) == 0)
			SetItemMinWidth(_ttoi(pstrValue));
		else if(_tcsicmp(pstrName, _T("itemtextcolor")) == 0 ){
			SetItemTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0 ){
			SetItemHotTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("selitemtextcolor")) == 0 ){
			SetSelItemTextColor(GlobalManager::GetColor(strValue));
		}
		else if(_tcsicmp(pstrName, _T("selitemhottextcolor")) == 0 ){
			SetSelItemHotTextColor(GlobalManager::GetColor(strValue));
		}
		else if (_tcsicmp(pstrName, L"itemindentation") == 0) {
			item_indentation_ = _ttoi(pstrValue);
		}
		else if (_tcsicmp(pstrName, L"maxitemindetation") == 0) {
			max_item_indentation_ = _ttoi(pstrValue);
		}

		else List::SetAttribute(strName, strValue);
	}

}