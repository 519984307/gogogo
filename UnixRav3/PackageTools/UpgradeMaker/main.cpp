#include "upgrademaker.h"

int main() {

    upgrademaker test("D49170C0-B076-4795-B079-0F97560485AF",
                 "xav",
                 "3.0.0.1",
                 "/home/miwoo/workspace/esm_dev/php/src/rsextension/extensions/test/esmpack/dest",
                 "/home/miwoo/workspace/esm_dev/php/src/rsextension/extensions/test/esmpack/dest1");
    int err = test.make();
    printf("erro is %d\n", err);
    return 0;
}
