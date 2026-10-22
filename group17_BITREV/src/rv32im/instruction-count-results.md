# CP2 任務卡三：指令統計

## 統計範圍與方法

- 核心範圍：`bitrev` label 至 `ret`，不含 `test_main`、列印函式及測試 harness。
- 靜態指令數：以 GNU RISC-V GCC 組譯，再用 GNU `objdump` 檢視 RV32IM 機器碼；指令位置各計一次。
- 動態指令數：Ripes CLI 的 `--iret` 實測完整 11-vector 測試程式。這個總數包含核心、呼叫端、測試迴圈及列印函式，不可當成核心函式的動態指令數。
- 核心每次呼叫的動態數由固定路徑核算：核心沒有條件分支或迴圈，每次執行剛好經過 36 條機器指令。11 組測資各呼叫一次，因此核心合計 396 條；這是以反組譯路徑和呼叫次數計算，不是 Ripes 的函式區段計數器讀值。
- 條件分支只統計 `beq`、`bne`、`blt`、`bge` 等條件式分支；`ret` 展開為 `jalr`，是返回控制流程，不列為條件分支。

## 統計結果

| 統計範圍 | 靜態指令數 | 動態指令數 | 靜態條件分支數 | 動態條件分支執行數 |
|---|---:|---:|---:|---:|
| `bitrev` 函式，單次呼叫 | 36（含 `ret`；不含 `ret` 為 35） | 36（固定路徑核算） | 0 | 0 |
| `bitrev` 函式，CP1 11 組輸入各呼叫一次 | 36 | 396（36 × 11，固定路徑核算） | 0 | 0 |
| 完整 Ripes 11-vector 測試程式 | 不適用於核心函式比較 | 6487（Ripes `--iret` 實測） | 不納入核心統計 | 不納入核心統計 |

核心機器指令的靜態分布如下：

| RV32IM 機器指令 | 數量 |
|---|---:|
| `lui` | 4 |
| `addi` | 4 |
| `xori`（反組譯器以 `not` 別名顯示） | 4 |
| `and` | 8 |
| `slli` | 5 |
| `srli` | 5 |
| `or` | 5 |
| `jalr`（由 `ret` 展開） | 1 |
| **總計** | **36** |

核心無條件分支指令。測試 harness 自身含條件分支及迴圈控制，因此完整程式的 6487 條動態指令包含這些額外工作。

## 工具與可重現命令

靜態組譯／反組譯工具：

- GNU RISC-V GCC 16.2.0
- GNU Binutils `objdump` 2.47.20260726
- 目標參數：`-march=rv32im -mabi=ilp32`

```sh
riscv64-elf-gcc -march=rv32im -mabi=ilp32 \
  -c /tmp/group17-bitrev.S -o /tmp/group17-bitrev.o
riscv64-elf-objdump -d --start-address=0x4 --stop-address=0x94 \
  /tmp/group17-bitrev.o
```

動態測試工具：

- Ripes `v2.2.6-106-g5b8a616`（macOS universal2）
- 處理器：`RV32_5S`；ISA 擴充：`M`

此 C-only 分支不重複納入任務 A 的 `bitrev.S`。測試來源為 PR #9 的固定 commit
`2c6a8d5faaf1c0668d2fb863faa0a63e00b788ae`（commit `2c6a8d5` 僅修正註解中的
不配對括號，機器碼不變），先下載到暫存位置：

```sh
curl -L \
  https://raw.githubusercontent.com/faxyu729/2026-Computer-Architecture/2c6a8d5faaf1c0668d2fb863faa0a63e00b788ae/group17_BITREV/src/rv32im/bitrev.S \
  -o /tmp/group17-bitrev.S
```

以下命令執行測試並在終端機同時顯示逐筆結果和 Ripes 指令計數：

```sh
Ripes \
  --mode cli \
  --src /tmp/group17-bitrev.S \
  -t asm --proc RV32_5S --isaexts M --timeout 30000 \
  --iret
```

Ripes 終端機應顯示 `PASSED=11 FAILED=0` 和 `instructions retired = 6487`。本次實際終端機截圖見 [`cp2_instr_count.png`](../../report/figures/cp2_instr_count.png)。Ripes 計數器、暫存器和處理器資訊另存於 [`ripes-instruction-count.txt`](../../report/cp2-c/ripes-instruction-count.txt)，逐筆測試輸出另存於 [`ripes-instruction-count-console.txt`](../../report/cp2-c/ripes-instruction-count-console.txt)。來源 SHA-256 與 CP2-B manifest 一致，見 [`source-provenance.txt`](../../report/cp2-c/source-provenance.txt)。核心反組譯輸出見 [`instruction-count-objdump.txt`](instruction-count-objdump.txt)。

如需另存 Ripes 計數器輸出檔，可在命令中加入 `--regs --runinfo --output <輸出檔路徑>`；此時 Ripes 的計數器與處理器資訊會寫入該檔案。提交用截圖則使用上方未帶 `--output` 的命令，讓測試摘要和指令計數都顯示在同一個終端機畫面。

## 檔案對照

任務卡三指定的統計表由負責 CP2 報告彙整的組員放入 `group17_BITREV/report/CP2.md`；本分支不修改該共用報告。統計方法與反組譯原始資料保存在 `group17_BITREV/src/rv32im/`，Ripes 計數器及逐筆測試終端輸出保存在 `group17_BITREV/report/cp2-c/`。
