# src/rv32im/

CP2：以 RV32IM 組合語言實作 32-bit BITREV，並統計靜態／動態指令數與分支數。

## 檔案

| 檔案 | 說明 |
|---|---|
| `bitrev.S` | RV32IM 核心實作（分治 5 步遮罩交換），另含 Ripes 測試 harness |
| `BUILD_AND_SIMULATION.md` | 組譯／模擬步驟、Ripes 設定與重現方式 |
| `run-ripes.ps1` | Windows PowerShell 腳本，自動跑 11 組測資並產生紀錄 |
| `instruction-count-results.md` | 指令統計方法與結果 |
| `instruction-count-objdump.txt` | 核心反組譯（靜態指令數來源） |
| `.gitattributes` | 強制 `bitrev.S` 使用 LF，避免換行造成 hash 不一致 |

測試紀錄與截圖放在 [`../../report/cp2-b/`](../../report/cp2-b/) 與
[`../../report/figures/`](../../report/figures/)。

## 核心設計

- 介面：`uint32_t bitrev(uint32_t x)`，輸入與輸出都使用 `a0`。
- 演算法：分治 5 步遮罩交換（16 / 8 / 4 / 2 / 1 位元）。
- 暫存器：`a0` 輸入／輸出；`t0`、`t1` 互補遮罩；`t2`、`t3` 中間值。
- leaf function，不使用堆疊、沒有條件分支；單次呼叫固定 36 條指令。

## 重現方式

Ripes 使用內建組譯器（GUI 或 CLI）：

```text
Ripes.exe --mode cli --src group17_BITREV/src/rv32im/bitrev.S -t asm --proc RV32_5S --isaexts M --timeout 30000 --regs --runinfo --output group17_BITREV/report/cp2-b/ripes-registers.txt
```

GNU RISC-V GCC 交叉組譯驗證：

```sh
riscv64-unknown-elf-gcc -march=rv32im -mabi=ilp32 -c group17_BITREV/src/rv32im/bitrev.S -o /tmp/bitrev.o
```

## 測試結果

CP1 的 11 組測資（含 4 組邊界值）全部 PASS，Ripes 輸出
`PASSED=11 FAILED=0`。逐筆結果見
[`../../report/cp2-b/test-results.md`](../../report/cp2-b/test-results.md)，
截圖見 [`../../report/figures/`](../../report/figures/)。

## 指令統計摘要

| 範圍 | 靜態 | 動態 | 條件分支 |
|---|---:|---:|---:|
| `bitrev` 核心（單次呼叫） | 36（不含 `ret` 為 35） | 36 | 0 |
| `bitrev` 核心（11 次呼叫） | 36 | 396（36 × 11，固定路徑核算） | 0 |
| 完整測試程式（11 組） | — | 6487（Ripes `--iret` 實測） | 不計 |

詳細方法、工具版本與可重現命令見
[`instruction-count-results.md`](instruction-count-results.md)，完整報告見
[`../../report/CP2.md`](../../report/CP2.md)。

## 對應 Checkpoint

- CP2：RV32IM 組合語言實作與指令統計（僅使用 RV32IM）
