#include "application.h"

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{
    auto app = Application::getAppInstance();
    app->setHInstance(hInstance);
    app->init();
    app->run();

    return 0;
}
