
#include "Lab5Application.h"

int main(int argc, char *argv[])
{
    Lab5Application application("Lab1_withFramework", "1.0");

    application.Init();

    application.Run();

    application.stop();
}
