//割り込み関連処理

#include "bootpack.h"

//PICの初期化
void init_pic(void)
{
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);

    io_out8(PIC0_ICW1, 0x11);
    io_out8(PIC0_ICW2, 0x20);
    io_out8(PIC0_ICW3, 1 << 2);
    io_out8(PIC0_ICW4, 0x01);

    io_out8(PIC1_ICW1, 0x11);
    io_out8(PIC1_ICW2, 0x28);
    io_out8(PIC1_ICW3, 2);
    io_out8(PIC1_ICW4, 0x01);

    io_out8(PIC0_IMR, 0xfb);
    io_out8(PIC1_IMR, 0xff);

    return;
}

//PS/2キーボードからの割り込み
void inthandler21(int *esp)
{
    struct BOOTINFO *bootInfo = (struct BOOTINFO *)ADRESS_BOOTINFO;
    boxfill8(bootInfo->vram, bootInfo->screenX, COLOR8_000000, 0, 0, 32 * 8 -1, 15);
    putfonts8_asc(bootInfo->vram, bootInfo->screenX, 0, 0, COLOR8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
    for(;;)
    {
        io_hlt();
    }
}

//PS/2マウスからの割り込み
void inthandler2c(int *esp)
{
    struct BOOTINFO *bootInfo = (struct BOOTINFO *)ADRESS_BOOTINFO;
    boxfill8(bootInfo->vram, bootInfo->screenX, COLOR8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(bootInfo->vram, bootInfo->screenX, 0, 0, COLOR8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for(;;)
    {
        io_hlt();
    }
}

//PIC0からの不完全割り込み対策
//Athlon64X2機などでは、チップセットの都合によりPICの初期化時に個の割り込みが起こるらしい
//この割り込みは、PIC初期化時の電気的ノイズによって発生するので、なんらかの処理を行う必要はない
void inthandler27(int *esp)
{
    io_out8(PIC0_OCW2, 0x67);
    return;
}