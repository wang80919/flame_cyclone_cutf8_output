cd /d %~dp0

rem https://symbl.cc/cn/unicode-table/#cjk-symbols-and-punctuation

rem 基本拉丁字母
CUtf8.exe -start 00 -end 7f -code 0 x-ch 1 -hex 2 -name utf8_1

rem 拉丁字母扩充
CUtf8.exe -start 80 -end 7ff -code 0 x-ch 1 -hex 2 -name utf8_1

rem 国际音标扩展
CUtf8.exe -start 250 -end 2AF -code 0 x-ch 1 -hex 2 -name utf8_1

rem 中日韩字符集兼容
CUtf8.exe -start 3300 -end 33FF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区A
CUtf8.exe -start 3400 -end 4DBF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 易经六十四卦符号
CUtf8.exe -start 4DC0 -end 4DFF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字
CUtf8.exe -start 4E00 -end 9fff -code 0 x-ch 1 -hex 2 -name utf8_3

rem 日文平假名/片假名
CUtf8.exe -start 3040 -end 30FF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩兼容表意文字
CUtf8.exe -start F900 -end FAFF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区B
CUtf8.exe -start 20000 -end 2A6DF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区C
CUtf8.exe -start 2A700 -end 2B73F -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区D
CUtf8.exe -start 2B740 -end 2B81F -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区E
CUtf8.exe -start 2B820 -end 2CEAF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩统一表意文字扩展区D
CUtf8.exe -start 2CEB0 -end 2EBEF -code 0 x-ch 1 -hex 2 -name utf8_3

rem 中日韩兼容文字补充区
CUtf8.exe -start 2F800 -end 2FA1F -code 0 x-ch 1 -hex 2 -name utf8_3
