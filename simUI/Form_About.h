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
	private: System::Diagnostics::Process^  process1;
	private: System::Windows::Forms::Label^  labelTitle;
	private: System::Windows::Forms::Label^  labelVersion;

	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->process1 = (gcnew System::Diagnostics::Process());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->labelVersion = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// process1
			// 
			this->process1->StartInfo->Domain = L"";
			this->process1->StartInfo->LoadUserProfile = false;
			this->process1->StartInfo->Password = nullptr;
			this->process1->StartInfo->StandardErrorEncoding = nullptr;
			this->process1->StartInfo->StandardOutputEncoding = nullptr;
			this->process1->StartInfo->UserName = L"";
			this->process1->SynchronizingObject = this;
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
			// Form_About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 56);
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
