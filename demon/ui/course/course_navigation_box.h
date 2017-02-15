#ifndef MSYK_UI_COURSE_NAVIGATION_BOX_H__
#define MSYK_UI_COURSE_NAVIGATION_BOX_H__

#include "dynamic_box.h"

enum NavigationIndex
{
	kNavigationNon = -1,
	kNavigationBasicInfo,			// ������Ϣ
	kNavigationKnowledgePoint,      // ֪ʶ�㽲��
	kNavigationSupportingExercises, // ������ϰ
	kNavigationExerciseExplain,		// ϰ�⽲��
	kNavigationSupportingPapers,	// �����Ծ�
	kNavigationRelatingInfo,		// �������
};

using OnNavigationItemSelected = std::function<void(NavigationIndex)>;

class CousreNavigationBox : public DynamicBox
{
public:
	CousreNavigationBox(OnNavigationItemSelected cb = nullptr);
	virtual void OnInit();

	void SelectedItem(NavigationIndex index);

private:
	OnNavigationItemSelected on_item_selected_ = nullptr;
	ui::List* list_ = nullptr;
};


#endif // MSYK_UI_COURSE_NAVIGATION_BOX_H__