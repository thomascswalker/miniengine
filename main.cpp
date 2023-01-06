#include "src/application.h"

using namespace Graphics;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR pCmdLine,
    _In_ int nCmdShow)
{
    auto app = Application::getAppInstance();
    app->setHInstance(hInstance);
    app->init();
    int exitCode = app->run();

    return exitCode;
}
