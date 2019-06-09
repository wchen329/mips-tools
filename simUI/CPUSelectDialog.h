#pragma once

namespace simUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for CPUSelectDialog
	/// </summary>
	public ref class CPUSelectDialog : public System::Windows::Forms::Form
	{
	public:
		CPUSelectDialog(void)
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
		~CPUSelectDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBoxCPUOps;
	private: System::Windows::Forms::RadioButton^  radioButton_CPU3;

	private: System::Windows::Forms::RadioButton^  radioButtonCPU2;

	private: System::Windows::Forms::RadioButton^  radioButton_CPU1;

	private: System::Windows::Forms::RadioButton^  radioButton_CPU0;

	public: int getCPUType()
			{
				if(this->radioButton_CPU0->Checked)
				{
					return 0;
				}
				else if(this->radioButton_CPU1->Checked)
				{
					return 1;
				}
				else
				{
					return 0;
				}

			}

	protected: 

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
			this->groupBoxCPUOps = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton_CPU0 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton_CPU1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCPU2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton_CPU3 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBoxCPUOps->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBoxCPUOps
			// 
			this->groupBoxCPUOps->Controls->Add(this->radioButton_CPU3);
			this->groupBoxCPUOps->Controls->Add(this->radioButtonCPU2);
			this->groupBoxCPUOps->Controls->Add(this->radioButton_CPU1);
			this->groupBoxCPUOps->Controls->Add(this->radioButton_CPU0);
			this->groupBoxCPUOps->Location = System::Drawing::Point(12, 12);
			this->groupBoxCPUOps->Name = L"groupBoxCPUOps";
			this->groupBoxCPUOps->Size = System::Drawing::Size(509, 177);
			this->groupBoxCPUOps->TabIndex = 0;
			this->groupBoxCPUOps->TabStop = false;
			this->groupBoxCPUOps->Text = L"CPU Options";
			// 
			// radioButton_CPU0
			// 
			this->radioButton_CPU0->AutoSize = true;
			this->radioButton_CPU0->Checked = true;
			this->radioButton_CPU0->Location = System::Drawing::Point(6, 19);
			this->radioButton_CPU0->Name = L"radioButton_CPU0";
			this->radioButton_CPU0->Size = System::Drawing::Size(211, 17);
			this->radioButton_CPU0->TabIndex = 0;
			this->radioButton_CPU0->TabStop = true;
			this->radioButton_CPU0->Text = L"Single Cycle, Standard MIPS Processor";
			this->radioButton_CPU0->UseVisualStyleBackColor = true;
			// 
			// radioButton_CPU1
			// 
			this->radioButton_CPU1->AutoSize = true;
			this->radioButton_CPU1->Location = System::Drawing::Point(6, 42);
			this->radioButton_CPU1->Name = L"radioButton_CPU1";
			this->radioButton_CPU1->Size = System::Drawing::Size(186, 17);
			this->radioButton_CPU1->TabIndex = 1;
			this->radioButton_CPU1->Text = L"Five Stage Pipeline (MIPS R4000)";
			this->radioButton_CPU1->UseVisualStyleBackColor = true;
			// 
			// radioButtonCPU2
			// 
			this->radioButtonCPU2->AutoSize = true;
			this->radioButtonCPU2->Enabled = false;
			this->radioButtonCPU2->Location = System::Drawing::Point(6, 65);
			this->radioButtonCPU2->Name = L"radioButtonCPU2";
			this->radioButtonCPU2->Size = System::Drawing::Size(175, 17);
			this->radioButtonCPU2->TabIndex = 2;
			this->radioButtonCPU2->Text = L"Superscalar (MIPS R10k Clone)";
			this->radioButtonCPU2->UseVisualStyleBackColor = true;
			// 
			// radioButton_CPU3
			// 
			this->radioButton_CPU3->AutoSize = true;
			this->radioButton_CPU3->Enabled = false;
			this->radioButton_CPU3->Location = System::Drawing::Point(6, 88);
			this->radioButton_CPU3->Name = L"radioButton_CPU3";
			this->radioButton_CPU3->Size = System::Drawing::Size(72, 17);
			this->radioButton_CPU3->TabIndex = 3;
			this->radioButton_CPU3->TabStop = true;
			this->radioButton_CPU3->Text = L"Intel 8086";
			this->radioButton_CPU3->UseVisualStyleBackColor = true;
			// 
			// CPUSelectDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(534, 212);
			this->Controls->Add(this->groupBoxCPUOps);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CPUSelectDialog";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->Text = L"CPU Type Selection";
			this->groupBoxCPUOps->ResumeLayout(false);
			this->groupBoxCPUOps->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
