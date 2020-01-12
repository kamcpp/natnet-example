#pragma once
#include <string>

namespace NatNetExample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	using namespace NatNetML;

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
	private: System::Windows::Forms::ListBox^ mLogListBox;

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->mConnectToMotiveButton = (gcnew System::Windows::Forms::Button());
			this->mLogListBox = (gcnew System::Windows::Forms::ListBox());
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
			this->mConnectToMotiveButton->Click += gcnew System::EventHandler(this, &MainForm::ConnectToMotiveButton_Click);
			// 
			// mLogListBox
			// 
			this->mLogListBox->FormattingEnabled = true;
			this->mLogListBox->Location = System::Drawing::Point(12, 55);
			this->mLogListBox->Name = L"mLogListBox";
			this->mLogListBox->Size = System::Drawing::Size(387, 537);
			this->mLogListBox->TabIndex = 1;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(902, 607);
			this->Controls->Add(this->mLogListBox);
			this->Controls->Add(this->mConnectToMotiveButton);
			this->Name = L"MainForm";
			this->Text = L"NatNet Example (by Kamran Amini)";
			this->ResumeLayout(false);

		}
#pragma endregion

	private:

		NatNetClientML^ mNatNetClient;

		System::Void ConnectToMotiveButton_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			String^ strLocalIP = "127.0.0.1";
			String^ strServerIP = "127.0.0.1";

			mNatNetClient = gcnew NatNetClientML();
			// mNatNetClient->OnFrameReady += gcnew FrameReadyEventHandler(this, &MainForm::OnFrameReady);
			// mNatNetClient->OnFrameReady2 += gcnew FrameReadyEventHandler2(this, &MainForm::OnFrameReady2);

			/*
			// [NatNet] for testing only - event signature format required by some types of .NET applications (e.g. MatLab)
			m_NatNet.OnFrameReady2 += new FrameReadyEventHandler2(m_NatNet_OnFrameReady2);
			*/

			// [NatNet] print version info
			array<int>^ ver = gcnew array<int>(4);
			ver = mNatNetClient->NatNetVersion();
			String^ strVersion = String::Format("NatNet Version : {0}.{1}.{2}.{3}", ver[0], ver[1], ver[2], ver[3]);
			Log(strVersion);

			NatNetClientML::ConnectParams^ connectParams = gcnew NatNetClientML::ConnectParams();
			connectParams->ConnectionType = ConnectionType::Unicast;
			connectParams->ServerAddress = strServerIP;
			connectParams->LocalAddress = strLocalIP;
			int returnCode = mNatNetClient->Connect(connectParams);
			if (returnCode == 0)
			{
				Log("Initialization Succeeded.");
			}
			else
			{
				Log("Error Initializing.");
			}

			ServerDescription^ serverDescription = gcnew ServerDescription();
			returnCode = mNatNetClient->GetServerDescription(serverDescription);
			if (returnCode == 0)
			{
				Log("Connection Succeeded.");
				Log("   Server App Name: " + serverDescription->HostApp);
				Log(String::Format("   Server App Version: {0}.{1}.{2}.{3}",
					serverDescription->HostAppVersion[0], serverDescription->HostAppVersion[1],
					serverDescription->HostAppVersion[2], serverDescription->HostAppVersion[3]));
				Log(String::Format("   Server NatNet Version: {0}.{1}.{2}.{3}",
					serverDescription->NatNetVersion[0], serverDescription->NatNetVersion[1],
					serverDescription->NatNetVersion[2], serverDescription->NatNetVersion[3]));

				String^ serverIP = String::Format("{0}.{1}.{2}.{3}",
					serverDescription->HostComputerAddress[0], serverDescription->HostComputerAddress[1],
					serverDescription->HostComputerAddress[2], serverDescription->HostComputerAddress[3]);


				if (serverDescription->HostApp->Contains("TrackingTools") || serverDescription->HostApp->Contains("Motive"))
					float m_ServerToMillimeters = 1000.0f;

				int^ nBytes = gcnew int(0);
				array<unsigned char>^ response = gcnew array<unsigned char>(10000);
				int rc;
				rc = mNatNetClient->SendMessageAndWait("FrameRate", response, *nBytes);
				if (rc == 0)
				{
					try
					{
						float m_ServerFramerate = BitConverter::ToSingle(response, 0);
						Log(String::Format("   Camera Framerate: {0}", m_ServerFramerate));
					}
					catch (Exception ^ ex)
					{
						Log("ERROR: " + ex->Message);
					}
				}

				// [NatNet] [optional] Query mocap server for the current analog framerate
				rc = mNatNetClient->SendMessageAndWait("AnalogSamplesPerMocapFrame", response, *nBytes);
				if (rc == 0)
				{
					try
					{
						int mAnalogSamplesPerMocpaFrame = BitConverter::ToInt32(response, 0);
						Log(String::Format("   Analog Samples Per Camera Frame: {0}", mAnalogSamplesPerMocpaFrame));
					}
					catch (Exception ^ ex)
					{
						Log("ERROR: " + ex->Message);
					}
				}

				// [NatNet] [optional] Query mocap server for the current up axis
				rc = mNatNetClient->SendMessageAndWait("UpAxis", response, *nBytes);
				if (rc == 0)
				{
					int m_UpAxis = BitConverter::ToInt32(response, 0);
				}

				List<DataDescriptor^>^ descs = gcnew List<DataDescriptor^>();
				bool bSuccess = mNatNetClient->GetDataDescriptions(descs);

				Thread^ pollThread = gcnew Thread(gcnew ThreadStart(this, &MainForm::PollThread));
				pollThread->Start();
			}
		}

		void PollThread()
		{
			while (true) {
				FrameOfMocapData^ frame = mNatNetClient->GetLastFrameOfData();
				ProcessFrame(frame);
			}
		}

		void ProcessFrame(FrameOfMocapData^ frame)
		{
			Log("New Frame ...");
		}		

		delegate void UpdateListDelegate(String^ Message);

		void Log(String^ str)
		{
			if (this->InvokeRequired)
			{
				this->Invoke(gcnew UpdateListDelegate(this, &MainForm::LogOnUI), str);
			}
			else
			{
				mLogListBox->Items->Add(str);
			}
		}

		void LogOnUI(String^ str) {
			mLogListBox->Items->Add(str);
		}
	};
}
