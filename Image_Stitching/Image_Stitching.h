#pragma once
#include <atlstr.h>
#include <msclr\marshal_cppstd.h>

#include "Image.h"
#include "Process.h"
#include "Homography.h"
#include "Panorama.h"
#include "Correlation.h"
#include "Dots.h"


namespace Image_Stitching {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class Image_Stitching : public System::Windows::Forms::Form
	{
	public:
		Image_Stitching(void)
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
		~Image_Stitching()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::PictureBox^ pictureBox4;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ labelImg2Dot4;
	private: System::Windows::Forms::Label^ labelImg2Dot3;
	private: System::Windows::Forms::Label^ labelImg2Dot2;
	private: System::Windows::Forms::Label^ labelImg2Dot1;
	private: System::Windows::Forms::Label^ labelResultS;

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
		/// 
		BYTE* buffer1, * buffer2;
		bool isFirstLine = true;
		int currCornerID;
		xy* currVec = new xy;
		xy* prevVec = new xy;
		xy* pocDot = new xy;
		xy* prevPanoSize = new xy;
		xy* currPanoSize;
		int lineCounter;
		BYTE2** LaplacePyramid1;
		BYTE2** LaplacePyramid2;
		BYTE* panorama1;
		BYTE* panorama2;
		int imgCounter;
		BYTE* intensity1;
		BYTE* intensity2;

		double** outReal = new double* [2];
		double** outImag = new double* [2];
		double** match1 = new double* [3];
		double** match2 = new double* [3];

	private: System::Windows::Forms::Label^ imgIndexLbl;
	private: System::Windows::Forms::Label^ label14;


	private: System::Windows::Forms::Button^ saveButton;

		   void FreePictureBox(PictureBox^ pictureBox) {
			   delete[] pictureBox->Image;
			   pictureBox->Image = nullptr;
		   }

		   void ShowColorImage(BYTE* Image, int width, int height) {

			   FreePictureBox(pictureBox4);
			   Bitmap^ surface = gcnew Bitmap(width, height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			   pictureBox4->Width = width;
			   pictureBox4->Height = height;
			   pictureBox4->Image = surface;

			   Color c;
			   int psw, bufpos, row, column;
			   psw = width * 3;
			   for (row = 0; row < height; row++) {
				   for (column = 0; column < width; column++) {
					   bufpos = (height - row - 1) * psw + column * 3;
					   c = Color::FromArgb(Image[bufpos + 2], Image[bufpos + 1], Image[bufpos]);
					   surface->SetPixel(column, row, c);
				   }
			   }

			   pictureBox4->Refresh();
		   }

		   void MaxPOC(double* POC, xy* pocDot, int width, int height) {

			   double max = -10000000.0;
			   int i = 0;
			   for (int row = 0; row < height; row++)
				   for (int col = 0; col < width; col++)
				   {
					   i = row * width + col;

					   if (POC[i] > max) {
						   max = POC[i];
						   pocDot->y = row;
						   pocDot->x = col;
					   }
				   }
		   }

		   /*
		   * cornerID
		   *
			   0 ********* 1
			   * 		   *
			   * 		   *
			   * 		   *
			   2 ********* 3
		   */
		   void LineUpdate(int cornerID, xy* vec) {

			   switch (cornerID)
			   {
			   case 0:
				   lineCounter += vec->y;
				   break;

			   case 1:
				   lineCounter += vec->y;
				   break;

			   case 2:
				   lineCounter -= vec->y;
				   if (lineCounter < 0) lineCounter = 0;
				   break;

			   case 3:
				   lineCounter -= vec->y;
				   if (lineCounter < 0) lineCounter = 0;
				   break;

			   default:
				   break;
			   }
		   }

		   /*void ShowImageIntensity(BYTE* img1, BYTE* img2, int width, int height) {

			   Bitmap^ surface1 = gcnew Bitmap(width, height);
			   Bitmap^ surface2 = gcnew Bitmap(width, height);
			   pictureBox1->Image = surface1;
			   pictureBox2->Image = surface2;

			   Color c1, c2;
			   for (int row = 0; row < height; row++) {
				   for (int column = 0; column < width; column++) {

					   c1 = Color::FromArgb(img1[row * width + column], img1[row * width + column], img1[row * width + column]);
					   c2 = Color::FromArgb(img2[row * width + column], img2[row * width + column], img2[row * width + column]);
					   surface1->SetPixel(column, row, c1);
					   surface2->SetPixel(column, row, c2);
				   }
			   }

			   pictureBox1->Refresh();
			   pictureBox2->Refresh();

		   }*/

		   void InitializeComponent(void)
		   {
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			   this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			   this->label8 = (gcnew System::Windows::Forms::Label());
			   this->label9 = (gcnew System::Windows::Forms::Label());
			   this->label10 = (gcnew System::Windows::Forms::Label());
			   this->label11 = (gcnew System::Windows::Forms::Label());
			   this->labelImg2Dot4 = (gcnew System::Windows::Forms::Label());
			   this->labelImg2Dot3 = (gcnew System::Windows::Forms::Label());
			   this->labelImg2Dot2 = (gcnew System::Windows::Forms::Label());
			   this->labelImg2Dot1 = (gcnew System::Windows::Forms::Label());
			   this->labelResultS = (gcnew System::Windows::Forms::Label());
			   this->imgIndexLbl = (gcnew System::Windows::Forms::Label());
			   this->label14 = (gcnew System::Windows::Forms::Label());
			   this->saveButton = (gcnew System::Windows::Forms::Button());
			   this->menuStrip1->SuspendLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // menuStrip1
			   // 
			   this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			   this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			   this->menuStrip1->Location = System::Drawing::Point(0, 0);
			   this->menuStrip1->Name = L"menuStrip1";
			   this->menuStrip1->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
			   this->menuStrip1->Size = System::Drawing::Size(1924, 28);
			   this->menuStrip1->TabIndex = 2;
			   this->menuStrip1->Text = L"menuStrip1";
			   // 
			   // fileToolStripMenuItem
			   // 
			   this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			   this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			   this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
			   this->fileToolStripMenuItem->Text = L"File";
			   // 
			   // openToolStripMenuItem
			   // 
			   this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			   this->openToolStripMenuItem->Size = System::Drawing::Size(128, 26);
			   this->openToolStripMenuItem->Text = L"Open";
			   this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Image_Stitching::openToolStripMenuItem_Click);
			   // 
			   // openFileDialog1
			   // 
			   this->openFileDialog1->FileName = L"openFileDialog1";
			   this->openFileDialog1->Filter = L"bmp files (*.bmp)|*.bmp";
			   this->openFileDialog1->Multiselect = true;
			   // 
			   // pictureBox4
			   // 
			   this->pictureBox4->Location = System::Drawing::Point(27, 81);
			   this->pictureBox4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			   this->pictureBox4->Name = L"pictureBox4";
			   this->pictureBox4->Size = System::Drawing::Size(800, 754);
			   this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			   this->pictureBox4->TabIndex = 7;
			   this->pictureBox4->TabStop = false;
			   // 
			   // label8
			   // 
			   this->label8->AutoSize = true;
			   this->label8->Location = System::Drawing::Point(1853, 1018);
			   this->label8->Name = L"label8";
			   this->label8->Size = System::Drawing::Size(46, 17);
			   this->label8->TabIndex = 29;
			   this->label8->Text = L"Dot 4:";
			   // 
			   // label9
			   // 
			   this->label9->AutoSize = true;
			   this->label9->Location = System::Drawing::Point(1853, 988);
			   this->label9->Name = L"label9";
			   this->label9->Size = System::Drawing::Size(46, 17);
			   this->label9->TabIndex = 28;
			   this->label9->Text = L"Dot 3:";
			   // 
			   // label10
			   // 
			   this->label10->AutoSize = true;
			   this->label10->Location = System::Drawing::Point(1663, 1015);
			   this->label10->Name = L"label10";
			   this->label10->Size = System::Drawing::Size(46, 17);
			   this->label10->TabIndex = 27;
			   this->label10->Text = L"Dot 2:";
			   // 
			   // label11
			   // 
			   this->label11->AutoSize = true;
			   this->label11->Location = System::Drawing::Point(1663, 988);
			   this->label11->Name = L"label11";
			   this->label11->Size = System::Drawing::Size(50, 17);
			   this->label11->TabIndex = 26;
			   this->label11->Text = L"Dot 1: ";
			   // 
			   // labelImg2Dot4
			   // 
			   this->labelImg2Dot4->AutoSize = true;
			   this->labelImg2Dot4->Location = System::Drawing::Point(1905, 1018);
			   this->labelImg2Dot4->Name = L"labelImg2Dot4";
			   this->labelImg2Dot4->Size = System::Drawing::Size(20, 17);
			   this->labelImg2Dot4->TabIndex = 24;
			   this->labelImg2Dot4->Text = L"...";
			   // 
			   // labelImg2Dot3
			   // 
			   this->labelImg2Dot3->AutoSize = true;
			   this->labelImg2Dot3->Location = System::Drawing::Point(1905, 988);
			   this->labelImg2Dot3->Name = L"labelImg2Dot3";
			   this->labelImg2Dot3->Size = System::Drawing::Size(20, 17);
			   this->labelImg2Dot3->TabIndex = 23;
			   this->labelImg2Dot3->Text = L"...";
			   // 
			   // labelImg2Dot2
			   // 
			   this->labelImg2Dot2->AutoSize = true;
			   this->labelImg2Dot2->Location = System::Drawing::Point(1715, 1015);
			   this->labelImg2Dot2->Name = L"labelImg2Dot2";
			   this->labelImg2Dot2->Size = System::Drawing::Size(20, 17);
			   this->labelImg2Dot2->TabIndex = 22;
			   this->labelImg2Dot2->Text = L"...";
			   // 
			   // labelImg2Dot1
			   // 
			   this->labelImg2Dot1->AutoSize = true;
			   this->labelImg2Dot1->Location = System::Drawing::Point(1715, 988);
			   this->labelImg2Dot1->Name = L"labelImg2Dot1";
			   this->labelImg2Dot1->Size = System::Drawing::Size(20, 17);
			   this->labelImg2Dot1->TabIndex = 21;
			   this->labelImg2Dot1->Text = L"...";
			   // 
			   // labelResultS
			   // 
			   this->labelResultS->AutoSize = true;
			   this->labelResultS->Location = System::Drawing::Point(24, 50);
			   this->labelResultS->Name = L"labelResultS";
			   this->labelResultS->Size = System::Drawing::Size(20, 17);
			   this->labelResultS->TabIndex = 31;
			   this->labelResultS->Text = L"...";
			   // 
			   // imgIndexLbl
			   // 
			   this->imgIndexLbl->AutoSize = true;
			   this->imgIndexLbl->Location = System::Drawing::Point(211, 50);
			   this->imgIndexLbl->Name = L"imgIndexLbl";
			   this->imgIndexLbl->Size = System::Drawing::Size(20, 17);
			   this->imgIndexLbl->TabIndex = 35;
			   this->imgIndexLbl->Text = L"...";
			   // 
			   // label14
			   // 
			   this->label14->AutoSize = true;
			   this->label14->Location = System::Drawing::Point(147, 50);
			   this->label14->Name = L"label14";
			   this->label14->Size = System::Drawing::Size(58, 17);
			   this->label14->TabIndex = 34;
			   this->label14->Text = L"image : ";
			   // 
			   // saveButton
			   // 
			   this->saveButton->Location = System::Drawing::Point(755, 40);
			   this->saveButton->Name = L"saveButton";
			   this->saveButton->Size = System::Drawing::Size(72, 36);
			   this->saveButton->TabIndex = 36;
			   this->saveButton->Text = L"Save";
			   this->saveButton->UseVisualStyleBackColor = true;
			   this->saveButton->Click += gcnew System::EventHandler(this, &Image_Stitching::saveButton_Click);
			   // 
			   // Image_Stitching
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1924, 846);
			   this->Controls->Add(this->saveButton);
			   this->Controls->Add(this->imgIndexLbl);
			   this->Controls->Add(this->label14);
			   this->Controls->Add(this->labelResultS);
			   this->Controls->Add(this->label8);
			   this->Controls->Add(this->label9);
			   this->Controls->Add(this->label10);
			   this->Controls->Add(this->label11);
			   this->Controls->Add(this->labelImg2Dot4);
			   this->Controls->Add(this->labelImg2Dot3);
			   this->Controls->Add(this->labelImg2Dot2);
			   this->Controls->Add(this->labelImg2Dot1);
			   this->Controls->Add(this->pictureBox4);
			   this->Controls->Add(this->menuStrip1);
			   this->MainMenuStrip = this->menuStrip1;
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"Image_Stitching";
			   this->Text = L"Image Stitching";
			   this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			   this->menuStrip1->ResumeLayout(false);
			   this->menuStrip1->PerformLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion
	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

			long size;
			int width, height;
			int width1, height1;
			xy position;
			imgCounter = 0;
			lineCounter = 0;

			while (imgCounter < openFileDialog1->FileNames->GetLength(0) - 1) {

				if (imgCounter == 0) {

					CString path1 = openFileDialog1->FileNames[imgCounter];
					// LoadBMP can read only 24 bit image depth
					buffer1 = LoadBMP(width, height, size, (LPCTSTR)path1);
					intensity1 = ConvertBMPToIntensity(buffer1, width, height);

					outReal[0] = new double[width * height];
					outImag[0] = new double[width * height];
					FFT2D(intensity1, outReal[0], outImag[0], width, height);


					CString path2 = openFileDialog1->FileNames[imgCounter + 1];
					// LoadBMP can read only 24 bit image depth
					buffer2 = LoadBMP(width, height, size, (LPCTSTR)path2);
					intensity2 = ConvertBMPToIntensity(buffer2, width, height);

					outReal[1] = new double[width * height];
					outImag[1] = new double[width * height];
					FFT2D(intensity2, outReal[1], outImag[1], width, height);


					// Phase Only Correlation result 
					double* POC = PhaseCorrelation(outReal[0], outImag[0], outReal[1], outImag[1], width, height);
					MaxPOC(POC, pocDot, width, height);

					delete[] outReal[0];
					delete[] outImag[0];
					delete[] POC;


					currCornerID = ZoneDetection(intensity1, intensity2, width, height, currVec, pocDot);
					// ShowImageIntensity(intensity1, intensity2, width, height);
					// Eksenlerce kesismeyen Random 4 nokta bul
					xy* img1Dots = Rand4Dots(currCornerID, pocDot, width, height);

					if (img1Dots == NULL) {
						MessageBox::Show("4 eslesme noktasi bulunamadi");
					}
					else {

						// Img2 uzerinde eslesen noktalari bul
						xy* img2Dots = MatchingDots(currCornerID, img1Dots, currVec);
						LineUpdate(currCornerID, currVec);


						for (int i = 0; i < 3; i++) {
							match1[i] = new double[4];
							match2[i] = new double[4];
						}
						for (int i = 0; i < 4; i++) {
							match1[0][i] = double(img1Dots[i].x);
							match1[1][i] = double(img1Dots[i].y);
							match1[2][i] = 1.0;
							match2[0][i] = double(img2Dots[i].x);
							match2[1][i] = double(img2Dots[i].y);
							match2[2][i] = 1.0;
						}

						double** homography = homography2d(match1, match2, 4);

						LaplacePyramid1 = new BYTE2 * [4];
						LaplacePyramid2 = new BYTE2 * [4];
						currPanoSize = ReSizePanorama(homography, width, height, width, height, position);
						labelResultS->Text = currPanoSize->x + " x " + currPanoSize->y;

						if (lineCounter == 0)
							isFirstLine = true;
						else isFirstLine = false;


						/*LaplacePyramid(homography, buffer1, buffer2, width, height, width, height, LaplacePyramid1, LaplacePyramid2, width1, height1, *currPanoSize, position);
						BYTE* r = P2(homography, width, height, *currPanoSize, position, LaplacePyramid1, LaplacePyramid2, width1, height1, width, height, isFirstLine, currCornerID, prevVec, currVec);
						ShowColorImage(r, width1, height1);
						delete[] r;*/

						LaplacePyramid(homography, buffer1, buffer2, width, height, width, height, LaplacePyramid1, LaplacePyramid2, width1, height1, *currPanoSize, position);
						panorama1 = PanaromicImage(homography, width, height, *currPanoSize, position, LaplacePyramid1, LaplacePyramid2, width1, height1, width, height, isFirstLine, currCornerID, prevVec, currVec);
						ShowColorImage(panorama1, currPanoSize->x, currPanoSize->y);

						prevPanoSize->x = currPanoSize->x;
						prevPanoSize->y = currPanoSize->y;
						prevVec->x = 0;
						prevVec->y = 0;
						UpdatePrevVec(currCornerID, prevVec, currVec);

						delete[] img2Dots;
						for (size_t i = 0; i < 3; i++)
							delete[] homography[i];
						delete[] homography;
					}

					delete[] intensity1;
					intensity1 = intensity2;
					delete[] currPanoSize;
					delete[] buffer1;
					delete[] img1Dots;
				}
				else {
					buffer1 = buffer2;
					outReal[0] = outReal[1];
					outImag[0] = outImag[1];

					CString path = openFileDialog1->FileNames[imgCounter + 1];
					// LoadBMP can read only 24 bit image depth
					buffer2 = LoadBMP(width, height, size, (LPCTSTR)path);
					intensity2 = ConvertBMPToIntensity(buffer2, width, height);

					outReal[1] = new double[width * height];
					outImag[1] = new double[width * height];

					FFT2D(intensity2, outReal[1], outImag[1], width, height);

					// Phase Only Correlation result 
					double* POC = PhaseCorrelation(outReal[0], outImag[0], outReal[1], outImag[1], width, height);
					MaxPOC(POC, pocDot, width, height);

					delete[] outReal[0];
					delete[] outImag[0];
					delete[] POC;


					currCornerID = ZoneDetection(intensity1, intensity2, width, height, currVec, pocDot);
					xy* img1Dots = Rand4Dots(currCornerID, pocDot, width, height);

					if (img1Dots == NULL) {
						MessageBox::Show("4 eslesme noktasi bulunamadi");
					}
					else {
						xy* img2Dots = MatchingDots(currCornerID, img1Dots, currVec);
						LineUpdate(currCornerID, currVec);
						xy* img1PanoDots = PanoDots(prevVec, currCornerID, img1Dots);

						for (int i = 0; i < 4; i++) {
							match1[0][i] = double(img1PanoDots[i].x);
							match1[1][i] = double(img1PanoDots[i].y);
							match1[2][i] = 1.0;
							match2[0][i] = double(img2Dots[i].x);
							match2[1][i] = double(img2Dots[i].y);
							match2[2][i] = 1.0;
						}

						double** homography = homography2d(match1, match2, 4);
						currPanoSize = ReSizePanorama(homography, prevPanoSize->x, prevPanoSize->y, width, height, position);
						labelResultS->Text = currPanoSize->x + " x " + currPanoSize->y;

						if (lineCounter == 0)
							isFirstLine = true;
						else isFirstLine = false;


						/*LaplacePyramid(homography, panorama1, buffer2, prevPanoSize->x, prevPanoSize->y, width, height, LaplacePyramid1, LaplacePyramid2, width1, height1, *currPanoSize, position);
						BYTE* r = P2(homography, prevPanoSize->x, prevPanoSize->y, *currPanoSize, position, LaplacePyramid1, LaplacePyramid2, width1, height1, width, height, isFirstLine, currCornerID, prevVec, currVec);
						ShowColorImage(r, width1, height1);
						delete[] r;*/

						LaplacePyramid(homography, panorama1, buffer2, prevPanoSize->x, prevPanoSize->y, width, height, LaplacePyramid1, LaplacePyramid2, width1, height1, *currPanoSize, position);
						panorama2 = PanaromicImage(homography, prevPanoSize->x, prevPanoSize->y, *currPanoSize, position, LaplacePyramid1, LaplacePyramid2, width1, height1, width, height, isFirstLine, currCornerID, prevVec, currVec);
						ShowColorImage(panorama2, currPanoSize->x, currPanoSize->y);
						UpdatePrevVec(currCornerID, prevVec, currVec);


						delete[] img1PanoDots;
						delete[] img2Dots;

						for (int i = 0; i < 3; i++)
							delete[] homography[i];
						delete[] homography;
					}

					prevPanoSize->x = currPanoSize->x;
					prevPanoSize->y = currPanoSize->y;

					delete[] panorama1;
					panorama1 = panorama2;
					delete[] intensity1;
					intensity1 = intensity2;

					delete[] currPanoSize;
					delete[] buffer1;
					delete[] img1Dots;
				}
				imgCounter++;
				imgIndexLbl->Text = (imgCounter + 1).ToString();
				imgIndexLbl->Refresh();
			}
			ShowColorImage(panorama1, prevPanoSize->x, prevPanoSize->y);
			delete[] panorama1;
			for (size_t i = 0; i < 3; i++) {
				delete[] match1[i];
				delete[] match2[i];
			}
		}
	}

	private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e) {
		this->pictureBox4->Image->Save("..\\SavedImg_" + (imgCounter + 1).ToString() + ".bmp");
		MessageBox::Show("SavedImg_" + (imgCounter + 1).ToString() + ".bmp kaydedildi.");
	}
	};
}
