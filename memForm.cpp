#include "memForm.h"

/* Main entry point */

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ argv) {
	
	System::String^ fileName = nullptr;
	if (argv->Length == 1) fileName = argv[0];

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew mem::App(fileName));

	return 0;
}
