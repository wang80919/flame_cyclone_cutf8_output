#include <iostream>
#include <fstream>
#include <windows.h>
#include <tchar.h>

std::string FormatA(LPCSTR pFormat, ...);
bool ToFileA(const std::string& strFile, const std::string& str);
void UTF8_Output(uint32_t uStart, uint32_t uEnd, int code, int charater, int hex, const std::string& strFileNamePrefix);
void ConsoleOutput(const char* pFormat, ...);
void PrintHelp();
int GetUtf8CharacterCount(const std::string& strContent);

// 
// @brief: 获取UTF8字符个数
// @param: strContent   文本内容
// @ret: int 若返回值 >= 0, 表示字符个数, 若返回值 < 0, 表示文本内容不是合法的 UTF8 编码字符串
int GetUtf8CharacterCount(const std::string& strContent)
{
    bool fResult = true;    // 操作结果
    bool fBom = true;       // BOM(Byte Order Mark)
    int nByteCount = 0;     // 字节计数
    int nChCount = 0;       // 字符计数

    for (const unsigned char ch: strContent)
    {
        // 普通 Ascii 也是utf8一部分
        if (ch < 0x7F)
        {
            nChCount++;
            continue;
        }

        // 检查 UTF-8 首字节
        if (0 == nByteCount)
        {
            if (ch >= 0xC0)
            {
                uint8_t u8CodeMask  = 0xC0;     // 11000000
                uint8_t u8DataMask = 0x1F;      // 000xxxxx
                int nCount = 2;                 // 有效字节数量: 2-6

                // 检索字符使用的字节数量
                while(u8CodeMask <= 0xFC)
                {
                    uint8_t u8MaskMax = u8CodeMask | u8DataMask;
                    if (ch >= u8CodeMask && ch <= u8MaskMax)
                    {
                        nByteCount = nCount;
                        break;
                    }

                    u8CodeMask = (u8CodeMask >> 1) | 0x80;
                    u8DataMask = u8DataMask >> 1;
                    nCount++;
                }

                if (0 == nByteCount)
                {
                    fResult = false;
                    break;
                }

                if (0xEF == ch && 3 == nByteCount)
                {
                    fBom = true;
                }

                nByteCount--;
            }
            else
            {
                fResult = false;
                break;
            }
        }
        else
        {
            // 非首字节掩码: 10xxxxxx
            if (0x80 != (ch & 0xC0))
            {
                fResult = false;
                break;
            }

            if (fBom)
            {
                if (0xBB != ch && 2 == nByteCount)
                {
                    fBom = false;
                }

                if (0xBF != ch && 1 == nByteCount)
                {
                    fBom = false;
                }
            }

            nByteCount--;

            if (0 == nByteCount)
            {
                if (!fBom)
                {
                    nChCount++;
                }

                fBom = false;
            }
        }
    }

    if (!fResult)
    {
        return -1;
    }

    return nChCount;
}

int main(int argc, char* argv[])
{
    uint32_t uStart = 0x4E00;
    uint32_t uEnd = 0x9FFF;
    int codeIndex = 0;
    int chIndex = 1;
    int hexIndex = 2;
    std::string strFileNamePrefix = "\xef\xbb\xbf 1234567890";

    int uCount = GetUtf8CharacterCount(strFileNamePrefix);

    if (argc < 3)
    {
        PrintHelp();
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (0 == _stricmp(argv[i], "-help"))
        {
            PrintHelp();
            return 0;
        }

        if (0 == _stricmp(argv[i], "-start") && (i + 1 < argc))
        {
            uStart = strtoul(argv[++i], NULL, 16);
            continue;
        }

        if (0 == _stricmp(argv[i], "-end") && (i + 1 < argc))
        {
            uEnd = strtoul(argv[++i], NULL, 16);
            continue;
        }

        if (0 == _stricmp(argv[i], "-code") && (i + 1 < argc))
        {
            codeIndex = strtol(argv[++i], NULL, 16);
            continue;
        }

        if (0 == _stricmp(argv[i], "-ch") && (i + 1 < argc))
        {
            chIndex = strtol(argv[++i], NULL, 16);
            continue;
        }

        if (0 == _stricmp(argv[i], "-hex") && (i + 1 < argc))
        {
            hexIndex = strtol(argv[++i], NULL, 16);
            continue;
        }

        if (0 == _stricmp(argv[i], "-name") && (i + 1 < argc))
        {
            strFileNamePrefix = argv[++i];
            continue;
        }
    }

    clock_t tmBegin = ::clock();
    UTF8_Output(uStart, uEnd, codeIndex, chIndex, hexIndex, strFileNamePrefix);
    clock_t tmEnd = ::clock();

    ConsoleOutput("Output cost time: %d ms\r\n", tmEnd - tmBegin);

    return 0;
}

void PrintHelp()
{
    ConsoleOutput("==========Utf8 Output usage==========\r\n");
    ConsoleOutput("example: ");
    ConsoleOutput("\r\n");
    ConsoleOutput("    CUtf8.exe -start 4E00 -end 9fff -code 0 x-ch 1 -hex 2 -name utf8\r\n");
    ConsoleOutput("argument: ");
    ConsoleOutput("\r\n");
    ConsoleOutput("    -start: Unicode code point start position (Range: 0x00 - 0x1FFFFF, 默认: 0x4E00)\r\n");
    ConsoleOutput("      -end: Unicode code point end position (Range: 0x00 - 0x1FFFFF, 默认: 0x9FFF)\r\n");
    ConsoleOutput("     -code: Code column index (Range: 0 - 2, Default: 0)\r\n");
    ConsoleOutput("       -ch: Character column index (Range: 0 - 2, Default: 1)\r\n");
    ConsoleOutput("      -hex: Hex column index (Range: 0 - 2, Default: 2)\r\n");
    ConsoleOutput("     -name: Output filename prefix (Default: Utf8)\r\n");
    ConsoleOutput("\r\n");

    ConsoleOutput("==========Utf8 输出用法==========\r\n");
    ConsoleOutput("示例: ");
    ConsoleOutput("\r\n");
    ConsoleOutput("    CUtf8.exe -start 4E00 -end 9fff -code 0 x-ch 1 -hex 2 -name utf8\r\n");
    ConsoleOutput("命令参数: ");
    ConsoleOutput("\r\n");
    ConsoleOutput("    -start: Unicode 码位起始位置 (范围: 0x00 - 0x1FFFFF, 默认: 0x4E00)\r\n");
    ConsoleOutput("      -end: Unicode 码位结束位置 (范围: 0x00 - 0x1FFFFF, 默认: 0x9FFF)\r\n");
    ConsoleOutput("     -code: 码位列序号 (范围: -1 - 2, 默认: 0)\r\n");
    ConsoleOutput("       -ch: 字符列序号 (范围: -1 - 2, 默认: 1)\r\n");
    ConsoleOutput("      -hex: 十六进制序号 (范围: -1 - 2, 默认: 2)\r\n");
    ConsoleOutput("     -name: 输出文件名前缀 (默认: Utf8)\r\n");
    ConsoleOutput("\r\n");
}

void ConsoleOutput(const char* pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::string strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //格式化输出字符串
        int nSize = _vsnprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
            ::WriteConsoleA(console, strResult.c_str(), nSize, NULL, NULL);
            break;
        }

        //缓冲大小超限终止
        if (nCchCount >= INT32_MAX)
        {
            break;
        }

        //重新分配缓冲
        nCchCount *= 2;
        strResult.resize(nCchCount);

    } while (true);

    va_end(args);
}

void UTF8_Output(uint32_t uStart, uint32_t uEnd, int code, int ch, int hex, const std::string& strFileNamePrefix)
{
    uint8_t szBuf[MAX_PATH] = { 0 };
    std::string strOutput;
    strOutput += "\xef\xbb\xbf";

    int nOutColumn = 0;

    if (code >= 0) nOutColumn++;
    if (ch >= 0) nOutColumn++;
    if (hex >= 0) nOutColumn++;

    for (uint32_t i = uStart; i <= uEnd; i++)
    {
        std::string strhex;
        std::string strCode;

        // 1字节
        // 0xxxxxxx
        if (i >= 0x00000000 && i <= 0x0000007F)
        {
            szBuf[0] = i;
            szBuf[1] = 0;
            strhex = FormatA("0x%02X", szBuf[0]);
            strCode = FormatA("U+%02X", i);
        }

        // 2字节
        // 110xxxxx 10xxxxxx
        if (i >= 0x00000080 && i <= 0x000007FF)
        {
            szBuf[0] = ((i >>  6) & 0x1F) | 0xC0;
            szBuf[1] = ((i & 0x3F)) | 0x80;
            szBuf[2] = 0;
            strhex = FormatA("0x%02X%02X", szBuf[0], szBuf[1]);
            strCode = FormatA("U+%04X", i);
        }

        // 3字节
        // 1110xxxx 10xxxxxx 10xxxxxx
        if (i >= 0x00000800 && i <= 0x0000FFFF)
        {
            szBuf[0] = ((i >> 12) & 0x0F) | 0xE0;
            szBuf[1] = ((i >>  6) & 0x3F) | 0x80;
            szBuf[2] = ((i & 0x3F)) | 0x80;
            szBuf[3] = 0;
            strhex = FormatA("0x%02X%02X%02X", szBuf[0], szBuf[1], szBuf[2]);
            strCode = FormatA("U+%04X", i);
        }

        // 4字节
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (i >= 0x00010000 && i <= 0x001FFFFF)
        {
            szBuf[0] = ((i >> 18) & 0x07) | 0xF0;
            szBuf[1] = ((i >> 12) & 0x3F) | 0x80;
            szBuf[2] = ((i >>  6) & 0x3F) | 0x80;
            szBuf[3] = ((i & 0x3F)) | 0x80;
            szBuf[4] = 0;
            strhex = FormatA("0x%02X%02X%02X%02X", szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
            strCode = FormatA("U+%06X", i);
        }

        // 5字节
        // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (i >= 0x00200000 && i <= 0x03FFFFFF)
        {
            szBuf[0] = ((i >> 24) & 0x03) | 0xF8;
            szBuf[1] = ((i >> 18) & 0x3F) | 0x80;
            szBuf[2] = ((i >> 12) & 0x3F) | 0x80;
            szBuf[3] = ((i >>  6) & 0x3F) | 0x80;
            szBuf[4] = ((i & 0x3F)) | 0x80;
            szBuf[5] = 0;
            strhex = FormatA("0x%02X%02X%02X%02X%02X", szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
            strCode = FormatA("U+%08X", i);
        }

        // 6字节
        // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (i >= 0x04000000 && i <= 0x7FFFFFFF)
        {
            szBuf[0] = ((i >> 30) & 0x01) | 0xFC;
            szBuf[1] = ((i >> 24) & 0x3F) | 0x80;
            szBuf[2] = ((i >> 18) & 0x3F) | 0x80;
            szBuf[3] = ((i >> 12) & 0x3F) | 0x80;
            szBuf[4] = ((i >>  6) & 0x3F) | 0x80;
            szBuf[5] = ((i & 0x3F)) | 0x80;
            szBuf[6] = 0;
            strhex = FormatA("0x%02X%02X%02X%02X%02X%02X", szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4], szBuf[5]);
            strCode = FormatA("U+%08X", i);
        }

        for (int nIndex = 0; nIndex < nOutColumn; nIndex++)
        {
            if (nIndex == code)
            {
                strOutput += strCode;
                if (nIndex < 2)
                {
                    strOutput += " ";
                }
                continue;
            }

            if (nIndex == ch)
            {
                strOutput += (char*)szBuf;
                if (nIndex < 2)
                {
                    strOutput += " ";
                }
                continue;
            }

            if (nIndex == hex)
            {
                strOutput += strhex;
                if (nIndex < 2)
                {
                    strOutput += " ";
                }
                continue;
            }
        }

        if (i < uEnd)
        {
            strOutput += "\r\n";
        }
    }

    std::string strFileName = strFileNamePrefix;
    strFileName += FormatA("_%08X_%08X", uStart, uEnd);

    for (int nIndex = 0; nIndex < nOutColumn; nIndex++)
    {
        if (nIndex == code)
        {
            strFileName += "_code";
            continue;
        }

        if (nIndex == ch)
        {
            strFileName += "_ch";
            continue;
        }

        if (nIndex == hex)
        {
            strFileName += "_hex";
            continue;
        }
    }

    uint32_t uCount = 0;
    if (uStart <= uEnd)
    {
        uCount = uEnd - uStart + 1;
    }

    strFileName += FormatA("(%d)", uCount);
    strFileName += ".txt";

    ToFileA(strFileName, strOutput);
}

std::string FormatA(LPCSTR pFormat, ...)
{
    size_t nCchCount = MAX_PATH;
    std::string strResult(nCchCount, 0);
    va_list args;

    va_start(args, pFormat);

    do
    {
        //成功则赋值字符串并终止循环
        int nSize = _vsnprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
        if (-1 != nSize)
        {
            strResult.resize(nSize);
            break;
        }

        //缓冲大小超限终止
        if (nCchCount >= INT32_MAX)
        {
            break;
        }

        //重新分配缓冲
        nCchCount *= 2;
        strResult.resize(nCchCount);

    } while (true);

    va_end(args);

    return strResult;
}

bool ToFileA(const std::string& strFile, const std::string& str)
{
    std::ofstream outputFile(strFile.c_str(), std::ios::binary | std::ios::out);
    if (!outputFile.is_open())
    {
        return false;
    }

    outputFile.write(str.c_str(), str.size());
    outputFile.close();

    return true;
}
