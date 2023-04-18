#include "view.h"

int main()
{
    AlphaConfig conf = {};
    AlphaConfigCtor(&conf, "Table.bmp", "AskhatCat.bmp");

    AlphaRun(&conf);

    AlphaConfigDtor(&conf);
    return 0;
}
