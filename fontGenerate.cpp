// fontGenerate.cpp : 
// 字库生成
//

#include <iostream>
#include "stbText.h"

stbText stb; //绘制文字
//stbText stb("./xx.ttf"); //绘制文字

// unicode 
// 控制符和基本拉丁(0000-007F)         OK  256
// 控制符和拉丁 - 1增补(0080 - 00FF)
// 中日韩部首增补(2E80-2EFF)
// 中日韩符号和标点(3000-303F)         OK  63
// 中日韩笔画(31C0-31EF)
// 中日韩字符集兼容(3300-33FF)
// 中日韩统一表意文字扩展A (3400-4DBF) OK  6592
// 中日韩统一表意文字(4E00 - 9FFF)     OK  20992
// 中日韩兼容形式(FE30-FE4F)           OK  32
// 半角及全角形式(FF00 - FFEF)         OK  240
// 
// 0X00-0XFF 256
// 3000-303F 64
// 3400-9FFF 27648
// FE00-FFFF 512


// 16 Byte
static uint8_t getPoint8x16(uint8_t* buf, int x, int y) {
    return buf[(y < 8 ? 0 : 8) + x] & (1 << (y < 8 ? y : (y - 8)));
}

// 16 Byte
static void setPoint8x16(uint8_t* buf, int x, int y) {
    buf[(y < 8 ? 0 : 8) + x] |= (1 << (y < 8 ? y : (y - 8)));
}

static void save8x16(uint32_t begin, uint32_t end) {
    char path[64];
    snprintf(path, sizeof(path), "font8x16_%04X_%04X.bin", begin, end);

    auto f = fopen(path, "wb");
    if (!f) {
        puts("ERROR!!!");
        return;
    }
    uint8_t wordBuf[16];
    for (uint32_t i = begin; i <= end; i++) {
        auto buf = stb.getWordPixel(i);
        memset(wordBuf, 0, sizeof(wordBuf));
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 8; x++) {
                if (buf[y * 16 + x])
                    setPoint8x16(wordBuf, x, y);
            }
        }
        fwrite(wordBuf, 1, sizeof(wordBuf), f);
    }
    fclose(f);
    printf("Done: %s", path);
}

static void load8x16(const char* path, uint32_t beginCode, uint32_t codePoint) {
    if (codePoint < beginCode)
        return;

    auto f = fopen(path, "rb");
    fseek(f, 16 * (codePoint - beginCode), SEEK_SET);
    uint8_t wordBuf[16] = {};
    fread(wordBuf, 1, sizeof(wordBuf), f);

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 8; x++) {
            cout << (getPoint8x16(wordBuf, x, y) ? "HH" : "--");
        }
        cout << '\n';
    }
    cout << '\n';
}


// 32 Byte
static uint8_t getPoint16x16(uint8_t* buf, int x, int y) {
    return buf[(y < 8 ? 0 : 16) + x] & (1 << (y < 8 ? y : (y - 8)));
}

// 32 Byte
static void setPoint16x16(uint8_t* buf, int x, int y) {
    buf[(y < 8 ? 0 : 16) + x] |= (1 << (y < 8 ? y : (y - 8)));
}

static void save16x16(uint32_t begin, uint32_t end) {
    char path[64];
    snprintf(path, sizeof(path), "font16x16_%04X_%04X.bin", begin, end);

    auto f = fopen(path, "wb");
    if (!f) {
        puts("ERROR!!!");
        return;
    }
    uint8_t wordBuf[32];
    for (uint32_t i = begin; i <= end; i++) {
        auto buf = stb.getWordPixel((i==0xfe48 || i==0xfe49)? begin :i);
        memset(wordBuf, 0, sizeof(wordBuf));
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                if(buf[y * 16 + x])
                    setPoint16x16(wordBuf, x, y);
            }
        }
        fwrite(wordBuf, 1, sizeof(wordBuf), f);
    }
    fclose(f);
    printf("Done: %s", path);
}

static void load16x16(const char* path, uint32_t beginCode, uint32_t codePoint) {
    if (codePoint < beginCode)
        return;

    auto f = fopen(path, "rb");
    fseek(f, 32 * (codePoint - beginCode), SEEK_SET);
    uint8_t wordBuf[32] = {};
    fread(wordBuf, 1, sizeof(wordBuf), f);

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < ((codePoint<255)?8:16); x++) {
            cout << (getPoint16x16(wordBuf, x, y)?"HH":"--");
        }
        cout << '\n';
    }

    cout << '\n';
}





static void preview8x16(uint32_t codePoint) {
    auto buf = stb.getWordPixel(codePoint);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 8; x++) {
            std::cout << (buf[y * 16 + x] ? "HH" : "--");
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

static void preview16x16(uint32_t codePoint) {
    //auto buf = stb.getWordPixel("金。，￥是？、，");
    auto buf = stb.getWordPixel(codePoint);

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            std::cout << (buf[y * 16 + x] ? "HH" : "--");
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}


static int mainStb()
{
    //save8x16(0x00, 0x1FF);

    //save16x16(0x3000, 0x303f);
    //save16x16(0x3400, 0x9fff);

    //save16x16(0x3000, 0x9fff);

    //save16x16(0xFE30, 0xFFFF); //MSYHMONO.ttf  0XFE48 0XFE49 有问题
    //save16x16(0xFF00, 0xFFEF);

    //load16x16("font16x16_3400_9fff.bin", 0x3400, stb.utf8Str2Code("思"));
    //load16x16("font16x16_3400_9fff.bin", 0x3400, stb.utf8Str2Code("考"));
    //load8x16("font8x16_0000_01FF.bin", 0, 'j');
    load16x16("font16x16UniFont_0000_FFFF.bin", 0, 0X6522);

    //cout << "Hello!\n";
    return 0;
}


/*

// 所有字符均以16x16保存，显示时则按需是否显示右侧空白区域 x:8~16
void save16x16UniFont(cv::Mat& img, uint32_t begin, uint32_t end) {
    char path[64];
    snprintf(path, sizeof(path), "font16x16UniFont_%04X_%04X.bin", begin, end);

    auto f = fopen(path, "wb");
    if (!f) {
        puts("ERROR!!!");
        return;
    }
    uint8_t wordBuf[32];
    for (uint32_t i = begin; i <= end; i++) {
        int x0 = (i & 0xff) * 16;
        int y0 = (i >> 8) * 16;
        memset(wordBuf, 0, sizeof(wordBuf));
        if (img.channels() == 3) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    if (img.at<cv::Vec3b>(y0 + y, x0 + x)[1] == 0)
                        setPoint16x16(wordBuf, x, y);
                }
            }
        }
        else if (img.channels() == 4) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    if (img.at<cv::Vec4b>(y0 + y, x0 + x)[1] == 0)
                        setPoint16x16(wordBuf, x, y);
                }
            }
        }
        else if (img.channels() == 1) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    if (img.at<uchar>(y0 + y, x0 + x) == 0)
                        setPoint16x16(wordBuf, x, y);
                }
            }
        }
        else {
            cout << "Error: img.channel:" << img.channels() << endl;
        }
        fwrite(wordBuf, 1, sizeof(wordBuf), f);
    }
    fclose(f);
    printf("Done: %s", path);
}


void preview16x16UniFont(cv::Mat& img, uint32_t codePoint) {
    int x0 = (codePoint & 0xff) * 16;
    int y0 = (codePoint >> 8) * 16;

    if (img.channels() == 3) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                std::cout << ((img.at<cv::Vec3b>(y0 + y, x0 + x)[1] == 0) ? "@@" : "  ");
            }
            std::cout << '\n';
        }
    }
    else if (img.channels() == 4) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                std::cout << ((img.at<cv::Vec4b>(y0 + y, x0 + x)[1] == 0) ? "@@" : "  ");
            }
            std::cout << '\n';
        }
    }
    else if (img.channels() == 1) {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                std::cout << ((img.at<uchar>(y0 + y, x0 + x) == 0) ? "@@" : "  ");
            }
            std::cout << '\n';
        }
    }
    else {
        cout << "Error: img.channel:" << img.channels() << endl;
    }
    std::cout << '\n';
}


void mainUniFont() {
    auto img = cv::imread("unifont15.png"); // 24Bit
    //preview16x16UniFont(img, 'a');
    save16x16UniFont(img, 0, 0xffff);
}
*/

int main() {
    mainStb();
    //mainUniFont();

    return 0;
}
