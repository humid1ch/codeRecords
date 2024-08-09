#include "inc/man.h"
#include "inc/shop.h"
#include "inc/admin.h"
#include "inc/util.h"

int main() {
    mansInit();
    shopInit();
    adminInit();
    mainMenu();

    return 0;
}
