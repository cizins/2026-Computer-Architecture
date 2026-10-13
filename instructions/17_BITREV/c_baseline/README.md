# CP1 — BITREV C baseline（繳交成品）

CP1 檢查點「C 參考實作」的最終成品，集中於此資料夾。

## 檔案與分工

| 檔案 | 角色 | 說明 |
|---|---|---|
| `bitrev.h` | A（1114405020） | 宣告 `uint32_t bitrev(uint32_t x)` |
| `bitrev.c` | A（1114405020） | 分治位元交換（16/8/4/2/1，5 步無迴圈） |
| `test_vectors.h` | B（1114405042） | 11 組測資（≥10，含邊界值）與預期輸出 |
| `test_bitrev.c` | C（1114405041） | 測試驅動，另以 naive 逐位元實作交叉比對 |
| `Makefile` | C（1114405041） | 編譯與執行 |

## 使用方式

於本目錄執行：

```sh
make run     # 編譯並執行全部測資（使用 -Wall -Wextra -Werror）
make clean   # 清除執行檔
```

`make run` 會比較三者是否一致：

1. `bitrev()` 的實際輸出
2. 測資指定的預期輸出
3. naive 逐位元參考實作的輸出

## 驗證結果

目前共有 11 組測資，涵蓋零值、全一、最低/最高位元、左右 16-bit
對調、交錯 byte、對稱圖樣與隨機值。三者一致的結果為：

```text
BITREV CP1 test suite (11 vectors)
11/11 passed, 0 failed
```

## 報告

- CP1 完整報告：`report/CP1.md`（D，1114405012）

> CP2 的 RISC-V 編譯（`riscv64-elf-gcc -S -march=rv32im -mabi=ilp32`）留待下一階段。