
#include "Lab4Application.h"

int main(int argc, char *argv[])
{
    Lab4Application application("Lab1_withFramework", "1.0");

    application.Init();

    application.Run();

    application.stop();
}
