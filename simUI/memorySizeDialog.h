#pragma once

namespace simUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for memorySizeDialog
	/// </summary>
	public ref class memorySizeDialog : public System::Windows::Forms::Form
	{
	public:
		memorySizeDialog(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		int getMemoryBits()
		{
			return this->trackBarMemBits->Value;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~memorySizeDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TrackBar^  trackBarMemBits;
	protected: 



	private: System::Windows::Forms::GroupBox^  groupBoxMBit;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  labelOneBit;
	private: System::Windows::Forms::Label^  labelTMS;

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
			this->trackBarMemBits = (gcnew System::Windows::Forms::TrackBar());
			this->groupBoxMBit = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->labelOneBit = (gcnew System::Windows::Forms::Label());
			this->labelTMS = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBarMemBits))->BeginInit();
			this->groupBoxMBit->SuspendLayout();
			this->SuspendLayout();
			// 
			// trackBarMemBits
			// 
			this->trackBarMemBits->Location = System::Drawing::Point(6, 19);
			this->trackBarMemBits->Maximum = 20;
			this->trackBarMemBits->Minimum = 2;
			this->trackBarMemBits->Name = L"trackBarMemBits";
			this->trackBarMemBits->Size = System::Drawing::Size(458, 45);
			this->trackBarMemBits->TabIndex = 0;
			this->trackBarMemBits->Value = 2;
			this->trackBarMemBits->ValueChanged += gcnew System::EventHandler(this, &memorySizeDialog::trackBarMemBits_ValueChanged);
			// 
			// groupBoxMBit
			// 
			this->groupBoxMBit->Controls->Add(this->label1);
			this->groupBoxMBit->Controls->Add(this->labelOneBit);
			this->groupBoxMBit->Controls->Add(this->trackBarMemBits);
			this->groupBoxMBit->Location = System::Drawing::Point(12, 12);
			this->groupBoxMBit->Name = L"groupBoxMBit";
			this->groupBoxMBit->Size = System::Drawing::Size(470, 120);
			this->groupBoxMBit->TabIndex = 4;
			this->groupBoxMBit->TabStop = false;
			this->groupBoxMBit->Text = L"Memory Bit Width";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(426, 51);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"20 bits";
			// 
			// labelOneBit
			// 
			this->labelOneBit->AutoSize = true;
			this->labelOneBit->Location = System::Drawing::Point(6, 51);
			this->labelOneBit->Name = L"labelOneBit";
			this->labelOneBit->Size = System::Drawing::Size(32, 13);
			this->labelOneBit->TabIndex = 1;
			this->labelOneBit->Text = L"2 bits";
			// 
			// labelTMS
			// 
			this->labelTMS->AutoSize = true;
			this->labelTMS->Location = System::Drawing::Point(10, 135);
			this->labelTMS->Name = L"labelTMS";
			this->labelTMS->Size = System::Drawing::Size(134, 13);
			this->labelTMS->TabIndex = 5;
			this->labelTMS->Text = L"Total Memory Size: 4 bytes";
			// 
			// memorySizeDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(494, 182);
			this->Controls->Add(this->labelTMS);
			this->Controls->Add(this->groupBoxMBit);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"memorySizeDialog";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->Text = L"Memory Size";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBarMemBits))->EndInit();
			this->groupBoxMBit->ResumeLayout(false);
			this->groupBoxMBit->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void trackBarMemBits_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 labelTMS->Text = "Total Memory Size: " + (2 << (trackBarMemBits->Value - 1)) + " bytes";
			 }
};
}
