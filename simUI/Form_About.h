#pragma once
#include "standard_properties.h"

namespace simUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form_About
	/// </summary>
	public ref class Form_About : public System::Windows::Forms::Form
	{
	public:
		Form_About(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form_About()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Label^  labelTitle;
	private: System::Windows::Forms::Label^  labelVersion;
	private: System::Windows::Forms::ImageList^  imageList1;
	private: System::ComponentModel::IContainer^  components;

	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->labelVersion = (gcnew System::Windows::Forms::Label());
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->SuspendLayout();
			// 
			// labelTitle
			// 
			this->labelTitle->AutoSize = true;
			this->labelTitle->Location = System::Drawing::Point(9, 9);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(27, 13);
			this->labelTitle->TabIndex = 0;
			this->labelTitle->Text = L"Title";
			// 
			// labelVersion
			// 
			this->labelVersion->AutoSize = true;
			this->labelVersion->Location = System::Drawing::Point(9, 22);
			this->labelVersion->Name = L"labelVersion";
			this->labelVersion->Size = System::Drawing::Size(42, 13);
			this->labelVersion->TabIndex = 1;
			this->labelVersion->Text = L"Version";
			// 
			// imageList1
			// 
			this->imageList1->ColorDepth = System::Windows::Forms::ColorDepth::Depth8Bit;
			this->imageList1->ImageSize = System::Drawing::Size(16, 16);
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			// 
			// Form_About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 98);
			this->Controls->Add(this->labelVersion);
			this->Controls->Add(this->labelTitle);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Form_About";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->Text = L"About simUI";
			this->Load += gcnew System::EventHandler(this, &Form_About::Form_About_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form_About_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->labelTitle->Text = APPLICATION_NAME;
				 this->labelVersion->Text = APPLICATION_VER_STR;
			 }
};
}
