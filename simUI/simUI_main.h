#pragma once
#include "Form_About.h"

namespace simUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form_simUI : public System::Windows::Forms::Form
	{
	public:
		Form_simUI(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form_simUI()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip_main;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStripMain;



	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  projectToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  simulationToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStripSimulation;

	private: System::Windows::Forms::ToolStrip^  toolStripIO;

	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolbarsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  iOBarToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  simulationToolbarToolStripMenuItem;
	private: System::Windows::Forms::FontDialog^  fontDialogTextEditor;
	private: System::Windows::Forms::SplitContainer^  splitContainerMain;
	private: System::Windows::Forms::SplitContainer^  splitContainerTextConsole;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::RichTextBox^  richTextBoxConsole;
	private: System::Windows::Forms::RichTextBox^  richTextBoxTextEditor;
	private: System::Windows::Forms::ToolStripMenuItem^  fontToolStripMenuItem;
	private: System::Windows::Forms::FontDialog^  fontDialog1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButtonSimulate;




















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
			this->menuStrip_main = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->projectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->simulationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fontToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolbarsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->iOBarToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->simulationToolbarToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStripMain = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripSimulation = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButtonSimulate = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripIO = (gcnew System::Windows::Forms::ToolStrip());
			this->fontDialogTextEditor = (gcnew System::Windows::Forms::FontDialog());
			this->splitContainerMain = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->splitContainerTextConsole = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->richTextBoxTextEditor = (gcnew System::Windows::Forms::RichTextBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->richTextBoxConsole = (gcnew System::Windows::Forms::RichTextBox());
			this->fontDialog1 = (gcnew System::Windows::Forms::FontDialog());
			this->menuStrip_main->SuspendLayout();
			this->toolStripSimulation->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerMain))->BeginInit();
			this->splitContainerMain->Panel1->SuspendLayout();
			this->splitContainerMain->Panel2->SuspendLayout();
			this->splitContainerMain->SuspendLayout();
			this->tabControl1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerTextConsole))->BeginInit();
			this->splitContainerTextConsole->Panel1->SuspendLayout();
			this->splitContainerTextConsole->Panel2->SuspendLayout();
			this->splitContainerTextConsole->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip_main
			// 
			this->menuStrip_main->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->projectToolStripMenuItem, this->simulationToolStripMenuItem, this->viewToolStripMenuItem, 
				this->toolsToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip_main->Location = System::Drawing::Point(0, 0);
			this->menuStrip_main->Name = L"menuStrip_main";
			this->menuStrip_main->Size = System::Drawing::Size(993, 24);
			this->menuStrip_main->TabIndex = 0;
			this->menuStrip_main->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// projectToolStripMenuItem
			// 
			this->projectToolStripMenuItem->Name = L"projectToolStripMenuItem";
			this->projectToolStripMenuItem->Size = System::Drawing::Size(56, 20);
			this->projectToolStripMenuItem->Text = L"Project";
			// 
			// simulationToolStripMenuItem
			// 
			this->simulationToolStripMenuItem->Name = L"simulationToolStripMenuItem";
			this->simulationToolStripMenuItem->Size = System::Drawing::Size(76, 20);
			this->simulationToolStripMenuItem->Text = L"Simulation";
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fontToolStripMenuItem, 
				this->toolbarsToolStripMenuItem});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// fontToolStripMenuItem
			// 
			this->fontToolStripMenuItem->Name = L"fontToolStripMenuItem";
			this->fontToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->fontToolStripMenuItem->Text = L"Font";
			this->fontToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_simUI::fontToolStripMenuItem_Click);
			// 
			// toolbarsToolStripMenuItem
			// 
			this->toolbarsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->iOBarToolStripMenuItem, 
				this->simulationToolbarToolStripMenuItem});
			this->toolbarsToolStripMenuItem->Name = L"toolbarsToolStripMenuItem";
			this->toolbarsToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->toolbarsToolStripMenuItem->Text = L"Toolbars";
			// 
			// iOBarToolStripMenuItem
			// 
			this->iOBarToolStripMenuItem->Checked = true;
			this->iOBarToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->iOBarToolStripMenuItem->Name = L"iOBarToolStripMenuItem";
			this->iOBarToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->iOBarToolStripMenuItem->Text = L"I/O Toolbar";
			this->iOBarToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_simUI::iOBarToolStripMenuItem_Click);
			// 
			// simulationToolbarToolStripMenuItem
			// 
			this->simulationToolbarToolStripMenuItem->Checked = true;
			this->simulationToolbarToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->simulationToolbarToolStripMenuItem->Name = L"simulationToolbarToolStripMenuItem";
			this->simulationToolbarToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->simulationToolbarToolStripMenuItem->Text = L"Simulation Toolbar";
			this->simulationToolbarToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_simUI::simulationToolbarToolStripMenuItem_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_simUI::aboutToolStripMenuItem_Click);
			// 
			// statusStripMain
			// 
			this->statusStripMain->Location = System::Drawing::Point(0, 554);
			this->statusStripMain->Name = L"statusStripMain";
			this->statusStripMain->Size = System::Drawing::Size(993, 22);
			this->statusStripMain->TabIndex = 3;
			this->statusStripMain->Text = L"statusStrip1";
			// 
			// toolStripSimulation
			// 
			this->toolStripSimulation->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStripSimulation->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripButtonSimulate});
			this->toolStripSimulation->Location = System::Drawing::Point(0, 49);
			this->toolStripSimulation->Name = L"toolStripSimulation";
			this->toolStripSimulation->Size = System::Drawing::Size(993, 25);
			this->toolStripSimulation->TabIndex = 2;
			// 
			// toolStripButtonSimulate
			// 
			this->toolStripButtonSimulate->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripButtonSimulate->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonSimulate->Name = L"toolStripButtonSimulate";
			this->toolStripButtonSimulate->Size = System::Drawing::Size(57, 22);
			this->toolStripButtonSimulate->Text = L"Simulate";
			this->toolStripButtonSimulate->Click += gcnew System::EventHandler(this, &Form_simUI::toolStripButtonSimulate_Click);
			// 
			// toolStripIO
			// 
			this->toolStripIO->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStripIO->Location = System::Drawing::Point(0, 24);
			this->toolStripIO->Name = L"toolStripIO";
			this->toolStripIO->Size = System::Drawing::Size(993, 25);
			this->toolStripIO->TabIndex = 1;
			// 
			// splitContainerMain
			// 
			this->splitContainerMain->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainerMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerMain->Location = System::Drawing::Point(0, 74);
			this->splitContainerMain->Name = L"splitContainerMain";
			// 
			// splitContainerMain.Panel1
			// 
			this->splitContainerMain->Panel1->Controls->Add(this->tabControl1);
			// 
			// splitContainerMain.Panel2
			// 
			this->splitContainerMain->Panel2->Controls->Add(this->splitContainerTextConsole);
			this->splitContainerMain->Size = System::Drawing::Size(993, 480);
			this->splitContainerMain->SplitterDistance = 143;
			this->splitContainerMain->TabIndex = 4;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(139, 476);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(131, 450);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(131, 450);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// splitContainerTextConsole
			// 
			this->splitContainerTextConsole->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainerTextConsole->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerTextConsole->Location = System::Drawing::Point(0, 0);
			this->splitContainerTextConsole->Name = L"splitContainerTextConsole";
			this->splitContainerTextConsole->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerTextConsole.Panel1
			// 
			this->splitContainerTextConsole->Panel1->Controls->Add(this->tabControl2);
			// 
			// splitContainerTextConsole.Panel2
			// 
			this->splitContainerTextConsole->Panel2->Controls->Add(this->richTextBoxConsole);
			this->splitContainerTextConsole->Size = System::Drawing::Size(842, 476);
			this->splitContainerTextConsole->SplitterDistance = 343;
			this->splitContainerTextConsole->TabIndex = 0;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPage3);
			this->tabControl2->Controls->Add(this->tabPage4);
			this->tabControl2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl2->Location = System::Drawing::Point(0, 0);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(840, 341);
			this->tabControl2->TabIndex = 0;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->richTextBoxTextEditor);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(832, 315);
			this->tabPage3->TabIndex = 0;
			this->tabPage3->Text = L"tabPage3";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// richTextBoxTextEditor
			// 
			this->richTextBoxTextEditor->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBoxTextEditor->Location = System::Drawing::Point(3, 3);
			this->richTextBoxTextEditor->Name = L"richTextBoxTextEditor";
			this->richTextBoxTextEditor->Size = System::Drawing::Size(826, 309);
			this->richTextBoxTextEditor->TabIndex = 0;
			this->richTextBoxTextEditor->Text = L"";
			// 
			// tabPage4
			// 
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(832, 315);
			this->tabPage4->TabIndex = 1;
			this->tabPage4->Text = L"tabPage4";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// richTextBoxConsole
			// 
			this->richTextBoxConsole->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBoxConsole->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBoxConsole->Location = System::Drawing::Point(0, 0);
			this->richTextBoxConsole->Name = L"richTextBoxConsole";
			this->richTextBoxConsole->ReadOnly = true;
			this->richTextBoxConsole->Size = System::Drawing::Size(840, 127);
			this->richTextBoxConsole->TabIndex = 0;
			this->richTextBoxConsole->Text = L"";
			// 
			// Form_simUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(993, 576);
			this->Controls->Add(this->splitContainerMain);
			this->Controls->Add(this->statusStripMain);
			this->Controls->Add(this->toolStripSimulation);
			this->Controls->Add(this->toolStripIO);
			this->Controls->Add(this->menuStrip_main);
			this->MainMenuStrip = this->menuStrip_main;
			this->Name = L"Form_simUI";
			this->Text = L"simUI";
			this->Load += gcnew System::EventHandler(this, &Form_simUI::Form_simUI_Load);
			this->menuStrip_main->ResumeLayout(false);
			this->menuStrip_main->PerformLayout();
			this->toolStripSimulation->ResumeLayout(false);
			this->toolStripSimulation->PerformLayout();
			this->splitContainerMain->Panel1->ResumeLayout(false);
			this->splitContainerMain->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerMain))->EndInit();
			this->splitContainerMain->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->splitContainerTextConsole->Panel1->ResumeLayout(false);
			this->splitContainerTextConsole->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerTextConsole))->EndInit();
			this->splitContainerTextConsole->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tabPage3->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void Form_simUI_Load(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void simulationToolbarToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			if(this->toolStripSimulation->Visible)
			{
				 this->toolStripSimulation->Visible = false;
				 this->simulationToolbarToolStripMenuItem->Checked = false;
			}
			else
			{
				this->toolStripSimulation->Visible = true;
				this->simulationToolbarToolStripMenuItem->Checked = true;
			}
		 }
private: System::Void iOBarToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(this->toolStripIO->Visible)
			 {
				this->toolStripIO->Visible = false;
				this->iOBarToolStripMenuItem->Checked = false;
			 }
			 else
			 {
				 this->toolStripIO->Visible = true;
				 this->iOBarToolStripMenuItem->Checked = true;
			 }
		 }
private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			Form_About^ about = gcnew Form_About();
			about->ShowDialog();
		 }
private: System::Void fontToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 fontDialogTextEditor->ShowDialog();
		 }
private: System::Void toolStripButtonSimulate_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->richTextBoxConsole->Text += "Starting new instance of MIPS Tools...\n";
			 
		 }
};
}

