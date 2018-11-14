//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1Click(TObject *Sender)
{
  memo1->Lines->Clear();

  SECURITY_ATTRIBUTES da;
  ZeroMemory(&da, sizeof(SECURITY_ATTRIBUTES));
  da.nLength = sizeof(SECURITY_ATTRIBUTES);
  da.bInheritHandle = true;

  da.lpSecurityDescriptor = NULL;
  HANDLE ReadPipeHandle;
  HANDLE WritePipeHandle;

  if (!CreatePipe(&ReadPipeHandle,&WritePipeHandle,&da,0)) {
      RaiseLastWin32Error();
  }

  STARTUPINFO zy;
  ZeroMemory(&zy, sizeof(STARTUPINFO));
  zy.cb = sizeof(STARTUPINFO);
  zy.dwFlags = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
  zy.wShowWindow = SW_HIDE;
  zy.hStdOutput = WritePipeHandle;
  zy.hStdError = WritePipeHandle;

  PROCESS_INFORMATION ap;
  ZeroMemory(&ap, sizeof(PROCESS_INFORMATION));


  AnsiString sciezka = ExpandFileName(ExtractFilePath(Application->ExeName));

  Edit1->Text = sciezka + "src\\php\\php.exe";
  Edit2->Text = "-f \"" +sciezka + "src\\api.php\"";

  if (!CreateProcess(Edit1->Text.c_str(), Edit2->Text.c_str(), NULL, NULL, true, 0, NULL, NULL, &zy, &ap)) {
      RaiseLastWin32Error();
  }

  char Data[1024];

  for (;;)
  {
    DWORD BytesRead;
    DWORD TotalBytes;
    DWORD BytesLeft;

    if (!PeekNamedPipe(ReadPipeHandle, Data, sizeof(Data), &BytesRead,
        &TotalBytes,&BytesLeft)) { RaiseLastWin32Error(); }

    if(BytesRead != NULL) {
       if(!ReadFile(ReadPipeHandle, Data, sizeof(Data)-1, &BytesRead, NULL)) {
          RaiseLastWin32Error();
       }

       if (Data != NULL) {
           Data[BytesRead]='\0';
           memo1->Lines->Add(AnsiString(Data));
       }
    } else {
             if (WaitForSingleObject(ap.hProcess, 0) == WAIT_OBJECT_0) {
                 break;
             }
    }
  }
  CloseHandle(ap.hThread);
  CloseHandle(ap.hProcess);
  CloseHandle(ReadPipeHandle);
  CloseHandle(WritePipeHandle);

  Form1->Height = 520;
  Image3->Picture->LoadFromFile(sciezka + "wallet.jpeg");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image3Click(TObject *Sender)
{
  if (SaveDialog1->Execute())
  {
    Image3->Picture->SaveToFile(SaveDialog1->FileName);
  }
}
//---------------------------------------------------------------------------

