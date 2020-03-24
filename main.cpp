//
// メインプログラム
//

// MessageBox の準備
#if defined(_MSC_VER)
#  define NOMINMAX
#  include <Windows.h>
#  include <atlstr.h>
#elif defined(__APPLE__)
#  include <CoreFoundation/CoreFoundation.h>
#else
#  include <iostream>
#endif
#define HEADER_STR "ゲームグラフィックス特論"

// ウィンドウ関連の処理
#define USE_IMGUI
#include "Window.h"

// アプリケーション本体
extern void app();

//
// メインプログラム
//
int main() try
{
  // ウィンドウ関連の初期設定
  Window::init(4, 1);

  // アプリケーションの実行
  app();
}
catch (const std::runtime_error &e)
{
  // エラーメッセージを表示する
#if defined(_MSC_VER)
  MessageBox(NULL, CString(e.what()), TEXT(HEADER_STR), MB_ICONERROR);
#elif defined(__APPLE__)
  // the following code is copied from http://blog.jorgearimany.com/2010/05/messagebox-from-windows-to-mac.html
  // convert the strings from char* to CFStringRef
  CFStringRef msg_ref = CFStringCreateWithCString(NULL, e.what(), kCFStringEncodingUTF8);

  // result code from the message box
  CFOptionFlags result;

  //launch the message box
  CFUserNotificationDisplayAlert(
    0,                                 // no timeout
    kCFUserNotificationNoteAlertLevel, // change it depending message_type flags ( MB_ICONASTERISK.... etc.)
    NULL,                              // icon url, use default, you can change it depending message_type flags
    NULL,                              // not used
    NULL,                              // localization of strings
    CFSTR(HEADER_STR),                 // header text
    msg_ref,                           // message text
    NULL,                              // default "ok" text in button
    NULL,                              // alternate button title
    NULL,                              // other button title, null--> no other button
    &result                            // response flags
  );

  // Clean up the strings
  CFRelease(msg_ref);
#else
  std::cerr << HEADER_STR << ": " << e.what() << '\n';
#endif

  // ブログラムを終了する
  return EXIT_FAILURE;
}
