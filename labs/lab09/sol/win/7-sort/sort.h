#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace My4sort {

/* <summary>
 *  Summary for sort
 *
 * WARNING: If you change the name of this class, you will need
 * to change the 'Resource File Name' property for the managed
 * resource compiler tool associated with all .resx files this
 * class depends on.  Otherwise, the designers will not be able
 * to interact properly with localized resources associated with
 * this form.
 * </summary>
 */
public ref class sort : public System::Windows::Forms::Form
{
public:
	sort(void)
	{
		InitializeComponent();
		/* TODO: Add the constructor code here */
	}

protected:
	/* Clean up any resources being used. */
	~sort()
	{
		if (components)
			delete components;
	}

private:
	/* Required designer variable.*/
	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
	/* Required method for Designer support - do not modify
	 * the contents of this method with the code editor.
	 */
	void InitializeComponent(void)
	{
		this->components =
			gcnew System::ComponentModel::Container();
		this->Size = System::Drawing::Size(300, 300);
		this->Text = L"sort";
		this->Padding =
			System::Windows::Forms::Padding(0);
		this->AutoScaleMode =
			System::Windows::Forms::AutoScaleMode::Font;
	}
#pragma endregion
};
}
