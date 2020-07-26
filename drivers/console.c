#include "console.h"

struct{
	uint8_t cursor_x;
	uint8_t cursor_y;
}point = {0,0};

static void move_cursor()
{

    uint16_t cursorLocation = point.cursor_y * 80 + point.cursor_x;
    
    // 在这里用到的两个内部寄存器的编号为14与15，分别表示光标位置
    // 的高8位与低8位。

    outb(0x3D4, 14);                    // 告诉 VGA 我们要设置光标的高字节
    outb(0x3D5, cursorLocation >> 8);   // 发送高 8 位
    outb(0x3D4, 15);                    // 告诉 VGA 我们要设置光标的低字节
    outb(0x3D5, cursorLocation);        // 发送低 8 位
}

void console_clear()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    for (int i = 0; i < 80 * 25; i++) {
          video_memory[i] = blank;
    }

    point.cursor_x = 0;
    point.cursor_y = 0;
    move_cursor();
}


static void scroll()
{
    // attribute_byte 被构造出一个黑底白字的描述格式
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);  // space 是 0x20

    // point.cursor_y 到 25 的时候，就该换行了
    if (point.cursor_y >= 25) {
        // 将所有行的显示数据复制到上一行，第一行永远消失了...
        int i;
        
        for (i = 0 * 80; i < 24 * 80; i++) {
              video_memory[i] = video_memory[i+80];
        }

        // 最后的一行数据现在填充空格，不显示任何字符
        for (i = 24 * 80; i < 25 * 80; i++) {
              video_memory[i] = blank;
        }
        
        // 向上移动了一行，所以 point.cursor_y 现在是 24
        point.cursor_y = 24;
    }
}


void console_putc_color(char c, real_color_t back, real_color_t fore)
{
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;

    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute = attribute_byte << 8;

    if (c == 0x08 && point.cursor_x) {
	point.cursor_x--;
    } else if (c == 0x09) {
        point.cursor_x = (point.cursor_x+8) & ~(8-1);
    } else if (c == '\r') {
        point.cursor_x = 0;
    } else if (c == '\n') {
        point.cursor_x = 0;
        point.cursor_y++;
    } else if (c >= ' ') {
        video_memory[point.cursor_y*80 + point.cursor_x] = c | attribute;
        point.cursor_x++;
    }

    if (point.cursor_x >= 80) {
        point.cursor_x = 0;
        point.cursor_y ++;
    }

    scroll();

    move_cursor();
}

void console_write(char *cstr)
{
    while (*cstr) {
          console_putc_color(*cstr++, rc_black, rc_white);
    }
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
    while (*cstr) {
          console_putc_color(*cstr++, back, fore);
    }
}
