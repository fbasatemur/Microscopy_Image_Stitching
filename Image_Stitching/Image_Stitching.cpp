#include "Image_Stitching.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Image_Stitching::Image_Stitching form;
	Application::Run(% form);
}