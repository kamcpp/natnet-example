#pragma once

namespace NatNetExample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
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
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ mConnectToMotiveButton;
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
			this->mConnectToMotiveButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// mConnectToMotiveButton
			// 
			this->mConnectToMotiveButton->Location = System::Drawing::Point(12, 12);
			this->mConnectToMotiveButton->Name = L"mConnectToMotiveButton";
			this->mConnectToMotiveButton->Size = System::Drawing::Size(165, 37);
			this->mConnectToMotiveButton->TabIndex = 0;
			this->mConnectToMotiveButton->Text = L"Connect To Motive";
			this->mConnectToMotiveButton->UseVisualStyleBackColor = true;
			this->mConnectToMotiveButton->Click += gcnew System::EventHandler(this, &MainForm::mConnectToMotiveButton_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(525, 417);
			this->Controls->Add(this->mConnectToMotiveButton);
			this->Name = L"MainForm";
			this->Text = L"NatNet Example (by Kamran Amini)";
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void mConnectToMotiveButton_Click(System::Object^ sender, System::EventArgs^ e) {
		MessageBox::Show("HELLO");
	}
	};
}
