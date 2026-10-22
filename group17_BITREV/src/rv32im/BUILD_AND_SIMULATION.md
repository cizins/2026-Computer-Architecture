# CP2-B — 編譯與模擬

負責人：1114405041 A1SimpleSmile。實測日期：2026-10-22（Asia/Taipei）。

## 工具與輸入

- Ripes **v2.3.0**，Windows x86-64 可攜版；版本由 GUI「Help → Version」確認。
- 處理器：**5-stage processor / RV32_5S**；ISA：**RV32IM**，啟用 M，未啟用 C。
- `bitrev.S`：延續任務 A／PR #4 的原始程式，核心與測試 harness 均未修改。
- `../c_baseline/test_vectors.h`：CP1 的全部 11 組測資，含 0、全 1、LSB、MSB 四組邊界值。
- 本流程直接使用 Ripes 內建組譯器組譯 `.S`，不需要先產生 ELF。
  因此沒有 GCC 交叉編譯步驟；若另用 GNU RISC-V GCC，必須指定
  `-march=rv32im -mabi=ilp32`。本次未宣稱驗證 GNU 組譯或 Linux 執行。
- 程式內的 `ecall` 使用 Ripes 服務編號，應在 Ripes 中執行。

## GUI 重現步驟（已由負責人手動操作）

1. 開啟 Ripes。用晶片圖示選擇 32-bit 5-stage processor，啟用 M；右下角確認 `ISA: RV32IM`。
2. 點 **File → Load Program**（Ctrl+O），File type 選 **Source file**，選取本目錄 `bitrev.S`，按 OK。
3. 在 Editor 確認 Input type 為 Assembly，能看到原始碼及組譯後的指令，沒有組譯錯誤。
4. 點工具列 **雙箭頭 ▶▶** 執行到結束；Console 應列出 T1～T11 全部 PASS、
   `PASSED=11 FAILED=0` 及 `Program exited with code: 0`。
5. 保留執行後狀態。在 Processor 的 Registers 選 Hex，記錄 x18/s2=11、x19/s3=11、x20/s4=0。
6. 切到左側 Memory，Display type 選 Hex。Go to register 選 s5/x21，查看結果區及 flags。
   本檔的資料布局如下；每列 Word 為 32 位元，畫面位址由上往下遞減。
7. 查看 `results[]` 的 11 個 word 與 `flags[]` 的 11 個 1；Go to register 選 s6/x22
   可跳到總結資料附近。先保留執行後截圖，再按重設，以便額外觀察資料從初始 0 變成結果。
8. 點 **Help → Version**，記錄 v2.3.0。

| 記憶體區域 | 位址 | 執行後內容 |
|---|---|---|
| results[0..10] | 0x10000058～0x10000080 | 見 [逐筆結果](../../report/cp2-b/test-results.md) |
| flags[0..10] | 0x10000084～0x100000AC | 11 個 0x00000001 |
| summary_pass | 0x100000B0 | 實際 0x0000000B |
| summary_fail | 0x100000B4 | 實際 0x00000000 |

位址適用於這份未修改的程式。若資料區或測資數量改動，須重新確認位址。

## CLI 重現步驟（已實際執行）

下載並解壓 [官方 Ripes v2.3.0 Windows 可攜版](https://github.com/mortbopet/Ripes/releases/tag/v2.3.0)。
在本目錄開 PowerShell，將路徑改為自己的 Ripes.exe：

```powershell
.\run-ripes.ps1 -RipesPath 'C:\tools\Ripes\Ripes.exe'
```

腳本以 Ripes 的內建組譯器組譯並執行原始程式，等待完成、擷取 stdout/stderr 與暫存器。
Windows GUI executable 使用 `Start-Process -Wait` 擷取輸出，避免直接呼叫時沒有等待或沒有 Console 輸出。
實際 CLI 參數為：

```text
Ripes.exe --mode cli --src "bitrev.S 的完整路徑" -t asm --proc RV32_5S --isaexts M --timeout 30000 --regs --runinfo --output "暫存器紀錄完整路徑"
```

本機實際完整命令保存在 [command.txt](../../report/cp2-b/command.txt)。
GUI 與 CLI 的 sp/gp 初值可能不同；本程式不使用堆疊，驗收比較測試結果、s2/s3/s4 及資料區，
不要求所有暫存器都相同。

腳本核對 11 組 input/expected/actual 與 CP1 header、PASS 總結、程式結束碼及 s2/s3/s4。
失敗會拋出錯誤；成功才產生測試表與 manifest。輸出存於 `report/cp2-b/`：

- `command.txt`：本次執行的完整命令。
- `ripes-stdout.txt`：Ripes 實際輸出，11/11 PASS。
- `ripes-stderr.txt`：本次為空。
- `ripes-registers.txt`：最終暫存器與處理器設定。
- `test-results.md`：從本次 stdout 核對並產生的 11 組結果表。
- `manifest.json`：時間、原始碼／CP1 測資／Ripes.exe 的 SHA-256 與結果。

本次未啟用 `--iret`、`--cycles` 等效能統計；指令統計由任務 C 處理，
也不將包含列印 harness 的整個程式當成 bitrev 核心的指令數。

## CP1 C baseline 交叉驗證

另外在 WSL Ubuntu 用 **GCC 15.2.0** 執行原有 C 測試程式。
在 `group17_BITREV/src/c_baseline/` 中的等效重現指令：

```sh
gcc --version
gcc -Wall -Wextra -std=c11 bitrev.c test_bitrev.c -o /tmp/cp2-bitrev-cp1
/tmp/cp2-bitrev-cp1
```

本次實際執行的工作目錄及指令見 [CP1 執行說明](../../report/cp2-b/cp1-command.txt)，
原始輸出見 [cp1-baseline.txt](../../report/cp2-b/cp1-baseline.txt)：
`Total: 11, Passed: 11, Failed: 0`，結束碼 0。
這是主機 C 正確性比對，不是 RV32IM GCC 交叉編譯。

## 佐證與遇到的問題

| 圖片 | 證明內容 |
|---|---|
| [cp2_ripes_tests_01.png](../../report/figures/cp2_ripes_tests_01.png) | 手動 GUI 執行的 11 組輸出、PASS 總結、exit 0；右下角 RV32IM |
| [cp2_ripes_regs.png](../../report/figures/cp2_ripes_regs.png) | s2=11、s3=11、s4=0；5-stage processor / RV32IM |
| [cp2_ripes_memory_results.png](../../report/figures/cp2_ripes_memory_results.png) | results[] 全部 11 組實際結果 |
| [cp2_ripes_memory_flags.png](../../report/figures/cp2_ripes_memory_flags.png) | flags[] 全部為 1 |
| [cp2_ripes_summary_values.png](../../report/figures/cp2_ripes_summary_values.png) | summary_pass=11、summary_fail=0 的記憶體內容 |
| [cp2_ripes_version.png](../../report/figures/cp2_ripes_version.png) | Ripes v2.3.0 |

- 載入 `.S` 時若選 Executable (ELF)，會顯示 `Error: Not an ELF file`；改選 **Source file** 即可。
- 本版選單名稱是 **Load Program**、**Help → Version**。
- Computer Use 未獲准存取 Ripes 視窗，因此 GUI 操作與截圖由負責人手動完成；CLI 由 AI 協助執行與核對。
- 原命名 `cp2_ripes_memory_summary.png` 的圖片只拍到 flags，交付時按內容另存為 `cp2_ripes_memory_flags.png`；
  使用者另外補拍 `cp2_ripes_summary_values.png`，已確認 B0=11、B4=0。

本任務交付組譯／模擬流程與正確性紀錄；任務 C 的靜態／動態指令統計、任務 D 的完整六段式報告仍由其負責人完成。
