
#include "Lab2Application.h"

int main(int argc, char *argv[])
{
    Lab2Application application("Lab1_withFramework", "1.0");

    application.Init();

    application.Run();

    return application.stop();
}
