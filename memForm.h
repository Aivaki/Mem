#pragma once

#include "appClasses.hpp"


/* Form code */


/* Code from another file space */
using namespace code;

namespace mem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Diagnostics;
	using namespace Microsoft::Win32;
	using namespace System::Configuration;

	/* Dependencies */
	LocalPlayer Mem;
	updater::Updater Updater;
	randomName::RandomNameClass Name;

	static const char* appName = Name.name_LPCSTR_tmp(); // Static global exe string for name return
	CSGOMemory mem(L"csgo.exe", memory::SafeMemory_AllAccess, CSGOMemory::ConstructProcessName()); // Getting CSGO process

	System::Void returnExeName() noexcept(true) {
		try {
			std::string filename = appName;

			if (std::filesystem::exists(appName)) {
				rename(filename.c_str(), Name.name_LPCSTR_exe());
			}
			else {
				filename.erase(filename.find('.'), 1);
				if (std::filesystem::exists(filename)) {
					rename(filename.c_str(), Name.name_LPCSTR_exe());
				}
			}
		}
		catch (...) { exit(0); } // Catching something and exiting
	}

	/* Error handlers */
	System::Void customError(Exception^ error) {
		try {
			String^ caption = "Error";
			String^ message = error->ToString();

			DialogResult result = MessageBox::Show(message, caption, MessageBoxButtons::OK, MessageBoxIcon::None);
			if (result == DialogResult::OK) {
				returnExeName();
				return;
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}
	//
	System::Void fileSystemError() noexcept(true) {
		try {
			String^ caption = "Error";
			String^ message = "Last activities folder has not exists.";

			DialogResult result = MessageBox::Show(message, caption, MessageBoxButtons::OK, MessageBoxIcon::None);
			if (result == DialogResult::OK) {
				returnExeName();
				return;
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}
	//
	System::Void error() noexcept(true) {
		try {
			String^ caption = "Error";
			String^ message = "Something went wrong.";

			DialogResult result = MessageBox::Show(message, caption, MessageBoxButtons::OK, MessageBoxIcon::None);
			if (result == DialogResult::OK) {
				returnExeName();
				return;
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}

	/* Getting system folders */
	std::string winDir_stdString() {
		std::string winDir_string;
		try {
			char winDir[256];
			GetWindowsDirectory(winDir, 256);
			winDir_string = winDir;
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting

		return winDir_string;
	}
	//
	std::string systemRoot_stdString() {
		std::string winSysDir_string;
		try {
			char winSysDir[256];
			GetSystemDirectory(winSysDir, 256);
			winSysDir_string = winSysDir;
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting

		return winSysDir_string;
	}
	//
	std::string currentUser_stdString() {
		std::string currentUserDir_string;
		try {
			char* pValue;
			size_t len;
			errno_t err = _dupenv_s(&pValue, &len, "APPDATA");

			currentUserDir_string = pValue;
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting

		return currentUserDir_string;
	}

	/* Setters */
	System::Void setExeName() {
		try {
			msclr::interop::marshal_context^ marshal = gcnew msclr::interop::marshal_context();
			std::string filename = marshal->marshal_as<std::string>(Environment::GetCommandLineArgs()[0]);

			if (std::filesystem::exists(filename)) {
				rename(filename.c_str(), appName);
			}
			else {
				filename += appExtension_stdString;
				rename(filename.c_str(), appName);
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}
	//
	System::Void setExtension() {
		try {
			std::string extension_string = "powershell.exe -NoProfile -ExecutionPolicy Unrestricted -Command \"& {cmd /c 'assoc " + appExtension_stdString + "=exefile' } - Verb RunAs\";"; // Setting extension with admin rights
			WinExec(extension_string.c_str(), SW_HIDE);
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}

	/* For delete files in folder */
	void deleteDirContent(const std::filesystem::path& dir_path) {
		try {
			for (auto& path : std::filesystem::directory_iterator(dir_path)) {
				std::filesystem::remove_all(path);
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}

	/* Deleting all recent activities */
	System::Void deleteLastActivityReg() {
		try {
			if (Registry::CurrentUser->OpenSubKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU")) {
				Registry::CurrentUser->DeleteSubKeyTree("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU");
			}
			if (Registry::CurrentUser->OpenSubKey("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU")) {
				Registry::CurrentUser->DeleteSubKeyTree("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU");
			}
			Registry::CurrentUser->Close();
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}
	//
	System::Void deletePrefetchFolder() {
		try {
			std::error_code errorCode;
			const static std::string prefetchDir = winDir_stdString() + "\\Prefetch";

			if (std::filesystem::exists(prefetchDir)) {
				if (!std::filesystem::remove_all(prefetchDir, errorCode)) { fileSystemError(); }
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}
	//
	System::Void deleteRecentFolder() {
		try {
			const static std::string recentDir = currentUser_stdString() + "\\Microsoft\\Windows\\Recent";

			if (std::filesystem::exists(recentDir)) {
				deleteDirContent(recentDir);
			}
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}

	System::Void createMemSubKey() {
		try {
			if (!Registry::CurrentUser->OpenSubKey("Software\\Classes\\*\\.mem") && Registry::CurrentUser->OpenSubKey("Software\\Classes\\*")) {
				RegistryKey^ key;
				key = Registry::CurrentUser->CreateSubKey("Software\\Classes\\*\\.mem");

				/* Aim */
				key->SetValue("aimState", "false", RegistryValueKind::String);
				key->SetValue("aimMode", "legit", RegistryValueKind::String);
				key->SetValue("aimSmoothMode", "no smooth", RegistryValueKind::String);
				key->SetValue("aimTargetMode", "legs", RegistryValueKind::String);
			}
			Registry::CurrentUser->Close();
		}
		catch (Exception^ e) { customError(e); } // Catching something and exiting
	}

	/* All in one function */
	System::Void launchParam() {
		myApp::AppClass App;

		setExtension(); // Setting extension

		createMemSubKey(); // Creating subkey in registry for cheat settings

		deleteLastActivityReg(); // Deleting last activity register folder

		setExeName(); // Setting name to file

		App.appMutex(); // Creating app mutex

		deletePrefetchFolder(); // Deleting last activity prefetch folder
		deleteRecentFolder(); // Deleting last activity recent folder
	}

	/// <summary>
	/// MyForm
	/// </summary>
	public ref class App : public System::Windows::Forms::Form
	{
	public:

		App(String^)
		{
			launchParam();
			InitializeComponent();
		}

	protected:
		~App()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


	protected:





























	private: System::Windows::Forms::Label^ machineLabel;
	private: System::Windows::Forms::Label^ accountLabel;




	private: System::Windows::Forms::GroupBox^ mainBox;





	private: System::Windows::Forms::FlowLayoutPanel^ aimbotBox;
	private: System::Windows::Forms::CheckBox^ aimbotCheckBox;


	private: System::Windows::Forms::GroupBox^ aimbotGroupBox;
	private: System::Windows::Forms::FlowLayoutPanel^ aimbotFlowLayoutPanel;
	private: System::Windows::Forms::GroupBox^ accountGroupBox;



	private: System::Windows::Forms::CheckBox^ aimbotStateCheckBox;



	private: System::Windows::Forms::LinkLabel^ creatorLinkLabel;





	private: System::Windows::Forms::ToolTip^ toolTip;


	private: System::Windows::Forms::LinkLabel^ teamLinkLabel;
	private: System::Windows::Forms::LinkLabel^ accomplicesLinkLabel;
	private: System::Windows::Forms::GroupBox^ otherGroupBox;
	private: System::Windows::Forms::GroupBox^ machineGroupBox;
private: System::ComponentModel::BackgroundWorker^ aimBackgroundWorker;


private: System::Windows::Forms::CheckBox^ aimTargetCheckBox;
private: System::Windows::Forms::CheckBox^ aimSmoothCheckBox;
private: System::Windows::Forms::LinkLabel^ offsetsLinkLabel;









































	private: System::ComponentModel::IContainer^ components;







































	protected:

	protected:

	private:

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->mainBox = (gcnew System::Windows::Forms::GroupBox());
			this->aimbotBox = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->aimbotGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->aimbotFlowLayoutPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->aimbotStateCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->aimbotCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->aimTargetCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->aimSmoothCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->offsetsLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->accomplicesLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->teamLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->creatorLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->machineLabel = (gcnew System::Windows::Forms::Label());
			this->accountLabel = (gcnew System::Windows::Forms::Label());
			this->accountGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->machineGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->otherGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->aimBackgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->mainBox->SuspendLayout();
			this->aimbotBox->SuspendLayout();
			this->aimbotGroupBox->SuspendLayout();
			this->aimbotFlowLayoutPanel->SuspendLayout();
			this->accountGroupBox->SuspendLayout();
			this->machineGroupBox->SuspendLayout();
			this->otherGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// mainBox
			// 
			this->mainBox->Controls->Add(this->aimbotBox);
			this->mainBox->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->mainBox->Location = System::Drawing::Point(7, 41);
			this->mainBox->Name = L"mainBox";
			this->mainBox->Size = System::Drawing::Size(410, 120);
			this->mainBox->TabIndex = 0;
			this->mainBox->TabStop = false;
			this->mainBox->Text = L"Main";
			this->mainBox->UseCompatibleTextRendering = true;
			// 
			// aimbotBox
			// 
			this->aimbotBox->Controls->Add(this->aimbotGroupBox);
			this->aimbotBox->Location = System::Drawing::Point(6, 28);
			this->aimbotBox->Name = L"aimbotBox";
			this->aimbotBox->Size = System::Drawing::Size(398, 70);
			this->aimbotBox->TabIndex = 2;
			// 
			// aimbotGroupBox
			// 
			this->aimbotGroupBox->Controls->Add(this->aimbotFlowLayoutPanel);
			this->aimbotGroupBox->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->aimbotGroupBox->Location = System::Drawing::Point(3, 3);
			this->aimbotGroupBox->Name = L"aimbotGroupBox";
			this->aimbotGroupBox->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->aimbotGroupBox->Size = System::Drawing::Size(392, 60);
			this->aimbotGroupBox->TabIndex = 0;
			this->aimbotGroupBox->TabStop = false;
			this->aimbotGroupBox->Text = L"Aimbot";
			this->toolTip->SetToolTip(this->aimbotGroupBox, L"Aimbot settings");
			this->aimbotGroupBox->UseCompatibleTextRendering = true;
			// 
			// aimbotFlowLayoutPanel
			// 
			this->aimbotFlowLayoutPanel->Controls->Add(this->aimbotStateCheckBox);
			this->aimbotFlowLayoutPanel->Controls->Add(this->aimbotCheckBox);
			this->aimbotFlowLayoutPanel->Controls->Add(this->aimTargetCheckBox);
			this->aimbotFlowLayoutPanel->Controls->Add(this->aimSmoothCheckBox);
			this->aimbotFlowLayoutPanel->Location = System::Drawing::Point(6, 24);
			this->aimbotFlowLayoutPanel->Name = L"aimbotFlowLayoutPanel";
			this->aimbotFlowLayoutPanel->Size = System::Drawing::Size(380, 30);
			this->aimbotFlowLayoutPanel->TabIndex = 4;
			// 
			// aimbotStateCheckBox
			// 
			this->aimbotStateCheckBox->AutoSize = true;
			this->aimbotStateCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->aimbotStateCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->aimbotStateCheckBox->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->aimbotStateCheckBox->Location = System::Drawing::Point(3, 3);
			this->aimbotStateCheckBox->Name = L"aimbotStateCheckBox";
			this->aimbotStateCheckBox->Size = System::Drawing::Size(45, 24);
			this->aimbotStateCheckBox->TabIndex = 1;
			this->aimbotStateCheckBox->TabStop = false;
			this->aimbotStateCheckBox->Text = L"Off";
			this->aimbotStateCheckBox->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->toolTip->SetToolTip(this->aimbotStateCheckBox, L"Current state of aimbot");
			this->aimbotStateCheckBox->UseCompatibleTextRendering = true;
			this->aimbotStateCheckBox->UseVisualStyleBackColor = true;
			this->aimbotStateCheckBox->CheckedChanged += gcnew System::EventHandler(this, &App::aimbotStateCheckBox_CheckedChanged);
			// 
			// aimbotCheckBox
			// 
			this->aimbotCheckBox->AutoSize = true;
			this->aimbotCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->aimbotCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->aimbotCheckBox->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->aimbotCheckBox->Location = System::Drawing::Point(54, 3);
			this->aimbotCheckBox->Name = L"aimbotCheckBox";
			this->aimbotCheckBox->Size = System::Drawing::Size(57, 24);
			this->aimbotCheckBox->TabIndex = 0;
			this->aimbotCheckBox->TabStop = false;
			this->aimbotCheckBox->Text = L"Legit";
			this->aimbotCheckBox->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->aimbotCheckBox->ThreeState = true;
			this->toolTip->SetToolTip(this->aimbotCheckBox, L"Current mode of aimbot");
			this->aimbotCheckBox->UseCompatibleTextRendering = true;
			this->aimbotCheckBox->UseVisualStyleBackColor = true;
			this->aimbotCheckBox->CheckStateChanged += gcnew System::EventHandler(this, &App::aimbotCheckBox_CheckStateChanged);
			// 
			// aimTargetCheckBox
			// 
			this->aimTargetCheckBox->AutoSize = true;
			this->aimTargetCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->aimTargetCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->aimTargetCheckBox->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->aimTargetCheckBox->Location = System::Drawing::Point(117, 3);
			this->aimTargetCheckBox->Name = L"aimTargetCheckBox";
			this->aimTargetCheckBox->Size = System::Drawing::Size(55, 24);
			this->aimTargetCheckBox->TabIndex = 2;
			this->aimTargetCheckBox->TabStop = false;
			this->aimTargetCheckBox->Text = L"Legs";
			this->aimTargetCheckBox->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->aimTargetCheckBox->ThreeState = true;
			this->toolTip->SetToolTip(this->aimTargetCheckBox, L"Current target mode of aimbot");
			this->aimTargetCheckBox->UseCompatibleTextRendering = true;
			this->aimTargetCheckBox->UseVisualStyleBackColor = true;
			this->aimTargetCheckBox->CheckStateChanged += gcnew System::EventHandler(this, &App::aimTargetCheckBox_CheckStateChanged);
			// 
			// aimSmoothCheckBox
			// 
			this->aimSmoothCheckBox->AutoSize = true;
			this->aimSmoothCheckBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->aimSmoothCheckBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->aimSmoothCheckBox->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->aimSmoothCheckBox->Location = System::Drawing::Point(178, 3);
			this->aimSmoothCheckBox->Name = L"aimSmoothCheckBox";
			this->aimSmoothCheckBox->Size = System::Drawing::Size(104, 24);
			this->aimSmoothCheckBox->TabIndex = 0;
			this->aimSmoothCheckBox->TabStop = false;
			this->aimSmoothCheckBox->Text = L"No smooth";
			this->aimSmoothCheckBox->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->aimSmoothCheckBox->ThreeState = true;
			this->toolTip->SetToolTip(this->aimSmoothCheckBox, L"Current smooth mode of aimbot");
			this->aimSmoothCheckBox->UseCompatibleTextRendering = true;
			this->aimSmoothCheckBox->UseVisualStyleBackColor = true;
			this->aimSmoothCheckBox->CheckStateChanged += gcnew System::EventHandler(this, &App::aimSmoothCheckBox_CheckStateChanged);
			// 
			// offsetsLinkLabel
			// 
			this->offsetsLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlText;
			this->offsetsLinkLabel->AutoEllipsis = true;
			this->offsetsLinkLabel->AutoSize = true;
			this->offsetsLinkLabel->BackColor = System::Drawing::Color::Transparent;
			this->offsetsLinkLabel->DisabledLinkColor = System::Drawing::SystemColors::ControlText;
			this->offsetsLinkLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->offsetsLinkLabel->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
			this->offsetsLinkLabel->LinkColor = System::Drawing::SystemColors::ControlText;
			this->offsetsLinkLabel->Location = System::Drawing::Point(192, 13);
			this->offsetsLinkLabel->Name = L"offsetsLinkLabel";
			this->offsetsLinkLabel->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->offsetsLinkLabel->Size = System::Drawing::Size(92, 22);
			this->offsetsLinkLabel->TabIndex = 5;
			this->offsetsLinkLabel->TabStop = true;
			this->offsetsLinkLabel->Text = L"Latest update: ";
			this->toolTip->SetToolTip(this->offsetsLinkLabel, L"Latest offsets update");
			this->offsetsLinkLabel->UseCompatibleTextRendering = true;
			this->offsetsLinkLabel->VisitedLinkColor = System::Drawing::SystemColors::ControlText;
			this->offsetsLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &App::offsetsLinkLabel_LinkClicked);
			// 
			// accomplicesLinkLabel
			// 
			this->accomplicesLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlText;
			this->accomplicesLinkLabel->AutoEllipsis = true;
			this->accomplicesLinkLabel->AutoSize = true;
			this->accomplicesLinkLabel->BackColor = System::Drawing::Color::Transparent;
			this->accomplicesLinkLabel->DisabledLinkColor = System::Drawing::SystemColors::ControlText;
			this->accomplicesLinkLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->accomplicesLinkLabel->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
			this->accomplicesLinkLabel->LinkColor = System::Drawing::SystemColors::ControlText;
			this->accomplicesLinkLabel->Location = System::Drawing::Point(64, 13);
			this->accomplicesLinkLabel->Name = L"accomplicesLinkLabel";
			this->accomplicesLinkLabel->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->accomplicesLinkLabel->Size = System::Drawing::Size(78, 22);
			this->accomplicesLinkLabel->TabIndex = 4;
			this->accomplicesLinkLabel->TabStop = true;
			this->accomplicesLinkLabel->Text = L"Accomplices";
			this->toolTip->SetToolTip(this->accomplicesLinkLabel, L"People who morally helped in the creation");
			this->accomplicesLinkLabel->UseCompatibleTextRendering = true;
			this->accomplicesLinkLabel->VisitedLinkColor = System::Drawing::SystemColors::ControlText;
			this->accomplicesLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &App::accomplicesLinkLabel_LinkClicked);
			// 
			// teamLinkLabel
			// 
			this->teamLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlText;
			this->teamLinkLabel->AutoEllipsis = true;
			this->teamLinkLabel->AutoSize = true;
			this->teamLinkLabel->BackColor = System::Drawing::Color::Transparent;
			this->teamLinkLabel->DisabledLinkColor = System::Drawing::SystemColors::ControlText;
			this->teamLinkLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->teamLinkLabel->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
			this->teamLinkLabel->LinkColor = System::Drawing::SystemColors::ControlText;
			this->teamLinkLabel->Location = System::Drawing::Point(150, 13);
			this->teamLinkLabel->Name = L"teamLinkLabel";
			this->teamLinkLabel->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->teamLinkLabel->Size = System::Drawing::Size(36, 22);
			this->teamLinkLabel->TabIndex = 3;
			this->teamLinkLabel->TabStop = true;
			this->teamLinkLabel->Text = L"Team";
			this->toolTip->SetToolTip(this->teamLinkLabel, L"Creator team");
			this->teamLinkLabel->UseCompatibleTextRendering = true;
			this->teamLinkLabel->VisitedLinkColor = System::Drawing::SystemColors::ControlText;
			this->teamLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &App::teamLinkLabel_LinkClicked);
			// 
			// creatorLinkLabel
			// 
			this->creatorLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlText;
			this->creatorLinkLabel->AutoEllipsis = true;
			this->creatorLinkLabel->AutoSize = true;
			this->creatorLinkLabel->BackColor = System::Drawing::Color::Transparent;
			this->creatorLinkLabel->DisabledLinkColor = System::Drawing::SystemColors::ControlText;
			this->creatorLinkLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->creatorLinkLabel->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
			this->creatorLinkLabel->LinkColor = System::Drawing::SystemColors::ControlText;
			this->creatorLinkLabel->Location = System::Drawing::Point(6, 13);
			this->creatorLinkLabel->Name = L"creatorLinkLabel";
			this->creatorLinkLabel->Size = System::Drawing::Size(49, 22);
			this->creatorLinkLabel->TabIndex = 0;
			this->creatorLinkLabel->TabStop = true;
			this->creatorLinkLabel->Text = L"Creator";
			this->toolTip->SetToolTip(this->creatorLinkLabel, L"Creator of this cheat");
			this->creatorLinkLabel->UseCompatibleTextRendering = true;
			this->creatorLinkLabel->VisitedLinkColor = System::Drawing::SystemColors::ControlText;
			this->creatorLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &App::creatorLinkLabel_LinkClicked);
			// 
			// machineLabel
			// 
			this->machineLabel->AutoEllipsis = true;
			this->machineLabel->AutoSize = true;
			this->machineLabel->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->machineLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->machineLabel->ForeColor = System::Drawing::SystemColors::ControlText;
			this->machineLabel->Location = System::Drawing::Point(1, 13);
			this->machineLabel->Name = L"machineLabel";
			this->machineLabel->Size = System::Drawing::Size(57, 20);
			this->machineLabel->TabIndex = 0;
			this->machineLabel->Text = L"Machine";
			this->toolTip->SetToolTip(this->machineLabel, L"Current system");
			this->machineLabel->UseCompatibleTextRendering = true;
			// 
			// accountLabel
			// 
			this->accountLabel->AutoEllipsis = true;
			this->accountLabel->AutoSize = true;
			this->accountLabel->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->accountLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->accountLabel->ForeColor = System::Drawing::SystemColors::ControlText;
			this->accountLabel->Location = System::Drawing::Point(1, 13);
			this->accountLabel->Name = L"accountLabel";
			this->accountLabel->Size = System::Drawing::Size(54, 20);
			this->accountLabel->TabIndex = 0;
			this->accountLabel->Text = L"Account";
			this->toolTip->SetToolTip(this->accountLabel, L"Current system account");
			this->accountLabel->UseCompatibleTextRendering = true;
			// 
			// accountGroupBox
			// 
			this->accountGroupBox->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->accountGroupBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->accountGroupBox->Controls->Add(this->accountLabel);
			this->accountGroupBox->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->accountGroupBox->Location = System::Drawing::Point(7, 0);
			this->accountGroupBox->Name = L"accountGroupBox";
			this->accountGroupBox->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->accountGroupBox->Size = System::Drawing::Size(410, 35);
			this->accountGroupBox->TabIndex = 0;
			this->accountGroupBox->TabStop = false;
			this->accountGroupBox->Text = L"Account";
			this->accountGroupBox->UseCompatibleTextRendering = true;
			// 
			// machineGroupBox
			// 
			this->machineGroupBox->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->machineGroupBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->machineGroupBox->Controls->Add(this->machineLabel);
			this->machineGroupBox->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->machineGroupBox->Location = System::Drawing::Point(7, 208);
			this->machineGroupBox->Name = L"machineGroupBox";
			this->machineGroupBox->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->machineGroupBox->Size = System::Drawing::Size(410, 35);
			this->machineGroupBox->TabIndex = 0;
			this->machineGroupBox->TabStop = false;
			this->machineGroupBox->Text = L"Machine";
			this->machineGroupBox->UseCompatibleTextRendering = true;
			// 
			// toolTip
			// 
			this->toolTip->BackColor = System::Drawing::Color::Transparent;
			this->toolTip->ForeColor = System::Drawing::SystemColors::ControlText;
			this->toolTip->StripAmpersands = true;
			// 
			// otherGroupBox
			// 
			this->otherGroupBox->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->otherGroupBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->otherGroupBox->Controls->Add(this->creatorLinkLabel);
			this->otherGroupBox->Controls->Add(this->offsetsLinkLabel);
			this->otherGroupBox->Controls->Add(this->accomplicesLinkLabel);
			this->otherGroupBox->Controls->Add(this->teamLinkLabel);
			this->otherGroupBox->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->otherGroupBox->Location = System::Drawing::Point(7, 167);
			this->otherGroupBox->Name = L"otherGroupBox";
			this->otherGroupBox->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->otherGroupBox->Size = System::Drawing::Size(410, 35);
			this->otherGroupBox->TabIndex = 1;
			this->otherGroupBox->TabStop = false;
			this->otherGroupBox->Text = L"Other";
			this->otherGroupBox->UseCompatibleTextRendering = true;
			// 
			// aimBackgroundWorker
			// 
			this->aimBackgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &App::aimBackgroundWorker_DoWork);
			// 
			// App
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(96, 96);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(424, 251);
			this->Controls->Add(this->mainBox);
			this->Controls->Add(this->otherGroupBox);
			this->Controls->Add(this->machineGroupBox);
			this->Controls->Add(this->accountGroupBox);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Text", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->KeyPreview = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"App";
			this->Opacity = 0.99;
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->TopMost = true;
			this->Activated += gcnew System::EventHandler(this, &App::App_Activated);
			this->Deactivate += gcnew System::EventHandler(this, &App::App_Deactivate);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &App::App_FormClosing);
			this->Load += gcnew System::EventHandler(this, &App::App_Load);
			this->Leave += gcnew System::EventHandler(this, &App::App_Leave);
			this->mainBox->ResumeLayout(false);
			this->aimbotBox->ResumeLayout(false);
			this->aimbotGroupBox->ResumeLayout(false);
			this->aimbotFlowLayoutPanel->ResumeLayout(false);
			this->aimbotFlowLayoutPanel->PerformLayout();
			this->accountGroupBox->ResumeLayout(false);
			this->accountGroupBox->PerformLayout();
			this->machineGroupBox->ResumeLayout(false);
			this->machineGroupBox->PerformLayout();
			this->otherGroupBox->ResumeLayout(false);
			this->otherGroupBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

#pragma region App classes code

		ref class MemRegister
		{
		private:
			RegistryKey^ key;

			Boolean state;
		public:
			MemRegister() {
				if (!Registry::CurrentUser->OpenSubKey("Software\\Classes\\*\\.mem")) {
					createMemSubKey();
				}
				key = Registry::CurrentUser->CreateSubKey("Software\\Classes\\*\\.mem");
			}
			~MemRegister() {
				Registry::CurrentUser->Close();
			}

			///Name - register value name
			String^ get(String^ name) {
				try {
					return key->GetValue(name)->ToString();
				}
				catch (Exception^ e) { customError(e); } // Catching something and exiting

				return nullptr;
			}

			///Name - register value name
			///Value - register value of this name
			System::Void set(String^ name, String^ value) {
				try {
					key->SetValue(name, value, RegistryValueKind::String);
				}
				catch (Exception^ e) { customError(e); } // Catching something and exiting
			}

			///Name - register state
			///( example: String^ bindsState = Cfg.get(Cfg.bindsState); Cfg.loadState(bindsState); )
			Boolean loadState(String^ name) {
				try {
					if (name == falseState) {
						return state = false;
					}
					else { return state = true; }
				}
				catch (Exception^ e) { customError(e); } // Catching something and exiting

				return nullptr;
			}

			String^ trueState = "true";
			String^ falseState = "false";

			String^ aimState = "aimState";
			String^ aimMode = "aimMode";
			String^ aimSmoothMode = "aimSmoothMode";
			String^ aimTargetMode = "aimTargetMode";

			String^ legitMode = "legit";
			String^ semiMode = "semi";
			String^ rageMode = "rage";

			String^ noSmoothMode = "no smooth";
			String^ smoothMode = "smooth";
			String^ maxSmoothMode = "max smooth";

			String^ legsTargetMode = "legs";
			String^ bodyTargetMode = "body";
			String^ headTargetMode = "head";
		};
		MemRegister Cfg;

#pragma endregion App classes code

#pragma region App functions code

		/* App opacity */
		System::Void activeAppOpacity() {
			App::Opacity = 0.999;
		}
		//
		System::Void nonActiveAppOpacity() {
			App::Opacity = 0.0;
		}

		/* Cfg loading */
		System::Void loadAimState() {
			try {
				String^ aimState = Cfg.get(Cfg.aimState);

				if (aimState == "false") {
					aimbotStateCheckBox->CheckState = CheckState::Unchecked;
					Mem.setAimActive(false);
						
					aimbotStateCheckBox->Text = "Off";
				}
				else {
					aimbotStateCheckBox->CheckState = CheckState::Checked;
					Mem.setAimActive(true);

					aimbotStateCheckBox->Text = "On";
				}
			}
			catch (Exception^ e) { customError(e); } // Catching something and exiting
		}
		//
		System::Void loadAimMode() {
			try {
				String^ aimMode = Cfg.get(Cfg.aimMode);

				if (aimMode == "legit") {
					aimbotCheckBox->CheckState = CheckState::Unchecked;
					Mem.setAimFov(1);

					aimbotCheckBox->Text = "Legit";
				}
				else if (aimMode == "semi") {
					aimbotCheckBox->CheckState = CheckState::Checked;
					Mem.setAimFov(7);

					aimbotCheckBox->Text = "Semi";
				}
				else { 
					aimbotCheckBox->CheckState = CheckState::Indeterminate;
					Mem.setAimFov(70);

					aimbotCheckBox->Text = "Rage";
				}
			}
			catch (Exception^ e) { customError(e); } // Catching something and exiting
		}
		//
		System::Void loadAimSmooth() {
			try {
				String^ aimSmoothMode = Cfg.get(Cfg.aimSmoothMode);

				if (aimSmoothMode == "no smooth") {
					aimSmoothCheckBox->CheckState = CheckState::Unchecked;
					Mem.setAimSmooth(1);

					aimSmoothCheckBox->Text = "No smooth";
				}
				else if (aimSmoothMode == "smooth") {
					aimSmoothCheckBox->CheckState = CheckState::Checked;
					Mem.setAimSmooth(3);

					aimSmoothCheckBox->Text = "Smooth";
				}
				else {
					aimSmoothCheckBox->CheckState = CheckState::Indeterminate;
					Mem.setAimSmooth(6);

					aimSmoothCheckBox->Text = "Max smooth";
				}
			}
			catch (Exception^ e) { customError(e); } // Catching something and exiting
		}
		//
		System::Void loadAimTarget() {
			try {
				String^ aimTargetMode = Cfg.get(Cfg.aimTargetMode);

				if (aimTargetMode == "legs") {
					aimTargetCheckBox->CheckState = CheckState::Unchecked; 
					Mem.setAimBone(1);

					aimTargetCheckBox->Text = "Legs";
				}
				else if (aimTargetMode == "body") {
					aimTargetCheckBox->CheckState = CheckState::Checked;
					Mem.setAimBone(6);

					aimTargetCheckBox->Text = "Body";
				}
				else { 
					aimTargetCheckBox->CheckState = CheckState::Indeterminate; 
					Mem.setAimBone(8);

					aimTargetCheckBox->Text = "Head"; }
			}
			catch (Exception^ e) { customError(e); } // Catching something and exiting
		}
		//
		System::Void loadCheckBoxes() {
			try {
				String^ aimState = Cfg.get(Cfg.aimState);

				aimbotStateCheckBox->Checked = Cfg.loadState(aimState); 
				loadAimState();
				loadAimMode();
				loadAimSmooth();
				loadAimTarget();
			}
			catch (Exception^ e) { customError(e); } // Catch something and exiting
		}

#pragma endregion App functions code

#pragma region App events code

		/* App load and closing events */
	private: System::Void App_Load(System::Object^ sender, System::EventArgs^ e) {

		randomName::RandomNameClass AppNameOnLoad;

		loadCheckBoxes(); // Loading all checkboxes

		offsetsLinkLabel->Text += gcnew System::String(Updater.updateDate_string().c_str());

		/* Running cheat threads */
		aimBackgroundWorker->RunWorkerAsync();
		
		App::Text = gcnew System::String(AppNameOnLoad.name_string().c_str()); // Setting a random name for form

		accountLabel->Text = System::Environment::UserName; // Username label
		machineLabel->Text = System::Environment::MachineName; // LocalMachine label
	}
	private: System::Void Closing()
	{
		myApp::AppClass App;

		App.deleteAppMutex(); // Deleting app mutex
	}

		   /* App activated and deactivate events */
	private: System::Void App_Activated(System::Object^ sender, System::EventArgs^ e) {

		randomName::RandomNameClass AppNameOnActivated;

		App::Text = gcnew System::String(AppNameOnActivated.name_string().c_str()); // Setting app form a random name 
		activeAppOpacity(); // Setting app active opacity
	}
	private: System::Void App_Deactivate(System::Object^ sender, System::EventArgs^ e) {
		nonActiveAppOpacity(); // Setting app non-active opacity
	}

#pragma endregion App events code

		   /* Leave and form closing */
	private: System::Void App_Leave(System::Object^ sender, System::EventArgs^ e) {
		returnExeName();
	}
	private: System::Void App_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		returnExeName();
	}

		   /* Checkboxes */
	private: System::Void aimbotCheckBox_CheckStateChanged(System::Object^ sender, System::EventArgs^ e) {
		if (aimbotCheckBox->CheckState.ToString() == "Unchecked") { 
			Cfg.set(Cfg.aimMode, Cfg.legitMode); 
			Mem.setAimFov(1);

			aimbotCheckBox->Text = "Legit";
		}
		if (aimbotCheckBox->CheckState.ToString() == "Checked") { 
			Cfg.set(Cfg.aimMode, Cfg.semiMode);
			Mem.setAimFov(7);
			
			aimbotCheckBox->Text = "Semi"; 
		}
		if (aimbotCheckBox->CheckState.ToString() == "Indeterminate") {
			Cfg.set(Cfg.aimMode, Cfg.rageMode);
			Mem.setAimFov(70);
			
			aimbotCheckBox->Text = "Rage";
		}
	}
		   //
	private: System::Void aimbotStateCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (aimbotStateCheckBox->Checked) {
			Cfg.set(Cfg.aimState, Cfg.trueState);
			Mem.setAimActive(true);
			
			aimbotStateCheckBox->Text = "On";
		}
		else {
			Cfg.set(Cfg.aimState, Cfg.falseState);
			Mem.setAimActive(false);

			aimbotStateCheckBox->Text = "Off";
		}
	}
		   //
	private: System::Void aimTargetCheckBox_CheckStateChanged(System::Object^ sender, System::EventArgs^ e) {
		if (aimTargetCheckBox->CheckState.ToString() == "Unchecked") {
			Cfg.set(Cfg.aimTargetMode, Cfg.legsTargetMode);
			Mem.setAimBone(1);
			
			aimTargetCheckBox->Text = "Legs";
		}
		if (aimTargetCheckBox->CheckState.ToString() == "Checked") {
			Cfg.set(Cfg.aimTargetMode, Cfg.bodyTargetMode); 
			Mem.setAimBone(6);

			aimTargetCheckBox->Text = "Body";
		}
		if (aimTargetCheckBox->CheckState.ToString() == "Indeterminate") {
			Cfg.set(Cfg.aimTargetMode, Cfg.headTargetMode);
			Mem.setAimBone(8);
			
			aimTargetCheckBox->Text = "Head";
		}
	}
		   //
	private: System::Void aimSmoothCheckBox_CheckStateChanged(System::Object^ sender, System::EventArgs^ e) {
		if (aimSmoothCheckBox->CheckState.ToString() == "Unchecked") {
			Cfg.set(Cfg.aimSmoothMode, Cfg.noSmoothMode);
			Mem.setAimSmooth(1);

			aimSmoothCheckBox->Text = "No smooth";
		}
		if (aimSmoothCheckBox->CheckState.ToString() == "Checked") {
			Cfg.set(Cfg.aimSmoothMode, Cfg.smoothMode);
			Mem.setAimSmooth(3);

			aimSmoothCheckBox->Text = "Smooth";
		}
		if (aimSmoothCheckBox->CheckState.ToString() == "Indeterminate") {
			Cfg.set(Cfg.aimSmoothMode, Cfg.maxSmoothMode);
			Mem.setAimSmooth(6);

			aimSmoothCheckBox->Text = "Max smooth";
		}
	}

		   /* Label links */
	private: System::Void creatorLinkLabel_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
		Diagnostics::Process::Start("https://vk.com/aivaki");
	}
		   //
	private: System::Void teamLinkLabel_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
		Diagnostics::Process::Start("https://vk.com/aitopvk");
	}
		   //
	private: System::Void accomplicesLinkLabel_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
		Diagnostics::Process::Start("https://vk.com/qwasf114");
		Diagnostics::Process::Start("https://vk.com/xtoyanaxyi");
	}
		   //
	private: System::Void offsetsLinkLabel_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
		Diagnostics::Process::Start("https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.hpp");
	}

			/* Cheat backgroundworkers */
	private: System::Void aimBackgroundWorker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {
		Mem.aimbot(mem);
	}

};
}