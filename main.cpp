#include "view.h"

int main()
{
    AlphaConfig conf = {};
    AlphaConfigCtor(&conf, DEFAULT_BACKGROUND, DEFAULT_FOREGROUND);

    AlphaRun(&conf);

    AlphaConfigDtor(&conf);
    return 0;
}
