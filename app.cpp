// ウィンドウ関連の処理
#define USE_IMGUI
#include "Window.h"

// 光源データ
constexpr GgSimpleShader::Light light =
{
  { 0.2f, 0.2f, 0.2f, 1.0f }, // 環境光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 拡散反射光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 鏡面反射光成分
  { 0.0f, 0.0f, 1.0f, 1.0f }  // 光源位置
};

#ifdef USE_IMGUI
// フレームバッファオブジェクトのサイズ
constexpr int fboWidth(300);  // フレームバッファオブジェクトの幅
constexpr int fboHeight(300); // フレームバッファオブジェクトの高さ
#endif

//
// アプリケーション本体
//
void app()
{
  // ウィンドウを作成する
  Window window("DrawOpenLGLinImGuiWindow");

  // シェーダを作成する
  const GgSimpleShader simple("simple.vert", "simple.frag");

  // 図形データを読み込む (大きさを正規化する)
  const GgSimpleObj object("logo.obj", true);

  // 光源データから光源のバッファオブジェクトを作成する
  const GgSimpleShader::LightBuffer lightBuffer(light);

  // ビュー変換行列を設定する
  const GgMatrix mv(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

#ifdef USE_IMGUI
  //
  // ImGui の初期設定
  //

  //ImGuiIO& io = ImGui::GetIO();
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // カラーバッファ用のテクスチャを用意する
  GLuint cb;
  glGenTextures(1, &cb);
  glBindTexture(GL_TEXTURE_2D, cb);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // デプスバッファ用のレンダーバッファを用意する
  GLuint rb;
  glGenRenderbuffers(1, &rb);
  glBindRenderbuffer(GL_RENDERBUFFER, rb);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fboWidth, fboHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // フレームバッファオブジェクトを作成する
  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);

  // フレームバッファオブジェクトにカラーバッファとしてテクスチャを結合する
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb, 0);

  // フレームバッファオブジェクトにデプスバッファとしてレンダーバッファを結合する
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);

  // フレームバッファオブジェクトの結合を解除する
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

  // 背景色を設定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 隠面消去処理を設定する
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // オブジェクトのオイラー角
  float roll(0.0f), pitch(0.0f), yaw(0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // オブジェクトのモデル変換行列を設定する
    GgMatrix mm(window.getTrackball());

#ifdef USE_IMGUI
    //
    // ImGui によるユーザインタフェース
    //

    // ImGui のフレームを準備する
    ImGui::NewFrame();

    // ImGui のフレームに一つ目の ImGui のウィンドウを作成する
    ImGui::Begin("Control panel");

    // FPS など表示してみたりする
    ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);

    // スライダでオイラー角を設定する
    ImGui::SliderAngle("Roll", &roll);
    ImGui::SliderAngle("Pitch", &pitch);
    ImGui::SliderAngle("Yaw", &yaw);

    // Quit ボタンのクリックでループを抜けるようにする
    if (ImGui::Button("Quit")) window.setClose();

    // ImGui のウィンドウの作成を終了する
    ImGui::End();

    // モデル変換行列にオイラー角を乗じる
    mm = mm.rotateY(yaw).rotateX(pitch).rotateZ(roll);

    // フレームバッファオブジェクトのサイズをもとに投影変換行列を設定する
    const GgMatrix mp(ggPerspective(0.5f, (GLfloat)fboWidth / (GLfloat)fboHeight, 1.0f, 15.0f));
#else
    // 開かれているウィンドウのサイズをもとに投影変換行列を設定する
    const GgMatrix mp(ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f));
#endif

    // シェーダプログラムを指定する
    simple.use(mp, mv * mm, lightBuffer);

#ifdef USE_IMGUI
    // フレームバッファオブジェクトを結合する
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    // ビューポートをフレームバッファオブジェクトのサイズにする
    glViewport(0, 0, fboWidth, fboHeight);

    // カラーバッファを消去する (glClear() でも構わない)
    constexpr GLfloat color[]{ 0.2f, 0.3f, 0.5f, 0.8f }, depth(1.0f);
    glClearBufferfv(GL_COLOR, 0, color);
    glClearBufferfv(GL_DEPTH, 0, &depth);
#endif

    // 図形を描画する
    object.draw();

#ifdef USE_IMGUI
    // フレームバッファオブジェクトの結合を解除する
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ビューポートを復帰する
    window.resetViewport();

    // ImGui のフレームに二つ目の ImGui のウィンドウを作成する
    ImGui::Begin("OpenGL");
    
    // テクスチャを ImGui のウィンドウに描く
    ImGui::Image((void*)(intptr_t)cb, ImVec2(fboWidth, fboHeight));

    // ImGui のウィンドウの作成を終了する
    ImGui::End();

    // ImGui のフレームに描画する
    ImGui::Render();
#endif

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
