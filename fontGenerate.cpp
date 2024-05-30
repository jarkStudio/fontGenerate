// fontGenerate.cpp : 
// 字库生成
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "stbText.h"
#include "GBK_TAB.h"

stbText stb; //绘制文字
//stbText stb("./xx.ttf"); //绘制文字

// unicode 
// 控制符和基本拉丁(0000-007F)
// 控制符和拉丁 - 1增补(0080 - 00FF)
// 中日韩部首增补(2E80-2EFF)
// 中日韩符号和标点(3000-303F)
// 中日韩笔画(31C0-31EF)
// 中日韩字符集兼容(3300-33FF)
// 中日韩统一表意文字扩展A (3400-4DBF)
// 中日韩统一表意文字(4E00 - 9FFF)
// 中日韩兼容形式(FE30-FE4F)
// 半角及全角形式(FF00 - FFEF)


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
    fclose(f);

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
    fclose(f);

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
    //load16x16("font16x16UniFont_0000_FFFF.bin", 0, 0X6522);
    load16x16("font16x16UniFont_0000_FFFF.bin", 0, 0x9648);

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

// GBK 采用双 字节表示
// 总体编码范围为 8140-FEFE
// 首字节在 81-FE 之间
// 尾字节在 40-FE 之间，剔除 xx7F 一条线。

// 输出全部GBK字符到文本
static void generateGBK() {
    auto f = fopen("GBK.txt", "wb");
    if (!f) return;

    for (uint8_t i = 0x81; i <= 0xfe; i++) {
        for (uint8_t j = 0x40; j <= 0xfe; j++) {
            if (j == 0x7f)continue;
            uint8_t tmp[2] = { i, j };

            // 自定义区换成全角空格
            if (0xa1 <= i && i <= 0xa7 && 0x40 <= j && j <= 0xa0) { // 自定义区 限制使用
                tmp[0] = 0xa1;
                tmp[1] = 0xa1;
            }
            if (0xaa <= i && i <= 0xaf && 0xa1 <= j && j <= 0xfe) { // 自定义区
                tmp[0] = 0xa1;
                tmp[1] = 0xa1;
            }
            if (0xf8 <= i && i <= 0xfe && 0xa1 <= j && j <= 0xfe) { // 自定义区
                tmp[0] = 0xa1;
                tmp[1] = 0xa1;
            }
            fwrite(tmp, 1, 2, f);
        }
        fwrite("\n", 1, 1, f);
    }
    fclose(f);
    std::cout << "Done.";
}


// 建立GBK到Unicode转换表，CP936.TXT 转换表下载 http://www.gnu.org/directory/libiconv.html
static void generateGBK2UnicodeTab() {
    //47880 Bytes 47KB 栈内
    //const int highBytes = 0xfe - 0x81 + 1; // 126
    //const int lowBytes = 0xfe - 0x40 + 1;  // 191
    uint16_t tab[126][192]={ 0 }; // 191+1 为了内存对齐

    std::ifstream file("CP936_GBK2UNICODE.TXT"); // 打开文件
    std::string line;
    std::vector<std::pair<int, int>> hex_pairs; // 存储解析后的十六进制数字对

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string hex1, hex2;
            ss >> hex1 >> hex2; // 分割字符串

            // 将十六进制字符串转换为整数
            int num1 = std::stoi(hex1, nullptr, 16);
            int num2 = std::stoi(hex2, nullptr, 16);

            if (num1 < 0x8140 || 0xfefe < num1 || num2 >= 0xffff) {
                std::cerr << "跳过 " << num1 << " " << num2 << '\n';
                continue;
            }
            tab[(num1 >> 8) - 0x81][(num1 & 0xff) - 0x40] = num2;
        }
        file.close(); // 关闭文件
    }
    else {
        std::cerr << "Unable to open file." << std::endl;
        return;
    }
    file.close();

    auto f = fopen("GBK_TAB.cpp", "w");
    const char* header = "#include\"GBK_TAB.h\"\n"
        "// const int highBytes = 0xfe - 0x81 + 1; // 126\n"
        "// const int lowBytes  = 0xfe - 0x40 + 1; // 191\n"
        "const uint16_t gbk2unicode[126][192]={ // 191+1 为了内存对齐\n";
    fwrite(header, 1, strlen(header), f);

    for (int i = 0; i < 126; i++) {
        fwrite("    ", 1, 4, f);
        for (int j = 0; j < 192; j++) {
            fprintf(f, "0x%04X, ", tab[i][j]);
        }
        fwrite("\n", 1, 1, f);
    }
    fwrite("};\n", 1, 3, f);
    fclose(f);
    std::cout << "Done.\n";
}


static void load16x16toGBKBuffer() {
    uint8_t* unicodeBuff = new uint8_t[2ULL * 1024 * 1024];

    auto f = fopen("font16x16UniFont_0000_FFFF.bin", "rb");
    fread(unicodeBuff, 1, 2ULL * 1024 * 1024, f);
    fclose(f);

    f = fopen("font16x16GBK_8140_FEFE.bin", "wb");

    uint8_t tmp[32] = {};
    for (int i = 0; i < 126; i++) {
        for (int j = 0; j < 192; j++) {
            int codePoint = gbk2unicode[i][j];
            if (codePoint)
                memcpy(tmp, unicodeBuff + (codePoint * 32), 32);
            else
                memset(tmp, 0, 32);
            fwrite(tmp, 32, 1, f);
        }
    }
    fclose(f);
    cout << "Done: ont16x16GBK_8140_FEFE.bin\n";
}

static void preview16x16GBK(const char* path, const vector<uint16_t>& codePoints) {
    auto f = fopen(path, "rb");
    int i = 1;
    for (auto codePoint : codePoints) {
        if (codePoint < 0X8140 || 0XFEFE < codePoint) {
            cout << "codePoint ERROR" << codePoint << "\n";
            continue;
        }
        printf("%d 0x%X\n", i++, codePoint);
        codePoint= (((codePoint>>8) - 0x81)*192) + ((codePoint & 0xff) - 0x40);
        fseek(f, 32 * codePoint, SEEK_SET);
        uint8_t wordBuf[32] = {};
        fread(wordBuf, 1, sizeof(wordBuf), f);

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < ((codePoint < 255) ? 8 : 16); x++) {
                cout << (getPoint16x16(wordBuf, x, y) ? "HH" : "--");
            }
            cout << '\n';
        }
        cout << '\n';
    }
    fclose(f);
}

static void mainGBK() {
    //generateGBK();
    //generateGBK2UnicodeTab();
    //load16x16toGBKBuffer();
    preview16x16GBK("font16x16GBK_8140_FEFE.bin", { 0xb3c2, 0xcbbc, 0xbddc });
}

int main() {
    //mainStb();
    //mainUniFont();
    mainGBK();


    return 0;
}
